//  -------------------------------------------------------------------------
//  Copyright (C) 2016 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_IBINARYSHADERCACHE_H
#define RAMSES_IBINARYSHADERCACHE_H

#include "SceneAPI/ResourceContentHash.h"
#include "SceneAPI/SceneId.h"
#include "RendererAPI/Types.h"
#include <mutex>

namespace ramses_internal
{
    class IBinaryShaderCache
    {
    public:
        virtual ~IBinaryShaderCache() {};

        virtual void deviceSupportsBinaryShaderFormats(const std::vector<BinaryShaderFormatID>& supportedFormats) = 0;

        virtual bool hasBinaryShader(ResourceContentHash effectHash) const = 0;
        virtual uint32_t getBinaryShaderSize(ResourceContentHash effectHash) const = 0;
        virtual BinaryShaderFormatID getBinaryShaderFormat(ResourceContentHash effectHash) const = 0;
        virtual void getBinaryShaderData(ResourceContentHash effectHash, UInt8* buffer, UInt32 bufferSize) const = 0;

        virtual bool shouldBinaryShaderBeCached(ResourceContentHash effectHash, SceneId sceneId) const = 0;

        virtual void storeBinaryShader(ResourceContentHash effectHash, SceneId sceneId, const uint8_t* binaryShaderData, uint32_t binaryShaderDataSize, BinaryShaderFormatID binaryShaderFormat) = 0;
        virtual void binaryShaderUploaded(ResourceContentHash effectHash, Bool success) const = 0;
        virtual std::once_flag& binaryShaderFormatsReported() = 0;
    };
}

#endif
