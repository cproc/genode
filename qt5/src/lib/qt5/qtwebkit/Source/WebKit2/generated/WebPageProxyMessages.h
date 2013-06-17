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

#ifndef WebPageProxyMessages_h
#define WebPageProxyMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "SandboxExtension.h"
#include "ShareableBitmap.h"
#include "SharedMemory.h"
#include "StringReference.h"
#include "WebContextMenuItemData.h"
#include "WebHitTestResult.h"
#include "WebPopupItem.h"
#include <WebCore/FloatRect.h>
#include <WebCore/IntRect.h>
#include <WebCore/KeyboardEvent.h>
#include <WebCore/TextCheckerClient.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace CoreIPC {
    class DataReference;
}

namespace WTF {
    class String;
}

namespace WebCore {
    class Cursor;
    class ProtectionSpace;
    struct GrammarDetail;
    struct WindowFeatures;
    class Color;
    class IntSize;
    class ResourceResponse;
    class DragData;
    struct ViewportAttributes;
    class ResourceRequest;
    class FloatRect;
    class IntRect;
    class IntPoint;
    struct DragSession;
    class ResourceError;
    class AuthenticationChallenge;
    struct FileChooserSettings;
}

namespace WebKit {
    struct PlatformPopupMenuData;
    class IntentServiceInfo;
    class PlatformCertificateInfo;
    class QtNetworkRequestData;
    struct EditorState;
    class StringPairVector;
    struct WebPageCreationParameters;
    class IntentData;
    struct DictionaryPopupInfo;
    class InjectedBundleUserMessageEncoder;
    struct WindowGeometry;
    struct AttributedString;
}

namespace Messages {
namespace WebPageProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebPageProxy");
}

enum Kind {
    CreateNewPageID,
    ShowPageID,
    ClosePageID,
    RunJavaScriptAlertID,
    RunJavaScriptConfirmID,
    RunJavaScriptPromptID,
    ShouldInterruptJavaScriptID,
    MouseDidMoveOverElementID,
    UnavailablePluginButtonClickedID,
    DidChangeViewportPropertiesID,
    DidReceiveEventID,
    StopResponsivenessTimerID,
    SetCursorID,
    SetCursorHiddenUntilMouseMovesID,
    SetStatusTextID,
    SetToolTipID,
    SetFocusID,
    TakeFocusID,
    FocusedFrameChangedID,
    FrameSetLargestFrameChangedID,
    SetRenderTreeSizeID,
    SetToolbarsAreVisibleID,
    GetToolbarsAreVisibleID,
    SetMenuBarIsVisibleID,
    GetMenuBarIsVisibleID,
    SetStatusBarIsVisibleID,
    GetStatusBarIsVisibleID,
    SetIsResizableID,
    GetIsResizableID,
    SetWindowFrameID,
    GetWindowFrameID,
    ScreenToWindowID,
    WindowToScreenID,
    RunBeforeUnloadConfirmPanelID,
    PageDidScrollID,
    RunOpenPanelID,
    PrintFrameID,
    RunModalID,
    NotifyScrollerThumbIsVisibleInRectID,
    RecommendedScrollbarStyleDidChangeID,
    DidChangeScrollbarsForMainFrameID,
    DidChangeScrollOffsetPinningForMainFrameID,
    DidChangePageCountID,
    DidFailToInitializePluginID,
    DidBlockInsecurePluginVersionID,
    SetCanShortCircuitHorizontalWheelEventsID,
#if PLATFORM(EFL)
    HandleInputMethodKeydownID,
#endif
#if USE(TILED_BACKING_STORE)
    PageDidRequestScrollID,
#endif
#if USE(TILED_BACKING_STORE)
    PageTransitionViewportReadyID,
#endif
#if PLATFORM(QT)
    DidFindZoomableAreaID,
#endif
#if PLATFORM(QT)
    AuthenticationRequiredRequestID,
#endif
#if PLATFORM(QT)
    CertificateVerificationRequestID,
#endif
#if PLATFORM(QT)
    ProxyAuthenticationRequiredRequestID,
#endif
#if PLATFORM(QT) || PLATFORM(EFL)
    DidChangeContentsSizeID,
#endif
#if ENABLE(TOUCH_EVENTS)
    NeedTouchEventsID,
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    ShowColorChooserID,
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    SetColorChooserColorID,
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    EndColorChooserID,
#endif
    DecidePolicyForResponseID,
    DecidePolicyForNavigationActionID,
    DecidePolicyForNewWindowActionID,
    UnableToImplementPolicyID,
#if ENABLE(WEB_INTENTS)
    DidReceiveIntentForFrameID,
#endif
#if ENABLE(WEB_INTENTS_TAG)
    RegisterIntentServiceForFrameID,
#endif
    DidChangeProgressID,
    DidFinishProgressID,
    DidStartProgressID,
    DidCreateMainFrameID,
    DidCreateSubframeID,
    DidSaveFrameToPageCacheID,
    DidRestoreFrameFromPageCacheID,
    DidCommitLoadForFrameID,
    DidFailLoadForFrameID,
    DidFailProvisionalLoadForFrameID,
    DidFinishDocumentLoadForFrameID,
    DidFinishLoadForFrameID,
    DidFirstLayoutForFrameID,
    DidFirstVisuallyNonEmptyLayoutForFrameID,
    DidNewFirstVisuallyNonEmptyLayoutID,
    DidLayoutID,
    DidReceiveServerRedirectForProvisionalLoadForFrameID,
    DidRemoveFrameFromHierarchyID,
    DidStartProvisionalLoadForFrameID,
    DidReceiveTitleForFrameID,
    DidDisplayInsecureContentForFrameID,
    DidRunInsecureContentForFrameID,
    DidDetectXSSForFrameID,
    DidSameDocumentNavigationForFrameID,
    FrameDidBecomeFrameSetID,
    DidInitiateLoadForResourceID,
    DidSendRequestForResourceID,
    DidReceiveResponseForResourceID,
    DidReceiveContentLengthForResourceID,
    DidFinishLoadForResourceID,
    DidFailLoadForResourceID,
#if PLATFORM(QT)
    ResolveApplicationSchemeRequestID,
#endif
    DidFinishLoadingDataForCustomRepresentationID,
    WillSubmitFormID,
    VoidCallbackID,
    DataCallbackID,
    ImageCallbackID,
    StringCallbackID,
    ScriptValueCallbackID,
    ComputedPagesCallbackID,
    ValidateCommandCallbackID,
#if PLATFORM(GTK)
    PrintFinishedCallbackID,
#endif
    PageScaleFactorDidChangeID,
#if PLATFORM(GTK)
    GetEditorCommandsForKeyEventID,
#endif
#if PLATFORM(GTK)
    BindAccessibilityTreeID,
#endif
    BackForwardAddItemID,
    BackForwardGoToItemID,
    BackForwardItemAtIndexID,
    BackForwardBackListCountID,
    BackForwardForwardListCountID,
    BackForwardClearID,
    ShouldGoToBackForwardListItemID,
    WillGoToBackForwardListItemID,
    RegisterEditCommandForUndoID,
    ClearAllEditCommandsID,
    CanUndoRedoID,
    ExecuteUndoRedoID,
    EditorStateChangedID,
#if PLATFORM(WIN)
    DidChangeCompositionSelectionID,
#endif
#if PLATFORM(QT)
    WillSetInputMethodStateID,
#endif
    DidCountStringMatchesID,
    SetFindIndicatorID,
    DidFindStringID,
    DidFailToFindStringID,
#if PLATFORM(WIN)
    DidInstallOrUninstallPageOverlayID,
#endif
    ShowPopupMenuID,
    HidePopupMenuID,
#if PLATFORM(WIN)
    SetPopupMenuSelectedIndexID,
#endif
#if ENABLE(CONTEXT_MENUS)
    ShowContextMenuID,
#endif
    CanAuthenticateAgainstProtectionSpaceInFrameID,
    DidReceiveAuthenticationChallengeID,
    ExceededDatabaseQuotaID,
    RequestGeolocationPermissionForFrameID,
    RequestNotificationPermissionID,
    ShowNotificationID,
#if USE(UNIFIED_TEXT_CHECKING)
    CheckTextOfParagraphID,
#endif
    CheckSpellingOfStringID,
    CheckGrammarOfStringID,
    SpellingUIIsShowingID,
    UpdateSpellingUIWithMisspelledWordID,
    UpdateSpellingUIWithGrammarStringID,
    GetGuessesForWordID,
    LearnWordID,
    IgnoreWordID,
#if ENABLE(DRAG_SUPPORT)
    DidPerformDragControllerActionID,
#endif
#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
    SetDragImageID,
#endif
#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
    SetPromisedDataID,
#endif
#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
    StartDragDropID,
#endif
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
    StartDragID,
#endif
#if PLATFORM(MAC)
    DidPerformDictionaryLookupID,
#endif
#if PLATFORM(MAC)
    InterpretQueuedKeyEventID,
#endif
#if PLATFORM(MAC)
    ExecuteSavedCommandBySelectorID,
#endif
#if PLATFORM(MAC)
    RegisterWebProcessAccessibilityTokenID,
#endif
#if PLATFORM(MAC)
    PluginFocusOrWindowFocusChangedID,
#endif
#if PLATFORM(MAC)
    SetPluginComplexTextInputStateID,
#endif
#if PLATFORM(MAC)
    GetIsSpeakingID,
#endif
#if PLATFORM(MAC)
    SpeakID,
#endif
#if PLATFORM(MAC)
    StopSpeakingID,
#endif
#if PLATFORM(MAC)
    MakeFirstResponderID,
#endif
#if PLATFORM(MAC)
    SearchWithSpotlightID,
#endif
#if PLATFORM(MAC)
    SubstitutionsPanelIsShowingID,
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    ShowCorrectionPanelID,
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    DismissCorrectionPanelID,
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    DismissCorrectionPanelSoonID,
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    RecordAutocorrectionResponseID,
#endif
#if USE(DICTATION_ALTERNATIVES)
    ShowDictationAlternativeUIID,
#endif
#if USE(DICTATION_ALTERNATIVES)
    DismissDictationAlternativeUIID,
#endif
#if USE(DICTATION_ALTERNATIVES)
    RemoveDictationAlternativesID,
#endif
#if USE(DICTATION_ALTERNATIVES)
    DictationAlternativesID,
#endif
#if PLATFORM(WIN)
    SetGestureReachedScrollingLimitID,
#endif
#if PLATFORM(WIN)
    ScheduleChildWindowGeometryUpdateID,
#endif
#if PLUGIN_ARCHITECTURE(X11)
    CreatePluginContainerID,
#endif
#if PLUGIN_ARCHITECTURE(X11)
    WindowedPluginGeometryDidChangeID,
#endif
    SaveRecentSearchesID,
    LoadRecentSearchesID,
#if USE(SOUP)
    DidReceiveURIRequestID,
#endif
    SavePDFToFileInDownloadsFolderID,
};

