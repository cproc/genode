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

#ifndef WebPageMessages_h
#define WebPageMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "SandboxExtension.h"
#include "SharedMemory.h"
#include "StringReference.h"
#include <WebCore/DictationAlternative.h>
#include <WebCore/Editor.h>
#include <WebCore/KeyboardEvent.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace CoreIPC {
    class DataReference;
}

namespace WTF {
    class String;
}

namespace WebCore {
    class Color;
    class IntPoint;
    class DragData;
    class ResourceRequest;
    class FloatPoint;
    class IntRect;
    class IntSize;
}

namespace WebKit {
    class WebContextUserMessageEncoder;
    struct PrintInfo;
    class QtNetworkReplyData;
    struct WebPreferencesStore;
    class WebMouseEvent;
    class WebContextMenuItemData;
    struct AttributedString;
    class IntentData;
    class SessionState;
    class WebWheelEvent;
    class WebKeyboardEvent;
    class WebTouchEvent;
    struct EditorState;
}

namespace Messages {
namespace WebPage {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebPage");
}

enum Kind {
    SetActiveID,
    SetFocusedID,
    SetInitialFocusID,
    SetIsInWindowID,
    SetDrawsBackgroundID,
    SetDrawsTransparentBackgroundID,
    ViewWillStartLiveResizeID,
    ViewWillEndLiveResizeID,
    KeyEventID,
    KeyEventSyncForTestingID,
    MouseEventID,
    MouseEventSyncForTestingID,
    WheelEventSyncForTestingID,
#if ENABLE(TOUCH_EVENTS)
    TouchEventID,
#endif
#if ENABLE(TOUCH_EVENTS)
    TouchEventSyncForTestingID,
#endif
#if ENABLE(TOUCH_EVENTS) && PLATFORM(QT)
    HighlightPotentialActivationID,
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    DidEndColorChooserID,
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    DidChooseColorID,
#endif
#if ENABLE(CONTEXT_MENUS)
    ContextMenuHiddenID,
#endif
    ScrollByID,
    CenterSelectionInVisibleAreaID,
    GoBackID,
    GoForwardID,
    GoToBackForwardItemID,
    TryRestoreScrollPositionID,
    LoadHTMLStringID,
    LoadAlternateHTMLStringID,
    LoadPlainTextStringID,
    LoadWebArchiveDataID,
    LoadURLID,
    LoadURLRequestID,
    LinkClickedID,
    ReloadID,
    StopLoadingID,
#if PLATFORM(QT)
    ApplicationSchemeReplyID,
#endif
#if PLATFORM(QT)
    RegisterApplicationSchemeID,
#endif
#if PLATFORM(QT)
    SetUserScriptsID,
#endif
    StopLoadingFrameID,
    RestoreSessionID,
    RestoreSessionAndNavigateToCurrentItemID,
    DidRemoveBackForwardItemID,
    SetWillGoToBackForwardItemCallbackEnabledID,
    DidReceivePolicyDecisionID,
    ClearSelectionID,
    GetContentsAsStringID,
#if ENABLE(MHTML)
    GetContentsAsMHTMLDataID,
#endif
    GetMainResourceDataOfFrameID,
    GetResourceDataFromFrameID,
    GetRenderTreeExternalRepresentationID,
    GetSelectionOrContentsAsStringID,
    GetSourceForFrameID,
    GetWebArchiveOfFrameID,
    RunJavaScriptInMainFrameID,
    ForceRepaintID,
#if PLATFORM(MAC)
    PerformDictionaryLookupAtLocationID,
#endif
    PreferencesDidChangeID,
    SetUserAgentID,
    SetCustomTextEncodingNameID,
    SuspendActiveDOMObjectsAndAnimationsID,
    ResumeActiveDOMObjectsAndAnimationsID,
#if USE(TILED_BACKING_STORE)
    SetViewportSizeID,
#endif
    CloseID,
    TryCloseID,
    ValidateCommandID,
    ExecuteEditCommandID,
    DidRemoveEditCommandID,
    ReapplyEditCommandID,
    UnapplyEditCommandID,
    SetPageAndTextZoomFactorsID,
    SetPageZoomFactorID,
    SetTextZoomFactorID,
    WindowScreenDidChangeID,
    ScalePageID,
    SetUseFixedLayoutID,
    SetFixedLayoutSizeID,
    ListenForLayoutMilestonesID,
    SetSuppressScrollbarAnimationsID,
    SetPaginationModeID,
    SetPaginationBehavesLikeColumnsID,
    SetPageLengthID,
    SetGapBetweenPagesID,
    PostInjectedBundleMessageID,
    FindStringID,
    HideFindUIID,
    CountStringMatchesID,
#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
    PerformDragControllerActionID,
#endif
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
    PerformDragControllerActionID,
#endif
#if !PLATFORM(WIN) && !PLATFORM(QT) && !PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
    PerformDragControllerActionID,
#endif
#if ENABLE(DRAG_SUPPORT)
    DragEndedID,
#endif
    DidChangeSelectedIndexForActivePopupMenuID,
    SetTextForActivePopupMenuID,
#if PLATFORM(GTK)
    FailedToShowPopupMenuID,
#endif
#if PLATFORM(QT)
    HidePopupMenuID,
#endif
#if PLATFORM(QT)
    SelectedIndexID,
#endif
#if ENABLE(CONTEXT_MENUS)
    DidSelectItemFromActiveContextMenuID,
#endif
    DidChooseFilesForOpenPanelID,
    DidCancelForOpenPanelID,
#if ENABLE(WEB_PROCESS_SANDBOX)
    ExtendSandboxForFileFromOpenPanelID,
#endif
    AdvanceToNextMisspellingID,
    ChangeSpellingToWordID,
#if USE(APPKIT)
    UppercaseWordID,
#endif
#if USE(APPKIT)
    LowercaseWordID,
#endif
#if USE(APPKIT)
    CapitalizeWordID,
#endif
#if USE(APPKIT)
    SetSmartInsertDeleteEnabledID,
#endif
#if ENABLE(GEOLOCATION)
    DidReceiveGeolocationPermissionDecisionID,
#endif
    DidReceiveNotificationPermissionDecisionID,
    SetWindowResizerSizeID,
    BeginPrintingID,
    EndPrintingID,
    ComputePagesForPrintingID,
#if PLATFORM(MAC) || PLATFORM(WIN)
    DrawRectToImageID,
#endif
#if PLATFORM(MAC) || PLATFORM(WIN)
    DrawPagesToPDFID,
#endif
#if PLATFORM(GTK)
    DrawPagesForPrintingID,
#endif
    SetMediaVolumeID,
    SetMayStartMediaWhenInWindowID,
    SetMemoryCacheMessagesEnabledID,
    DummyID,
    SetCanRunBeforeUnloadConfirmPanelID,
    SetCanRunModalID,
#if ENABLE(WEB_INTENTS)
    DeliverIntentToFrameID,
#endif
#if PLATFORM(EFL)
    SetThemePathID,
#endif
#if USE(TILED_BACKING_STORE)
    CommitPageTransitionViewportID,
#endif
#if PLATFORM(QT)
    SetCompositionID,
#endif
#if PLATFORM(QT)
    ConfirmCompositionID,
#endif
#if PLATFORM(QT)
    CancelCompositionID,
#endif
#if PLATFORM(MAC)
    SendComplexTextInputToPluginID,
#endif
#if PLATFORM(MAC)
    SetWindowIsVisibleID,
#endif
#if PLATFORM(MAC)
    WindowAndViewFramesChangedID,
#endif
#if PLATFORM(MAC)
    RegisterUIProcessAccessibilityTokensID,
#endif
#if PLATFORM(MAC)
    GetStringSelectionForPasteboardID,
#endif
#if PLATFORM(MAC)
    GetDataSelectionForPasteboardID,
#endif
#if PLATFORM(MAC)
    ReadSelectionFromPasteboardID,
#endif
#if PLATFORM(MAC)
    SetCompositionID,
#endif
#if PLATFORM(MAC)
    ConfirmCompositionID,
#endif
#if PLATFORM(MAC)
    CancelCompositionID,
#endif
#if PLATFORM(MAC)
    InsertTextID,
#endif
#if PLATFORM(MAC)
    GetMarkedRangeID,
#endif
#if PLATFORM(MAC)
    GetSelectedRangeID,
#endif
#if PLATFORM(MAC)
    GetAttributedSubstringFromRangeID,
#endif
#if PLATFORM(MAC)
    CharacterIndexForPointID,
#endif
#if PLATFORM(MAC)
    FirstRectForCharacterRangeID,
#endif
#if PLATFORM(MAC)
    ExecuteKeypressCommandsID,
#endif
#if PLATFORM(MAC)
    ShouldDelayWindowOrderingEventID,
#endif
#if PLATFORM(MAC)
    AcceptsFirstMouseID,
#endif
#if PLATFORM(MAC)
    InsertDictatedTextID,
#endif
#if PLATFORM(WIN)
    ConfirmCompositionID,
#endif
#if PLATFORM(WIN)
    SetCompositionID,
#endif
#if PLATFORM(WIN)
    FirstRectForCharacterInSelectedRangeID,
#endif
#if PLATFORM(WIN)
    GetSelectedTextID,
#endif
#if PLATFORM(WIN)
    GestureWillBeginID,
#endif
#if PLATFORM(WIN)
    GestureDidScrollID,
#endif
#if PLATFORM(WIN)
    GestureDidEndID,
#endif
#if PLATFORM(EFL)
    ConfirmCompositionID,
#endif
#if PLATFORM(EFL)
    SetCompositionID,
#endif
#if PLATFORM(EFL)
    CancelCompositionID,
#endif
#if PLATFORM(QT)
    FindZoomableAreaForPointID,
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    HandleAlternativeTextUIResultID,
#endif
#if ENABLE(PAGE_VISIBILITY_API) || ENABLE(HIDDEN_PAGE_DOM_TIMER_THROTTLING)
    SetVisibilityStateID,
#endif
#if PLATFORM(GTK) && USE(TEXTURE_MAPPER_GL)
    SetAcceleratedCompositingWindowIdID,
#endif
};

struct SetActive : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetActiveID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetActive"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetActive(bool active)
        : CoreIPC::Arguments1<bool>(active)
    {
    }
};

