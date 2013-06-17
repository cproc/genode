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

#if ENABLE(FULLSCREEN_API)

#include "WebFullScreenManagerProxy.h"

#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebCoreArgumentCoders.h"
#include "WebFullScreenManagerProxyMessages.h"
#include <WebCore/IntRect.h>

namespace WebKit {

void WebFullScreenManagerProxy::didReceiveWebFullScreenManagerProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::EnterFullScreen::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::EnterFullScreen>(decoder, this, &WebFullScreenManagerProxy::enterFullScreen);
        return;
    }
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::ExitFullScreen::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::ExitFullScreen>(decoder, this, &WebFullScreenManagerProxy::exitFullScreen);
        return;
    }
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::BeganEnterFullScreen::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::BeganEnterFullScreen>(decoder, this, &WebFullScreenManagerProxy::beganEnterFullScreen);
        return;
    }
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::BeganExitFullScreen::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::BeganExitFullScreen>(decoder, this, &WebFullScreenManagerProxy::beganExitFullScreen);
        return;
    }
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::Close::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::Close>(decoder, this, &WebFullScreenManagerProxy::close);
        return;
    }
    ASSERT_NOT_REACHED();
}

void WebFullScreenManagerProxy::didReceiveSyncWebFullScreenManagerProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebFullScreenManagerProxy::SupportsFullScreen::name()) {
        CoreIPC::handleMessage<Messages::WebFullScreenManagerProxy::SupportsFullScreen>(decoder, *replyEncoder, this, &WebFullScreenManagerProxy::supportsFullScreen);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(FULLSCREEN_API)
