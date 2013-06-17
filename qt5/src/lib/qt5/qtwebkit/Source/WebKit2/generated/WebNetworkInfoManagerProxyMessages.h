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

#ifndef WebNetworkInfoManagerProxyMessages_h
#define WebNetworkInfoManagerProxyMessages_h

#if ENABLE(NETWORK_INFO)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"


namespace Messages {
namespace WebNetworkInfoManagerProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebNetworkInfoManagerProxy");
}

enum Kind {
    StartUpdatingID,
    StopUpdatingID,
    GetBandwidthID,
    IsMeteredID,
};

struct StartUpdating : CoreIPC::Arguments0 {
    static const Kind messageID = StartUpdatingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartUpdating"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StopUpdating : CoreIPC::Arguments0 {
    static const Kind messageID = StopUpdatingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopUpdating"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct GetBandwidth : CoreIPC::Arguments0 {
    static const Kind messageID = GetBandwidthID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetBandwidth"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<double&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct IsMetered : CoreIPC::Arguments0 {
    static const Kind messageID = IsMeteredID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("IsMetered"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

} // namespace WebNetworkInfoManagerProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebNetworkInfoManagerProxy::Kind> {
    static const MessageClass messageClass = MessageClassWebNetworkInfoManagerProxy;
};

} // namespace CoreIPC

#endif // ENABLE(NETWORK_INFO)

#endif // WebNetworkInfoManagerProxyMessages_h
