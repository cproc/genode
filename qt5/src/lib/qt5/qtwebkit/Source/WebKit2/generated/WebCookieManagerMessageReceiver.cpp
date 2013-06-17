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

#include "WebCookieManager.h"

#include "ArgumentCoders.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebCookieManagerMessages.h"
#include <wtf/text/WTFString.h>

namespace WebKit {

void WebCookieManager::didReceiveWebCookieManagerMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebCookieManager::GetHostnamesWithCookies::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::GetHostnamesWithCookies>(decoder, this, &WebCookieManager::getHostnamesWithCookies);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::DeleteCookiesForHostname::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::DeleteCookiesForHostname>(decoder, this, &WebCookieManager::deleteCookiesForHostname);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::DeleteAllCookies::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::DeleteAllCookies>(decoder, this, &WebCookieManager::deleteAllCookies);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::SetHTTPCookieAcceptPolicy::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::SetHTTPCookieAcceptPolicy>(decoder, this, &WebCookieManager::setHTTPCookieAcceptPolicy);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::GetHTTPCookieAcceptPolicy::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::GetHTTPCookieAcceptPolicy>(decoder, this, &WebCookieManager::getHTTPCookieAcceptPolicy);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::StartObservingCookieChanges::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::StartObservingCookieChanges>(decoder, this, &WebCookieManager::startObservingCookieChanges);
        return;
    }
    if (decoder.messageName() == Messages::WebCookieManager::StopObservingCookieChanges::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::StopObservingCookieChanges>(decoder, this, &WebCookieManager::stopObservingCookieChanges);
        return;
    }
#if USE(SOUP)
    if (decoder.messageName() == Messages::WebCookieManager::SetCookiePersistentStorage::name()) {
        CoreIPC::handleMessage<Messages::WebCookieManager::SetCookiePersistentStorage>(decoder, this, &WebCookieManager::setCookiePersistentStorage);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
