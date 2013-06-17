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

#ifndef PluginControllerProxyMessages_h
#define PluginControllerProxyMessages_h

#if ENABLE(PLUGIN_PROCESS)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "ShareableBitmap.h"
#include "StringReference.h"
#include <wtf/ThreadSafeRefCounted.h>

namespace CoreIPC {
    class Connection;
    class DataReference;
}

namespace WTF {
    class String;
}

namespace WebCore {
    class AffineTransform;
    class IntRect;
    class IntSize;
}

namespace WebKit {
    class WebMouseEvent;
    class WebWheelEvent;
    class WebKeyboardEvent;
}

namespace Messages {
namespace PluginControllerProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("PluginControllerProxy");
}

enum Kind {
    GeometryDidChangeID,
    FrameDidFinishLoadingID,
    FrameDidFailID,
    DidEvaluateJavaScriptID,
    StreamDidReceiveResponseID,
    StreamDidReceiveDataID,
    StreamDidFinishLoadingID,
    StreamDidFailID,
    ManualStreamDidReceiveResponseID,
    ManualStreamDidReceiveDataID,
    ManualStreamDidFinishLoadingID,
    ManualStreamDidFailID,
    HandleMouseEventID,
    HandleWheelEventID,
    HandleMouseEnterEventID,
    HandleMouseLeaveEventID,
    HandleKeyboardEventID,
    HandleEditingCommandID,
    IsEditingCommandEnabledID,
    HandlesPageScaleFactorID,
    SetFocusID,
    DidUpdateID,
    PaintEntirePluginID,
    GetPluginScriptableNPObjectID,
#if PLATFORM(MAC)
    SendComplexTextInputID,
#endif
#if PLATFORM(MAC)
    WindowFocusChangedID,
#endif
#if PLATFORM(MAC)
    WindowAndViewFramesChangedID,
#endif
#if PLATFORM(MAC)
    WindowVisibilityChangedID,
#endif
#if PLATFORM(MAC)
    SetLayerHostingModeID,
#endif
    SnapshotID,
    StorageBlockingStateChangedID,
    PrivateBrowsingStateChangedID,
    GetFormValueID,
};

struct GeometryDidChange : CoreIPC::Arguments5<const WebCore::IntSize&, const WebCore::IntRect&, const WebCore::AffineTransform&, float, const WebKit::ShareableBitmap::Handle&> {
    static const Kind messageID = GeometryDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GeometryDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<const WebCore::IntSize&, const WebCore::IntRect&, const WebCore::AffineTransform&, float, const WebKit::ShareableBitmap::Handle&> DecodeType;
    GeometryDidChange(const WebCore::IntSize& pluginSize, const WebCore::IntRect& clipRect, const WebCore::AffineTransform& pluginToRootViewTransform, float scaleFactor, const WebKit::ShareableBitmap::Handle& backingStoreHandle)
        : CoreIPC::Arguments5<const WebCore::IntSize&, const WebCore::IntRect&, const WebCore::AffineTransform&, float, const WebKit::ShareableBitmap::Handle&>(pluginSize, clipRect, pluginToRootViewTransform, scaleFactor, backingStoreHandle)
    {
    }
};

struct FrameDidFinishLoading : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = FrameDidFinishLoadingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FrameDidFinishLoading"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit FrameDidFinishLoading(uint64_t requestID)
        : CoreIPC::Arguments1<uint64_t>(requestID)
    {
    }
};

struct FrameDidFail : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = FrameDidFailID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FrameDidFail"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    FrameDidFail(uint64_t requestID, bool wasCancelled)
        : CoreIPC::Arguments2<uint64_t, bool>(requestID, wasCancelled)
    {
    }
};

struct DidEvaluateJavaScript : CoreIPC::Arguments2<uint64_t, const WTF::String&> {
    static const Kind messageID = DidEvaluateJavaScriptID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidEvaluateJavaScript"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WTF::String&> DecodeType;
    DidEvaluateJavaScript(uint64_t requestID, const WTF::String& result)
        : CoreIPC::Arguments2<uint64_t, const WTF::String&>(requestID, result)
    {
    }
};