struct CreateNewPage : CoreIPC::Arguments4<const WebCore::ResourceRequest&, const WebCore::WindowFeatures&, uint32_t, int32_t> {
    static const Kind messageID = CreateNewPageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateNewPage"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<uint64_t&, WebKit::WebPageCreationParameters&> Reply;
    typedef CoreIPC::Arguments4<const WebCore::ResourceRequest&, const WebCore::WindowFeatures&, uint32_t, int32_t> DecodeType;
    CreateNewPage(const WebCore::ResourceRequest& request, const WebCore::WindowFeatures& windowFeatures, uint32_t modifiers, int32_t mouseButton)
        : CoreIPC::Arguments4<const WebCore::ResourceRequest&, const WebCore::WindowFeatures&, uint32_t, int32_t>(request, windowFeatures, modifiers, mouseButton)
    {
    }
};

struct ShowPage : CoreIPC::Arguments0 {
    static const Kind messageID = ShowPageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowPage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ClosePage : CoreIPC::Arguments1<bool> {
    static const Kind messageID = ClosePageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClosePage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit ClosePage(bool stopResponsivenessTimer)
        : CoreIPC::Arguments1<bool>(stopResponsivenessTimer)
    {
    }
};

struct RunJavaScriptAlert : CoreIPC::Arguments2<uint64_t, const WTF::String&> {
    static const Kind messageID = RunJavaScriptAlertID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunJavaScriptAlert"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments0 Reply;
    typedef CoreIPC::Arguments2<uint64_t, const WTF::String&> DecodeType;
    RunJavaScriptAlert(uint64_t frameID, const WTF::String& message)
        : CoreIPC::Arguments2<uint64_t, const WTF::String&>(frameID, message)
    {
    }
};

struct RunJavaScriptConfirm : CoreIPC::Arguments2<uint64_t, const WTF::String&> {
    static const Kind messageID = RunJavaScriptConfirmID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunJavaScriptConfirm"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments2<uint64_t, const WTF::String&> DecodeType;
    RunJavaScriptConfirm(uint64_t frameID, const WTF::String& message)
        : CoreIPC::Arguments2<uint64_t, const WTF::String&>(frameID, message)
    {
    }
};

struct RunJavaScriptPrompt : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WTF::String&> {
    static const Kind messageID = RunJavaScriptPromptID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunJavaScriptPrompt"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WTF::String&> Reply;
    typedef CoreIPC::Arguments3<uint64_t, const WTF::String&, const WTF::String&> DecodeType;
    RunJavaScriptPrompt(uint64_t frameID, const WTF::String& message, const WTF::String& defaultValue)
        : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WTF::String&>(frameID, message, defaultValue)
    {
    }
};

struct ShouldInterruptJavaScript : CoreIPC::Arguments0 {
    static const Kind messageID = ShouldInterruptJavaScriptID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShouldInterruptJavaScript"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct MouseDidMoveOverElement : CoreIPC::Arguments3<const WebKit::WebHitTestResult::Data&, uint32_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = MouseDidMoveOverElementID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("MouseDidMoveOverElement"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<WebKit::WebHitTestResult::Data&, uint32_t&> DecodeType;
    MouseDidMoveOverElement(const WebKit::WebHitTestResult::Data& hitTestResultData, uint32_t modifiers, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<const WebKit::WebHitTestResult::Data&, uint32_t, const WebKit::InjectedBundleUserMessageEncoder&>(hitTestResultData, modifiers, userData)
    {
    }
};

struct UnavailablePluginButtonClicked : CoreIPC::Arguments4<uint32_t, const WTF::String&, const WTF::String&, const WTF::String&> {
    static const Kind messageID = UnavailablePluginButtonClickedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UnavailablePluginButtonClicked"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint32_t, const WTF::String&, const WTF::String&, const WTF::String&> DecodeType;
    UnavailablePluginButtonClicked(uint32_t pluginUnavailabilityReason, const WTF::String& mimeType, const WTF::String& url, const WTF::String& pluginsPageURL)
        : CoreIPC::Arguments4<uint32_t, const WTF::String&, const WTF::String&, const WTF::String&>(pluginUnavailabilityReason, mimeType, url, pluginsPageURL)
    {
    }
};

struct DidChangeViewportProperties : CoreIPC::Arguments1<const WebCore::ViewportAttributes&> {
    static const Kind messageID = DidChangeViewportPropertiesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeViewportProperties"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::ViewportAttributes&> DecodeType;
    explicit DidChangeViewportProperties(const WebCore::ViewportAttributes& attributes)
        : CoreIPC::Arguments1<const WebCore::ViewportAttributes&>(attributes)
    {
    }
};

struct DidReceiveEvent : CoreIPC::Arguments2<uint32_t, bool> {
    static const Kind messageID = DidReceiveEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveEvent"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, bool> DecodeType;
    DidReceiveEvent(uint32_t type, bool handled)
        : CoreIPC::Arguments2<uint32_t, bool>(type, handled)
    {
    }
};

struct StopResponsivenessTimer : CoreIPC::Arguments0 {
    static const Kind messageID = StopResponsivenessTimerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopResponsivenessTimer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetCursor : CoreIPC::Arguments1<const WebCore::Cursor&> {
    static const Kind messageID = SetCursorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCursor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::Cursor&> DecodeType;
    explicit SetCursor(const WebCore::Cursor& cursor)
        : CoreIPC::Arguments1<const WebCore::Cursor&>(cursor)
    {
    }
};

struct SetCursorHiddenUntilMouseMoves : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetCursorHiddenUntilMouseMovesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCursorHiddenUntilMouseMoves"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetCursorHiddenUntilMouseMoves(bool hiddenUntilMouseMoves)
        : CoreIPC::Arguments1<bool>(hiddenUntilMouseMoves)
    {
    }
};

struct SetStatusText : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetStatusTextID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetStatusText"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetStatusText(const WTF::String& statusText)
        : CoreIPC::Arguments1<const WTF::String&>(statusText)
    {
    }
};

struct SetToolTip : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetToolTipID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetToolTip"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetToolTip(const WTF::String& toolTip)
        : CoreIPC::Arguments1<const WTF::String&>(toolTip)
    {
    }
};

struct SetFocus : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetFocusID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetFocus"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetFocus(bool focused)
        : CoreIPC::Arguments1<bool>(focused)
    {
    }
};

struct TakeFocus : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = TakeFocusID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("TakeFocus"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit TakeFocus(uint32_t direction)
        : CoreIPC::Arguments1<uint32_t>(direction)
    {
    }
};

struct FocusedFrameChanged : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = FocusedFrameChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FocusedFrameChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit FocusedFrameChanged(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct FrameSetLargestFrameChanged : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = FrameSetLargestFrameChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FrameSetLargestFrameChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit FrameSetLargestFrameChanged(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct SetRenderTreeSize : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = SetRenderTreeSizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetRenderTreeSize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit SetRenderTreeSize(uint64_t treeSize)
        : CoreIPC::Arguments1<uint64_t>(treeSize)
    {
    }
};

struct SetToolbarsAreVisible : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetToolbarsAreVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetToolbarsAreVisible"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetToolbarsAreVisible(bool toolbarsAreVisible)
        : CoreIPC::Arguments1<bool>(toolbarsAreVisible)
    {
    }
};

struct GetToolbarsAreVisible : CoreIPC::Arguments0 {
    static const Kind messageID = GetToolbarsAreVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetToolbarsAreVisible"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetMenuBarIsVisible : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetMenuBarIsVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetMenuBarIsVisible"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetMenuBarIsVisible(bool menuBarIsVisible)
        : CoreIPC::Arguments1<bool>(menuBarIsVisible)
    {
    }
};

struct GetMenuBarIsVisible : CoreIPC::Arguments0 {
    static const Kind messageID = GetMenuBarIsVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetMenuBarIsVisible"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetStatusBarIsVisible : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetStatusBarIsVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetStatusBarIsVisible"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetStatusBarIsVisible(bool statusBarIsVisible)
        : CoreIPC::Arguments1<bool>(statusBarIsVisible)
    {
    }
};

struct GetStatusBarIsVisible : CoreIPC::Arguments0 {
    static const Kind messageID = GetStatusBarIsVisibleID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetStatusBarIsVisible"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetIsResizable : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetIsResizableID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetIsResizable"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetIsResizable(bool isResizable)
        : CoreIPC::Arguments1<bool>(isResizable)
    {
    }
};

