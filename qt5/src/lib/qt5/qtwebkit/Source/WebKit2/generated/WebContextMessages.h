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

#ifndef WebContextMessages_h
#define WebContextMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "SharedMemory.h"
#include "StringReference.h"
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WTF {
    class String;
}

namespace WebCore {
    class Color;
}

namespace WebKit {
    struct StatisticsData;
}

namespace Messages {
namespace WebContext {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebContext");
}

enum Kind {
    AddVisitedLinkHashID,
    DidGetWebCoreStatisticsID,
#if PLATFORM(MAC)
    GetPasteboardTypesID,
#endif
#if PLATFORM(MAC)
    GetPasteboardPathnamesForTypeID,
#endif
#if PLATFORM(MAC)
    GetPasteboardStringForTypeID,
#endif
#if PLATFORM(MAC)
    GetPasteboardBufferForTypeID,
#endif
#if PLATFORM(MAC)
    PasteboardCopyID,
#endif
#if PLATFORM(MAC)
    GetPasteboardChangeCountID,
#endif
#if PLATFORM(MAC)
    GetPasteboardUniqueNameID,
#endif
#if PLATFORM(MAC)
    GetPasteboardColorID,
#endif
#if PLATFORM(MAC)
    GetPasteboardURLID,
#endif
#if PLATFORM(MAC)
    AddPasteboardTypesID,
#endif
#if PLATFORM(MAC)
    SetPasteboardTypesID,
#endif
#if PLATFORM(MAC)
    SetPasteboardPathnamesForTypeID,
#endif
#if PLATFORM(MAC)
    SetPasteboardStringForTypeID,
#endif
#if PLATFORM(MAC)
    SetPasteboardBufferForTypeID,
#endif
#if !PLATFORM(MAC)
    DummyID,
#endif
};

struct AddVisitedLinkHash : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = AddVisitedLinkHashID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AddVisitedLinkHash"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit AddVisitedLinkHash(uint64_t linkHash)
        : CoreIPC::Arguments1<uint64_t>(linkHash)
    {
    }
};

struct DidGetWebCoreStatistics : CoreIPC::Arguments2<const WebKit::StatisticsData&, uint64_t> {
    static const Kind messageID = DidGetWebCoreStatisticsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidGetWebCoreStatistics"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebKit::StatisticsData&, uint64_t> DecodeType;
    DidGetWebCoreStatistics(const WebKit::StatisticsData& statisticsData, uint64_t callbackID)
        : CoreIPC::Arguments2<const WebKit::StatisticsData&, uint64_t>(statisticsData, callbackID)
    {
    }
};

#if PLATFORM(MAC)
struct GetPasteboardTypes : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = GetPasteboardTypesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardTypes"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<WTF::String>&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit GetPasteboardTypes(const WTF::String& pasteboardName)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardName)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardPathnamesForType : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = GetPasteboardPathnamesForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardPathnamesForType"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<WTF::String>&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    GetPasteboardPathnamesForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(pasteboardName, pasteboardType)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardStringForType : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = GetPasteboardStringForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardStringForType"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    GetPasteboardStringForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(pasteboardName, pasteboardType)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardBufferForType : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = GetPasteboardBufferForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardBufferForType"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<WebKit::SharedMemory::Handle&, uint64_t&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    GetPasteboardBufferForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(pasteboardName, pasteboardType)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct PasteboardCopy : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = PasteboardCopyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PasteboardCopy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    PasteboardCopy(const WTF::String& fromPasteboard, const WTF::String& toPasteboard)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(fromPasteboard, toPasteboard)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardChangeCount : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = GetPasteboardChangeCountID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardChangeCount"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit GetPasteboardChangeCount(const WTF::String& pasteboardName)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardName)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardUniqueName : CoreIPC::Arguments0 {
    static const Kind messageID = GetPasteboardUniqueNameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardUniqueName"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardColor : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = GetPasteboardColorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardColor"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::Color&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit GetPasteboardColor(const WTF::String& pasteboardName)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardName)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetPasteboardURL : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = GetPasteboardURLID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPasteboardURL"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit GetPasteboardURL(const WTF::String& pasteboardName)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardName)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct AddPasteboardTypes : CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&> {
    static const Kind messageID = AddPasteboardTypesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AddPasteboardTypes"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&> DecodeType;
    AddPasteboardTypes(const WTF::String& pasteboardName, const Vector<WTF::String>& pasteboardTypes)
        : CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&>(pasteboardName, pasteboardTypes)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetPasteboardTypes : CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&> {
    static const Kind messageID = SetPasteboardTypesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPasteboardTypes"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&> DecodeType;
    SetPasteboardTypes(const WTF::String& pasteboardName, const Vector<WTF::String>& pasteboardTypes)
        : CoreIPC::Arguments2<const WTF::String&, const Vector<WTF::String>&>(pasteboardName, pasteboardTypes)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetPasteboardPathnamesForType : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const Vector<WTF::String>&> {
    static const Kind messageID = SetPasteboardPathnamesForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPasteboardPathnamesForType"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const Vector<WTF::String>&> DecodeType;
    SetPasteboardPathnamesForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType, const Vector<WTF::String>& pathnames)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const Vector<WTF::String>&>(pasteboardName, pasteboardType, pathnames)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetPasteboardStringForType : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> {
    static const Kind messageID = SetPasteboardStringForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPasteboardStringForType"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> DecodeType;
    SetPasteboardStringForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType, const WTF::String& string)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&>(pasteboardName, pasteboardType, string)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetPasteboardBufferForType : CoreIPC::Arguments4<const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t> {
    static const Kind messageID = SetPasteboardBufferForTypeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPasteboardBufferForType"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t> DecodeType;
    SetPasteboardBufferForType(const WTF::String& pasteboardName, const WTF::String& pasteboardType, const WebKit::SharedMemory::Handle& handle, uint64_t size)
        : CoreIPC::Arguments4<const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t>(pasteboardName, pasteboardType, handle, size)
    {
    }
};
#endif

#if !PLATFORM(MAC)
struct Dummy : CoreIPC::Arguments0 {
    static const Kind messageID = DummyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Dummy"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

} // namespace WebContext
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebContext::Kind> {
    static const MessageClass messageClass = MessageClassWebContext;
};

} // namespace CoreIPC

#endif // WebContextMessages_h
