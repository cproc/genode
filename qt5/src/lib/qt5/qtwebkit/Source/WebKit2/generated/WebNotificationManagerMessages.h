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

#ifndef WebNotificationManagerMessages_h
#define WebNotificationManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WTF {
    class String;
}

namespace Messages {
namespace WebNotificationManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebNotificationManager");
}

enum Kind {
    DidShowNotificationID,
    DidClickNotificationID,
    DidCloseNotificationsID,
    DidUpdateNotificationDecisionID,
    DidRemoveNotificationDecisionsID,
};

struct DidShowNotification : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidShowNotificationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidShowNotification"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidShowNotification(uint64_t notificationID)
        : CoreIPC::Arguments1<uint64_t>(notificationID)
    {
    }
};

struct DidClickNotification : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidClickNotificationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidClickNotification"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidClickNotification(uint64_t notificationID)
        : CoreIPC::Arguments1<uint64_t>(notificationID)
    {
    }
};

struct DidCloseNotifications : CoreIPC::Arguments1<const Vector<uint64_t>&> {
    static const Kind messageID = DidCloseNotificationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCloseNotifications"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<uint64_t>&> DecodeType;
    explicit DidCloseNotifications(const Vector<uint64_t>& notificationIDs)
        : CoreIPC::Arguments1<const Vector<uint64_t>&>(notificationIDs)
    {
    }
};

struct DidUpdateNotificationDecision : CoreIPC::Arguments2<const WTF::String&, bool> {
    static const Kind messageID = DidUpdateNotificationDecisionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidUpdateNotificationDecision"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, bool> DecodeType;
    DidUpdateNotificationDecision(const WTF::String& originString, bool allowed)
        : CoreIPC::Arguments2<const WTF::String&, bool>(originString, allowed)
    {
    }
};

struct DidRemoveNotificationDecisions : CoreIPC::Arguments1<const Vector<WTF::String>&> {
    static const Kind messageID = DidRemoveNotificationDecisionsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRemoveNotificationDecisions"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<WTF::String>&> DecodeType;
    explicit DidRemoveNotificationDecisions(const Vector<WTF::String>& originStrings)
        : CoreIPC::Arguments1<const Vector<WTF::String>&>(originStrings)
    {
    }
};

} // namespace WebNotificationManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebNotificationManager::Kind> {
    static const MessageClass messageClass = MessageClassWebNotificationManager;
};

} // namespace CoreIPC

#endif // WebNotificationManagerMessages_h
