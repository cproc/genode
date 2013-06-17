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

#include "WebPageGroupProxy.h"

#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebCoreArgumentCoders.h"
#include "WebPageGroupProxyMessages.h"
#include <WebCore/UserScript.h>
#include <WebCore/UserStyleSheet.h>

namespace WebKit {

void WebPageGroupProxy::didReceiveWebPageGroupProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebPageGroupProxy::AddUserStyleSheet::name()) {
        CoreIPC::handleMessage<Messages::WebPageGroupProxy::AddUserStyleSheet>(decoder, this, &WebPageGroupProxy::addUserStyleSheet);
        return;
    }
    if (decoder.messageName() == Messages::WebPageGroupProxy::AddUserScript::name()) {
        CoreIPC::handleMessage<Messages::WebPageGroupProxy::AddUserScript>(decoder, this, &WebPageGroupProxy::addUserScript);
        return;
    }
    if (decoder.messageName() == Messages::WebPageGroupProxy::RemoveAllUserStyleSheets::name()) {
        CoreIPC::handleMessage<Messages::WebPageGroupProxy::RemoveAllUserStyleSheets>(decoder, this, &WebPageGroupProxy::removeAllUserStyleSheets);
        return;
    }
    if (decoder.messageName() == Messages::WebPageGroupProxy::RemoveAllUserScripts::name()) {
        CoreIPC::handleMessage<Messages::WebPageGroupProxy::RemoveAllUserScripts>(decoder, this, &WebPageGroupProxy::removeAllUserScripts);
        return;
    }
    if (decoder.messageName() == Messages::WebPageGroupProxy::RemoveAllUserContent::name()) {
        CoreIPC::handleMessage<Messages::WebPageGroupProxy::RemoveAllUserContent>(decoder, this, &WebPageGroupProxy::removeAllUserContent);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