struct StreamDidReceiveResponse : CoreIPC::Arguments6<uint64_t, const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&> {
    static const Kind messageID = StreamDidReceiveResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StreamDidReceiveResponse"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments6<uint64_t, const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&> DecodeType;
    StreamDidReceiveResponse(uint64_t streamID, const WTF::String& responseURLString, uint32_t streamLength, uint32_t lastModifiedTime, const WTF::String& mimeType, const WTF::String& headers)
        : CoreIPC::Arguments6<uint64_t, const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&>(streamID, responseURLString, streamLength, lastModifiedTime, mimeType, headers)
    {
    }
};

struct StreamDidReceiveData : CoreIPC::Arguments2<uint64_t, const CoreIPC::DataReference&> {
    static const Kind messageID = StreamDidReceiveDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StreamDidReceiveData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const CoreIPC::DataReference&> DecodeType;
    StreamDidReceiveData(uint64_t streamID, const CoreIPC::DataReference& data)
        : CoreIPC::Arguments2<uint64_t, const CoreIPC::DataReference&>(streamID, data)
    {
    }
};

struct StreamDidFinishLoading : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = StreamDidFinishLoadingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StreamDidFinishLoading"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit StreamDidFinishLoading(uint64_t streamID)
        : CoreIPC::Arguments1<uint64_t>(streamID)
    {
    }
};

struct StreamDidFail : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = StreamDidFailID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StreamDidFail"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    StreamDidFail(uint64_t streamID, bool wasCancelled)
        : CoreIPC::Arguments2<uint64_t, bool>(streamID, wasCancelled)
    {
    }
};

struct ManualStreamDidReceiveResponse : CoreIPC::Arguments5<const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&> {
    static const Kind messageID = ManualStreamDidReceiveResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ManualStreamDidReceiveResponse"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&> DecodeType;
    ManualStreamDidReceiveResponse(const WTF::String& responseURLString, uint32_t streamLength, uint32_t lastModifiedTime, const WTF::String& mimeType, const WTF::String& headers)
        : CoreIPC::Arguments5<const WTF::String&, uint32_t, uint32_t, const WTF::String&, const WTF::String&>(responseURLString, streamLength, lastModifiedTime, mimeType, headers)
    {
    }
};

struct ManualStreamDidReceiveData : CoreIPC::Arguments1<const CoreIPC::DataReference&> {
    static const Kind messageID = ManualStreamDidReceiveDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ManualStreamDidReceiveData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const CoreIPC::DataReference&> DecodeType;
    explicit ManualStreamDidReceiveData(const CoreIPC::DataReference& data)
        : CoreIPC::Arguments1<const CoreIPC::DataReference&>(data)
    {
    }
};

struct ManualStreamDidFinishLoading : CoreIPC::Arguments0 {
    static const Kind messageID = ManualStreamDidFinishLoadingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ManualStreamDidFinishLoading"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ManualStreamDidFail : CoreIPC::Arguments1<bool> {
    static const Kind messageID = ManualStreamDidFailID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ManualStreamDidFail"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit ManualStreamDidFail(bool wasCancelled)
        : CoreIPC::Arguments1<bool>(wasCancelled)
    {
    }
};

struct HandleMouseEvent : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = HandleMouseEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleMouseEvent"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<CoreIPC::Connection>, PassOwnPtr<CoreIPC::MessageEncoder>);
        ~DelayedReply();

        bool send(bool handled);

    private:
        RefPtr<CoreIPC::Connection> m_connection;
        OwnPtr<CoreIPC::MessageEncoder> m_encoder;
    };

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit HandleMouseEvent(const WebKit::WebMouseEvent& mouseEvent)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(mouseEvent)
    {
    }
};

struct HandleWheelEvent : CoreIPC::Arguments1<const WebKit::WebWheelEvent&> {
    static const Kind messageID = HandleWheelEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleWheelEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebWheelEvent&> DecodeType;
    explicit HandleWheelEvent(const WebKit::WebWheelEvent& wheelEvent)
        : CoreIPC::Arguments1<const WebKit::WebWheelEvent&>(wheelEvent)
    {
    }
};

struct HandleMouseEnterEvent : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = HandleMouseEnterEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleMouseEnterEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit HandleMouseEnterEvent(const WebKit::WebMouseEvent& mouseEvent)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(mouseEvent)
    {
    }
};

struct HandleMouseLeaveEvent : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = HandleMouseLeaveEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleMouseLeaveEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit HandleMouseLeaveEvent(const WebKit::WebMouseEvent& mouseEvent)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(mouseEvent)
    {
    }
};

