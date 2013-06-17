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

#include "WebContext.h"

#if PLATFORM(MAC)
#include "ArgumentCoders.h"
#endif
#include "HandleMessage.h"
#include "MessageDecoder.h"
#if PLATFORM(MAC)
#include "SharedMemory.h"
#endif
#include "StatisticsData.h"
#include "WebContextMessages.h"
#if PLATFORM(MAC)
#include "WebCoreArgumentCoders.h"
#endif
#if PLATFORM(MAC)
#include <WebCore/Color.h>
#endif
#if PLATFORM(MAC)
#include <wtf/Vector.h>
#endif
#if PLATFORM(MAC)
#include <wtf/text/WTFString.h>
#endif

namespace WebKit {

void WebContext::didReceiveWebContextMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebContext::AddVisitedLinkHash::name()) {
        CoreIPC::handleMessage<Messages::WebContext::AddVisitedLinkHash>(decoder, this, &WebContext::addVisitedLinkHash);
        return;
    }
    if (decoder.messageName() == Messages::WebContext::DidGetWebCoreStatistics::name()) {
        CoreIPC::handleMessage<Messages::WebContext::DidGetWebCoreStatistics>(decoder, this, &WebContext::didGetWebCoreStatistics);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::PasteboardCopy::name()) {
        CoreIPC::handleMessage<Messages::WebContext::PasteboardCopy>(decoder, this, &WebContext::pasteboardCopy);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::AddPasteboardTypes::name()) {
        CoreIPC::handleMessage<Messages::WebContext::AddPasteboardTypes>(decoder, this, &WebContext::addPasteboardTypes);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::SetPasteboardTypes::name()) {
        CoreIPC::handleMessage<Messages::WebContext::SetPasteboardTypes>(decoder, this, &WebContext::setPasteboardTypes);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::SetPasteboardPathnamesForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::SetPasteboardPathnamesForType>(decoder, this, &WebContext::setPasteboardPathnamesForType);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::SetPasteboardStringForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::SetPasteboardStringForType>(decoder, this, &WebContext::setPasteboardStringForType);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::SetPasteboardBufferForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::SetPasteboardBufferForType>(decoder, this, &WebContext::setPasteboardBufferForType);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

void WebContext::didReceiveSyncWebContextMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardTypes::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardTypes>(decoder, *replyEncoder, this, &WebContext::getPasteboardTypes);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardPathnamesForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardPathnamesForType>(decoder, *replyEncoder, this, &WebContext::getPasteboardPathnamesForType);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardStringForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardStringForType>(decoder, *replyEncoder, this, &WebContext::getPasteboardStringForType);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardBufferForType::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardBufferForType>(decoder, *replyEncoder, this, &WebContext::getPasteboardBufferForType);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardChangeCount::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardChangeCount>(decoder, *replyEncoder, this, &WebContext::getPasteboardChangeCount);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardUniqueName::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardUniqueName>(decoder, *replyEncoder, this, &WebContext::getPasteboardUniqueName);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardColor::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardColor>(decoder, *replyEncoder, this, &WebContext::getPasteboardColor);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::GetPasteboardURL::name()) {
        CoreIPC::handleMessage<Messages::WebContext::GetPasteboardURL>(decoder, *replyEncoder, this, &WebContext::getPasteboardURL);
        return;
    }
#endif
#if !PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebContext::Dummy::name()) {
        CoreIPC::handleMessage<Messages::WebContext::Dummy>(decoder, *replyEncoder, this, &WebContext::dummy);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