struct GetIsResizable : CoreIPC::Arguments0 {
    static const Kind messageID = GetIsResizableID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetIsResizable"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetWindowFrame : CoreIPC::Arguments1<const WebCore::FloatRect&> {
    static const Kind messageID = SetWindowFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetWindowFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::FloatRect&> DecodeType;
    explicit SetWindowFrame(const WebCore::FloatRect& windowFrame)
        : CoreIPC::Arguments1<const WebCore::FloatRect&>(windowFrame)
    {
    }
};

struct GetWindowFrame : CoreIPC::Arguments0 {
    static const Kind messageID = GetWindowFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetWindowFrame"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::FloatRect&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct ScreenToWindow : CoreIPC::Arguments1<const WebCore::IntPoint&> {
    static const Kind messageID = ScreenToWindowID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ScreenToWindow"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::IntPoint&> Reply;
    typedef CoreIPC::Arguments1<const WebCore::IntPoint&> DecodeType;
    explicit ScreenToWindow(const WebCore::IntPoint& screenPoint)
        : CoreIPC::Arguments1<const WebCore::IntPoint&>(screenPoint)
    {
    }
};

struct WindowToScreen : CoreIPC::Arguments1<const WebCore::IntRect&> {
    static const Kind messageID = WindowToScreenID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowToScreen"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebCore::IntRect&> Reply;
    typedef CoreIPC::Arguments1<const WebCore::IntRect&> DecodeType;
    explicit WindowToScreen(const WebCore::IntRect& rect)
        : CoreIPC::Arguments1<const WebCore::IntRect&>(rect)
    {
    }
};

struct RunBeforeUnloadConfirmPanel : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = RunBeforeUnloadConfirmPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunBeforeUnloadConfirmPanel"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    RunBeforeUnloadConfirmPanel(const WTF::String& message, uint64_t frameID)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(message, frameID)
    {
    }
};

struct PageDidScroll : CoreIPC::Arguments0 {
    static const Kind messageID = PageDidScrollID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PageDidScroll"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct RunOpenPanel : CoreIPC::Arguments2<uint64_t, const WebCore::FileChooserSettings&> {
    static const Kind messageID = RunOpenPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunOpenPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebCore::FileChooserSettings&> DecodeType;
    RunOpenPanel(uint64_t frameID, const WebCore::FileChooserSettings& parameters)
        : CoreIPC::Arguments2<uint64_t, const WebCore::FileChooserSettings&>(frameID, parameters)
    {
    }
};

struct PrintFrame : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = PrintFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PrintFrame"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments0 Reply;
    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit PrintFrame(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct RunModal : CoreIPC::Arguments0 {
    static const Kind messageID = RunModalID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RunModal"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct NotifyScrollerThumbIsVisibleInRect : CoreIPC::Arguments1<const WebCore::IntRect&> {
    static const Kind messageID = NotifyScrollerThumbIsVisibleInRectID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("NotifyScrollerThumbIsVisibleInRect"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntRect&> DecodeType;
    explicit NotifyScrollerThumbIsVisibleInRect(const WebCore::IntRect& scrollerThumb)
        : CoreIPC::Arguments1<const WebCore::IntRect&>(scrollerThumb)
    {
    }
};

struct RecommendedScrollbarStyleDidChange : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = RecommendedScrollbarStyleDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RecommendedScrollbarStyleDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit RecommendedScrollbarStyleDidChange(int32_t newStyle)
        : CoreIPC::Arguments1<int32_t>(newStyle)
    {
    }
};

struct DidChangeScrollbarsForMainFrame : CoreIPC::Arguments2<bool, bool> {
    static const Kind messageID = DidChangeScrollbarsForMainFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeScrollbarsForMainFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<bool, bool> DecodeType;
    DidChangeScrollbarsForMainFrame(bool hasHorizontalScrollbar, bool hasVerticalScrollbar)
        : CoreIPC::Arguments2<bool, bool>(hasHorizontalScrollbar, hasVerticalScrollbar)
    {
    }
};

struct DidChangeScrollOffsetPinningForMainFrame : CoreIPC::Arguments4<bool, bool, bool, bool> {
    static const Kind messageID = DidChangeScrollOffsetPinningForMainFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeScrollOffsetPinningForMainFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<bool, bool, bool, bool> DecodeType;
    DidChangeScrollOffsetPinningForMainFrame(bool pinnedToLeftSide, bool pinnedToRightSide, bool pinnedToTopSide, bool pinnedToBottomSide)
        : CoreIPC::Arguments4<bool, bool, bool, bool>(pinnedToLeftSide, pinnedToRightSide, pinnedToTopSide, pinnedToBottomSide)
    {
    }
};

struct DidChangePageCount : CoreIPC::Arguments1<const unsigned&> {
    static const Kind messageID = DidChangePageCountID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangePageCount"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const unsigned&> DecodeType;
    explicit DidChangePageCount(const unsigned& pageCount)
        : CoreIPC::Arguments1<const unsigned&>(pageCount)
    {
    }
};

struct DidFailToInitializePlugin : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DidFailToInitializePluginID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFailToInitializePlugin"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DidFailToInitializePlugin(const WTF::String& mimeType)
        : CoreIPC::Arguments1<const WTF::String&>(mimeType)
    {
    }
};

struct DidBlockInsecurePluginVersion : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = DidBlockInsecurePluginVersionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidBlockInsecurePluginVersion"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    DidBlockInsecurePluginVersion(const WTF::String& mimeType, const WTF::String& urlString)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(mimeType, urlString)
    {
    }
};

struct SetCanShortCircuitHorizontalWheelEvents : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetCanShortCircuitHorizontalWheelEventsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCanShortCircuitHorizontalWheelEvents"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetCanShortCircuitHorizontalWheelEvents(bool canShortCircuitHorizontalWheelEvents)
        : CoreIPC::Arguments1<bool>(canShortCircuitHorizontalWheelEvents)
    {
    }
};

