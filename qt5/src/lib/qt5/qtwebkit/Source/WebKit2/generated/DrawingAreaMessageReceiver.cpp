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

#include "DrawingArea.h"

#if PLATFORM(MAC)
#include "ColorSpaceData.h"
#endif
#include "DrawingAreaMessages.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebCoreArgumentCoders.h"
#include <WebCore/IntSize.h>

namespace WebKit {

void DrawingArea::didReceiveDrawingAreaMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::DrawingArea::UpdateBackingStoreState::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::UpdateBackingStoreState>(decoder, this, &DrawingArea::updateBackingStoreState);
        return;
    }
    if (decoder.messageName() == Messages::DrawingArea::DidUpdate::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::DidUpdate>(decoder, this, &DrawingArea::didUpdate);
        return;
    }
    if (decoder.messageName() == Messages::DrawingArea::SuspendPainting::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::SuspendPainting>(decoder, this, &DrawingArea::suspendPainting);
        return;
    }
    if (decoder.messageName() == Messages::DrawingArea::ResumePainting::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::ResumePainting>(decoder, this, &DrawingArea::resumePainting);
        return;
    }
    if (decoder.messageName() == Messages::DrawingArea::PageCustomRepresentationChanged::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::PageCustomRepresentationChanged>(decoder, this, &DrawingArea::pageCustomRepresentationChanged);
        return;
    }
    if (decoder.messageName() == Messages::DrawingArea::SetLayerHostingMode::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::SetLayerHostingMode>(decoder, this, &DrawingArea::setLayerHostingMode);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::DrawingArea::UpdateGeometry::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::UpdateGeometry>(decoder, this, &DrawingArea::updateGeometry);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::DrawingArea::SetDeviceScaleFactor::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::SetDeviceScaleFactor>(decoder, this, &DrawingArea::setDeviceScaleFactor);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::DrawingArea::SetColorSpace::name()) {
        CoreIPC::handleMessage<Messages::DrawingArea::SetColorSpace>(decoder, this, &DrawingArea::setColorSpace);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
