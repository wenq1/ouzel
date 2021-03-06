// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "graphics/RenderTarget.h"

namespace ouzel
{
    namespace graphics
    {
        class RenderTargetEmpty: public RenderTarget
        {
        public:
            RenderTargetEmpty();

        protected:
            virtual bool upload() override;
        };
    } // namespace graphics
} // namespace ouzel