#if PLATFORM(EFL)
struct HandleInputMethodKeydown : CoreIPC::Arguments0 {
    static const Kind messageID = HandleInputMethodKeydownID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HandleInputMethodKeydown"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(TILED_BACKING_STORE)
struct PageDidRequestScroll : CoreIPC::Arguments1<const WebCore::IntPoint&> {
    static const Kind messageID = PageDidRequestScrollID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PageDidRequestScroll"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntPoint&> DecodeType;
    explicit PageDidRequestScroll(const WebCore::IntPoint& point)
        : CoreIPC::Arguments1<const WebCore::IntPoint&>(point)
    {
    }
};
#endif

#if USE(TILED_BACKING_STORE)
struct PageTransitionViewportReady : CoreIPC::Arguments0 {
    static const Kind messageID = PageTransitionViewportReadyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PageTransitionViewportReady"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(QT)
struct DidFindZoomableArea : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntRect&> {
    static const Kind messageID = DidFindZoomableAreaID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFindZoomableArea"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntRect&> DecodeType;
    DidFindZoomableArea(const WebCore::IntPoint& target, const WebCore::IntRect& area)
        : CoreIPC::Arguments2<const WebCore::IntPoint&, const WebCore::IntRect&>(target, area)
    {
    }
};
#endif

#if PLATFORM(QT)
struct AuthenticationRequiredRequest : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> {
    static const Kind messageID = AuthenticationRequiredRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AuthenticationRequiredRequest"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<WTF::String&, WTF::String&> Reply;
    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&> DecodeType;
    AuthenticationRequiredRequest(const WTF::String& hostname, const WTF::String& realm, const WTF::String& prefilledUsername)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const WTF::String&>(hostname, realm, prefilledUsername)
    {
    }
};
#endif

#if PLATFORM(QT)
struct CertificateVerificationRequest : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = CertificateVerificationRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CertificateVerificationRequest"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit CertificateVerificationRequest(const WTF::String& hostname)
        : CoreIPC::Arguments1<const WTF::String&>(hostname)
    {
    }
};
#endif

#if PLATFORM(QT)
struct ProxyAuthenticationRequiredRequest : CoreIPC::Arguments3<const WTF::String&, uint16_t, const WTF::String&> {
    static const Kind messageID = ProxyAuthenticationRequiredRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ProxyAuthenticationRequiredRequest"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<WTF::String&, WTF::String&> Reply;
    typedef CoreIPC::Arguments3<const WTF::String&, uint16_t, const WTF::String&> DecodeType;
    ProxyAuthenticationRequiredRequest(const WTF::String& hostname, uint16_t port, const WTF::String& prefilledUsername)
        : CoreIPC::Arguments3<const WTF::String&, uint16_t, const WTF::String&>(hostname, port, prefilledUsername)
    {
    }
};
#endif

#if PLATFORM(QT) || PLATFORM(EFL)
struct DidChangeContentsSize : CoreIPC::Arguments1<const WebCore::IntSize&> {
    static const Kind messageID = DidChangeContentsSizeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeContentsSize"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntSize&> DecodeType;
    explicit DidChangeContentsSize(const WebCore::IntSize& newSize)
        : CoreIPC::Arguments1<const WebCore::IntSize&>(newSize)
    {
    }
};
#endif

#if ENABLE(TOUCH_EVENTS)
struct NeedTouchEvents : CoreIPC::Arguments1<bool> {
    static const Kind messageID = NeedTouchEventsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("NeedTouchEvents"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit NeedTouchEvents(bool needTouchEvents)
        : CoreIPC::Arguments1<bool>(needTouchEvents)
    {
    }
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
struct ShowColorChooser : CoreIPC::Arguments2<const WebCore::Color&, const WebCore::IntRect&> {
    static const Kind messageID = ShowColorChooserID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowColorChooser"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::Color&, const WebCore::IntRect&> DecodeType;
    ShowColorChooser(const WebCore::Color& initialColor, const WebCore::IntRect& elementRect)
        : CoreIPC::Arguments2<const WebCore::Color&, const WebCore::IntRect&>(initialColor, elementRect)
    {
    }
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
struct SetColorChooserColor : CoreIPC::Arguments1<const WebCore::Color&> {
    static const Kind messageID = SetColorChooserColorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetColorChooserColor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::Color&> DecodeType;
    explicit SetColorChooserColor(const WebCore::Color& color)
        : CoreIPC::Arguments1<const WebCore::Color&>(color)
    {
    }
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
struct EndColorChooser : CoreIPC::Arguments0 {
    static const Kind messageID = EndColorChooserID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("EndColorChooser"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

struct DecidePolicyForResponse : CoreIPC::Arguments5<uint64_t, const WebCore::ResourceResponse&, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DecidePolicyForResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DecidePolicyForResponse"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments3<bool&, uint64_t&, uint64_t&> Reply;
    typedef CoreIPC::Arguments4<uint64_t&, WebCore::ResourceResponse&, WebCore::ResourceRequest&, uint64_t&> DecodeType;
    DecidePolicyForResponse(uint64_t frameID, const WebCore::ResourceResponse& response, const WebCore::ResourceRequest& request, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments5<uint64_t, const WebCore::ResourceResponse&, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, response, request, listenerID, userData)
    {
    }
};

struct DecidePolicyForNavigationAction : CoreIPC::Arguments7<uint64_t, uint32_t, uint32_t, int32_t, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DecidePolicyForNavigationActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DecidePolicyForNavigationAction"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments3<bool&, uint64_t&, uint64_t&> Reply;
    typedef CoreIPC::Arguments6<uint64_t&, uint32_t&, uint32_t&, int32_t&, WebCore::ResourceRequest&, uint64_t&> DecodeType;
    DecidePolicyForNavigationAction(uint64_t frameID, uint32_t navigationType, uint32_t modifiers, int32_t mouseButton, const WebCore::ResourceRequest& request, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments7<uint64_t, uint32_t, uint32_t, int32_t, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, navigationType, modifiers, mouseButton, request, listenerID, userData)
    {
    }
};

struct DecidePolicyForNewWindowAction : CoreIPC::Arguments8<uint64_t, uint32_t, uint32_t, int32_t, const WebCore::ResourceRequest&, const WTF::String&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DecidePolicyForNewWindowActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DecidePolicyForNewWindowAction"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments7<uint64_t&, uint32_t&, uint32_t&, int32_t&, WebCore::ResourceRequest&, WTF::String&, uint64_t&> DecodeType;
    DecidePolicyForNewWindowAction(uint64_t frameID, uint32_t navigationType, uint32_t modifiers, int32_t mouseButton, const WebCore::ResourceRequest& request, const WTF::String& frameName, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments8<uint64_t, uint32_t, uint32_t, int32_t, const WebCore::ResourceRequest&, const WTF::String&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, navigationType, modifiers, mouseButton, request, frameName, listenerID, userData)
    {
    }
};

struct UnableToImplementPolicy : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = UnableToImplementPolicyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UnableToImplementPolicy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WebCore::ResourceError&> DecodeType;
    UnableToImplementPolicy(uint64_t frameID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, error, userData)
    {
    }
};

#if ENABLE(WEB_INTENTS)
struct DidReceiveIntentForFrame : CoreIPC::Arguments3<uint64_t, const WebKit::IntentData&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidReceiveIntentForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveIntentForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WebKit::IntentData&> DecodeType;
    DidReceiveIntentForFrame(uint64_t frameID, const WebKit::IntentData& intent, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WebKit::IntentData&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, intent, userData)
    {
    }
};
#endif

#if ENABLE(WEB_INTENTS_TAG)
struct RegisterIntentServiceForFrame : CoreIPC::Arguments3<uint64_t, const WebKit::IntentServiceInfo&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = RegisterIntentServiceForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterIntentServiceForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WebKit::IntentServiceInfo&> DecodeType;
    RegisterIntentServiceForFrame(uint64_t frameID, const WebKit::IntentServiceInfo& serviceInfo, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WebKit::IntentServiceInfo&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, serviceInfo, userData)
    {
    }
};
#endif

struct DidChangeProgress : CoreIPC::Arguments1<double> {
    static const Kind messageID = DidChangeProgressID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeProgress"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit DidChangeProgress(double value)
        : CoreIPC::Arguments1<double>(value)
    {
    }
};

struct DidFinishProgress : CoreIPC::Arguments0 {
    static const Kind messageID = DidFinishProgressID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFinishProgress"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct DidStartProgress : CoreIPC::Arguments0 {
    static const Kind messageID = DidStartProgressID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidStartProgress"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct DidCreateMainFrame : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidCreateMainFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCreateMainFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidCreateMainFrame(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct DidCreateSubframe : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = DidCreateSubframeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCreateSubframe"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    DidCreateSubframe(uint64_t frameID, uint64_t parentFrameID)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, parentFrameID)
    {
    }
};

struct DidSaveFrameToPageCache : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DidSaveFrameToPageCacheID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidSaveFrameToPageCache"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DidSaveFrameToPageCache(uint64_t frameID)
        : CoreIPC::Arguments1<uint64_t>(frameID)
    {
    }
};

struct DidRestoreFrameFromPageCache : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = DidRestoreFrameFromPageCacheID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRestoreFrameFromPageCache"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    DidRestoreFrameFromPageCache(uint64_t frameID, uint64_t parentFrameID)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, parentFrameID)
    {
    }
};

struct DidCommitLoadForFrame : CoreIPC::Arguments5<uint64_t, const WTF::String&, bool, const WebKit::PlatformCertificateInfo&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidCommitLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCommitLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t&, WTF::String&, bool&, WebKit::PlatformCertificateInfo&> DecodeType;
    DidCommitLoadForFrame(uint64_t frameID, const WTF::String& mimeType, bool hasCustomRepresentation, const WebKit::PlatformCertificateInfo& certificateInfo, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments5<uint64_t, const WTF::String&, bool, const WebKit::PlatformCertificateInfo&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, mimeType, hasCustomRepresentation, certificateInfo, userData)
    {
    }
};

struct DidFailLoadForFrame : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFailLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFailLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WebCore::ResourceError&> DecodeType;
    DidFailLoadForFrame(uint64_t frameID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, error, userData)
    {
    }
};

struct DidFailProvisionalLoadForFrame : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFailProvisionalLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFailProvisionalLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WebCore::ResourceError&> DecodeType;
    DidFailProvisionalLoadForFrame(uint64_t frameID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, error, userData)
    {
    }
};

struct DidFinishDocumentLoadForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFinishDocumentLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFinishDocumentLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidFinishDocumentLoadForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidFinishLoadForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFinishLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFinishLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidFinishLoadForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidFirstLayoutForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFirstLayoutForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFirstLayoutForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidFirstLayoutForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidFirstVisuallyNonEmptyLayoutForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidFirstVisuallyNonEmptyLayoutForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFirstVisuallyNonEmptyLayoutForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidFirstVisuallyNonEmptyLayoutForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidNewFirstVisuallyNonEmptyLayout : CoreIPC::Arguments1<const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidNewFirstVisuallyNonEmptyLayoutID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidNewFirstVisuallyNonEmptyLayout"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
    explicit DidNewFirstVisuallyNonEmptyLayout(const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments1<const WebKit::InjectedBundleUserMessageEncoder&>(userData)
    {
    }
};

struct DidLayout : CoreIPC::Arguments2<uint32_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidLayoutID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidLayout"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t&> DecodeType;
    DidLayout(uint32_t type, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint32_t, const WebKit::InjectedBundleUserMessageEncoder&>(type, userData)
    {
    }
};

struct DidReceiveServerRedirectForProvisionalLoadForFrame : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidReceiveServerRedirectForProvisionalLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveServerRedirectForProvisionalLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WTF::String&> DecodeType;
    DidReceiveServerRedirectForProvisionalLoadForFrame(uint64_t frameID, const WTF::String& url, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, url, userData)
    {
    }
};

struct DidRemoveFrameFromHierarchy : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidRemoveFrameFromHierarchyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRemoveFrameFromHierarchy"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidRemoveFrameFromHierarchy(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidStartProvisionalLoadForFrame : CoreIPC::Arguments4<uint64_t, const WTF::String&, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidStartProvisionalLoadForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidStartProvisionalLoadForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t&, WTF::String&, WTF::String&> DecodeType;
    DidStartProvisionalLoadForFrame(uint64_t frameID, const WTF::String& url, const WTF::String& unreachableURL, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments4<uint64_t, const WTF::String&, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, url, unreachableURL, userData)
    {
    }
};

struct DidReceiveTitleForFrame : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidReceiveTitleForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveTitleForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t&, WTF::String&> DecodeType;
    DidReceiveTitleForFrame(uint64_t frameID, const WTF::String& title, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments3<uint64_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, title, userData)
    {
    }
};

struct DidDisplayInsecureContentForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidDisplayInsecureContentForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidDisplayInsecureContentForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidDisplayInsecureContentForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidRunInsecureContentForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidRunInsecureContentForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRunInsecureContentForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidRunInsecureContentForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidDetectXSSForFrame : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidDetectXSSForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidDetectXSSForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    DidDetectXSSForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, userData)
    {
    }
};

struct DidSameDocumentNavigationForFrame : CoreIPC::Arguments4<uint64_t, uint32_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = DidSameDocumentNavigationForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidSameDocumentNavigationForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t&, uint32_t&, WTF::String&> DecodeType;
    DidSameDocumentNavigationForFrame(uint64_t frameID, uint32_t type, const WTF::String& url, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments4<uint64_t, uint32_t, const WTF::String&, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, type, url, userData)
    {
    }
};