struct SetFocused : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetFocusedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetFocused"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetFocused(bool focused)
        : CoreIPC::Arguments1<bool>(focused)
    {
    }
};

struct SetInitialFocus : CoreIPC::Arguments3<bool, bool, const WebKit::WebKeyboardEvent&> {
    static const Kind messageID = SetInitialFocusID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetInitialFocus"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<bool, bool, const WebKit::WebKeyboardEvent&> DecodeType;
    SetInitialFocus(bool forward, bool isKeyboardEventValid, const WebKit::WebKeyboardEvent& event)
        : CoreIPC::Arguments3<bool, bool, const WebKit::WebKeyboardEvent&>(forward, isKeyboardEventValid, event)
    {
    }
};

struct SetIsInWindow : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetIsInWindowID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetIsInWindow"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetIsInWindow(bool isInWindow)
        : CoreIPC::Arguments1<bool>(isInWindow)
    {
    }
};

struct SetDrawsBackground : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetDrawsBackgroundID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetDrawsBackground"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetDrawsBackground(bool drawsBackground)
        : CoreIPC::Arguments1<bool>(drawsBackground)
    {
    }
};

struct SetDrawsTransparentBackground : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetDrawsTransparentBackgroundID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetDrawsTransparentBackground"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetDrawsTransparentBackground(bool drawsTransparentBackground)
        : CoreIPC::Arguments1<bool>(drawsTransparentBackground)
    {
    }
};

