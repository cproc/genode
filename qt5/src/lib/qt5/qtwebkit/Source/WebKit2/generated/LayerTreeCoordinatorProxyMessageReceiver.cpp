/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if USE(COORDINATED_GRAPHICS)

#include "LayerTreeCoordinatorProxy.h"

#include "ArgumentCoders.h"
#include "HandleMessage.h"
#include "LayerTreeCoordinatorProxyMessages.h"
#include "MessageDecoder.h"
#include "ShareableSurface.h"
#include "SurfaceUpdateInfo.h"
#include "WebCoreArgumentCoders.h"
#include "WebLayerTreeInfo.h"
#include <WebCore/Color.h>
#if ENABLE(CSS_SHADERS)
#include <WebCore/CustomFilterProgramInfo.h>
#endif
#if ENABLE(CSS_FILTERS)
#include <WebCore/FilterOperations.h>
#endif
#include <WebCore/GraphicsLayerAnimation.h>
#if USE(GRAPHICS_SURFACE)
#include <WebCore/GraphicsSurfaceToken.h>
#endif
#include <WebCore/IntPoint.h>
#include <WebCore/IntRect.h>
#include <WebCore/IntSize.h>
#include <wtf/Vector.h>

namespace WebKit {

void LayerTreeCoordinatorProxy::didReceiveLayerTreeCoordinatorProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetCompositingLayerState::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetCompositingLayerState>(decoder, this, &LayerTreeCoordinatorProxy::setCompositingLayerState);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetCompositingLayerChildren::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetCompositingLayerChildren>(decoder, this, &LayerTreeCoordinatorProxy::setCompositingLayerChildren);
        return;
    }
#if ENABLE(CSS_FILTERS)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetCompositingLayerFilters::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetCompositingLayerFilters>(decoder, this, &LayerTreeCoordinatorProxy::setCompositingLayerFilters);
        return;
    }
#endif
#if ENABLE(CSS_SHADERS)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::CreateCustomFilterProgram::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::CreateCustomFilterProgram>(decoder, this, &LayerTreeCoordinatorProxy::createCustomFilterProgram);
        return;
    }
#endif
#if ENABLE(CSS_SHADERS)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::RemoveCustomFilterProgram::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::RemoveCustomFilterProgram>(decoder, this, &LayerTreeCoordinatorProxy::removeCustomFilterProgram);
        return;
    }
#endif
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetRootCompositingLayer::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetRootCompositingLayer>(decoder, this, &LayerTreeCoordinatorProxy::setRootCompositingLayer);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::DeleteCompositingLayer::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::DeleteCompositingLayer>(decoder, this, &LayerTreeCoordinatorProxy::deleteCompositingLayer);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::CreateTileForLayer::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::CreateTileForLayer>(decoder, this, &LayerTreeCoordinatorProxy::createTileForLayer);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::UpdateTileForLayer::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::UpdateTileForLayer>(decoder, this, &LayerTreeCoordinatorProxy::updateTileForLayer);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::RemoveTileForLayer::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::RemoveTileForLayer>(decoder, this, &LayerTreeCoordinatorProxy::removeTileForLayer);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::CreateUpdateAtlas::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::CreateUpdateAtlas>(decoder, this, &LayerTreeCoordinatorProxy::createUpdateAtlas);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::RemoveUpdateAtlas::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::RemoveUpdateAtlas>(decoder, this, &LayerTreeCoordinatorProxy::removeUpdateAtlas);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::CreateImageBacking::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::CreateImageBacking>(decoder, this, &LayerTreeCoordinatorProxy::createImageBacking);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::UpdateImageBacking::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::UpdateImageBacking>(decoder, this, &LayerTreeCoordinatorProxy::updateImageBacking);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::ClearImageBackingContents::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::ClearImageBackingContents>(decoder, this, &LayerTreeCoordinatorProxy::clearImageBackingContents);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::RemoveImageBacking::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::RemoveImageBacking>(decoder, this, &LayerTreeCoordinatorProxy::removeImageBacking);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::DidRenderFrame::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::DidRenderFrame>(decoder, this, &LayerTreeCoordinatorProxy::didRenderFrame);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::DidChangeScrollPosition::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::DidChangeScrollPosition>(decoder, this, &LayerTreeCoordinatorProxy::didChangeScrollPosition);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetLayerAnimations::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetLayerAnimations>(decoder, this, &LayerTreeCoordinatorProxy::setLayerAnimations);
        return;
    }
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetAnimationsLocked::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetAnimationsLocked>(decoder, this, &LayerTreeCoordinatorProxy::setAnimationsLocked);
        return;
    }
#if ENABLE(REQUEST_ANIMATION_FRAME)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::RequestAnimationFrame::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::RequestAnimationFrame>(decoder, this, &LayerTreeCoordinatorProxy::requestAnimationFrame);
        return;
    }
#endif
#if USE(GRAPHICS_SURFACE)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::CreateCanvas::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::CreateCanvas>(decoder, this, &LayerTreeCoordinatorProxy::createCanvas);
        return;
    }
#endif
#if USE(GRAPHICS_SURFACE)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SyncCanvas::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SyncCanvas>(decoder, this, &LayerTreeCoordinatorProxy::syncCanvas);
        return;
    }
#endif
#if USE(GRAPHICS_SURFACE)
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::DestroyCanvas::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::DestroyCanvas>(decoder, this, &LayerTreeCoordinatorProxy::destroyCanvas);
        return;
    }
#endif
    if (decoder.messageName() == Messages::LayerTreeCoordinatorProxy::SetBackgroundColor::name()) {
        CoreIPC::handleMessage<Messages::LayerTreeCoordinatorProxy::SetBackgroundColor>(decoder, this, &LayerTreeCoordinatorProxy::setBackgroundColor);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // USE(COORDINATED_GRAPHICS)
