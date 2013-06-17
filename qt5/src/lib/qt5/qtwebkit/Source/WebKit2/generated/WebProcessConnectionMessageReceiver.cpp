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

#if ENABLE(PLUGIN_PROCESS)

#include "WebProcessConnection.h"

#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "PluginCreationParameters.h"
#include "WebProcessConnectionMessages.h"

namespace Messages {

namespace WebProcessConnection {

CreatePlugin::DelayedReply::DelayedReply(PassRefPtr<CoreIPC::Connection> connection, PassOwnPtr<CoreIPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(encoder)
{
}

CreatePlugin::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool CreatePlugin::DelayedReply::send(bool creationResult, bool wantsWheelEvents, uint32_t remoteLayerClientID)
{
    ASSERT(m_encoder);
    m_encoder->encode(creationResult);
    m_encoder->encode(wantsWheelEvents);
    m_encoder->encode(remoteLayerClientID);
    bool result = m_connection->sendSyncReply(m_encoder.release());
    m_connection = nullptr;
    return result;
}

} // namespace WebProcessConnection

} // namespace Messages

namespace WebKit {

void WebProcessConnection::didReceiveWebProcessConnectionMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebProcessConnection::CreatePluginAsynchronously::name()) {
        CoreIPC::handleMessage<Messages::WebProcessConnection::CreatePluginAsynchronously>(decoder, this, &WebProcessConnection::createPluginAsynchronously);
        return;
    }
    ASSERT_NOT_REACHED();
}

void WebProcessConnection::didReceiveSyncWebProcessConnectionMessage(CoreIPC::Connection* connection, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebProcessConnection::CreatePlugin::name()) {
        CoreIPC::handleMessageDelayed<Messages::WebProcessConnection::CreatePlugin>(connection, decoder, replyEncoder, this, &WebProcessConnection::createPlugin);
        return;
    }
    if (decoder.messageName() == Messages::WebProcessConnection::DestroyPlugin::name()) {
        CoreIPC::handleMessage<Messages::WebProcessConnection::DestroyPlugin>(decoder, *replyEncoder, this, &WebProcessConnection::destroyPlugin);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(PLUGIN_PROCESS)
