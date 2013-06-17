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

#if ENABLE(NETWORK_INFO)

#include "WebNetworkInfoManagerProxy.h"

#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebNetworkInfoManagerProxyMessages.h"

namespace WebKit {

void WebNetworkInfoManagerProxy::didReceiveWebNetworkInfoManagerProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebNetworkInfoManagerProxy::StartUpdating::name()) {
        CoreIPC::handleMessage<Messages::WebNetworkInfoManagerProxy::StartUpdating>(decoder, this, &WebNetworkInfoManagerProxy::startUpdating);
        return;
    }
    if (decoder.messageName() == Messages::WebNetworkInfoManagerProxy::StopUpdating::name()) {
        CoreIPC::handleMessage<Messages::WebNetworkInfoManagerProxy::StopUpdating>(decoder, this, &WebNetworkInfoManagerProxy::stopUpdating);
        return;
    }
    ASSERT_NOT_REACHED();
}

void WebNetworkInfoManagerProxy::didReceiveSyncWebNetworkInfoManagerProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebNetworkInfoManagerProxy::GetBandwidth::name()) {
        CoreIPC::handleMessage<Messages::WebNetworkInfoManagerProxy::GetBandwidth>(decoder, *replyEncoder, this, &WebNetworkInfoManagerProxy::getBandwidth);
        return;
    }
    if (decoder.messageName() == Messages::WebNetworkInfoManagerProxy::IsMetered::name()) {
        CoreIPC::handleMessage<Messages::WebNetworkInfoManagerProxy::IsMetered>(decoder, *replyEncoder, this, &WebNetworkInfoManagerProxy::isMetered);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(NETWORK_INFO)