struct ViewWillStartLiveResize : CoreIPC::Arguments0 {
    static const Kind messageID = ViewWillStartLiveResizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ViewWillStartLiveResize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ViewWillEndLiveResize : CoreIPC::Arguments0 {
    static const Kind messageID = ViewWillEndLiveResizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ViewWillEndLiveResize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct KeyEvent : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> {
    static const Kind messageID = KeyEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("KeyEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> DecodeType;
    explicit KeyEvent(const WebKit::WebKeyboardEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&>(event)
    {
    }
};

struct KeyEventSyncForTesting : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> {
    static const Kind messageID = KeyEventSyncForTestingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("KeyEventSyncForTesting"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&> DecodeType;
    explicit KeyEventSyncForTesting(const WebKit::WebKeyboardEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebKeyboardEvent&>(event)
    {
    }
};

struct MouseEvent : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = MouseEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("MouseEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit MouseEvent(const WebKit::WebMouseEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(event)
    {
    }
};

struct MouseEventSyncForTesting : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = MouseEventSyncForTestingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("MouseEventSyncForTesting"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit MouseEventSyncForTesting(const WebKit::WebMouseEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(event)
    {
    }
};

struct WheelEventSyncForTesting : CoreIPC::Arguments1<const WebKit::WebWheelEvent&> {
    static const Kind messageID = WheelEventSyncForTestingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WheelEventSyncForTesting"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebWheelEvent&> DecodeType;
    explicit WheelEventSyncForTesting(const WebKit::WebWheelEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebWheelEvent&>(event)
    {
    }
};

#if ENABLE(TOUCH_EVENTS)
struct TouchEvent : CoreIPC::Arguments1<const WebKit::WebTouchEvent&> {
    static const Kind messageID = TouchEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("TouchEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WebTouchEvent&> DecodeType;
    explicit TouchEvent(const WebKit::WebTouchEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebTouchEvent&>(event)
    {
    }
};
#endif

#if ENABLE(TOUCH_EVENTS)
struct TouchEventSyncForTesting : CoreIPC::Arguments1<const WebKit::WebTouchEvent&> {
    static const Kind messageID = TouchEventSyncForTestingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("TouchEventSyncForTesting"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebTouchEvent&> DecodeType;
    explicit TouchEventSyncForTesting(const WebKit::WebTouchEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebTouchEvent&>(event)
    {
    }
};
#endif

#if ENABLE(TOUCH_EVENTS) && PLATFORM(QT)
struct HighlightPotentialActivation : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&> {
    static const Kind messageID = HighlightPotentialActivationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HighlightPotentialActivation"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&> DecodeType;
    HighlightPotentialActivation(const WebCore::IntPoint& point, const WebCore::IntSize& area)
        : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&>(point, area)
    {
    }
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
struct DidEndColorChooser : CoreIPC::Arguments0 {
    static const Kind messageID = DidEndColorChooserID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidEndColorChooser"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
struct DidChooseColor : CoreIPC::Arguments1<const WebCore::Color&> {
    static const Kind messageID = DidChooseColorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChooseColor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::Color&> DecodeType;
    explicit DidChooseColor(const WebCore::Color& color)
        : CoreIPC::Arguments1<const WebCore::Color&>(color)
    {
    }
};
#endif

#if ENABLE(CONTEXT_MENUS)
struct ContextMenuHidden : CoreIPC::Arguments0 {
    static const Kind messageID = ContextMenuHiddenID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ContextMenuHidden"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

struct ScrollBy : CoreIPC::Arguments2<uint32_t, uint32_t> {
    static const Kind messageID = ScrollByID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ScrollBy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, uint32_t> DecodeType;
    ScrollBy(uint32_t scrollDirection, uint32_t scrollGranularity)
        : CoreIPC::Arguments2<uint32_t, uint32_t>(scrollDirection, scrollGranularity)
    {
    }
};

struct CenterSelectionInVisibleArea : CoreIPC::Arguments0 {
    static const Kind messageID = CenterSelectionInVisibleAreaID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CenterSelectionInVisibleArea"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct GoBack : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GoBackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GoBack"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GoBack(uint64_t backForwardItemID)
        : CoreIPC::Arguments1<uint64_t>(backForwardItemID)
    {
    }
};

struct GoForward : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GoForwardID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GoForward"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GoForward(uint64_t backForwardItemID)
        : CoreIPC::Arguments1<uint64_t>(backForwardItemID)
    {
    }
};

struct GoToBackForwardItem : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GoToBackForwardItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GoToBackForwardItem"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GoToBackForwardItem(uint64_t backForwardItemID)
        : CoreIPC::Arguments1<uint64_t>(backForwardItemID)
    {
    }
};

struct TryRestoreScrollPosition : CoreIPC::Arguments0 {
    static const Kind messageID = TryRestoreScrollPositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("TryRestoreScrollPosition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct LoadHTMLString : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = LoadHTMLStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadHTMLString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    LoadHTMLString(const WTF::String& htmlString, const WTF::String& baseURL)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(htmlString, baseURL)
    {
    }
};

struct LoadAlternateHTMLString : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> {
    static const Kind messageID = LoadAlternateHTMLStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadAlternateHTMLString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> DecodeType;
    LoadAlternateHTMLString(const WTF::String& htmlString, const WTF::String& baseURL, const WTF::String& unreachableURL)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&>(htmlString, baseURL, unreachableURL)
    {
    }
};

struct LoadPlainTextString : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = LoadPlainTextStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadPlainTextString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit LoadPlainTextString(const WTF::String& string)
        : CoreIPC::Arguments1<const WTF::String&>(string)
    {
    }
};

struct LoadWebArchiveData : CoreIPC::Arguments1<const CoreIPC::DataReference&> {
    static const Kind messageID = LoadWebArchiveDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadWebArchiveData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const CoreIPC::DataReference&> DecodeType;
    explicit LoadWebArchiveData(const CoreIPC::DataReference& webArchiveData)
        : CoreIPC::Arguments1<const CoreIPC::DataReference&>(webArchiveData)
    {
    }
};

struct LoadURL : CoreIPC::Arguments2<const WTF::String&, const WebKit::SandboxExtension::Handle&> {
    static const Kind messageID = LoadURLID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadURL"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WebKit::SandboxExtension::Handle&> DecodeType;
    LoadURL(const WTF::String& url, const WebKit::SandboxExtension::Handle& sandboxExtensionHandle)
        : CoreIPC::Arguments2<const WTF::String&, const WebKit::SandboxExtension::Handle&>(url, sandboxExtensionHandle)
    {
    }
};

struct LoadURLRequest : CoreIPC::Arguments2<const WebCore::ResourceRequest&, const WebKit::SandboxExtension::Handle&> {
    static const Kind messageID = LoadURLRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadURLRequest"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::ResourceRequest&, const WebKit::SandboxExtension::Handle&> DecodeType;
    LoadURLRequest(const WebCore::ResourceRequest& request, const WebKit::SandboxExtension::Handle& sandboxExtensionHandle)
        : CoreIPC::Arguments2<const WebCore::ResourceRequest&, const WebKit::SandboxExtension::Handle&>(request, sandboxExtensionHandle)
    {
    }
};

struct LinkClicked : CoreIPC::Arguments2<const WTF::String&, const WebKit::WebMouseEvent&> {
    static const Kind messageID = LinkClickedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LinkClicked"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WebKit::WebMouseEvent&> DecodeType;
    LinkClicked(const WTF::String& url, const WebKit::WebMouseEvent& event)
        : CoreIPC::Arguments2<const WTF::String&, const WebKit::WebMouseEvent&>(url, event)
    {
    }
};

struct Reload : CoreIPC::Arguments2<bool, const WebKit::SandboxExtension::Handle&> {
    static const Kind messageID = ReloadID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Reload"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<bool, const WebKit::SandboxExtension::Handle&> DecodeType;
    Reload(bool reloadFromOrigin, const WebKit::SandboxExtension::Handle& sandboxExtensionHandle)
        : CoreIPC::Arguments2<bool, const WebKit::SandboxExtension::Handle&>(reloadFromOrigin, sandboxExtensionHandle)
    {
    }
};

struct StopLoading : CoreIPC::Arguments0 {
    static const Kind messageID = StopLoadingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopLoading"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if PLATFORM(QT)
struct ApplicationSchemeReply : CoreIPC::Arguments1<const WebKit::QtNetworkReplyData&> {
    static const Kind messageID = ApplicationSchemeReplyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ApplicationSchemeReply"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::QtNetworkReplyData&> DecodeType;
    explicit ApplicationSchemeReply(const WebKit::QtNetworkReplyData& reply)
        : CoreIPC::Arguments1<const WebKit::QtNetworkReplyData&>(reply)
    {
    }
};
#endif

#if PLATFORM(QT)
struct RegisterApplicationScheme : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterApplicationSchemeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterApplicationScheme"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterApplicationScheme(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};
#endif

#if PLATFORM(QT)
struct SetUserScripts : CoreIPC::Arguments1<const WTF::Vector<WTF::String>&> {
    static const Kind messageID = SetUserScriptsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetUserScripts"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::Vector<WTF::String>&> DecodeType;
    explicit SetUserScripts(const WTF::Vector<WTF::String>& script)
        : CoreIPC::Arguments1<const WTF::Vector<WTF::String>&>(script)
    {
    }
};
#endif

struct StopLoadingFrame : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = StopLoadingFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopLoadingFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit StopLoadingFrame(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct RestoreSession : CoreIPC::Arguments1<const WebKit::SessionState&> {
    static const Kind messageID = RestoreSessionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RestoreSession"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::SessionState&> DecodeType;
    explicit RestoreSession(const WebKit::SessionState& state)
        : CoreIPC::Arguments1<const WebKit::SessionState&>(state)
    {
    }
};

struct RestoreSessionAndNavigateToCurrentItem : CoreIPC::Arguments1<const WebKit::SessionState&> {
    static const Kind messageID = RestoreSessionAndNavigateToCurrentItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RestoreSessionAndNavigateToCurrentItem"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::SessionState&> DecodeType;
    explicit RestoreSessionAndNavigateToCurrentItem(const WebKit::SessionState& state)
        : CoreIPC::Arguments1<const WebKit::SessionState&>(state)
    {
    }
};

struct DidRemoveBackForwardItem : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidRemoveBackForwardItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRemoveBackForwardItem"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidRemoveBackForwardItem(uint64_t backForwardItemID)
        : CoreIPC::Arguments1<uint64_t>(backForwardItemID)
    {
    }
};

struct SetWillGoToBackForwardItemCallbackEnabled : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetWillGoToBackForwardItemCallbackEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetWillGoToBackForwardItemCallbackEnabled"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetWillGoToBackForwardItemCallbackEnabled(bool willGoToBackForwardItemEnabled)
        : CoreIPC::Arguments1<bool>(willGoToBackForwardItemEnabled)
    {
    }
};

struct DidReceivePolicyDecision : CoreIPC::Arguments4<uint64_t, uint64_t, uint32_t, uint64_t> {
    static const Kind messageID = DidReceivePolicyDecisionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceivePolicyDecision"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t, uint64_t, uint32_t, uint64_t> DecodeType;
    DidReceivePolicyDecision(uint64_t frameID, uint64_t listenerID, uint32_t policyAction, uint64_t downloadID)
        : CoreIPC::Arguments4<uint64_t, uint64_t, uint32_t, uint64_t>(frameID, listenerID, policyAction, downloadID)
    {
    }
};

struct ClearSelection : CoreIPC::Arguments0 {
    static const Kind messageID = ClearSelectionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearSelection"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct GetContentsAsString : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetContentsAsStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetContentsAsString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetContentsAsString(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

#if ENABLE(MHTML)
struct GetContentsAsMHTMLData : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = GetContentsAsMHTMLDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetContentsAsMHTMLData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    GetContentsAsMHTMLData(uint64_t callbackID, bool useBinaryEncoding)
        : CoreIPC::Arguments2<uint64_t, bool>(callbackID, useBinaryEncoding)
    {
    }
};
#endif

struct GetMainResourceDataOfFrame : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = GetMainResourceDataOfFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetMainResourceDataOfFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    GetMainResourceDataOfFrame(uint64_t frameID, uint64_t callbackID)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, callbackID)
    {
    }
};

struct GetResourceDataFromFrame : CoreIPC::Arguments3<uint64_t, const WTF::String&, uint64_t> {
    static const Kind messageID = GetResourceDataFromFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetResourceDataFromFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, const WTF::String&, uint64_t> DecodeType;
    GetResourceDataFromFrame(uint64_t frameID, const WTF::String& resourceURL, uint64_t callbackID)
        : CoreIPC::Arguments3<uint64_t, const WTF::String&, uint64_t>(frameID, resourceURL, callbackID)
    {
    }
};

struct GetRenderTreeExternalRepresentation : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetRenderTreeExternalRepresentationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetRenderTreeExternalRepresentation"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetRenderTreeExternalRepresentation(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct GetSelectionOrContentsAsString : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetSelectionOrContentsAsStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetSelectionOrContentsAsString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetSelectionOrContentsAsString(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct GetSourceForFrame : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = GetSourceForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetSourceForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    GetSourceForFrame(uint64_t frameID, uint64_t callbackID)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, callbackID)
    {
    }
};

struct GetWebArchiveOfFrame : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = GetWebArchiveOfFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetWebArchiveOfFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    GetWebArchiveOfFrame(uint64_t frameID, uint64_t callbackID)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, callbackID)
    {
    }
};

struct RunJavaScriptInMainFrame : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = RunJavaScriptInMainFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunJavaScriptInMainFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    RunJavaScriptInMainFrame(const WTF::String& script, uint64_t callbackID)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(script, callbackID)
    {
    }
};

struct ForceRepaint : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ForceRepaintID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ForceRepaint"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ForceRepaint(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

#if PLATFORM(MAC)
struct PerformDictionaryLookupAtLocation : CoreIPC::Arguments1<const WebCore::FloatPoint&> {
    static const Kind messageID = PerformDictionaryLookupAtLocationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PerformDictionaryLookupAtLocation"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::FloatPoint&> DecodeType;
    explicit PerformDictionaryLookupAtLocation(const WebCore::FloatPoint& point)
        : CoreIPC::Arguments1<const WebCore::FloatPoint&>(point)
    {
    }
};
#endif

struct PreferencesDidChange : CoreIPC::Arguments1<const WebKit::WebPreferencesStore&> {
    static const Kind messageID = PreferencesDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PreferencesDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WebPreferencesStore&> DecodeType;
    explicit PreferencesDidChange(const WebKit::WebPreferencesStore& store)
        : CoreIPC::Arguments1<const WebKit::WebPreferencesStore&>(store)
    {
    }
};

struct SetUserAgent : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetUserAgentID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetUserAgent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetUserAgent(const WTF::String& userAgent)
        : CoreIPC::Arguments1<const WTF::String&>(userAgent)
    {
    }
};

struct SetCustomTextEncodingName : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetCustomTextEncodingNameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCustomTextEncodingName"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetCustomTextEncodingName(const WTF::String& encodingName)
        : CoreIPC::Arguments1<const WTF::String&>(encodingName)
    {
    }
};

struct SuspendActiveDOMObjectsAndAnimations : CoreIPC::Arguments0 {
    static const Kind messageID = SuspendActiveDOMObjectsAndAnimationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SuspendActiveDOMObjectsAndAnimations"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ResumeActiveDOMObjectsAndAnimations : CoreIPC::Arguments0 {
    static const Kind messageID = ResumeActiveDOMObjectsAndAnimationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ResumeActiveDOMObjectsAndAnimations"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if USE(TILED_BACKING_STORE)
struct SetViewportSize : CoreIPC::Arguments1<const WebCore::IntSize&> {
    static const Kind messageID = SetViewportSizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetViewportSize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntSize&> DecodeType;
    explicit SetViewportSize(const WebCore::IntSize& size)
        : CoreIPC::Arguments1<const WebCore::IntSize&>(size)
    {
    }
};
#endif

struct Close : CoreIPC::Arguments0 {
    static const Kind messageID = CloseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Close"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct TryClose : CoreIPC::Arguments0 {
    static const Kind messageID = TryCloseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("TryClose"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ValidateCommand : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = ValidateCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ValidateCommand"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    ValidateCommand(const WTF::String& name, uint64_t callbackID)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(name, callbackID)
    {
    }
};

struct ExecuteEditCommand : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ExecuteEditCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExecuteEditCommand"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ExecuteEditCommand(const WTF::String& name)
        : CoreIPC::Arguments1<const WTF::String&>(name)
    {
    }
};

struct DidRemoveEditCommand : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidRemoveEditCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRemoveEditCommand"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidRemoveEditCommand(uint64_t commandID)
        : CoreIPC::Arguments1<uint64_t>(commandID)
    {
    }
};

struct ReapplyEditCommand : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ReapplyEditCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ReapplyEditCommand"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ReapplyEditCommand(uint64_t commandID)
        : CoreIPC::Arguments1<uint64_t>(commandID)
    {
    }
};

struct UnapplyEditCommand : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = UnapplyEditCommandID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UnapplyEditCommand"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit UnapplyEditCommand(uint64_t commandID)
        : CoreIPC::Arguments1<uint64_t>(commandID)
    {
    }
};

struct SetPageAndTextZoomFactors : CoreIPC::Arguments2<double, double> {
    static const Kind messageID = SetPageAndTextZoomFactorsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPageAndTextZoomFactors"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<double, double> DecodeType;
    SetPageAndTextZoomFactors(double pageZoomFactor, double textZoomFactor)
        : CoreIPC::Arguments2<double, double>(pageZoomFactor, textZoomFactor)
    {
    }
};

struct SetPageZoomFactor : CoreIPC::Arguments1<double> {
    static const Kind messageID = SetPageZoomFactorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPageZoomFactor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit SetPageZoomFactor(double zoomFactor)
        : CoreIPC::Arguments1<double>(zoomFactor)
    {
    }
};

struct SetTextZoomFactor : CoreIPC::Arguments1<double> {
    static const Kind messageID = SetTextZoomFactorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetTextZoomFactor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit SetTextZoomFactor(double zoomFactor)
        : CoreIPC::Arguments1<double>(zoomFactor)
    {
    }
};

struct WindowScreenDidChange : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = WindowScreenDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowScreenDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit WindowScreenDidChange(uint64_t displayID)
        : CoreIPC::Arguments1<uint64_t>(displayID)
    {
    }
};

struct ScalePage : CoreIPC::Arguments2<double, const WebCore::IntPoint&> {
    static const Kind messageID = ScalePageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ScalePage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<double, const WebCore::IntPoint&> DecodeType;
    ScalePage(double scale, const WebCore::IntPoint& origin)
        : CoreIPC::Arguments2<double, const WebCore::IntPoint&>(scale, origin)
    {
    }
};

struct SetUseFixedLayout : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetUseFixedLayoutID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetUseFixedLayout"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetUseFixedLayout(bool fixed)
        : CoreIPC::Arguments1<bool>(fixed)
    {
    }
};

struct SetFixedLayoutSize : CoreIPC::Arguments1<const WebCore::IntSize&> {
    static const Kind messageID = SetFixedLayoutSizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetFixedLayoutSize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntSize&> DecodeType;
    explicit SetFixedLayoutSize(const WebCore::IntSize& size)
        : CoreIPC::Arguments1<const WebCore::IntSize&>(size)
    {
    }
};

struct ListenForLayoutMilestones : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = ListenForLayoutMilestonesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ListenForLayoutMilestones"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit ListenForLayoutMilestones(uint32_t milestones)
        : CoreIPC::Arguments1<uint32_t>(milestones)
    {
    }
};

