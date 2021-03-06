// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"
#include "graphics/RenderTarget.h"
#include "math/Size2.h"

#if defined(__OBJC__)
#import <Metal/Metal.h>
typedef MTLRenderPassDescriptor* MTLRenderPassDescriptorPtr;
typedef id<MTLTexture> MTLTexturePtr;
#else
typedef id MTLRenderPassDescriptorPtr;
typedef id MTLTexturePtr;
#endif

namespace ouzel
{
    namespace graphics
    {
        class RenderTargetMetal: public RenderTarget
        {
        public:
            RenderTargetMetal();
            virtual ~RenderTargetMetal();
            virtual void free() override;

            MTLRenderPassDescriptorPtr getRenderPassDescriptor() const { return renderPassDescriptor; }

        protected:
            virtual bool upload() override;

            MTLRenderPassDescriptorPtr renderPassDescriptor = Nil;
            MTLTexturePtr msaaTexture = Nil;
        };
    } // namespace graphics
} // namespace ouzel
