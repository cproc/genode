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

#include "EventDispatcher.h"

#include "EventDispatcherMessages.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebEvent.h"

namespace WebKit {

void EventDispatcher::didReceiveEventDispatcherMessageOnConnectionWorkQueue(CoreIPC::Connection* connection, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, bool& didHandleMessage)
{
#if COMPILER(MSVC)
#pragma warning(push)
#pragma warning(disable: 4065)
#endif
    if (decoder.messageName() == Messages::EventDispatcher::WheelEvent::name()) {
        CoreIPC::handleMessageOnConnectionQueue<Messages::EventDispatcher::WheelEvent>(connection, decoder, this, &EventDispatcher::wheelEvent);
        didHandleMessage = true;
        return;
    }
#if ENABLE(GESTURE_EVENTS)
    if (decoder.messageName() == Messages::EventDispatcher::GestureEvent::name()) {
        CoreIPC::handleMessageOnConnectionQueue<Messages::EventDispatcher::GestureEvent>(connection, decoder, this, &EventDispatcher::gestureEvent);
        didHandleMessage = true;
        return;
    }
#endif
#if COMPILER(MSVC)
#pragma warning(pop)
#endif
}


} // namespace WebKit
