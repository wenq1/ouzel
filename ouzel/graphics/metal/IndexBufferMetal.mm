// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "IndexBufferMetal.h"
#include "RendererMetal.h"
#include "core/Engine.h"
#include "utils/Log.h"

namespace ouzel
{
    namespace graphics
    {
        IndexBufferMetal::IndexBufferMetal()
        {
        }

        IndexBufferMetal::~IndexBufferMetal()
        {
            if (buffer)
            {
                [buffer release];
            }
        }

        void IndexBufferMetal::free()
        {
            IndexBuffer::free();

            if (buffer)
            {
                [buffer release];
                buffer = Nil;
            }

            bufferSize = 0;
        }

        bool IndexBufferMetal::upload()
        {
            if (uploadData.dirty)
            {
                RendererMetal* rendererMetal = static_cast<RendererMetal*>(sharedEngine->getRenderer());

                if (uploadData.dirty & INDEX_SIZE_DIRTY)
                {
                    switch (uploadData.indexSize)
                    {
                        case 2:
                            type = MTLIndexTypeUInt16;
                            bytesPerIndex = 2;
                            break;
                        case 4:
                            type = MTLIndexTypeUInt32;
                            bytesPerIndex = 4;
                            break;
                        default:
                            bytesPerIndex = 0;
                            Log(Log::Level::ERR) << "Invalid index size";
                            return false;
                    }

                    uploadData.dirty &= ~INDEX_SIZE_DIRTY;
                }

                if (uploadData.dirty & INDEX_BUFFER_DIRTY)
                {
                    if (!uploadData.data.empty())
                    {
                        if (!buffer || uploadData.data.size() > bufferSize)
                        {
                            if (buffer) [buffer release];

                            bufferSize = static_cast<uint32_t>(uploadData.data.size());

                            buffer = [rendererMetal->getDevice() newBufferWithLength:bufferSize
                                                                             options:MTLResourceCPUCacheModeWriteCombined];

                            if (!buffer)
                            {
                                Log(Log::Level::ERR) << "Failed to create Metal index buffer";
                                return false;
                            }
                        }

                        std::copy(uploadData.data.begin(), uploadData.data.end(), static_cast<uint8_t*>([buffer contents]));
                    }

                    uploadData.dirty &= ~INDEX_BUFFER_DIRTY;
                }

                uploadData.dirty = 0;
            }

            return true;
        }
    } // namespace graphics
} // namespace ouzel