struct SetSuppressScrollbarAnimations : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetSuppressScrollbarAnimationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetSuppressScrollbarAnimations"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetSuppressScrollbarAnimations(bool suppressAnimations)
        : CoreIPC::Arguments1<bool>(suppressAnimations)
    {
    }
};

struct SetPaginationMode : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = SetPaginationModeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPaginationMode"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit SetPaginationMode(uint32_t mode)
        : CoreIPC::Arguments1<uint32_t>(mode)
    {
    }
};

struct SetPaginationBehavesLikeColumns : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetPaginationBehavesLikeColumnsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPaginationBehavesLikeColumns"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetPaginationBehavesLikeColumns(bool behavesLikeColumns)
        : CoreIPC::Arguments1<bool>(behavesLikeColumns)
    {
    }
};

struct SetPageLength : CoreIPC::Arguments1<double> {
    static const Kind messageID = SetPageLengthID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPageLength"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit SetPageLength(double pageLength)
        : CoreIPC::Arguments1<double>(pageLength)
    {
    }
};

struct SetGapBetweenPages : CoreIPC::Arguments1<double> {
    static const Kind messageID = SetGapBetweenPagesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetGapBetweenPages"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit SetGapBetweenPages(double gap)
        : CoreIPC::Arguments1<double>(gap)
    {
    }
};

