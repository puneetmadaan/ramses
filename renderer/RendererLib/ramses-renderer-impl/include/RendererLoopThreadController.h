//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_RENDERERLOOPTHREADCONTROLLER_H
#define RAMSES_RENDERERLOOPTHREADCONTROLLER_H

#include "RendererAPI/ELoopMode.h"
#include "PlatformAbstraction/PlatformThread.h"
#include <mutex>
#include <condition_variable>

namespace ramses_internal
{
    class PlatformWatchdog;
    class DisplayDispatcher;

    class RendererLoopThreadController : public Runnable
    {
    public:
        RendererLoopThreadController(DisplayDispatcher& displayDispatcher, PlatformWatchdog& watchdog, std::chrono::milliseconds loopCountPeriod);
        ~RendererLoopThreadController();

        Bool startRendering();
        Bool isRendering() const;
        Bool stopRendering();
        void setMaximumFramerate(Float maximumFramerate);
        Float getMaximumFramerate() const;
        void setLoopMode(ELoopMode loopMode);

        void destroyRenderer();

    private:
        virtual void run() override;

        void calculateLooptimeAverage(const std::chrono::microseconds loopDuration, const uint64_t loopEndTime);

        std::chrono::milliseconds sleepToControlFramerate(std::chrono::microseconds loopDuration, std::chrono::microseconds minimumFrameDuration);

        DisplayDispatcher* m_displayDispatcher;
        PlatformWatchdog& m_watchdog;
        PlatformThread m_thread;
        mutable std::mutex m_lock;
        std::condition_variable m_sleepConditionVar;
        Bool m_doRendering;
        std::chrono::microseconds m_targetMinimumFrameDuration;
        Bool m_threadStarted;
        ELoopMode m_loopMode = ELoopMode::UpdateAndRender;
        std::condition_variable m_rendererDestroyedCondVar;
        Bool m_destroyRenderer;
        std::chrono::milliseconds m_loopCountPeriod;
        uint64_t m_lastPeriodLoopCountReportingTimeMicroseconds{ 0 };
        std::chrono::microseconds m_maximumLoopTimeInPeriod{ 0 };
        uint64_t m_numberOfLoopsInPeriod{ 0 };
        std::chrono::microseconds m_sumOfLoopTimeInPeriod{ 0 };
    };
}

#endif