struct HandleKeyboardEvent : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> {
    static const Kind messageID = HandleKeyboardEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleKeyboardEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> DecodeType;
    explicit HandleKeyboardEvent(const WebKit::WebKeyboardEvent& keyboardEvent)
        : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&>(keyboardEvent)
    {
    }
};

struct HandleEditingCommand : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = HandleEditingCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleEditingCommand"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    HandleEditingCommand(const WTF::String& commandName, const WTF::String& argument)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(commandName, argument)
    {
    }
};

struct IsEditingCommandEnabled : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = IsEditingCommandEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("IsEditingCommandEnabled"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit IsEditingCommandEnabled(const WTF::String& commandName)
        : CoreIPC::Arguments1<const WTF::String&>(commandName)
    {
    }
};

struct HandlesPageScaleFactor : CoreIPC::Arguments0 {
    static const Kind messageID = HandlesPageScaleFactorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandlesPageScaleFactor"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetFocus : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetFocusID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetFocus"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetFocus(bool isFocused)
        : CoreIPC::Arguments1<bool>(isFocused)
    {
    }
};

struct DidUpdate : CoreIPC::Arguments0 {
    static const Kind messageID = DidUpdateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidUpdate"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct PaintEntirePlugin : CoreIPC::Arguments0 {
    static const Kind messageID = PaintEntirePluginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PaintEntirePlugin"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments0 Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct GetPluginScriptableNPObject : CoreIPC::Arguments0 {
    static const Kind messageID = GetPluginScriptableNPObjectID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetPluginScriptableNPObject"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

#if PLATFORM(MAC)
struct SendComplexTextInput : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SendComplexTextInputID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SendComplexTextInput"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SendComplexTextInput(const WTF::String& textInput)
        : CoreIPC::Arguments1<const WTF::String&>(textInput)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct WindowFocusChanged : CoreIPC::Arguments1<bool> {
    static const Kind messageID = WindowFocusChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowFocusChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit WindowFocusChanged(bool hasFocus)
        : CoreIPC::Arguments1<bool>(hasFocus)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct WindowAndViewFramesChanged : CoreIPC::Arguments2<const WebCore::IntRect&, const WebCore::IntRect&> {
    static const Kind messageID = WindowAndViewFramesChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowAndViewFramesChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::IntRect&, const WebCore::IntRect&> DecodeType;
    WindowAndViewFramesChanged(const WebCore::IntRect& windowFrameInScreenCoordinates, const WebCore::IntRect& viewFrameInWindowCoordinates)
        : CoreIPC::Arguments2<const WebCore::IntRect&, const WebCore::IntRect&>(windowFrameInScreenCoordinates, viewFrameInWindowCoordinates)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct WindowVisibilityChanged : CoreIPC::Arguments1<bool> {
    static const Kind messageID = WindowVisibilityChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowVisibilityChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit WindowVisibilityChanged(bool isVisible)
        : CoreIPC::Arguments1<bool>(isVisible)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetLayerHostingMode : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = SetLayerHostingModeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetLayerHostingMode"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit SetLayerHostingMode(uint32_t layerHostingMode)
        : CoreIPC::Arguments1<uint32_t>(layerHostingMode)
    {
    }
};
#endif

struct Snapshot : CoreIPC::Arguments0 {
    static const Kind messageID = SnapshotID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Snapshot"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::ShareableBitmap::Handle&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct StorageBlockingStateChanged : CoreIPC::Arguments1<bool> {
    static const Kind messageID = StorageBlockingStateChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StorageBlockingStateChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit StorageBlockingStateChanged(bool storageBlockingEnabled)
        : CoreIPC::Arguments1<bool>(storageBlockingEnabled)
    {
    }
};

struct PrivateBrowsingStateChanged : CoreIPC::Arguments1<bool> {
    static const Kind messageID = PrivateBrowsingStateChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PrivateBrowsingStateChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit PrivateBrowsingStateChanged(bool isPrivateBrowsingEnabled)
        : CoreIPC::Arguments1<bool>(isPrivateBrowsingEnabled)
    {
    }
};

struct GetFormValue : CoreIPC::Arguments0 {
    static const Kind messageID = GetFormValueID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetFormValue"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<bool&, WTF::String&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

} // namespace PluginControllerProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::PluginControllerProxy::Kind> {
    static const MessageClass messageClass = MessageClassPluginControllerProxy;
};

} // namespace CoreIPC

#endif // ENABLE(PLUGIN_PROCESS)

#endif // PluginControllerProxyMessages_h
