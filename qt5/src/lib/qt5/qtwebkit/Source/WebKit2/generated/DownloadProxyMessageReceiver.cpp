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

#include "DownloadProxy.h"

#include "ArgumentCoders.h"
#include "DataReference.h"
#include "DownloadProxyMessages.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "SandboxExtension.h"
#include "WebCoreArgumentCoders.h"
#include <WebCore/AuthenticationChallenge.h>
#include <WebCore/ResourceError.h>
#include <WebCore/ResourceRequest.h>
#include <WebCore/ResourceResponse.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

void DownloadProxy::didReceiveDownloadProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::DownloadProxy::DidStart::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidStart>(decoder, this, &DownloadProxy::didStart);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidReceiveAuthenticationChallenge::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidReceiveAuthenticationChallenge>(decoder, this, &DownloadProxy::didReceiveAuthenticationChallenge);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidReceiveResponse::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidReceiveResponse>(decoder, this, &DownloadProxy::didReceiveResponse);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidReceiveData::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidReceiveData>(decoder, this, &DownloadProxy::didReceiveData);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidCreateDestination::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidCreateDestination>(decoder, this, &DownloadProxy::didCreateDestination);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidFinish::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidFinish>(decoder, this, &DownloadProxy::didFinish);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidFail::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidFail>(decoder, this, &DownloadProxy::didFail);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DidCancel::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DidCancel>(decoder, this, &DownloadProxy::didCancel);
        return;
    }
    ASSERT_NOT_REACHED();
}

void DownloadProxy::didReceiveSyncDownloadProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::DownloadProxy::ShouldDecodeSourceDataOfMIMEType::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::ShouldDecodeSourceDataOfMIMEType>(decoder, *replyEncoder, this, &DownloadProxy::shouldDecodeSourceDataOfMIMEType);
        return;
    }
    if (decoder.messageName() == Messages::DownloadProxy::DecideDestinationWithSuggestedFilename::name()) {
        CoreIPC::handleMessage<Messages::DownloadProxy::DecideDestinationWithSuggestedFilename>(decoder, *replyEncoder, this, &DownloadProxy::decideDestinationWithSuggestedFilename);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
