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

#ifndef WebResourceCacheManagerMessages_h
#define WebResourceCacheManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebKit {
    struct SecurityOriginData;
}

namespace Messages {
namespace WebResourceCacheManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebResourceCacheManager");
}

enum Kind {
    GetCacheOriginsID,
    ClearCacheForOriginID,
    ClearCacheForAllOriginsID,
};

struct GetCacheOrigins : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetCacheOriginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetCacheOrigins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetCacheOrigins(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct ClearCacheForOrigin : CoreIPC::Arguments2<const WebKit::SecurityOriginData&, uint32_t> {
    static const Kind messageID = ClearCacheForOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearCacheForOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebKit::SecurityOriginData&, uint32_t> DecodeType;
    ClearCacheForOrigin(const WebKit::SecurityOriginData& originIdentifier, uint32_t cachesToClear)
        : CoreIPC::Arguments2<const WebKit::SecurityOriginData&, uint32_t>(originIdentifier, cachesToClear)
    {
    }
};

struct ClearCacheForAllOrigins : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = ClearCacheForAllOriginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearCacheForAllOrigins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit ClearCacheForAllOrigins(uint32_t cachesToClear)
        : CoreIPC::Arguments1<uint32_t>(cachesToClear)
    {
    }
};

} // namespace WebResourceCacheManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebResourceCacheManager::Kind> {
    static const MessageClass messageClass = MessageClassWebResourceCacheManager;
};

} // namespace CoreIPC

#endif // WebResourceCacheManagerMessages_h
