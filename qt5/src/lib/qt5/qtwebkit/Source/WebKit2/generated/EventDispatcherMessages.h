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

#ifndef EventDispatcherMessages_h
#define EventDispatcherMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebKit {
    class WebWheelEvent;
    class WebGestureEvent;
}

namespace Messages {
namespace EventDispatcher {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("EventDispatcher");
}

enum Kind {
    WheelEventID,
#if ENABLE(GESTURE_EVENTS)
    GestureEventID,
#endif
};

struct WheelEvent : CoreIPC::Arguments4<uint64_t, const WebKit::WebWheelEvent&, bool, bool> {
    static const Kind messageID = WheelEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WheelEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t, const WebKit::WebWheelEvent&, bool, bool> DecodeType;
    WheelEvent(uint64_t pageID, const WebKit::WebWheelEvent& event, bool canGoBack, bool canGoForward)
        : CoreIPC::Arguments4<uint64_t, const WebKit::WebWheelEvent&, bool, bool>(pageID, event, canGoBack, canGoForward)
    {
    }
};

#if ENABLE(GESTURE_EVENTS)
struct GestureEvent : CoreIPC::Arguments2<uint64_t, const WebKit::WebGestureEvent&> {
    static const Kind messageID = GestureEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GestureEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::WebGestureEvent&> DecodeType;
    GestureEvent(uint64_t pageID, const WebKit::WebGestureEvent& event)
        : CoreIPC::Arguments2<uint64_t, const WebKit::WebGestureEvent&>(pageID, event)
    {
    }
};
#endif

} // namespace EventDispatcher
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::EventDispatcher::Kind> {
    static const MessageClass messageClass = MessageClassEventDispatcher;
};

} // namespace CoreIPC

#endif // EventDispatcherMessages_h
