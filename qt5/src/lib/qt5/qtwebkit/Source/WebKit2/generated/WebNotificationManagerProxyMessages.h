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

#ifndef WebNotificationManagerProxyMessages_h
#define WebNotificationManagerProxyMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"
#include <wtf/Vector.h>


namespace Messages {
namespace WebNotificationManagerProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebNotificationManagerProxy");
}

enum Kind {
    CancelID,
    DidDestroyNotificationID,
    ClearNotificationsID,
};

struct Cancel : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = CancelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Cancel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit Cancel(uint64_t notificationID)
        : CoreIPC::Arguments1<uint64_t>(notificationID)
    {
    }
};

struct DidDestroyNotification : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidDestroyNotificationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidDestroyNotification"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidDestroyNotification(uint64_t notificationID)
        : CoreIPC::Arguments1<uint64_t>(notificationID)
    {
    }
};

struct ClearNotifications : CoreIPC::Arguments1<const Vector<uint64_t>&> {
    static const Kind messageID = ClearNotificationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearNotifications"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<uint64_t>&> DecodeType;
    explicit ClearNotifications(const Vector<uint64_t>& notificationIDs)
        : CoreIPC::Arguments1<const Vector<uint64_t>&>(notificationIDs)
    {
    }
};

} // namespace WebNotificationManagerProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebNotificationManagerProxy::Kind> {
    static const MessageClass messageClass = MessageClassWebNotificationManagerProxy;
};

} // namespace CoreIPC

#endif // WebNotificationManagerProxyMessages_h