struct PostInjectedBundleMessage : CoreIPC::Arguments2<const WTF::String&, const WebKit::WebContextUserMessageEncoder&> {
    static const Kind messageID = PostInjectedBundleMessageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PostInjectedBundleMessage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<WTF::String&> DecodeType;
    PostInjectedBundleMessage(const WTF::String& messageName, const WebKit::WebContextUserMessageEncoder& messageBody)
        : CoreIPC::Arguments2<const WTF::String&, const WebKit::WebContextUserMessageEncoder&>(messageName, messageBody)
    {
    }
};

struct FindString : CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&> {
    static const Kind messageID = FindStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FindString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&> DecodeType;
    FindString(const WTF::String& string, uint32_t findOptions, const unsigned& maxMatchCount)
        : CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&>(string, findOptions, maxMatchCount)
    {
    }
};

struct HideFindUI : CoreIPC::Arguments0 {
    static const Kind messageID = HideFindUIID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HideFindUI"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct CountStringMatches : CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&> {
    static const Kind messageID = CountStringMatchesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CountStringMatches"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&> DecodeType;
    CountStringMatches(const WTF::String& string, uint32_t findOptions, const unsigned& maxMatchCount)
        : CoreIPC::Arguments3<const WTF::String&, uint32_t, const unsigned&>(string, findOptions, maxMatchCount)
    {
    }
};

#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
struct PerformDragControllerAction : CoreIPC::Arguments6<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String>>&, uint32_t> {
    static const Kind messageID = PerformDragControllerActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PerformDragControllerAction"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments6<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String>>&, uint32_t> DecodeType;
    PerformDragControllerAction(uint64_t action, const WebCore::IntPoint& clientPosition, const WebCore::IntPoint& globalPosition, uint64_t draggingSourceOperationMask, const HashMap<UINT,Vector<String>>& dataMap, uint32_t flags)
        : CoreIPC::Arguments6<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String>>&, uint32_t>(action, clientPosition, globalPosition, draggingSourceOperationMask, dataMap, flags)
    {
    }
};
#endif

#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
struct PerformDragControllerAction : CoreIPC::Arguments2<uint64_t, const WebCore::DragData&> {
    static const Kind messageID = PerformDragControllerActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PerformDragControllerAction"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebCore::DragData&> DecodeType;
    PerformDragControllerAction(uint64_t action, const WebCore::DragData& dragData)
        : CoreIPC::Arguments2<uint64_t, const WebCore::DragData&>(action, dragData)
    {
    }
};
#endif

#if !PLATFORM(WIN) && !PLATFORM(QT) && !PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
struct PerformDragControllerAction : CoreIPC::Arguments8<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const WTF::String&, uint32_t, const WebKit::SandboxExtension::Handle&, const WebKit::SandboxExtension::HandleArray&> {
    static const Kind messageID = PerformDragControllerActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PerformDragControllerAction"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments8<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const WTF::String&, uint32_t, const WebKit::SandboxExtension::Handle&, const WebKit::SandboxExtension::HandleArray&> DecodeType;
    PerformDragControllerAction(uint64_t action, const WebCore::IntPoint& clientPosition, const WebCore::IntPoint& globalPosition, uint64_t draggingSourceOperationMask, const WTF::String& dragStorageName, uint32_t flags, const WebKit::SandboxExtension::Handle& sandboxExtensionHandle, const WebKit::SandboxExtension::HandleArray& sandboxExtensionsForUpload)
        : CoreIPC::Arguments8<uint64_t, const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const WTF::String&, uint32_t, const WebKit::SandboxExtension::Handle&, const WebKit::SandboxExtension::HandleArray&>(action, clientPosition, globalPosition, draggingSourceOperationMask, dragStorageName, flags, sandboxExtensionHandle, sandboxExtensionsForUpload)
    {
    }
};
#endif

#if ENABLE(DRAG_SUPPORT)
struct DragEnded : CoreIPC::Arguments3<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t> {
    static const Kind messageID = DragEndedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DragEnded"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t> DecodeType;
    DragEnded(const WebCore::IntPoint& clientPosition, const WebCore::IntPoint& globalPosition, uint64_t operation)
        : CoreIPC::Arguments3<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t>(clientPosition, globalPosition, operation)
    {
    }
};
#endif

struct DidChangeSelectedIndexForActivePopupMenu : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = DidChangeSelectedIndexForActivePopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeSelectedIndexForActivePopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit DidChangeSelectedIndexForActivePopupMenu(int32_t newIndex)
        : CoreIPC::Arguments1<int32_t>(newIndex)
    {
    }
};

struct SetTextForActivePopupMenu : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = SetTextForActivePopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetTextForActivePopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit SetTextForActivePopupMenu(int32_t index)
        : CoreIPC::Arguments1<int32_t>(index)
    {
    }
};

