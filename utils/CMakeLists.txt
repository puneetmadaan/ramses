#  -------------------------------------------------------------------------
#  Copyright (C) 2018 BMW Car IT GmbH
#  -------------------------------------------------------------------------
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.
#  -------------------------------------------------------------------------

if(ramses-sdk_BUILD_TOOLS)
    ADD_SUBDIRECTORY(ramses-utils)
    ADD_SUBDIRECTORY(ramses-resource-tools)
    ADD_SUBDIRECTORY(ramses-shader-tools)
    ADD_SUBDIRECTORY(ramses-scene-viewer)
    ADD_SUBDIRECTORY(ramses-stream-viewer)
endif()
