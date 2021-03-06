//  -------------------------------------------------------------------------
//  Copyright (C) 2016 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_ICOMMUNICATIONSYSTEM_H
#define RAMSES_ICOMMUNICATIONSYSTEM_H

#include "TransportCommon/ServiceHandlerInterfaces.h"
#include "SceneAPI/SceneId.h"
#include "SceneAPI/SceneTypes.h"
#include "Components/ManagedResource.h"
#include "Utils/IPeriodicLogSupplier.h"
#include "Components/DcsmTypes.h"

namespace ramses_internal
{
    class Guid;
    class IConnectionStatusUpdateNotifier;
    class SceneActionCollection;
    class ISceneUpdateSerializer;

    class ICommunicationSystem : public IPeriodicLogSupplier
    {
    public:
        virtual ~ICommunicationSystem() override {}

        // connection management
        virtual bool connectServices() = 0;
        virtual bool disconnectServices() = 0;

        virtual IConnectionStatusUpdateNotifier& getRamsesConnectionStatusUpdateNotifier() = 0;
        virtual IConnectionStatusUpdateNotifier& getDcsmConnectionStatusUpdateNotifier() = 0;

        // scene
        virtual bool broadcastNewScenesAvailable(const SceneInfoVector& newScenes) = 0;
        virtual bool broadcastScenesBecameUnavailable(const SceneInfoVector& unavailableScenes) = 0;
        virtual bool sendScenesAvailable(const Guid& to, const SceneInfoVector& availableScenes) = 0;

        virtual bool sendSubscribeScene(const Guid& to, const SceneId& sceneId) = 0;
        virtual bool sendUnsubscribeScene(const Guid& to, const SceneId& sceneId) = 0;

        virtual bool sendInitializeScene(const Guid& to, const SceneId& sceneId) = 0;
        virtual bool sendSceneUpdate(const Guid& to, const SceneId& sceneId, const ISceneUpdateSerializer& serializer) = 0;

        virtual bool sendRendererEvent(const Guid& to, const SceneId& sceneId, const std::vector<Byte>& data) = 0;

        // dcsm provider -> consumer
        virtual bool sendDcsmBroadcastOfferContent(ContentID contentID, Category, ETechnicalContentType technicalContentType, const std::string& friendlyName) = 0;
        virtual bool sendDcsmOfferContent(const Guid& to, ContentID contentID, Category, ETechnicalContentType technicalContentType, const std::string& friendlyName) = 0;
        virtual bool sendDcsmContentDescription(const Guid& to, ContentID contentID, TechnicalContentDescriptor technicalContentDescriptor) = 0;
        virtual bool sendDcsmContentReady(const Guid& to, ContentID contentID) = 0;
        virtual bool sendDcsmContentEnableFocusRequest(const Guid& to, ContentID contentID, int32_t focusRequest) = 0;
        virtual bool sendDcsmContentDisableFocusRequest(const Guid& to, ContentID contentID, int32_t focusRequest) = 0;
        virtual bool sendDcsmBroadcastRequestStopOfferContent(ContentID contentID) = 0;
        virtual bool sendDcsmBroadcastForceStopOfferContent(ContentID contentID) = 0;
        virtual bool sendDcsmUpdateContentMetadata(const Guid& to, ContentID contentID, const DcsmMetadata& metadata) = 0;

        // dcsm consumer -> provider
        virtual bool sendDcsmCanvasSizeChange(const Guid& to, ContentID contentID, const CategoryInfo& categoryInfo, AnimationInformation) = 0;
        virtual bool sendDcsmContentStateChange(const Guid& to, ContentID contentID, EDcsmState status, const CategoryInfo&, AnimationInformation) = 0;


        // set service handlers
        virtual void setSceneProviderServiceHandler(ISceneProviderServiceHandler* handler) = 0;
        virtual void setSceneRendererServiceHandler(ISceneRendererServiceHandler* handler) = 0;

        virtual void setDcsmProviderServiceHandler(IDcsmProviderServiceHandler* handler) = 0;
        virtual void setDcsmConsumerServiceHandler(IDcsmConsumerServiceHandler* handler) = 0;


        // log triggers
        virtual void logConnectionInfo() = 0;
        virtual void triggerLogMessageForPeriodicLog() override = 0;
    };
}

#endif
