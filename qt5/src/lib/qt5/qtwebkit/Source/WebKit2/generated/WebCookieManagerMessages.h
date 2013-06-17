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

#ifndef WebCookieManagerMessages_h
#define WebCookieManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WTF {
    class String;
}

namespace Messages {
namespace WebCookieManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebCookieManager");
}

enum Kind {
    GetHostnamesWithCookiesID,
    DeleteCookiesForHostnameID,
    DeleteAllCookiesID,
    SetHTTPCookieAcceptPolicyID,
    GetHTTPCookieAcceptPolicyID,
    StartObservingCookieChangesID,
    StopObservingCookieChangesID,
#if USE(SOUP)
    SetCookiePersistentStorageID,
#endif
};

struct GetHostnamesWithCookies : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetHostnamesWithCookiesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetHostnamesWithCookies"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetHostnamesWithCookies(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct DeleteCookiesForHostname : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DeleteCookiesForHostnameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteCookiesForHostname"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DeleteCookiesForHostname(const WTF::String& hostname)
        : CoreIPC::Arguments1<const WTF::String&>(hostname)
    {
    }
};

struct DeleteAllCookies : CoreIPC::Arguments0 {
    static const Kind messageID = DeleteAllCookiesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteAllCookies"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetHTTPCookieAcceptPolicy : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = SetHTTPCookieAcceptPolicyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetHTTPCookieAcceptPolicy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit SetHTTPCookieAcceptPolicy(uint32_t policy)
        : CoreIPC::Arguments1<uint32_t>(policy)
    {
    }
};

struct GetHTTPCookieAcceptPolicy : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetHTTPCookieAcceptPolicyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetHTTPCookieAcceptPolicy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetHTTPCookieAcceptPolicy(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct StartObservingCookieChanges : CoreIPC::Arguments0 {
    static const Kind messageID = StartObservingCookieChangesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartObservingCookieChanges"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StopObservingCookieChanges : CoreIPC::Arguments0 {
    static const Kind messageID = StopObservingCookieChangesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopObservingCookieChanges"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if USE(SOUP)
struct SetCookiePersistentStorage : CoreIPC::Arguments2<const WTF::String&, uint32_t> {
    static const Kind messageID = SetCookiePersistentStorageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCookiePersistentStorage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint32_t> DecodeType;
    SetCookiePersistentStorage(const WTF::String& storagePath, uint32_t storageType)
        : CoreIPC::Arguments2<const WTF::String&, uint32_t>(storagePath, storageType)
    {
    }
};
#endif

} // namespace WebCookieManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebCookieManager::Kind> {
    static const MessageClass messageClass = MessageClassWebCookieManager;
};

} // namespace CoreIPC

#endif // WebCookieManagerMessages_h
