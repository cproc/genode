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

#ifndef WebMediaCacheManagerMessages_h
#define WebMediaCacheManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WTF {
    class String;
}

namespace Messages {
namespace WebMediaCacheManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebMediaCacheManager");
}

enum Kind {
    GetHostnamesWithMediaCacheID,
    ClearCacheForHostnameID,
    ClearCacheForAllHostnamesID,
};

struct GetHostnamesWithMediaCache : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetHostnamesWithMediaCacheID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetHostnamesWithMediaCache"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetHostnamesWithMediaCache(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct ClearCacheForHostname : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ClearCacheForHostnameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearCacheForHostname"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ClearCacheForHostname(const WTF::String& hostname)
        : CoreIPC::Arguments1<const WTF::String&>(hostname)
    {
    }
};

struct ClearCacheForAllHostnames : CoreIPC::Arguments0 {
    static const Kind messageID = ClearCacheForAllHostnamesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearCacheForAllHostnames"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

} // namespace WebMediaCacheManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebMediaCacheManager::Kind> {
    static const MessageClass messageClass = MessageClassWebMediaCacheManager;
};

} // namespace CoreIPC

#endif // WebMediaCacheManagerMessages_h