#if PLATFORM(GTK)
struct FailedToShowPopupMenu : CoreIPC::Arguments0 {
    static const Kind messageID = FailedToShowPopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FailedToShowPopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(QT)
struct HidePopupMenu : CoreIPC::Arguments0 {
    static const Kind messageID = HidePopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HidePopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(QT)
struct SelectedIndex : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = SelectedIndexID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SelectedIndex"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit SelectedIndex(int32_t newIndex)
        : CoreIPC::Arguments1<int32_t>(newIndex)
    {
    }
};
#endif

#if ENABLE(CONTEXT_MENUS)
struct DidSelectItemFromActiveContextMenu : CoreIPC::Arguments1<const WebKit::WebContextMenuItemData&> {
    static const Kind messageID = DidSelectItemFromActiveContextMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidSelectItemFromActiveContextMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WebContextMenuItemData&> DecodeType;
    explicit DidSelectItemFromActiveContextMenu(const WebKit::WebContextMenuItemData& menuItem)
        : CoreIPC::Arguments1<const WebKit::WebContextMenuItemData&>(menuItem)
    {
    }
};
#endif

struct DidChooseFilesForOpenPanel : CoreIPC::Arguments1<const Vector<WTF::String>&> {
    static const Kind messageID = DidChooseFilesForOpenPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChooseFilesForOpenPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<WTF::String>&> DecodeType;
    explicit DidChooseFilesForOpenPanel(const Vector<WTF::String>& fileURLs)
        : CoreIPC::Arguments1<const Vector<WTF::String>&>(fileURLs)
    {
    }
};

struct DidCancelForOpenPanel : CoreIPC::Arguments0 {
    static const Kind messageID = DidCancelForOpenPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCancelForOpenPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if ENABLE(WEB_PROCESS_SANDBOX)
struct ExtendSandboxForFileFromOpenPanel : CoreIPC::Arguments1<const WebKit::SandboxExtension::Handle&> {
    static const Kind messageID = ExtendSandboxForFileFromOpenPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExtendSandboxForFileFromOpenPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::SandboxExtension::Handle&> DecodeType;
    explicit ExtendSandboxForFileFromOpenPanel(const WebKit::SandboxExtension::Handle& sandboxExtensionHandle)
        : CoreIPC::Arguments1<const WebKit::SandboxExtension::Handle&>(sandboxExtensionHandle)
    {
    }
};
#endif

struct AdvanceToNextMisspelling : CoreIPC::Arguments1<bool> {
    static const Kind messageID = AdvanceToNextMisspellingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AdvanceToNextMisspelling"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit AdvanceToNextMisspelling(bool startBeforeSelection)
        : CoreIPC::Arguments1<bool>(startBeforeSelection)
    {
    }
};

struct ChangeSpellingToWord : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ChangeSpellingToWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ChangeSpellingToWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ChangeSpellingToWord(const WTF::String& word)
        : CoreIPC::Arguments1<const WTF::String&>(word)
    {
    }
};

#if USE(APPKIT)
struct UppercaseWord : CoreIPC::Arguments0 {
    static const Kind messageID = UppercaseWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UppercaseWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(APPKIT)
struct LowercaseWord : CoreIPC::Arguments0 {
    static const Kind messageID = LowercaseWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LowercaseWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(APPKIT)
struct CapitalizeWord : CoreIPC::Arguments0 {
    static const Kind messageID = CapitalizeWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CapitalizeWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(APPKIT)
struct SetSmartInsertDeleteEnabled : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetSmartInsertDeleteEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetSmartInsertDeleteEnabled"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetSmartInsertDeleteEnabled(bool isSmartInsertDeleteEnabled)
        : CoreIPC::Arguments1<bool>(isSmartInsertDeleteEnabled)
    {
    }
};
#endif

#if ENABLE(GEOLOCATION)
struct DidReceiveGeolocationPermissionDecision : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = DidReceiveGeolocationPermissionDecisionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveGeolocationPermissionDecision"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    DidReceiveGeolocationPermissionDecision(uint64_t geolocationID, bool allowed)
        : CoreIPC::Arguments2<uint64_t, bool>(geolocationID, allowed)
    {
    }
};
#endif

struct DidReceiveNotificationPermissionDecision : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = DidReceiveNotificationPermissionDecisionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveNotificationPermissionDecision"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    DidReceiveNotificationPermissionDecision(uint64_t notificationID, bool allowed)
        : CoreIPC::Arguments2<uint64_t, bool>(notificationID, allowed)
    {
    }
};

struct SetWindowResizerSize : CoreIPC::Arguments1<const WebCore::IntSize&> {
    static const Kind messageID = SetWindowResizerSizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetWindowResizerSize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntSize&> DecodeType;
    explicit SetWindowResizerSize(const WebCore::IntSize& intersectsView)
        : CoreIPC::Arguments1<const WebCore::IntSize&>(intersectsView)
    {
    }
};

struct BeginPrinting : CoreIPC::Arguments2<uint64_t, const WebKit::PrintInfo&> {
    static const Kind messageID = BeginPrintingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BeginPrinting"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::PrintInfo&> DecodeType;
    BeginPrinting(uint64_t frameID, const WebKit::PrintInfo& printInfo)
        : CoreIPC::Arguments2<uint64_t, const WebKit::PrintInfo&>(frameID, printInfo)
    {
    }
};

struct EndPrinting : CoreIPC::Arguments0 {
    static const Kind messageID = EndPrintingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("EndPrinting"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ComputePagesForPrinting : CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t> {
    static const Kind messageID = ComputePagesForPrintingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ComputePagesForPrinting"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t> DecodeType;
    ComputePagesForPrinting(uint64_t frameID, const WebKit::PrintInfo& printInfo, uint64_t callbackID)
        : CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t>(frameID, printInfo, callbackID)
    {
    }
};

#if PLATFORM(MAC) || PLATFORM(WIN)
struct DrawRectToImage : CoreIPC::Arguments4<uint64_t, const WebKit::PrintInfo&, const WebCore::IntRect&, uint64_t> {
    static const Kind messageID = DrawRectToImageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DrawRectToImage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t, const WebKit::PrintInfo&, const WebCore::IntRect&, uint64_t> DecodeType;
    DrawRectToImage(uint64_t frameID, const WebKit::PrintInfo& printInfo, const WebCore::IntRect& rect, uint64_t callbackID)
        : CoreIPC::Arguments4<uint64_t, const WebKit::PrintInfo&, const WebCore::IntRect&, uint64_t>(frameID, printInfo, rect, callbackID)
    {
    }
};
#endif

#if PLATFORM(MAC) || PLATFORM(WIN)
struct DrawPagesToPDF : CoreIPC::Arguments5<uint64_t, const WebKit::PrintInfo&, uint32_t, uint32_t, uint64_t> {
    static const Kind messageID = DrawPagesToPDFID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DrawPagesToPDF"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<uint64_t, const WebKit::PrintInfo&, uint32_t, uint32_t, uint64_t> DecodeType;
    DrawPagesToPDF(uint64_t frameID, const WebKit::PrintInfo& printInfo, uint32_t first, uint32_t count, uint64_t callbackID)
        : CoreIPC::Arguments5<uint64_t, const WebKit::PrintInfo&, uint32_t, uint32_t, uint64_t>(frameID, printInfo, first, count, callbackID)
    {
    }
};
#endif

#if PLATFORM(GTK)
struct DrawPagesForPrinting : CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t> {
    static const Kind messageID = DrawPagesForPrintingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DrawPagesForPrinting"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t> DecodeType;
    DrawPagesForPrinting(uint64_t frameID, const WebKit::PrintInfo& printInfo, uint64_t callbackID)
        : CoreIPC::Arguments3<uint64_t, const WebKit::PrintInfo&, uint64_t>(frameID, printInfo, callbackID)
    {
    }
};
#endif

struct SetMediaVolume : CoreIPC::Arguments1<float> {
    static const Kind messageID = SetMediaVolumeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetMediaVolume"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<float> DecodeType;
    explicit SetMediaVolume(float volume)
        : CoreIPC::Arguments1<float>(volume)
    {
    }
};

struct SetMayStartMediaWhenInWindow : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetMayStartMediaWhenInWindowID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetMayStartMediaWhenInWindow"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetMayStartMediaWhenInWindow(bool mayStartMedia)
        : CoreIPC::Arguments1<bool>(mayStartMedia)
    {
    }
};

struct SetMemoryCacheMessagesEnabled : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetMemoryCacheMessagesEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetMemoryCacheMessagesEnabled"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetMemoryCacheMessagesEnabled(bool memoryCacheMessagesEnabled)
        : CoreIPC::Arguments1<bool>(memoryCacheMessagesEnabled)
    {
    }
};

struct Dummy : CoreIPC::Arguments0 {
    static const Kind messageID = DummyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Dummy"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetCanRunBeforeUnloadConfirmPanel : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetCanRunBeforeUnloadConfirmPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCanRunBeforeUnloadConfirmPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetCanRunBeforeUnloadConfirmPanel(bool canRunBeforeUnloadConfirmPanel)
        : CoreIPC::Arguments1<bool>(canRunBeforeUnloadConfirmPanel)
    {
    }
};

struct SetCanRunModal : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetCanRunModalID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCanRunModal"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetCanRunModal(bool canRunModal)
        : CoreIPC::Arguments1<bool>(canRunModal)
    {
    }
};

