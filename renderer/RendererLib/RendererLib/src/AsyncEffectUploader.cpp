//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "RendererLib/AsyncEffectUploader.h"
#include "Platform_Base/Context_Base.h"
#include "RendererAPI/IRenderBackend.h"
#include "RendererAPI/IResourceUploadRenderBackend.h"
#include "RendererAPI/IDevice.h"
#include "RendererAPI/ISurface.h"
#include "RendererAPI/IPlatform.h"
#include "Resource/EffectResource.h"
#include "Utils/LogMacros.h"
#include "absl/algorithm/container.h"

namespace ramses_internal
{
    AsyncEffectUploader::AsyncEffectUploader(IPlatform& platform, IRenderBackend& renderBackend) : m_platform(platform)
        , m_renderBackend(renderBackend)
        , m_thread("R_EffectUpload")
    {
    }

    AsyncEffectUploader::~AsyncEffectUploader()
    {
        assert(!m_thread.isRunning());
    }

    bool AsyncEffectUploader::createResourceUploadRenderBackendAndStartThread()
    {
        assert(!m_thread.isRunning());

        //disable main context to be able to create shared context in new thread
        m_renderBackend.getSurface().disable();
        m_thread.start(*this);

        const auto success = m_creationSuccess.get_future().get();
        if (!success)
            m_thread.join();

        return success;
    }

    void AsyncEffectUploader::destroyResourceUploadRenderBackendAndStopThread()
    {
        assert(m_thread.isRunning() && !isCancelRequested());
        {
            std::unique_lock<std::mutex> guard(m_mutex);
            //call thread cancel inside critical section to avoid having deadlock on wait() inside uploadEffectsOrWait
            m_thread.cancel();
        }

        m_sleepConditionVar.notify_one();
        m_thread.join();
    }

    void AsyncEffectUploader::uploadEffectsOrWait(IResourceUploadRenderBackend& resourceUploadRenderBackend)
    {
        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::uploadEffectsOrWait: starting");

        EffectsRawResources effectsToUpload;
        {
            std::unique_lock<std::mutex> guard(m_mutex);
            m_sleepConditionVar.wait(guard, [&]() { return !m_effectsToUpload.empty() || !m_effectsUploadedCache.empty() || isCancelRequested(); });

            m_effectsUploaded.insert(m_effectsUploaded.end(), std::make_move_iterator(m_effectsUploadedCache.begin()), std::make_move_iterator(m_effectsUploadedCache.end()));
            m_effectsUploadedCache.clear();

            //assert none of the shaders to be uploaded next was already uploaded since last sync
            assert(absl::c_all_of(m_effectsToUpload, [&](const auto& toUpload) {
                return absl::c_find_if(m_effectsUploaded, [&](const auto& uploaded) {return uploaded.first == toUpload->getHash(); }) == m_effectsUploaded.cend();
                }));

            m_effectsToUpload.swap(effectsToUpload);
        }

        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::uploadEffectsOrWait: will upload: " << effectsToUpload.size() << ",  uploaded in cache:" << m_effectsUploadedCache.size());

        std::chrono::microseconds maxShaderUploadTime{ 0u };
        std::chrono::microseconds totalShaderUploadTime{ 0u };
        ResourceContentHash effectWithMaxUploadTime;

        for (const auto effectRes : effectsToUpload)
        {
            const auto& effectHash = effectRes->getHash();
            LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader uploading: " << effectHash);

            assert(absl::c_find_if(m_effectsUploadedCache, [&effectHash](const auto& u) {return effectHash == u.first; }) == m_effectsUploadedCache.cend());
            const auto shaderUploadStart = std::chrono::steady_clock::now();
            auto shaderResource = resourceUploadRenderBackend.getDevice().uploadShader(*effectRes);
            const auto shaderUploadTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - shaderUploadStart);

            m_effectsUploadedCache.emplace_back(effectHash, std::move(shaderResource));

            if (shaderUploadTime > maxShaderUploadTime || !effectWithMaxUploadTime.isValid())
            {
                maxShaderUploadTime = shaderUploadTime;
                effectWithMaxUploadTime = effectHash;
            }
            totalShaderUploadTime += shaderUploadTime;
        }

        if (!effectsToUpload.empty())
            LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader " << effectsToUpload.size() << " uploaded in "
                << totalShaderUploadTime.count() << " us ("
                << "Max: " << maxShaderUploadTime.count() << " us " << effectWithMaxUploadTime << ")");

        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::uploadEffectsOrWait: finished");
    }

    void AsyncEffectUploader::sync(const EffectsRawResources& effectsToUpload, EffectsGpuResources& uploadedResourcesOut)
    {
        assert(uploadedResourcesOut.empty());

        std::size_t totalEffectsToUpload = 0u;
        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::sync: starting");
        {
            std::lock_guard<std::mutex> guard(m_mutex);

            m_effectsToUpload.insert(m_effectsToUpload.cend(), effectsToUpload.cbegin(), effectsToUpload.cend());
            uploadedResourcesOut.swap(m_effectsUploaded);

            totalEffectsToUpload = m_effectsToUpload.size();
        }

        if (!effectsToUpload.empty() || !uploadedResourcesOut.empty())
        {
            LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader newToUpload: " << effectsToUpload.size()
                << ", totalPending: " << totalEffectsToUpload
                << ", uploaded: " << uploadedResourcesOut.size());
        }

        if (!effectsToUpload.empty())
            m_sleepConditionVar.notify_one();

        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::sync: finished");
    }

    void AsyncEffectUploader::run()
    {
        LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader creating render backend for resource uploading");
        auto resourceUploadRenderBackend = m_platform.createResourceUploadRenderBackend(m_renderBackend);
        if (!resourceUploadRenderBackend)
        {
            LOG_ERROR(CONTEXT_RENDERER, "AsyncEffectUploader failed creating resource upload render backend");
            m_creationSuccess.set_value(false);
            return;
        }
        LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader resource upload render backend created successfully");
        m_creationSuccess.set_value(true);

        while (!isCancelRequested())
            uploadEffectsOrWait(*resourceUploadRenderBackend);

        LOG_INFO(CONTEXT_RENDERER, "AsyncEffectUploader will destroy resource upload render backend");
        m_platform.destroyResourceUploadRenderBackend(*resourceUploadRenderBackend);
        LOG_TRACE(CONTEXT_RENDERER, "AsyncEffectUploader::run: exiting thread");
    }
}

