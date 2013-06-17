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

#ifndef WebProcessConnectionMessages_h
#define WebProcessConnectionMessages_h

#if ENABLE(PLUGIN_PROCESS)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"
#include <wtf/ThreadSafeRefCounted.h>

namespace CoreIPC {
    class Connection;
}

namespace WebKit {
    struct PluginCreationParameters;
}

namespace Messages {
namespace WebProcessConnection {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebProcessConnection");
}

enum Kind {
    CreatePluginID,
    CreatePluginAsynchronouslyID,
    DestroyPluginID,
};

struct CreatePlugin : CoreIPC::Arguments1<const WebKit::PluginCreationParameters&> {
    static const Kind messageID = CreatePluginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreatePlugin"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<CoreIPC::Connection>, PassOwnPtr<CoreIPC::MessageEncoder>);
        ~DelayedReply();

        bool send(bool creationResult, bool wantsWheelEvents, uint32_t remoteLayerClientID);

    private:
        RefPtr<CoreIPC::Connection> m_connection;
        OwnPtr<CoreIPC::MessageEncoder> m_encoder;
    };

    typedef CoreIPC::Arguments3<bool&, bool&, uint32_t&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::PluginCreationParameters&> DecodeType;
    explicit CreatePlugin(const WebKit::PluginCreationParameters& pluginCreationParameters)
        : CoreIPC::Arguments1<const WebKit::PluginCreationParameters&>(pluginCreationParameters)
    {
    }
};

struct CreatePluginAsynchronously : CoreIPC::Arguments1<const WebKit::PluginCreationParameters&> {
    static const Kind messageID = CreatePluginAsynchronouslyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreatePluginAsynchronously"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::PluginCreationParameters&> DecodeType;
    explicit CreatePluginAsynchronously(const WebKit::PluginCreationParameters& pluginCreationParameters)
        : CoreIPC::Arguments1<const WebKit::PluginCreationParameters&>(pluginCreationParameters)
    {
    }
};

struct DestroyPlugin : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = DestroyPluginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DestroyPlugin"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments0 Reply;
    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    DestroyPlugin(uint64_t pluginInstanceID, bool asynchronousCreationIncomplete)
        : CoreIPC::Arguments2<uint64_t, bool>(pluginInstanceID, asynchronousCreationIncomplete)
    {
    }
};

} // namespace WebProcessConnection
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebProcessConnection::Kind> {
    static const MessageClass messageClass = MessageClassWebProcessConnection;
};

} // namespace CoreIPC

#endif // ENABLE(PLUGIN_PROCESS)

#endif // WebProcessConnectionMessages_h
