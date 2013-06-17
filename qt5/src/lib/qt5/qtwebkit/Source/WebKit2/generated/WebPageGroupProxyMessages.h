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

#ifndef WebPageGroupProxyMessages_h
#define WebPageGroupProxyMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebCore {
    class UserStyleSheet;
    class UserScript;
}

namespace Messages {
namespace WebPageGroupProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebPageGroupProxy");
}

enum Kind {
    AddUserStyleSheetID,
    AddUserScriptID,
    RemoveAllUserStyleSheetsID,
    RemoveAllUserScriptsID,
    RemoveAllUserContentID,
};

struct AddUserStyleSheet : CoreIPC::Arguments1<const WebCore::UserStyleSheet&> {
    static const Kind messageID = AddUserStyleSheetID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AddUserStyleSheet"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::UserStyleSheet&> DecodeType;
    explicit AddUserStyleSheet(const WebCore::UserStyleSheet& userStyleSheet)
        : CoreIPC::Arguments1<const WebCore::UserStyleSheet&>(userStyleSheet)
    {
    }
};

struct AddUserScript : CoreIPC::Arguments1<const WebCore::UserScript&> {
    static const Kind messageID = AddUserScriptID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AddUserScript"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::UserScript&> DecodeType;
    explicit AddUserScript(const WebCore::UserScript& userScript)
        : CoreIPC::Arguments1<const WebCore::UserScript&>(userScript)
    {
    }
};

struct RemoveAllUserStyleSheets : CoreIPC::Arguments0 {
    static const Kind messageID = RemoveAllUserStyleSheetsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveAllUserStyleSheets"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct RemoveAllUserScripts : CoreIPC::Arguments0 {
    static const Kind messageID = RemoveAllUserScriptsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveAllUserScripts"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct RemoveAllUserContent : CoreIPC::Arguments0 {
    static const Kind messageID = RemoveAllUserContentID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveAllUserContent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

} // namespace WebPageGroupProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebPageGroupProxy::Kind> {
    static const MessageClass messageClass = MessageClassWebPageGroupProxy;
};

} // namespace CoreIPC

#endif // WebPageGroupProxyMessages_h
