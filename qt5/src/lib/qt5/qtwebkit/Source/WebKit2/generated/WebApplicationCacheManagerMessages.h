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

#ifndef WebApplicationCacheManagerMessages_h
#define WebApplicationCacheManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebKit {
    struct SecurityOriginData;
}

namespace Messages {
namespace WebApplicationCacheManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebApplicationCacheManager");
}

enum Kind {
    GetApplicationCacheOriginsID,
    DeleteEntriesForOriginID,
    DeleteAllEntriesID,
};

struct GetApplicationCacheOrigins : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetApplicationCacheOriginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetApplicationCacheOrigins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetApplicationCacheOrigins(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct DeleteEntriesForOrigin : CoreIPC::Arguments1<const WebKit::SecurityOriginData&> {
    static const Kind messageID = DeleteEntriesForOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteEntriesForOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::SecurityOriginData&> DecodeType;
    explicit DeleteEntriesForOrigin(const WebKit::SecurityOriginData& originIdentifier)
        : CoreIPC::Arguments1<const WebKit::SecurityOriginData&>(originIdentifier)
    {
    }
};

struct DeleteAllEntries : CoreIPC::Arguments0 {
    static const Kind messageID = DeleteAllEntriesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteAllEntries"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

} // namespace WebApplicationCacheManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebApplicationCacheManager::Kind> {
    static const MessageClass messageClass = MessageClassWebApplicationCacheManager;
};

} // namespace CoreIPC

#endif // WebApplicationCacheManagerMessages_h
