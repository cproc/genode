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

#ifndef WebDatabaseManagerProxyMessages_h
#define WebDatabaseManagerProxyMessages_h

#if ENABLE(SQL_DATABASE)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "OriginAndDatabases.h"
#include "StringReference.h"
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WTF {
    class String;
}

namespace Messages {
namespace WebDatabaseManagerProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebDatabaseManagerProxy");
}

enum Kind {
    DidGetDatabasesByOriginID,
    DidGetDatabaseOriginsID,
    DidModifyOriginID,
    DidModifyDatabaseID,
};

struct DidGetDatabasesByOrigin : CoreIPC::Arguments2<const Vector<WebKit::OriginAndDatabases>&, uint64_t> {
    static const Kind messageID = DidGetDatabasesByOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidGetDatabasesByOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const Vector<WebKit::OriginAndDatabases>&, uint64_t> DecodeType;
    DidGetDatabasesByOrigin(const Vector<WebKit::OriginAndDatabases>& originAndDatabases, uint64_t callbackID)
        : CoreIPC::Arguments2<const Vector<WebKit::OriginAndDatabases>&, uint64_t>(originAndDatabases, callbackID)
    {
    }
};

struct DidGetDatabaseOrigins : CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t> {
    static const Kind messageID = DidGetDatabaseOriginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidGetDatabaseOrigins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t> DecodeType;
    DidGetDatabaseOrigins(const Vector<WTF::String>& originIdentifiers, uint64_t callbackID)
        : CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t>(originIdentifiers, callbackID)
    {
    }
};

struct DidModifyOrigin : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DidModifyOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidModifyOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DidModifyOrigin(const WTF::String& originIdentifier)
        : CoreIPC::Arguments1<const WTF::String&>(originIdentifier)
    {
    }
};

struct DidModifyDatabase : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = DidModifyDatabaseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidModifyDatabase"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    DidModifyDatabase(const WTF::String& originIdentifier, const WTF::String& databaseIdentifier)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(originIdentifier, databaseIdentifier)
    {
    }
};

} // namespace WebDatabaseManagerProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebDatabaseManagerProxy::Kind> {
    static const MessageClass messageClass = MessageClassWebDatabaseManagerProxy;
};

} // namespace CoreIPC

#endif // ENABLE(SQL_DATABASE)

#endif // WebDatabaseManagerProxyMessages_h
