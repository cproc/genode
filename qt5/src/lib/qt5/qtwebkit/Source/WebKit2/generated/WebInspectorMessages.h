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

#ifndef WebInspectorMessages_h
#define WebInspectorMessages_h

#if ENABLE(INSPECTOR)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WTF {
    class String;
}

namespace Messages {
namespace WebInspector {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebInspector");
}

enum Kind {
    ShowID,
    CloseID,
    SetAttachedWindowID,
    ShowConsoleID,
    ShowResourcesID,
    ShowMainResourceForFrameID,
    StartJavaScriptDebuggingID,
    StopJavaScriptDebuggingID,
    StartJavaScriptProfilingID,
    StopJavaScriptProfilingID,
    StartPageProfilingID,
    StopPageProfilingID,
#if ENABLE(INSPECTOR_SERVER)
    DispatchMessageFromRemoteFrontendID,
#endif
#if ENABLE(INSPECTOR_SERVER)
    RemoteFrontendConnectedID,
#endif
#if ENABLE(INSPECTOR_SERVER)
    RemoteFrontendDisconnectedID,
#endif
};

struct Show : CoreIPC::Arguments0 {
    static const Kind messageID = ShowID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Show"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct Close : CoreIPC::Arguments0 {
    static const Kind messageID = CloseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Close"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetAttachedWindow : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetAttachedWindowID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetAttachedWindow"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetAttachedWindow(bool attached)
        : CoreIPC::Arguments1<bool>(attached)
    {
    }
};

struct ShowConsole : CoreIPC::Arguments0 {
    static const Kind messageID = ShowConsoleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowConsole"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ShowResources : CoreIPC::Arguments0 {
    static const Kind messageID = ShowResourcesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowResources"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ShowMainResourceForFrame : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ShowMainResourceForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowMainResourceForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ShowMainResourceForFrame(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct StartJavaScriptDebugging : CoreIPC::Arguments0 {
    static const Kind messageID = StartJavaScriptDebuggingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartJavaScriptDebugging"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StopJavaScriptDebugging : CoreIPC::Arguments0 {
    static const Kind messageID = StopJavaScriptDebuggingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopJavaScriptDebugging"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StartJavaScriptProfiling : CoreIPC::Arguments0 {
    static const Kind messageID = StartJavaScriptProfilingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartJavaScriptProfiling"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StopJavaScriptProfiling : CoreIPC::Arguments0 {
    static const Kind messageID = StopJavaScriptProfilingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopJavaScriptProfiling"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StartPageProfiling : CoreIPC::Arguments0 {
    static const Kind messageID = StartPageProfilingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartPageProfiling"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct StopPageProfiling : CoreIPC::Arguments0 {
    static const Kind messageID = StopPageProfilingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopPageProfiling"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if ENABLE(INSPECTOR_SERVER)
struct DispatchMessageFromRemoteFrontend : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DispatchMessageFromRemoteFrontendID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DispatchMessageFromRemoteFrontend"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DispatchMessageFromRemoteFrontend(const WTF::String& message)
        : CoreIPC::Arguments1<const WTF::String&>(message)
    {
    }
};
#endif

#if ENABLE(INSPECTOR_SERVER)
struct RemoteFrontendConnected : CoreIPC::Arguments0 {
    static const Kind messageID = RemoteFrontendConnectedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoteFrontendConnected"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if ENABLE(INSPECTOR_SERVER)
struct RemoteFrontendDisconnected : CoreIPC::Arguments0 {
    static const Kind messageID = RemoteFrontendDisconnectedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoteFrontendDisconnected"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

} // namespace WebInspector
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebInspector::Kind> {
    static const MessageClass messageClass = MessageClassWebInspector;
};

} // namespace CoreIPC

#endif // ENABLE(INSPECTOR)

#endif // WebInspectorMessages_h