#if ENABLE(WEB_INTENTS)
struct DeliverIntentToFrame : CoreIPC::Arguments2<uint64_t, const WebKit::IntentData&> {
    static const Kind messageID = DeliverIntentToFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeliverIntentToFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::IntentData&> DecodeType;
    DeliverIntentToFrame(uint64_t frameID, const WebKit::IntentData& intentData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::IntentData&>(frameID, intentData)
    {
    }
};
#endif

#if PLATFORM(EFL)
struct SetThemePath : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetThemePathID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetThemePath"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetThemePath(const WTF::String& themePath)
        : CoreIPC::Arguments1<const WTF::String&>(themePath)
    {
    }
};
#endif

#if USE(TILED_BACKING_STORE)
struct CommitPageTransitionViewport : CoreIPC::Arguments0 {
    static const Kind messageID = CommitPageTransitionViewportID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CommitPageTransitionViewport"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(QT)
struct SetComposition : CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t> {
    static const Kind messageID = SetCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t> DecodeType;
    SetComposition(const WTF::String& text, const WTF::Vector<WebCore::CompositionUnderline>& underlines, uint64_t selectionStart, uint64_t selectionEnd, uint64_t replacementRangeStart, uint64_t replacementRangeEnd)
        : CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t>(text, underlines, selectionStart, selectionEnd, replacementRangeStart, replacementRangeEnd)
    {
    }
};
#endif

#if PLATFORM(QT)
struct ConfirmComposition : CoreIPC::Arguments3<const WTF::String&, int64_t, int64_t> {
    static const Kind messageID = ConfirmCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ConfirmComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, int64_t, int64_t> DecodeType;
    ConfirmComposition(const WTF::String& text, int64_t selectionStart, int64_t selectionLength)
        : CoreIPC::Arguments3<const WTF::String&, int64_t, int64_t>(text, selectionStart, selectionLength)
    {
    }
};
#endif

#if PLATFORM(QT)
struct CancelComposition : CoreIPC::Arguments0 {
    static const Kind messageID = CancelCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CancelComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct SendComplexTextInputToPlugin : CoreIPC::Arguments2<uint64_t, const String&> {
    static const Kind messageID = SendComplexTextInputToPluginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SendComplexTextInputToPlugin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const String&> DecodeType;
    SendComplexTextInputToPlugin(uint64_t pluginComplexTextInputIdentifier, const String& textInput)
        : CoreIPC::Arguments2<uint64_t, const String&>(pluginComplexTextInputIdentifier, textInput)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetWindowIsVisible : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetWindowIsVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetWindowIsVisible"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetWindowIsVisible(bool windowIsVisible)
        : CoreIPC::Arguments1<bool>(windowIsVisible)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct WindowAndViewFramesChanged : CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, const WebCore::IntPoint&> {
    static const Kind messageID = WindowAndViewFramesChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowAndViewFramesChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, const WebCore::IntPoint&> DecodeType;
    WindowAndViewFramesChanged(const WebCore::IntRect& windowFrameInScreenCoordinates, const WebCore::IntRect& viewFrameInWindowCoordinates, const WebCore::IntPoint& accessibilityViewCoordinates)
        : CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, const WebCore::IntPoint&>(windowFrameInScreenCoordinates, viewFrameInWindowCoordinates, accessibilityViewCoordinates)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct RegisterUIProcessAccessibilityTokens : CoreIPC::Arguments2<const CoreIPC::DataReference&, const CoreIPC::DataReference&> {
    static const Kind messageID = RegisterUIProcessAccessibilityTokensID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterUIProcessAccessibilityTokens"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const CoreIPC::DataReference&, const CoreIPC::DataReference&> DecodeType;
    RegisterUIProcessAccessibilityTokens(const CoreIPC::DataReference& elemenToken, const CoreIPC::DataReference& windowToken)
        : CoreIPC::Arguments2<const CoreIPC::DataReference&, const CoreIPC::DataReference&>(elemenToken, windowToken)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetStringSelectionForPasteboard : CoreIPC::Arguments0 {
    static const Kind messageID = GetStringSelectionForPasteboardID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetStringSelectionForPasteboard"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct GetDataSelectionForPasteboard : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = GetDataSelectionForPasteboardID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetDataSelectionForPasteboard"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<WebKit::SharedMemory::Handle&, uint64_t&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit GetDataSelectionForPasteboard(const WTF::String& pasteboardType)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardType)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct ReadSelectionFromPasteboard : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ReadSelectionFromPasteboardID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ReadSelectionFromPasteboard"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ReadSelectionFromPasteboard(const WTF::String& pasteboardName)
        : CoreIPC::Arguments1<const WTF::String&>(pasteboardName)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetComposition : CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t> {
    static const Kind messageID = SetCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetComposition"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t> DecodeType;
    SetComposition(const WTF::String& text, const WTF::Vector<WebCore::CompositionUnderline>& underlines, uint64_t selectionStart, uint64_t selectionEnd, uint64_t replacementRangeStart, uint64_t replacementRangeEnd)
        : CoreIPC::Arguments6<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t, uint64_t, uint64_t, uint64_t>(text, underlines, selectionStart, selectionEnd, replacementRangeStart, replacementRangeEnd)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct ConfirmComposition : CoreIPC::Arguments0 {
    static const Kind messageID = ConfirmCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ConfirmComposition"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct CancelComposition : CoreIPC::Arguments0 {
    static const Kind messageID = CancelCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CancelComposition"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct InsertText : CoreIPC::Arguments3<const WTF::String&, uint64_t, uint64_t> {
    static const Kind messageID = InsertTextID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("InsertText"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<bool&, WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments3<const WTF::String&, uint64_t, uint64_t> DecodeType;
    InsertText(const WTF::String& text, uint64_t replacementRangeStart, uint64_t replacementRangeEnd)
        : CoreIPC::Arguments3<const WTF::String&, uint64_t, uint64_t>(text, replacementRangeStart, replacementRangeEnd)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetMarkedRange : CoreIPC::Arguments0 {
    static const Kind messageID = GetMarkedRangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetMarkedRange"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<uint64_t&, uint64_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct GetSelectedRange : CoreIPC::Arguments0 {
    static const Kind messageID = GetSelectedRangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetSelectedRange"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<uint64_t&, uint64_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct GetAttributedSubstringFromRange : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = GetAttributedSubstringFromRangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetAttributedSubstringFromRange"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::AttributedString&> Reply;
    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    GetAttributedSubstringFromRange(uint64_t location, uint64_t length)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(location, length)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct CharacterIndexForPoint : CoreIPC::Arguments1<const WebCore::IntPoint&> {
    static const Kind messageID = CharacterIndexForPointID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CharacterIndexForPoint"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments1<const WebCore::IntPoint&> DecodeType;
    explicit CharacterIndexForPoint(const WebCore::IntPoint& point)
        : CoreIPC::Arguments1<const WebCore::IntPoint&>(point)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct FirstRectForCharacterRange : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = FirstRectForCharacterRangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FirstRectForCharacterRange"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::IntRect&> Reply;
    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    FirstRectForCharacterRange(uint64_t location, uint64_t length)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(location, length)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct ExecuteKeypressCommands : CoreIPC::Arguments1<const Vector<WebCore::KeypressCommand>&> {
    static const Kind messageID = ExecuteKeypressCommandsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExecuteKeypressCommands"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<bool&, WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments1<const Vector<WebCore::KeypressCommand>&> DecodeType;
    explicit ExecuteKeypressCommands(const Vector<WebCore::KeypressCommand>& savedCommands)
        : CoreIPC::Arguments1<const Vector<WebCore::KeypressCommand>&>(savedCommands)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct ShouldDelayWindowOrderingEvent : CoreIPC::Arguments1<const WebKit::WebMouseEvent&> {
    static const Kind messageID = ShouldDelayWindowOrderingEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShouldDelayWindowOrderingEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::WebMouseEvent&> DecodeType;
    explicit ShouldDelayWindowOrderingEvent(const WebKit::WebMouseEvent& event)
        : CoreIPC::Arguments1<const WebKit::WebMouseEvent&>(event)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct AcceptsFirstMouse : CoreIPC::Arguments2<const int&, const WebKit::WebMouseEvent&> {
    static const Kind messageID = AcceptsFirstMouseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AcceptsFirstMouse"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments2<const int&, const WebKit::WebMouseEvent&> DecodeType;
    AcceptsFirstMouse(const int& eventNumber, const WebKit::WebMouseEvent& event)
        : CoreIPC::Arguments2<const int&, const WebKit::WebMouseEvent&>(eventNumber, event)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct InsertDictatedText : CoreIPC::Arguments4<const WTF::String&, uint64_t, uint64_t, const WTF::Vector<WebCore::DictationAlternative>&> {
    static const Kind messageID = InsertDictatedTextID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("InsertDictatedText"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<bool&, WebKit::EditorState&> Reply;
    typedef CoreIPC::Arguments4<const WTF::String&, uint64_t, uint64_t, const WTF::Vector<WebCore::DictationAlternative>&> DecodeType;
    InsertDictatedText(const WTF::String& text, uint64_t replacementRangeStart, uint64_t replacementRangeEnd, const WTF::Vector<WebCore::DictationAlternative>& dictationAlternatives)
        : CoreIPC::Arguments4<const WTF::String&, uint64_t, uint64_t, const WTF::Vector<WebCore::DictationAlternative>&>(text, replacementRangeStart, replacementRangeEnd, dictationAlternatives)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct ConfirmComposition : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ConfirmCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ConfirmComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ConfirmComposition(const WTF::String& compositionString)
        : CoreIPC::Arguments1<const WTF::String&>(compositionString)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct SetComposition : CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t> {
    static const Kind messageID = SetCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t> DecodeType;
    SetComposition(const WTF::String& compositionString, const WTF::Vector<WebCore::CompositionUnderline>& underlines, uint64_t cursorPosition)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t>(compositionString, underlines, cursorPosition)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct FirstRectForCharacterInSelectedRange : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = FirstRectForCharacterInSelectedRangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FirstRectForCharacterInSelectedRange"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::IntRect&> Reply;
    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit FirstRectForCharacterInSelectedRange(uint64_t characterPosition)
        : CoreIPC::Arguments1<uint64_t>(characterPosition)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct GetSelectedText : CoreIPC::Arguments0 {
    static const Kind messageID = GetSelectedTextID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetSelectedText"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(WIN)
struct GestureWillBegin : CoreIPC::Arguments1<const WebCore::IntPoint&> {
    static const Kind messageID = GestureWillBeginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GestureWillBegin"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WebCore::IntPoint&> DecodeType;
    explicit GestureWillBegin(const WebCore::IntPoint& point)
        : CoreIPC::Arguments1<const WebCore::IntPoint&>(point)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct GestureDidScroll : CoreIPC::Arguments1<const WebCore::IntSize&> {
    static const Kind messageID = GestureDidScrollID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GestureDidScroll"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntSize&> DecodeType;
    explicit GestureDidScroll(const WebCore::IntSize& size)
        : CoreIPC::Arguments1<const WebCore::IntSize&>(size)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct GestureDidEnd : CoreIPC::Arguments0 {
    static const Kind messageID = GestureDidEndID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GestureDidEnd"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(EFL)
struct ConfirmComposition : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ConfirmCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ConfirmComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ConfirmComposition(const WTF::String& compositionString)
        : CoreIPC::Arguments1<const WTF::String&>(compositionString)
    {
    }
};
#endif

#if PLATFORM(EFL)
struct SetComposition : CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t> {
    static const Kind messageID = SetCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t> DecodeType;
    SetComposition(const WTF::String& compositionString, const WTF::Vector<WebCore::CompositionUnderline>& underlines, uint64_t cursorPosition)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::Vector<WebCore::CompositionUnderline>&, uint64_t>(compositionString, underlines, cursorPosition)
    {
    }
};
#endif

#if PLATFORM(EFL)
struct CancelComposition : CoreIPC::Arguments0 {
    static const Kind messageID = CancelCompositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CancelComposition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(QT)
struct FindZoomableAreaForPoint : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&> {
    static const Kind messageID = FindZoomableAreaForPointID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FindZoomableAreaForPoint"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&> DecodeType;
    FindZoomableAreaForPoint(const WebCore::IntPoint& point, const WebCore::IntSize& area)
        : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntSize&>(point, area)
    {
    }
};
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
struct HandleAlternativeTextUIResult : CoreIPC::Arguments1<const String&> {
    static const Kind messageID = HandleAlternativeTextUIResultID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleAlternativeTextUIResult"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const String&> DecodeType;
    explicit HandleAlternativeTextUIResult(const String& result)
        : CoreIPC::Arguments1<const String&>(result)
    {
    }
};
#endif

#if ENABLE(PAGE_VISIBILITY_API) || ENABLE(HIDDEN_PAGE_DOM_TIMER_THROTTLING)
struct SetVisibilityState : CoreIPC::Arguments2<const int&, bool> {
    static const Kind messageID = SetVisibilityStateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetVisibilityState"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const int&, bool> DecodeType;
    SetVisibilityState(const int& visibilityState, bool isInitialState)
        : CoreIPC::Arguments2<const int&, bool>(visibilityState, isInitialState)
    {
    }
};
#endif

#if PLATFORM(GTK) && USE(TEXTURE_MAPPER_GL)
struct SetAcceleratedCompositingWindowId : CoreIPC::Arguments1<int64_t> {
    static const Kind messageID = SetAcceleratedCompositingWindowIdID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetAcceleratedCompositingWindowId"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int64_t> DecodeType;
    explicit SetAcceleratedCompositingWindowId(int64_t id)
        : CoreIPC::Arguments1<int64_t>(id)
    {
    }
};
#endif

} // namespace WebPage
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebPage::Kind> {
    static const MessageClass messageClass = MessageClassWebPage;
};

} // namespace CoreIPC

#endif // WebPageMessages_h