struct FrameDidBecomeFrameSet : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = FrameDidBecomeFrameSetID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FrameDidBecomeFrameSet"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    FrameDidBecomeFrameSet(uint64_t frameID, bool value)
        : CoreIPC::Arguments2<uint64_t, bool>(frameID, value)
    {
    }
};

struct DidInitiateLoadForResource : CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, bool> {
    static const Kind messageID = DidInitiateLoadForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidInitiateLoadForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, bool> DecodeType;
    DidInitiateLoadForResource(uint64_t frameID, uint64_t resourceIdentifier, const WebCore::ResourceRequest& request, bool pageIsProvisionallyLoading)
        : CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, bool>(frameID, resourceIdentifier, request, pageIsProvisionallyLoading)
    {
    }
};

struct DidSendRequestForResource : CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&> {
    static const Kind messageID = DidSendRequestForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidSendRequestForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&> DecodeType;
    DidSendRequestForResource(uint64_t frameID, uint64_t resourceIdentifier, const WebCore::ResourceRequest& request, const WebCore::ResourceResponse& redirectResponse)
        : CoreIPC::Arguments4<uint64_t, uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&>(frameID, resourceIdentifier, request, redirectResponse)
    {
    }
};

struct DidReceiveResponseForResource : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceResponse&> {
    static const Kind messageID = DidReceiveResponseForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveResponseForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceResponse&> DecodeType;
    DidReceiveResponseForResource(uint64_t frameID, uint64_t resourceIdentifier, const WebCore::ResourceResponse& response)
        : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceResponse&>(frameID, resourceIdentifier, response)
    {
    }
};

struct DidReceiveContentLengthForResource : CoreIPC::Arguments3<uint64_t, uint64_t, uint64_t> {
    static const Kind messageID = DidReceiveContentLengthForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveContentLengthForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, uint64_t, uint64_t> DecodeType;
    DidReceiveContentLengthForResource(uint64_t frameID, uint64_t resourceIdentifier, uint64_t contentLength)
        : CoreIPC::Arguments3<uint64_t, uint64_t, uint64_t>(frameID, resourceIdentifier, contentLength)
    {
    }
};

struct DidFinishLoadForResource : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = DidFinishLoadForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFinishLoadForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    DidFinishLoadForResource(uint64_t frameID, uint64_t resourceIdentifier)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(frameID, resourceIdentifier)
    {
    }
};

struct DidFailLoadForResource : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceError&> {
    static const Kind messageID = DidFailLoadForResourceID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFailLoadForResource"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceError&> DecodeType;
    DidFailLoadForResource(uint64_t frameID, uint64_t resourceIdentifier, const WebCore::ResourceError& error)
        : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceError&>(frameID, resourceIdentifier, error)
    {
    }
};

#if PLATFORM(QT)
struct ResolveApplicationSchemeRequest : CoreIPC::Arguments1<const WebKit::QtNetworkRequestData&> {
    static const Kind messageID = ResolveApplicationSchemeRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ResolveApplicationSchemeRequest"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::QtNetworkRequestData&> DecodeType;
    explicit ResolveApplicationSchemeRequest(const WebKit::QtNetworkRequestData& request)
        : CoreIPC::Arguments1<const WebKit::QtNetworkRequestData&>(request)
    {
    }
};
#endif

struct DidFinishLoadingDataForCustomRepresentation : CoreIPC::Arguments2<const WTF::String&, const CoreIPC::DataReference&> {
    static const Kind messageID = DidFinishLoadingDataForCustomRepresentationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFinishLoadingDataForCustomRepresentation"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const CoreIPC::DataReference&> DecodeType;
    DidFinishLoadingDataForCustomRepresentation(const WTF::String& suggestedFilename, const CoreIPC::DataReference& data)
        : CoreIPC::Arguments2<const WTF::String&, const CoreIPC::DataReference&>(suggestedFilename, data)
    {
    }
};

struct WillSubmitForm : CoreIPC::Arguments5<uint64_t, uint64_t, const WebKit::StringPairVector&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = WillSubmitFormID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WillSubmitForm"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint64_t&, uint64_t&, WebKit::StringPairVector&, uint64_t&> DecodeType;
    WillSubmitForm(uint64_t frameID, uint64_t sourceFrameID, const WebKit::StringPairVector& textFieldValues, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments5<uint64_t, uint64_t, const WebKit::StringPairVector&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(frameID, sourceFrameID, textFieldValues, listenerID, userData)
    {
    }
};

struct VoidCallback : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = VoidCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("VoidCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit VoidCallback(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct DataCallback : CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t> {
    static const Kind messageID = DataCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DataCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t> DecodeType;
    DataCallback(const CoreIPC::DataReference& resultData, uint64_t callbackID)
        : CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t>(resultData, callbackID)
    {
    }
};

struct ImageCallback : CoreIPC::Arguments2<const WebKit::ShareableBitmap::Handle&, uint64_t> {
    static const Kind messageID = ImageCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ImageCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebKit::ShareableBitmap::Handle&, uint64_t> DecodeType;
    ImageCallback(const WebKit::ShareableBitmap::Handle& bitmapHandle, uint64_t callbackID)
        : CoreIPC::Arguments2<const WebKit::ShareableBitmap::Handle&, uint64_t>(bitmapHandle, callbackID)
    {
    }
};

struct StringCallback : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = StringCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StringCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    StringCallback(const WTF::String& resultString, uint64_t callbackID)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(resultString, callbackID)
    {
    }
};

struct ScriptValueCallback : CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t> {
    static const Kind messageID = ScriptValueCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ScriptValueCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t> DecodeType;
    ScriptValueCallback(const CoreIPC::DataReference& resultData, uint64_t callbackID)
        : CoreIPC::Arguments2<const CoreIPC::DataReference&, uint64_t>(resultData, callbackID)
    {
    }
};

struct ComputedPagesCallback : CoreIPC::Arguments3<const Vector<WebCore::IntRect>&, double, uint64_t> {
    static const Kind messageID = ComputedPagesCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ComputedPagesCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const Vector<WebCore::IntRect>&, double, uint64_t> DecodeType;
    ComputedPagesCallback(const Vector<WebCore::IntRect>& pageRects, double totalScaleFactorForPrinting, uint64_t callbackID)
        : CoreIPC::Arguments3<const Vector<WebCore::IntRect>&, double, uint64_t>(pageRects, totalScaleFactorForPrinting, callbackID)
    {
    }
};

struct ValidateCommandCallback : CoreIPC::Arguments4<const WTF::String&, bool, int32_t, uint64_t> {
    static const Kind messageID = ValidateCommandCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ValidateCommandCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<const WTF::String&, bool, int32_t, uint64_t> DecodeType;
    ValidateCommandCallback(const WTF::String& command, bool isEnabled, int32_t state, uint64_t callbackID)
        : CoreIPC::Arguments4<const WTF::String&, bool, int32_t, uint64_t>(command, isEnabled, state, callbackID)
    {
    }
};

#if PLATFORM(GTK)
struct PrintFinishedCallback : CoreIPC::Arguments2<const WebCore::ResourceError&, uint64_t> {
    static const Kind messageID = PrintFinishedCallbackID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PrintFinishedCallback"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::ResourceError&, uint64_t> DecodeType;
    PrintFinishedCallback(const WebCore::ResourceError& error, uint64_t callbackID)
        : CoreIPC::Arguments2<const WebCore::ResourceError&, uint64_t>(error, callbackID)
    {
    }
};
#endif

struct PageScaleFactorDidChange : CoreIPC::Arguments1<double> {
    static const Kind messageID = PageScaleFactorDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PageScaleFactorDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit PageScaleFactorDidChange(double scaleFactor)
        : CoreIPC::Arguments1<double>(scaleFactor)
    {
    }
};

#if PLATFORM(GTK)
struct GetEditorCommandsForKeyEvent : CoreIPC::Arguments1<const AtomicString&> {
    static const Kind messageID = GetEditorCommandsForKeyEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetEditorCommandsForKeyEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<WTF::String>&> Reply;
    typedef CoreIPC::Arguments1<const AtomicString&> DecodeType;
    explicit GetEditorCommandsForKeyEvent(const AtomicString& eventType)
        : CoreIPC::Arguments1<const AtomicString&>(eventType)
    {
    }
};
#endif

#if PLATFORM(GTK)
struct BindAccessibilityTree : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = BindAccessibilityTreeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BindAccessibilityTree"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit BindAccessibilityTree(const WTF::String& plugID)
        : CoreIPC::Arguments1<const WTF::String&>(plugID)
    {
    }
};
#endif

struct BackForwardAddItem : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = BackForwardAddItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardAddItem"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit BackForwardAddItem(uint64_t itemID)
        : CoreIPC::Arguments1<uint64_t>(itemID)
    {
    }
};

struct BackForwardGoToItem : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = BackForwardGoToItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardGoToItem"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<WebKit::SandboxExtension::Handle&> Reply;
    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit BackForwardGoToItem(uint64_t itemID)
        : CoreIPC::Arguments1<uint64_t>(itemID)
    {
    }
};

struct BackForwardItemAtIndex : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = BackForwardItemAtIndexID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardItemAtIndex"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit BackForwardItemAtIndex(int32_t itemIndex)
        : CoreIPC::Arguments1<int32_t>(itemIndex)
    {
    }
};

