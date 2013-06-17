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

#ifndef WebDatabaseManagerMessages_h
#define WebDatabaseManagerMessages_h

#if ENABLE(SQL_DATABASE)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WTF {
    class String;
}

namespace Messages {
namespace WebDatabaseManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebDatabaseManager");
}

enum Kind {
    GetDatabasesByOriginID,
    GetDatabaseOriginsID,
    DeleteDatabaseWithNameForOriginID,
    DeleteDatabasesForOriginID,
    DeleteAllDatabasesID,
    SetQuotaForOriginID,
};

struct GetDatabasesByOrigin : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetDatabasesByOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetDatabasesByOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetDatabasesByOrigin(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct GetDatabaseOrigins : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetDatabaseOriginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetDatabaseOrigins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetDatabaseOrigins(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct DeleteDatabaseWithNameForOrigin : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = DeleteDatabaseWithNameForOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteDatabaseWithNameForOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    DeleteDatabaseWithNameForOrigin(const WTF::String& databaseIdentifier, const WTF::String& originIdentifier)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(databaseIdentifier, originIdentifier)
    {
    }
};

struct DeleteDatabasesForOrigin : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DeleteDatabasesForOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteDatabasesForOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DeleteDatabasesForOrigin(const WTF::String& originIdentifier)
        : CoreIPC::Arguments1<const WTF::String&>(originIdentifier)
    {
    }
};

struct DeleteAllDatabases : CoreIPC::Arguments0 {
    static const Kind messageID = DeleteAllDatabasesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteAllDatabases"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetQuotaForOrigin : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = SetQuotaForOriginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetQuotaForOrigin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    SetQuotaForOrigin(const WTF::String& originIdentifier, uint64_t quota)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(originIdentifier, quota)
    {
    }
};

} // namespace WebDatabaseManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebDatabaseManager::Kind> {
    static const MessageClass messageClass = MessageClassWebDatabaseManager;
};

} // namespace CoreIPC

#endif // ENABLE(SQL_DATABASE)

#endif // WebDatabaseManagerMessages_h
