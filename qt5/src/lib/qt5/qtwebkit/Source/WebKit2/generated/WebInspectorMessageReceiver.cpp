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

#if ENABLE(INSPECTOR)

#include "WebInspector.h"

#if ENABLE(INSPECTOR_SERVER)
#include "ArgumentCoders.h"
#endif
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebInspectorMessages.h"
#if ENABLE(INSPECTOR_SERVER)
#include <wtf/text/WTFString.h>
#endif

namespace WebKit {

void WebInspector::didReceiveWebInspectorMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebInspector::Show::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::Show>(decoder, this, &WebInspector::show);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::Close::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::Close>(decoder, this, &WebInspector::close);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::SetAttachedWindow::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::SetAttachedWindow>(decoder, this, &WebInspector::setAttachedWindow);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::ShowConsole::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::ShowConsole>(decoder, this, &WebInspector::showConsole);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::ShowResources::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::ShowResources>(decoder, this, &WebInspector::showResources);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::ShowMainResourceForFrame::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::ShowMainResourceForFrame>(decoder, this, &WebInspector::showMainResourceForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StartJavaScriptDebugging::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StartJavaScriptDebugging>(decoder, this, &WebInspector::startJavaScriptDebugging);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StopJavaScriptDebugging::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StopJavaScriptDebugging>(decoder, this, &WebInspector::stopJavaScriptDebugging);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StartJavaScriptProfiling::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StartJavaScriptProfiling>(decoder, this, &WebInspector::startJavaScriptProfiling);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StopJavaScriptProfiling::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StopJavaScriptProfiling>(decoder, this, &WebInspector::stopJavaScriptProfiling);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StartPageProfiling::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StartPageProfiling>(decoder, this, &WebInspector::startPageProfiling);
        return;
    }
    if (decoder.messageName() == Messages::WebInspector::StopPageProfiling::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::StopPageProfiling>(decoder, this, &WebInspector::stopPageProfiling);
        return;
    }
#if ENABLE(INSPECTOR_SERVER)
    if (decoder.messageName() == Messages::WebInspector::DispatchMessageFromRemoteFrontend::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::DispatchMessageFromRemoteFrontend>(decoder, this, &WebInspector::dispatchMessageFromRemoteFrontend);
        return;
    }
#endif
#if ENABLE(INSPECTOR_SERVER)
    if (decoder.messageName() == Messages::WebInspector::RemoteFrontendConnected::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::RemoteFrontendConnected>(decoder, this, &WebInspector::remoteFrontendConnected);
        return;
    }
#endif
#if ENABLE(INSPECTOR_SERVER)
    if (decoder.messageName() == Messages::WebInspector::RemoteFrontendDisconnected::name()) {
        CoreIPC::handleMessage<Messages::WebInspector::RemoteFrontendDisconnected>(decoder, this, &WebInspector::remoteFrontendDisconnected);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(INSPECTOR)