struct BackForwardBackListCount : CoreIPC::Arguments0 {
    static const Kind messageID = BackForwardBackListCountID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardBackListCount"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<int32_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct BackForwardForwardListCount : CoreIPC::Arguments0 {
    static const Kind messageID = BackForwardForwardListCountID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardForwardListCount"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<int32_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct BackForwardClear : CoreIPC::Arguments0 {
    static const Kind messageID = BackForwardClearID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("BackForwardClear"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct ShouldGoToBackForwardListItem : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ShouldGoToBackForwardListItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShouldGoToBackForwardListItem"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ShouldGoToBackForwardListItem(uint64_t itemID)
        : CoreIPC::Arguments1<uint64_t>(itemID)
    {
    }
};

struct WillGoToBackForwardListItem : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = WillGoToBackForwardListItemID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WillGoToBackForwardListItem"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t&> DecodeType;
    WillGoToBackForwardListItem(uint64_t itemID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments2<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>(itemID, userData)
    {
    }
};

struct RegisterEditCommandForUndo : CoreIPC::Arguments2<uint64_t, uint32_t> {
    static const Kind messageID = RegisterEditCommandForUndoID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterEditCommandForUndo"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint32_t> DecodeType;
    RegisterEditCommandForUndo(uint64_t commandID, uint32_t editAction)
        : CoreIPC::Arguments2<uint64_t, uint32_t>(commandID, editAction)
    {
    }
};

struct ClearAllEditCommands : CoreIPC::Arguments0 {
    static const Kind messageID = ClearAllEditCommandsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearAllEditCommands"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct CanUndoRedo : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = CanUndoRedoID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CanUndoRedo"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit CanUndoRedo(uint32_t action)
        : CoreIPC::Arguments1<uint32_t>(action)
    {
    }
};

struct ExecuteUndoRedo : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = ExecuteUndoRedoID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExecuteUndoRedo"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit ExecuteUndoRedo(uint32_t action)
        : CoreIPC::Arguments1<uint32_t>(action)
    {
    }
};

struct EditorStateChanged : CoreIPC::Arguments1<const WebKit::EditorState&> {
    static const Kind messageID = EditorStateChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("EditorStateChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::EditorState&> DecodeType;
    explicit EditorStateChanged(const WebKit::EditorState& editorState)
        : CoreIPC::Arguments1<const WebKit::EditorState&>(editorState)
    {
    }
};

#if PLATFORM(WIN)
struct DidChangeCompositionSelection : CoreIPC::Arguments1<bool> {
    static const Kind messageID = DidChangeCompositionSelectionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeCompositionSelection"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit DidChangeCompositionSelection(bool hasChanged)
        : CoreIPC::Arguments1<bool>(hasChanged)
    {
    }
};
#endif

#if PLATFORM(QT)
struct WillSetInputMethodState : CoreIPC::Arguments0 {
    static const Kind messageID = WillSetInputMethodStateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WillSetInputMethodState"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

struct DidCountStringMatches : CoreIPC::Arguments2<const WTF::String&, uint32_t> {
    static const Kind messageID = DidCountStringMatchesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidCountStringMatches"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint32_t> DecodeType;
    DidCountStringMatches(const WTF::String& string, uint32_t matchCount)
        : CoreIPC::Arguments2<const WTF::String&, uint32_t>(string, matchCount)
    {
    }
};

struct SetFindIndicator : CoreIPC::Arguments6<const WebCore::FloatRect&, const Vector<WebCore::FloatRect>&, float, const WebKit::ShareableBitmap::Handle&, bool, bool> {
    static const Kind messageID = SetFindIndicatorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetFindIndicator"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments6<const WebCore::FloatRect&, const Vector<WebCore::FloatRect>&, float, const WebKit::ShareableBitmap::Handle&, bool, bool> DecodeType;
    SetFindIndicator(const WebCore::FloatRect& selectionRect, const Vector<WebCore::FloatRect>& textRects, float contentImageScaleFactor, const WebKit::ShareableBitmap::Handle& contentImageHandle, bool fadeOut, bool animate)
        : CoreIPC::Arguments6<const WebCore::FloatRect&, const Vector<WebCore::FloatRect>&, float, const WebKit::ShareableBitmap::Handle&, bool, bool>(selectionRect, textRects, contentImageScaleFactor, contentImageHandle, fadeOut, animate)
    {
    }
};

struct DidFindString : CoreIPC::Arguments2<const WTF::String&, uint32_t> {
    static const Kind messageID = DidFindStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFindString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint32_t> DecodeType;
    DidFindString(const WTF::String& string, uint32_t matchCount)
        : CoreIPC::Arguments2<const WTF::String&, uint32_t>(string, matchCount)
    {
    }
};

struct DidFailToFindString : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = DidFailToFindStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidFailToFindString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit DidFailToFindString(const WTF::String& string)
        : CoreIPC::Arguments1<const WTF::String&>(string)
    {
    }
};

#if PLATFORM(WIN)
struct DidInstallOrUninstallPageOverlay : CoreIPC::Arguments1<bool> {
    static const Kind messageID = DidInstallOrUninstallPageOverlayID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidInstallOrUninstallPageOverlay"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit DidInstallOrUninstallPageOverlay(bool didInstall)
        : CoreIPC::Arguments1<bool>(didInstall)
    {
    }
};
#endif

struct ShowPopupMenu : CoreIPC::Arguments5<const WebCore::IntRect&, uint64_t, const Vector<WebKit::WebPopupItem>&, int32_t, const WebKit::PlatformPopupMenuData&> {
    static const Kind messageID = ShowPopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowPopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<const WebCore::IntRect&, uint64_t, const Vector<WebKit::WebPopupItem>&, int32_t, const WebKit::PlatformPopupMenuData&> DecodeType;
    ShowPopupMenu(const WebCore::IntRect& rect, uint64_t textDirection, const Vector<WebKit::WebPopupItem>& items, int32_t selectedIndex, const WebKit::PlatformPopupMenuData& data)
        : CoreIPC::Arguments5<const WebCore::IntRect&, uint64_t, const Vector<WebKit::WebPopupItem>&, int32_t, const WebKit::PlatformPopupMenuData&>(rect, textDirection, items, selectedIndex, data)
    {
    }
};

struct HidePopupMenu : CoreIPC::Arguments0 {
    static const Kind messageID = HidePopupMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("HidePopupMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if PLATFORM(WIN)
struct SetPopupMenuSelectedIndex : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = SetPopupMenuSelectedIndexID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPopupMenuSelectedIndex"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit SetPopupMenuSelectedIndex(int32_t selectedIndex)
        : CoreIPC::Arguments1<int32_t>(selectedIndex)
    {
    }
};
#endif

#if ENABLE(CONTEXT_MENUS)
struct ShowContextMenu : CoreIPC::Arguments4<const WebCore::IntPoint&, const WebKit::WebHitTestResult::Data&, const Vector<WebKit::WebContextMenuItemData>&, const WebKit::InjectedBundleUserMessageEncoder&> {
    static const Kind messageID = ShowContextMenuID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowContextMenu"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<WebCore::IntPoint&, WebKit::WebHitTestResult::Data&, Vector<WebKit::WebContextMenuItemData>&> DecodeType;
    ShowContextMenu(const WebCore::IntPoint& menuLocation, const WebKit::WebHitTestResult::Data& hitTestResultData, const Vector<WebKit::WebContextMenuItemData>& items, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : CoreIPC::Arguments4<const WebCore::IntPoint&, const WebKit::WebHitTestResult::Data&, const Vector<WebKit::WebContextMenuItemData>&, const WebKit::InjectedBundleUserMessageEncoder&>(menuLocation, hitTestResultData, items, userData)
    {
    }
};
#endif

struct CanAuthenticateAgainstProtectionSpaceInFrame : CoreIPC::Arguments2<uint64_t, const WebCore::ProtectionSpace&> {
    static const Kind messageID = CanAuthenticateAgainstProtectionSpaceInFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CanAuthenticateAgainstProtectionSpaceInFrame"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments2<uint64_t, const WebCore::ProtectionSpace&> DecodeType;
    CanAuthenticateAgainstProtectionSpaceInFrame(uint64_t frameID, const WebCore::ProtectionSpace& protectionSpace)
        : CoreIPC::Arguments2<uint64_t, const WebCore::ProtectionSpace&>(frameID, protectionSpace)
    {
    }
};

struct DidReceiveAuthenticationChallenge : CoreIPC::Arguments3<uint64_t, const WebCore::AuthenticationChallenge&, uint64_t> {
    static const Kind messageID = DidReceiveAuthenticationChallengeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveAuthenticationChallenge"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, const WebCore::AuthenticationChallenge&, uint64_t> DecodeType;
    DidReceiveAuthenticationChallenge(uint64_t frameID, const WebCore::AuthenticationChallenge& challenge, uint64_t challengeID)
        : CoreIPC::Arguments3<uint64_t, const WebCore::AuthenticationChallenge&, uint64_t>(frameID, challenge, challengeID)
    {
    }
};

struct ExceededDatabaseQuota : CoreIPC::Arguments8<uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t, uint64_t, uint64_t, uint64_t> {
    static const Kind messageID = ExceededDatabaseQuotaID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExceededDatabaseQuota"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments8<uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t, uint64_t, uint64_t, uint64_t> DecodeType;
    ExceededDatabaseQuota(uint64_t frameID, const WTF::String& originIdentifier, const WTF::String& databaseName, const WTF::String& databaseDisplayName, uint64_t currentQuota, uint64_t currentOriginUsage, uint64_t currentDatabaseUsage, uint64_t expectedUsage)
        : CoreIPC::Arguments8<uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t, uint64_t, uint64_t, uint64_t>(frameID, originIdentifier, databaseName, databaseDisplayName, currentQuota, currentOriginUsage, currentDatabaseUsage, expectedUsage)
    {
    }
};

struct RequestGeolocationPermissionForFrame : CoreIPC::Arguments3<uint64_t, uint64_t, const WTF::String&> {
    static const Kind messageID = RequestGeolocationPermissionForFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RequestGeolocationPermissionForFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, uint64_t, const WTF::String&> DecodeType;
    RequestGeolocationPermissionForFrame(uint64_t geolocationID, uint64_t frameID, const WTF::String& originIdentifier)
        : CoreIPC::Arguments3<uint64_t, uint64_t, const WTF::String&>(geolocationID, frameID, originIdentifier)
    {
    }
};

struct RequestNotificationPermission : CoreIPC::Arguments2<uint64_t, const WTF::String&> {
    static const Kind messageID = RequestNotificationPermissionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RequestNotificationPermission"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WTF::String&> DecodeType;
    RequestNotificationPermission(uint64_t requestID, const WTF::String& originIdentifier)
        : CoreIPC::Arguments2<uint64_t, const WTF::String&>(requestID, originIdentifier)
    {
    }
};

struct ShowNotification : CoreIPC::Arguments8<const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t> {
    static const Kind messageID = ShowNotificationID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowNotification"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments8<const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t> DecodeType;
    ShowNotification(const WTF::String& title, const WTF::String& body, const WTF::String& iconURL, const WTF::String& tag, const WTF::String& lang, const WTF::String& dir, const WTF::String& originIdentifier, uint64_t notificationID)
        : CoreIPC::Arguments8<const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, uint64_t>(title, body, iconURL, tag, lang, dir, originIdentifier, notificationID)
    {
    }
};

#if USE(UNIFIED_TEXT_CHECKING)
struct CheckTextOfParagraph : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = CheckTextOfParagraphID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CheckTextOfParagraph"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<WebCore::TextCheckingResult>&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    CheckTextOfParagraph(const WTF::String& text, uint64_t checkingTypes)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(text, checkingTypes)
    {
    }
};
#endif

struct CheckSpellingOfString : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = CheckSpellingOfStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CheckSpellingOfString"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<int32_t&, int32_t&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit CheckSpellingOfString(const WTF::String& text)
        : CoreIPC::Arguments1<const WTF::String&>(text)
    {
    }
};

struct CheckGrammarOfString : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = CheckGrammarOfStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CheckGrammarOfString"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments3<Vector<WebCore::GrammarDetail>&, int32_t&, int32_t&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit CheckGrammarOfString(const WTF::String& text)
        : CoreIPC::Arguments1<const WTF::String&>(text)
    {
    }
};

struct SpellingUIIsShowing : CoreIPC::Arguments0 {
    static const Kind messageID = SpellingUIIsShowingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SpellingUIIsShowing"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};

struct UpdateSpellingUIWithMisspelledWord : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = UpdateSpellingUIWithMisspelledWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UpdateSpellingUIWithMisspelledWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit UpdateSpellingUIWithMisspelledWord(const WTF::String& misspelledWord)
        : CoreIPC::Arguments1<const WTF::String&>(misspelledWord)
    {
    }
};

struct UpdateSpellingUIWithGrammarString : CoreIPC::Arguments2<const WTF::String&, const WebCore::GrammarDetail&> {
    static const Kind messageID = UpdateSpellingUIWithGrammarStringID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UpdateSpellingUIWithGrammarString"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const WebCore::GrammarDetail&> DecodeType;
    UpdateSpellingUIWithGrammarString(const WTF::String& badGrammarPhrase, const WebCore::GrammarDetail& grammarDetail)
        : CoreIPC::Arguments2<const WTF::String&, const WebCore::GrammarDetail&>(badGrammarPhrase, grammarDetail)
    {
    }
};

struct GetGuessesForWord : CoreIPC::Arguments2<const WTF::String&, const WTF::String&> {
    static const Kind messageID = GetGuessesForWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetGuessesForWord"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<WTF::String>&> Reply;
    typedef CoreIPC::Arguments2<const WTF::String&, const WTF::String&> DecodeType;
    GetGuessesForWord(const WTF::String& word, const WTF::String& context)
        : CoreIPC::Arguments2<const WTF::String&, const WTF::String&>(word, context)
    {
    }
};

struct LearnWord : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = LearnWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LearnWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit LearnWord(const WTF::String& word)
        : CoreIPC::Arguments1<const WTF::String&>(word)
    {
    }
};

struct IgnoreWord : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = IgnoreWordID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("IgnoreWord"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit IgnoreWord(const WTF::String& word)
        : CoreIPC::Arguments1<const WTF::String&>(word)
    {
    }
};

#if ENABLE(DRAG_SUPPORT)
struct DidPerformDragControllerAction : CoreIPC::Arguments1<const WebCore::DragSession&> {
    static const Kind messageID = DidPerformDragControllerActionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidPerformDragControllerAction"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::DragSession&> DecodeType;
    explicit DidPerformDragControllerAction(const WebCore::DragSession& dragSession)
        : CoreIPC::Arguments1<const WebCore::DragSession&>(dragSession)
    {
    }
};
#endif

#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
struct SetDragImage : CoreIPC::Arguments3<const WebCore::IntPoint&, const WebKit::ShareableBitmap::Handle&, bool> {
    static const Kind messageID = SetDragImageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetDragImage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebCore::IntPoint&, const WebKit::ShareableBitmap::Handle&, bool> DecodeType;
    SetDragImage(const WebCore::IntPoint& clientPosition, const WebKit::ShareableBitmap::Handle& dragImage, bool linkDrag)
        : CoreIPC::Arguments3<const WebCore::IntPoint&, const WebKit::ShareableBitmap::Handle&, bool>(clientPosition, dragImage, linkDrag)
    {
    }
};
#endif

#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
struct SetPromisedData : CoreIPC::Arguments10<const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t> {
    static const Kind messageID = SetPromisedDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPromisedData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments10<const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t> DecodeType;
    SetPromisedData(const WTF::String& pasteboardName, const WebKit::SharedMemory::Handle& imageHandle, uint64_t imageSize, const WTF::String& filename, const WTF::String& extension, const WTF::String& title, const WTF::String& url, const WTF::String& visibleURL, const WebKit::SharedMemory::Handle& archiveHandle, uint64_t archiveSize)
        : CoreIPC::Arguments10<const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WTF::String&, const WebKit::SharedMemory::Handle&, uint64_t>(pasteboardName, imageHandle, imageSize, filename, extension, title, url, visibleURL, archiveHandle, archiveSize)
    {
    }
};
#endif

#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
struct StartDragDrop : CoreIPC::Arguments10<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String> >&, uint64_t, const String&, const WebKit::SharedMemory::Handle&, const WebCore::IntSize&, const WebKit::SharedMemory::Handle&, bool> {
    static const Kind messageID = StartDragDropID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartDragDrop"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments10<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String> >&, uint64_t, const String&, const WebKit::SharedMemory::Handle&, const WebCore::IntSize&, const WebKit::SharedMemory::Handle&, bool> DecodeType;
    StartDragDrop(const WebCore::IntPoint& imagePoint, const WebCore::IntPoint& dragPoint, uint64_t okEffect, const HashMap<UINT,Vector<String> >& dataMap, uint64_t fileSize, const String& pathname, const WebKit::SharedMemory::Handle& fileContentHandle, const WebCore::IntSize& dragImageSize, const WebKit::SharedMemory::Handle& dragImage, bool linkDrag)
        : CoreIPC::Arguments10<const WebCore::IntPoint&, const WebCore::IntPoint&, uint64_t, const HashMap<UINT,Vector<String> >&, uint64_t, const String&, const WebKit::SharedMemory::Handle&, const WebCore::IntSize&, const WebKit::SharedMemory::Handle&, bool>(imagePoint, dragPoint, okEffect, dataMap, fileSize, pathname, fileContentHandle, dragImageSize, dragImage, linkDrag)
    {
    }
};
#endif

#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
struct StartDrag : CoreIPC::Arguments2<const WebCore::DragData&, const WebKit::ShareableBitmap::Handle&> {
    static const Kind messageID = StartDragID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartDrag"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::DragData&, const WebKit::ShareableBitmap::Handle&> DecodeType;
    StartDrag(const WebCore::DragData& dragData, const WebKit::ShareableBitmap::Handle& dragImage)
        : CoreIPC::Arguments2<const WebCore::DragData&, const WebKit::ShareableBitmap::Handle&>(dragData, dragImage)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct DidPerformDictionaryLookup : CoreIPC::Arguments2<const WebKit::AttributedString&, const WebKit::DictionaryPopupInfo&> {
    static const Kind messageID = DidPerformDictionaryLookupID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidPerformDictionaryLookup"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebKit::AttributedString&, const WebKit::DictionaryPopupInfo&> DecodeType;
    DidPerformDictionaryLookup(const WebKit::AttributedString& text, const WebKit::DictionaryPopupInfo& dictionaryPopupInfo)
        : CoreIPC::Arguments2<const WebKit::AttributedString&, const WebKit::DictionaryPopupInfo&>(text, dictionaryPopupInfo)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct InterpretQueuedKeyEvent : CoreIPC::Arguments1<const WebKit::EditorState&> {
    static const Kind messageID = InterpretQueuedKeyEventID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("InterpretQueuedKeyEvent"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments2<bool&, Vector<WebCore::KeypressCommand>&> Reply;
    typedef CoreIPC::Arguments1<const WebKit::EditorState&> DecodeType;
    explicit InterpretQueuedKeyEvent(const WebKit::EditorState& state)
        : CoreIPC::Arguments1<const WebKit::EditorState&>(state)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct ExecuteSavedCommandBySelector : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = ExecuteSavedCommandBySelectorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ExecuteSavedCommandBySelector"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit ExecuteSavedCommandBySelector(const WTF::String& selector)
        : CoreIPC::Arguments1<const WTF::String&>(selector)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct RegisterWebProcessAccessibilityToken : CoreIPC::Arguments1<const CoreIPC::DataReference&> {
    static const Kind messageID = RegisterWebProcessAccessibilityTokenID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterWebProcessAccessibilityToken"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const CoreIPC::DataReference&> DecodeType;
    explicit RegisterWebProcessAccessibilityToken(const CoreIPC::DataReference& data)
        : CoreIPC::Arguments1<const CoreIPC::DataReference&>(data)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct PluginFocusOrWindowFocusChanged : CoreIPC::Arguments2<uint64_t, bool> {
    static const Kind messageID = PluginFocusOrWindowFocusChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PluginFocusOrWindowFocusChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, bool> DecodeType;
    PluginFocusOrWindowFocusChanged(uint64_t pluginComplexTextInputIdentifier, bool pluginHasFocusAndWindowHasFocus)
        : CoreIPC::Arguments2<uint64_t, bool>(pluginComplexTextInputIdentifier, pluginHasFocusAndWindowHasFocus)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetPluginComplexTextInputState : CoreIPC::Arguments2<uint64_t, uint64_t> {
    static const Kind messageID = SetPluginComplexTextInputStateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetPluginComplexTextInputState"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, uint64_t> DecodeType;
    SetPluginComplexTextInputState(uint64_t pluginComplexTextInputIdentifier, uint64_t complexTextInputState)
        : CoreIPC::Arguments2<uint64_t, uint64_t>(pluginComplexTextInputIdentifier, complexTextInputState)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct GetIsSpeaking : CoreIPC::Arguments0 {
    static const Kind messageID = GetIsSpeakingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetIsSpeaking"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct Speak : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SpeakID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("Speak"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit Speak(const WTF::String& string)
        : CoreIPC::Arguments1<const WTF::String&>(string)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct StopSpeaking : CoreIPC::Arguments0 {
    static const Kind messageID = StopSpeakingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopSpeaking"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct MakeFirstResponder : CoreIPC::Arguments0 {
    static const Kind messageID = MakeFirstResponderID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("MakeFirstResponder"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC)
struct SearchWithSpotlight : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SearchWithSpotlightID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SearchWithSpotlight"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SearchWithSpotlight(const WTF::String& string)
        : CoreIPC::Arguments1<const WTF::String&>(string)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SubstitutionsPanelIsShowing : CoreIPC::Arguments0 {
    static const Kind messageID = SubstitutionsPanelIsShowingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SubstitutionsPanelIsShowing"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<bool&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
struct ShowCorrectionPanel : CoreIPC::Arguments5<int32_t, const WebCore::FloatRect&, const String&, const String&, const Vector<String>&> {
    static const Kind messageID = ShowCorrectionPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowCorrectionPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<int32_t, const WebCore::FloatRect&, const String&, const String&, const Vector<String>&> DecodeType;
    ShowCorrectionPanel(int32_t panelType, const WebCore::FloatRect& boundingBoxOfReplacedString, const String& replacedString, const String& replacementString, const Vector<String>& alternativeReplacementStrings)
        : CoreIPC::Arguments5<int32_t, const WebCore::FloatRect&, const String&, const String&, const Vector<String>&>(panelType, boundingBoxOfReplacedString, replacedString, replacementString, alternativeReplacementStrings)
    {
    }
};
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
struct DismissCorrectionPanel : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = DismissCorrectionPanelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DismissCorrectionPanel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit DismissCorrectionPanel(int32_t reason)
        : CoreIPC::Arguments1<int32_t>(reason)
    {
    }
};
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
struct DismissCorrectionPanelSoon : CoreIPC::Arguments1<int32_t> {
    static const Kind messageID = DismissCorrectionPanelSoonID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DismissCorrectionPanelSoon"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<String&> Reply;
    typedef CoreIPC::Arguments1<int32_t> DecodeType;
    explicit DismissCorrectionPanelSoon(int32_t reason)
        : CoreIPC::Arguments1<int32_t>(reason)
    {
    }
};
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
struct RecordAutocorrectionResponse : CoreIPC::Arguments3<int32_t, const String&, const String&> {
    static const Kind messageID = RecordAutocorrectionResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RecordAutocorrectionResponse"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<int32_t, const String&, const String&> DecodeType;
    RecordAutocorrectionResponse(int32_t responseType, const String& replacedString, const String& replacementString)
        : CoreIPC::Arguments3<int32_t, const String&, const String&>(responseType, replacedString, replacementString)
    {
    }
};
#endif

#if USE(DICTATION_ALTERNATIVES)
struct ShowDictationAlternativeUI : CoreIPC::Arguments2<const WebCore::FloatRect&, uint64_t> {
    static const Kind messageID = ShowDictationAlternativeUIID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ShowDictationAlternativeUI"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::FloatRect&, uint64_t> DecodeType;
    ShowDictationAlternativeUI(const WebCore::FloatRect& boundingBoxOfDictatedText, uint64_t dictationContext)
        : CoreIPC::Arguments2<const WebCore::FloatRect&, uint64_t>(boundingBoxOfDictatedText, dictationContext)
    {
    }
};
#endif

#if USE(DICTATION_ALTERNATIVES)
struct DismissDictationAlternativeUI : CoreIPC::Arguments0 {
    static const Kind messageID = DismissDictationAlternativeUIID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DismissDictationAlternativeUI"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(DICTATION_ALTERNATIVES)
struct RemoveDictationAlternatives : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = RemoveDictationAlternativesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveDictationAlternatives"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit RemoveDictationAlternatives(uint64_t dictationContext)
        : CoreIPC::Arguments1<uint64_t>(dictationContext)
    {
    }
};
#endif

#if USE(DICTATION_ALTERNATIVES)
struct DictationAlternatives : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = DictationAlternativesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DictationAlternatives"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<String>&> Reply;
    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit DictationAlternatives(uint64_t dictationContext)
        : CoreIPC::Arguments1<uint64_t>(dictationContext)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct SetGestureReachedScrollingLimit : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetGestureReachedScrollingLimitID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetGestureReachedScrollingLimit"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetGestureReachedScrollingLimit(bool limitReached)
        : CoreIPC::Arguments1<bool>(limitReached)
    {
    }
};
#endif

#if PLATFORM(WIN)
struct ScheduleChildWindowGeometryUpdate : CoreIPC::Arguments1<const WebKit::WindowGeometry&> {
    static const Kind messageID = ScheduleChildWindowGeometryUpdateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ScheduleChildWindowGeometryUpdate"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::WindowGeometry&> DecodeType;
    explicit ScheduleChildWindowGeometryUpdate(const WebKit::WindowGeometry& geometry)
        : CoreIPC::Arguments1<const WebKit::WindowGeometry&>(geometry)
    {
    }
};
#endif

#if PLUGIN_ARCHITECTURE(X11)
struct CreatePluginContainer : CoreIPC::Arguments0 {
    static const Kind messageID = CreatePluginContainerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreatePluginContainer"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<uint64_t&> Reply;
    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if PLUGIN_ARCHITECTURE(X11)
struct WindowedPluginGeometryDidChange : CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, uint64_t> {
    static const Kind messageID = WindowedPluginGeometryDidChangeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("WindowedPluginGeometryDidChange"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, uint64_t> DecodeType;
    WindowedPluginGeometryDidChange(const WebCore::IntRect& frameRect, const WebCore::IntRect& clipRect, uint64_t windowID)
        : CoreIPC::Arguments3<const WebCore::IntRect&, const WebCore::IntRect&, uint64_t>(frameRect, clipRect, windowID)
    {
    }
};
#endif

struct SaveRecentSearches : CoreIPC::Arguments2<const WTF::String&, const Vector<String>&> {
    static const Kind messageID = SaveRecentSearchesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SaveRecentSearches"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, const Vector<String>&> DecodeType;
    SaveRecentSearches(const WTF::String& name, const Vector<String>& searchItems)
        : CoreIPC::Arguments2<const WTF::String&, const Vector<String>&>(name, searchItems)
    {
    }
};

struct LoadRecentSearches : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = LoadRecentSearchesID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("LoadRecentSearches"); }
    static const bool isSync = true;

    typedef CoreIPC::Arguments1<Vector<String>&> Reply;
    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit LoadRecentSearches(const WTF::String& name)
        : CoreIPC::Arguments1<const WTF::String&>(name)
    {
    }
};

#if USE(SOUP)
struct DidReceiveURIRequest : CoreIPC::Arguments2<const WTF::String&, uint64_t> {
    static const Kind messageID = DidReceiveURIRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidReceiveURIRequest"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WTF::String&, uint64_t> DecodeType;
    DidReceiveURIRequest(const WTF::String& uriString, uint64_t requestID)
        : CoreIPC::Arguments2<const WTF::String&, uint64_t>(uriString, requestID)
    {
    }
};
#endif

struct SavePDFToFileInDownloadsFolder : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const CoreIPC::DataReference&> {
    static const Kind messageID = SavePDFToFileInDownloadsFolderID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SavePDFToFileInDownloadsFolder"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const CoreIPC::DataReference&> DecodeType;
    SavePDFToFileInDownloadsFolder(const WTF::String& suggestedFilename, const WTF::String& originatingURLString, const CoreIPC::DataReference& data)
        : CoreIPC::Arguments3<const WTF::String&, const WTF::String&, const CoreIPC::DataReference&>(suggestedFilename, originatingURLString, data)
    {
    }
};

} // namespace WebPageProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebPageProxy::Kind> {
    static const MessageClass messageClass = MessageClassWebPageProxy;
};

} // namespace CoreIPC

#endif // WebPageProxyMessages_h
