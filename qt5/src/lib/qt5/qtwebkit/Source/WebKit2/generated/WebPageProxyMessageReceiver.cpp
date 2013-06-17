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

#include "config.h"

#include "WebPageProxy.h"

#include "ArgumentCoders.h"
#if PLATFORM(MAC)
#include "AttributedString.h"
#endif
#include "DataReference.h"
#if PLATFORM(MAC)
#include "DictionaryPopupInfo.h"
#endif
#include "EditorState.h"
#include "HandleMessage.h"
#include "InjectedBundleUserMessageCoders.h"
#if ENABLE(WEB_INTENTS)
#include "IntentData.h"
#endif
#if ENABLE(WEB_INTENTS_TAG)
#include "IntentServiceInfo.h"
#endif
#include "MessageDecoder.h"
#include "PlatformCertificateInfo.h"
#include "PlatformPopupMenuData.h"
#if PLATFORM(QT)
#include "QtNetworkRequestData.h"
#endif
#include "SandboxExtension.h"
#include "ShareableBitmap.h"
#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT) || PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
#include "SharedMemory.h"
#endif
#include "StringPairVector.h"
#if ENABLE(CONTEXT_MENUS)
#include "WebContextMenuItemData.h"
#endif
#include "WebCoreArgumentCoders.h"
#include "WebHitTestResult.h"
#include "WebPageCreationParameters.h"
#include "WebPageProxyMessages.h"
#include "WebPopupItem.h"
#if PLATFORM(WIN)
#include "WindowGeometry.h"
#endif
#include <WebCore/AuthenticationChallenge.h>
#if ENABLE(INPUT_TYPE_COLOR)
#include <WebCore/Color.h>
#endif
#include <WebCore/Cursor.h>
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
#include <WebCore/DragData.h>
#endif
#if ENABLE(DRAG_SUPPORT)
#include <WebCore/DragSession.h>
#endif
#include <WebCore/FileChooser.h>
#include <WebCore/FloatRect.h>
#include <WebCore/IntPoint.h>
#include <WebCore/IntRect.h>
#if PLATFORM(QT) || PLATFORM(EFL) || PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
#include <WebCore/IntSize.h>
#endif
#if PLATFORM(MAC)
#include <WebCore/KeyboardEvent.h>
#endif
#include <WebCore/ProtectionSpace.h>
#include <WebCore/ResourceError.h>
#include <WebCore/ResourceRequest.h>
#include <WebCore/ResourceResponse.h>
#include <WebCore/TextCheckerClient.h>
#include <WebCore/ViewportArguments.h>
#include <WebCore/WindowFeatures.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

void WebPageProxy::didReceiveWebPageProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebPageProxy::ShowPage::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowPage>(decoder, this, &WebPageProxy::showPage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ClosePage::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ClosePage>(decoder, this, &WebPageProxy::closePage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::MouseDidMoveOverElement::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::MouseDidMoveOverElement>(decoder, this, &WebPageProxy::mouseDidMoveOverElement);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UnavailablePluginButtonClicked::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::UnavailablePluginButtonClicked>(decoder, this, &WebPageProxy::unavailablePluginButtonClicked);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeViewportProperties::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeViewportProperties>(decoder, this, &WebPageProxy::didChangeViewportProperties);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidReceiveEvent>(decoder, this, &WebPageProxy::didReceiveEvent);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::StopResponsivenessTimer::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::StopResponsivenessTimer>(decoder, this, &WebPageProxy::stopResponsivenessTimer);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetCursor::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetCursor>(decoder, this, &WebPageProxy::setCursor);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetCursorHiddenUntilMouseMoves::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetCursorHiddenUntilMouseMoves>(decoder, this, &WebPageProxy::setCursorHiddenUntilMouseMoves);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetStatusText::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetStatusText>(decoder, this, &WebPageProxy::setStatusText);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetToolTip::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetToolTip>(decoder, this, &WebPageProxy::setToolTip);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetFocus::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetFocus>(decoder, this, &WebPageProxy::setFocus);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::TakeFocus::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::TakeFocus>(decoder, this, &WebPageProxy::takeFocus);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::FocusedFrameChanged::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::FocusedFrameChanged>(decoder, this, &WebPageProxy::focusedFrameChanged);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::FrameSetLargestFrameChanged::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::FrameSetLargestFrameChanged>(decoder, this, &WebPageProxy::frameSetLargestFrameChanged);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetRenderTreeSize::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetRenderTreeSize>(decoder, this, &WebPageProxy::setRenderTreeSize);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetToolbarsAreVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetToolbarsAreVisible>(decoder, this, &WebPageProxy::setToolbarsAreVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetMenuBarIsVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetMenuBarIsVisible>(decoder, this, &WebPageProxy::setMenuBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetStatusBarIsVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetStatusBarIsVisible>(decoder, this, &WebPageProxy::setStatusBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetIsResizable::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetIsResizable>(decoder, this, &WebPageProxy::setIsResizable);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetWindowFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetWindowFrame>(decoder, this, &WebPageProxy::setWindowFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PageDidScroll::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PageDidScroll>(decoder, this, &WebPageProxy::pageDidScroll);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunOpenPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunOpenPanel>(decoder, this, &WebPageProxy::runOpenPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunModal::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunModal>(decoder, this, &WebPageProxy::runModal);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::NotifyScrollerThumbIsVisibleInRect::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::NotifyScrollerThumbIsVisibleInRect>(decoder, this, &WebPageProxy::notifyScrollerThumbIsVisibleInRect);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RecommendedScrollbarStyleDidChange::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RecommendedScrollbarStyleDidChange>(decoder, this, &WebPageProxy::recommendedScrollbarStyleDidChange);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeScrollbarsForMainFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeScrollbarsForMainFrame>(decoder, this, &WebPageProxy::didChangeScrollbarsForMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeScrollOffsetPinningForMainFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeScrollOffsetPinningForMainFrame>(decoder, this, &WebPageProxy::didChangeScrollOffsetPinningForMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangePageCount::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangePageCount>(decoder, this, &WebPageProxy::didChangePageCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailToInitializePlugin::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFailToInitializePlugin>(decoder, this, &WebPageProxy::didFailToInitializePlugin);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidBlockInsecurePluginVersion::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidBlockInsecurePluginVersion>(decoder, this, &WebPageProxy::didBlockInsecurePluginVersion);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetCanShortCircuitHorizontalWheelEvents::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetCanShortCircuitHorizontalWheelEvents>(decoder, this, &WebPageProxy::setCanShortCircuitHorizontalWheelEvents);
        return;
    }
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPageProxy::PageDidRequestScroll::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PageDidRequestScroll>(decoder, this, &WebPageProxy::pageDidRequestScroll);
        return;
    }
#endif
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPageProxy::PageTransitionViewportReady::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PageTransitionViewportReady>(decoder, this, &WebPageProxy::pageTransitionViewportReady);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::DidFindZoomableArea::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFindZoomableArea>(decoder, this, &WebPageProxy::didFindZoomableArea);
        return;
    }
#endif
#if PLATFORM(QT) || PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeContentsSize::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeContentsSize>(decoder, this, &WebPageProxy::didChangeContentsSize);
        return;
    }
#endif
#if ENABLE(TOUCH_EVENTS)
    if (decoder.messageName() == Messages::WebPageProxy::NeedTouchEvents::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::NeedTouchEvents>(decoder, this, &WebPageProxy::needTouchEvents);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::ShowColorChooser::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowColorChooser>(decoder, this, &WebPageProxy::showColorChooser);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::SetColorChooserColor::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetColorChooserColor>(decoder, this, &WebPageProxy::setColorChooserColor);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::EndColorChooser::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::EndColorChooser>(decoder, this, &WebPageProxy::endColorChooser);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForNewWindowAction::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForNewWindowAction>(decoder, this, &WebPageProxy::decidePolicyForNewWindowAction);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UnableToImplementPolicy::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::UnableToImplementPolicy>(decoder, this, &WebPageProxy::unableToImplementPolicy);
        return;
    }
#if ENABLE(WEB_INTENTS)
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveIntentForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidReceiveIntentForFrame>(decoder, this, &WebPageProxy::didReceiveIntentForFrame);
        return;
    }
#endif
#if ENABLE(WEB_INTENTS_TAG)
    if (decoder.messageName() == Messages::WebPageProxy::RegisterIntentServiceForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::RegisterIntentServiceForFrame>(decoder, this, &WebPageProxy::registerIntentServiceForFrame);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeProgress::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeProgress>(decoder, this, &WebPageProxy::didChangeProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishProgress::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFinishProgress>(decoder, this, &WebPageProxy::didFinishProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidStartProgress::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidStartProgress>(decoder, this, &WebPageProxy::didStartProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCreateMainFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidCreateMainFrame>(decoder, this, &WebPageProxy::didCreateMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCreateSubframe::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidCreateSubframe>(decoder, this, &WebPageProxy::didCreateSubframe);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidSaveFrameToPageCache::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidSaveFrameToPageCache>(decoder, this, &WebPageProxy::didSaveFrameToPageCache);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRestoreFrameFromPageCache::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidRestoreFrameFromPageCache>(decoder, this, &WebPageProxy::didRestoreFrameFromPageCache);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCommitLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidCommitLoadForFrame>(decoder, this, &WebPageProxy::didCommitLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFailLoadForFrame>(decoder, this, &WebPageProxy::didFailLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailProvisionalLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFailProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didFailProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishDocumentLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFinishDocumentLoadForFrame>(decoder, this, &WebPageProxy::didFinishDocumentLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFinishLoadForFrame>(decoder, this, &WebPageProxy::didFinishLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFirstLayoutForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFirstLayoutForFrame>(decoder, this, &WebPageProxy::didFirstLayoutForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFirstVisuallyNonEmptyLayoutForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidFirstVisuallyNonEmptyLayoutForFrame>(decoder, this, &WebPageProxy::didFirstVisuallyNonEmptyLayoutForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidNewFirstVisuallyNonEmptyLayout::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidNewFirstVisuallyNonEmptyLayout>(decoder, this, &WebPageProxy::didNewFirstVisuallyNonEmptyLayout);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidLayout::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidLayout>(decoder, this, &WebPageProxy::didLayout);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveServerRedirectForProvisionalLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidReceiveServerRedirectForProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didReceiveServerRedirectForProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRemoveFrameFromHierarchy::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidRemoveFrameFromHierarchy>(decoder, this, &WebPageProxy::didRemoveFrameFromHierarchy);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidStartProvisionalLoadForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidStartProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didStartProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveTitleForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidReceiveTitleForFrame>(decoder, this, &WebPageProxy::didReceiveTitleForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDisplayInsecureContentForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidDisplayInsecureContentForFrame>(decoder, this, &WebPageProxy::didDisplayInsecureContentForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRunInsecureContentForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidRunInsecureContentForFrame>(decoder, this, &WebPageProxy::didRunInsecureContentForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDetectXSSForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidDetectXSSForFrame>(decoder, this, &WebPageProxy::didDetectXSSForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidSameDocumentNavigationForFrame::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DidSameDocumentNavigationForFrame>(decoder, this, &WebPageProxy::didSameDocumentNavigationForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::FrameDidBecomeFrameSet::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::FrameDidBecomeFrameSet>(decoder, this, &WebPageProxy::frameDidBecomeFrameSet);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidInitiateLoadForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidInitiateLoadForResource>(decoder, this, &WebPageProxy::didInitiateLoadForResource);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidSendRequestForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidSendRequestForResource>(decoder, this, &WebPageProxy::didSendRequestForResource);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveResponseForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidReceiveResponseForResource>(decoder, this, &WebPageProxy::didReceiveResponseForResource);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveContentLengthForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidReceiveContentLengthForResource>(decoder, this, &WebPageProxy::didReceiveContentLengthForResource);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFinishLoadForResource>(decoder, this, &WebPageProxy::didFinishLoadForResource);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailLoadForResource::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFailLoadForResource>(decoder, this, &WebPageProxy::didFailLoadForResource);
        return;
    }
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::ResolveApplicationSchemeRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ResolveApplicationSchemeRequest>(decoder, this, &WebPageProxy::resolveApplicationSchemeRequest);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadingDataForCustomRepresentation::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFinishLoadingDataForCustomRepresentation>(decoder, this, &WebPageProxy::didFinishLoadingDataForCustomRepresentation);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::WillSubmitForm::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::WillSubmitForm>(decoder, this, &WebPageProxy::willSubmitForm);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::VoidCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::VoidCallback>(decoder, this, &WebPageProxy::voidCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DataCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DataCallback>(decoder, this, &WebPageProxy::dataCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ImageCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ImageCallback>(decoder, this, &WebPageProxy::imageCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::StringCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::StringCallback>(decoder, this, &WebPageProxy::stringCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ScriptValueCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ScriptValueCallback>(decoder, this, &WebPageProxy::scriptValueCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ComputedPagesCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ComputedPagesCallback>(decoder, this, &WebPageProxy::computedPagesCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ValidateCommandCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ValidateCommandCallback>(decoder, this, &WebPageProxy::validateCommandCallback);
        return;
    }
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::PrintFinishedCallback::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PrintFinishedCallback>(decoder, this, &WebPageProxy::printFinishedCallback);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::PageScaleFactorDidChange::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PageScaleFactorDidChange>(decoder, this, &WebPageProxy::pageScaleFactorDidChange);
        return;
    }
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::BindAccessibilityTree::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BindAccessibilityTree>(decoder, this, &WebPageProxy::bindAccessibilityTree);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardAddItem::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardAddItem>(decoder, this, &WebPageProxy::backForwardAddItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardClear::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardClear>(decoder, this, &WebPageProxy::backForwardClear);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::WillGoToBackForwardListItem::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::WillGoToBackForwardListItem>(decoder, this, &WebPageProxy::willGoToBackForwardListItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RegisterEditCommandForUndo::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RegisterEditCommandForUndo>(decoder, this, &WebPageProxy::registerEditCommandForUndo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ClearAllEditCommands::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ClearAllEditCommands>(decoder, this, &WebPageProxy::clearAllEditCommands);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::EditorStateChanged::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::EditorStateChanged>(decoder, this, &WebPageProxy::editorStateChanged);
        return;
    }
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeCompositionSelection::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidChangeCompositionSelection>(decoder, this, &WebPageProxy::didChangeCompositionSelection);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::WillSetInputMethodState::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::WillSetInputMethodState>(decoder, this, &WebPageProxy::willSetInputMethodState);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidCountStringMatches::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidCountStringMatches>(decoder, this, &WebPageProxy::didCountStringMatches);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetFindIndicator::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetFindIndicator>(decoder, this, &WebPageProxy::setFindIndicator);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFindString::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFindString>(decoder, this, &WebPageProxy::didFindString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailToFindString::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidFailToFindString>(decoder, this, &WebPageProxy::didFailToFindString);
        return;
    }
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPageProxy::DidInstallOrUninstallPageOverlay::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidInstallOrUninstallPageOverlay>(decoder, this, &WebPageProxy::didInstallOrUninstallPageOverlay);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::ShowPopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowPopupMenu>(decoder, this, &WebPageProxy::showPopupMenu);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::HidePopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::HidePopupMenu>(decoder, this, &WebPageProxy::hidePopupMenu);
        return;
    }
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPageProxy::SetPopupMenuSelectedIndex::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetPopupMenuSelectedIndex>(decoder, this, &WebPageProxy::setPopupMenuSelectedIndex);
        return;
    }
#endif
#if ENABLE(CONTEXT_MENUS)
    if (decoder.messageName() == Messages::WebPageProxy::ShowContextMenu::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::ShowContextMenu>(decoder, this, &WebPageProxy::showContextMenu);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveAuthenticationChallenge::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidReceiveAuthenticationChallenge>(decoder, this, &WebPageProxy::didReceiveAuthenticationChallenge);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RequestGeolocationPermissionForFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RequestGeolocationPermissionForFrame>(decoder, this, &WebPageProxy::requestGeolocationPermissionForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RequestNotificationPermission::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RequestNotificationPermission>(decoder, this, &WebPageProxy::requestNotificationPermission);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShowNotification::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowNotification>(decoder, this, &WebPageProxy::showNotification);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UpdateSpellingUIWithMisspelledWord::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::UpdateSpellingUIWithMisspelledWord>(decoder, this, &WebPageProxy::updateSpellingUIWithMisspelledWord);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UpdateSpellingUIWithGrammarString::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::UpdateSpellingUIWithGrammarString>(decoder, this, &WebPageProxy::updateSpellingUIWithGrammarString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::LearnWord::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::LearnWord>(decoder, this, &WebPageProxy::learnWord);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::IgnoreWord::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::IgnoreWord>(decoder, this, &WebPageProxy::ignoreWord);
        return;
    }
#if ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::DidPerformDragControllerAction::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidPerformDragControllerAction>(decoder, this, &WebPageProxy::didPerformDragControllerAction);
        return;
    }
#endif
#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::SetDragImage::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetDragImage>(decoder, this, &WebPageProxy::setDragImage);
        return;
    }
#endif
#if PLATFORM(MAC) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::SetPromisedData::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetPromisedData>(decoder, this, &WebPageProxy::setPromisedData);
        return;
    }
#endif
#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::StartDragDrop::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::StartDragDrop>(decoder, this, &WebPageProxy::startDragDrop);
        return;
    }
#endif
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::StartDrag::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::StartDrag>(decoder, this, &WebPageProxy::startDrag);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::DidPerformDictionaryLookup::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidPerformDictionaryLookup>(decoder, this, &WebPageProxy::didPerformDictionaryLookup);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::RegisterWebProcessAccessibilityToken::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RegisterWebProcessAccessibilityToken>(decoder, this, &WebPageProxy::registerWebProcessAccessibilityToken);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::PluginFocusOrWindowFocusChanged::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PluginFocusOrWindowFocusChanged>(decoder, this, &WebPageProxy::pluginFocusOrWindowFocusChanged);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::SetPluginComplexTextInputState::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetPluginComplexTextInputState>(decoder, this, &WebPageProxy::setPluginComplexTextInputState);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::Speak::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::Speak>(decoder, this, &WebPageProxy::speak);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::StopSpeaking::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::StopSpeaking>(decoder, this, &WebPageProxy::stopSpeaking);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::MakeFirstResponder::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::MakeFirstResponder>(decoder, this, &WebPageProxy::makeFirstResponder);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::SearchWithSpotlight::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SearchWithSpotlight>(decoder, this, &WebPageProxy::searchWithSpotlight);
        return;
    }
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    if (decoder.messageName() == Messages::WebPageProxy::ShowCorrectionPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowCorrectionPanel>(decoder, this, &WebPageProxy::showCorrectionPanel);
        return;
    }
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    if (decoder.messageName() == Messages::WebPageProxy::DismissCorrectionPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DismissCorrectionPanel>(decoder, this, &WebPageProxy::dismissCorrectionPanel);
        return;
    }
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    if (decoder.messageName() == Messages::WebPageProxy::RecordAutocorrectionResponse::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RecordAutocorrectionResponse>(decoder, this, &WebPageProxy::recordAutocorrectionResponse);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::ShowDictationAlternativeUI::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShowDictationAlternativeUI>(decoder, this, &WebPageProxy::showDictationAlternativeUI);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::DismissDictationAlternativeUI::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DismissDictationAlternativeUI>(decoder, this, &WebPageProxy::dismissDictationAlternativeUI);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::RemoveDictationAlternatives::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RemoveDictationAlternatives>(decoder, this, &WebPageProxy::removeDictationAlternatives);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPageProxy::SetGestureReachedScrollingLimit::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SetGestureReachedScrollingLimit>(decoder, this, &WebPageProxy::setGestureReachedScrollingLimit);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPageProxy::ScheduleChildWindowGeometryUpdate::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ScheduleChildWindowGeometryUpdate>(decoder, this, &WebPageProxy::scheduleChildWindowGeometryUpdate);
        return;
    }
#endif
#if PLUGIN_ARCHITECTURE(X11)
    if (decoder.messageName() == Messages::WebPageProxy::WindowedPluginGeometryDidChange::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::WindowedPluginGeometryDidChange>(decoder, this, &WebPageProxy::windowedPluginGeometryDidChange);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SaveRecentSearches::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SaveRecentSearches>(decoder, this, &WebPageProxy::saveRecentSearches);
        return;
    }
#if USE(SOUP)
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveURIRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DidReceiveURIRequest>(decoder, this, &WebPageProxy::didReceiveURIRequest);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SavePDFToFileInDownloadsFolder::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SavePDFToFileInDownloadsFolder>(decoder, this, &WebPageProxy::savePDFToFileInDownloadsFolder);
        return;
    }
    ASSERT_NOT_REACHED();
}

void WebPageProxy::didReceiveSyncWebPageProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebPageProxy::CreateNewPage::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CreateNewPage>(decoder, *replyEncoder, this, &WebPageProxy::createNewPage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptAlert::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunJavaScriptAlert>(decoder, *replyEncoder, this, &WebPageProxy::runJavaScriptAlert);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptConfirm::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunJavaScriptConfirm>(decoder, *replyEncoder, this, &WebPageProxy::runJavaScriptConfirm);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptPrompt::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunJavaScriptPrompt>(decoder, *replyEncoder, this, &WebPageProxy::runJavaScriptPrompt);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShouldInterruptJavaScript::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShouldInterruptJavaScript>(decoder, *replyEncoder, this, &WebPageProxy::shouldInterruptJavaScript);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetToolbarsAreVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetToolbarsAreVisible>(decoder, *replyEncoder, this, &WebPageProxy::getToolbarsAreVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetMenuBarIsVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetMenuBarIsVisible>(decoder, *replyEncoder, this, &WebPageProxy::getMenuBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetStatusBarIsVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetStatusBarIsVisible>(decoder, *replyEncoder, this, &WebPageProxy::getStatusBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetIsResizable::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetIsResizable>(decoder, *replyEncoder, this, &WebPageProxy::getIsResizable);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetWindowFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetWindowFrame>(decoder, *replyEncoder, this, &WebPageProxy::getWindowFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ScreenToWindow::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ScreenToWindow>(decoder, *replyEncoder, this, &WebPageProxy::screenToWindow);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::WindowToScreen::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::WindowToScreen>(decoder, *replyEncoder, this, &WebPageProxy::windowToScreen);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunBeforeUnloadConfirmPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::RunBeforeUnloadConfirmPanel>(decoder, *replyEncoder, this, &WebPageProxy::runBeforeUnloadConfirmPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PrintFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::PrintFrame>(decoder, *replyEncoder, this, &WebPageProxy::printFrame);
        return;
    }
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPageProxy::HandleInputMethodKeydown::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::HandleInputMethodKeydown>(decoder, *replyEncoder, this, &WebPageProxy::handleInputMethodKeydown);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::AuthenticationRequiredRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::AuthenticationRequiredRequest>(decoder, *replyEncoder, this, &WebPageProxy::authenticationRequiredRequest);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::CertificateVerificationRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CertificateVerificationRequest>(decoder, *replyEncoder, this, &WebPageProxy::certificateVerificationRequest);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPageProxy::ProxyAuthenticationRequiredRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ProxyAuthenticationRequiredRequest>(decoder, *replyEncoder, this, &WebPageProxy::proxyAuthenticationRequiredRequest);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForResponse::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForResponse>(decoder, *replyEncoder, this, &WebPageProxy::decidePolicyForResponse);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForNavigationAction::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForNavigationAction>(decoder, *replyEncoder, this, &WebPageProxy::decidePolicyForNavigationAction);
        return;
    }
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::GetEditorCommandsForKeyEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetEditorCommandsForKeyEvent>(decoder, *replyEncoder, this, &WebPageProxy::getEditorCommandsForKeyEvent);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardGoToItem::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardGoToItem>(decoder, *replyEncoder, this, &WebPageProxy::backForwardGoToItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardItemAtIndex::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardItemAtIndex>(decoder, *replyEncoder, this, &WebPageProxy::backForwardItemAtIndex);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardBackListCount::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardBackListCount>(decoder, *replyEncoder, this, &WebPageProxy::backForwardBackListCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardForwardListCount::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::BackForwardForwardListCount>(decoder, *replyEncoder, this, &WebPageProxy::backForwardForwardListCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShouldGoToBackForwardListItem::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ShouldGoToBackForwardListItem>(decoder, *replyEncoder, this, &WebPageProxy::shouldGoToBackForwardListItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CanUndoRedo::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CanUndoRedo>(decoder, *replyEncoder, this, &WebPageProxy::canUndoRedo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ExecuteUndoRedo::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ExecuteUndoRedo>(decoder, *replyEncoder, this, &WebPageProxy::executeUndoRedo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CanAuthenticateAgainstProtectionSpaceInFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CanAuthenticateAgainstProtectionSpaceInFrame>(decoder, *replyEncoder, this, &WebPageProxy::canAuthenticateAgainstProtectionSpaceInFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ExceededDatabaseQuota::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ExceededDatabaseQuota>(decoder, *replyEncoder, this, &WebPageProxy::exceededDatabaseQuota);
        return;
    }
#if USE(UNIFIED_TEXT_CHECKING)
    if (decoder.messageName() == Messages::WebPageProxy::CheckTextOfParagraph::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CheckTextOfParagraph>(decoder, *replyEncoder, this, &WebPageProxy::checkTextOfParagraph);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::CheckSpellingOfString::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CheckSpellingOfString>(decoder, *replyEncoder, this, &WebPageProxy::checkSpellingOfString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CheckGrammarOfString::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CheckGrammarOfString>(decoder, *replyEncoder, this, &WebPageProxy::checkGrammarOfString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SpellingUIIsShowing::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SpellingUIIsShowing>(decoder, *replyEncoder, this, &WebPageProxy::spellingUIIsShowing);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetGuessesForWord::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetGuessesForWord>(decoder, *replyEncoder, this, &WebPageProxy::getGuessesForWord);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::InterpretQueuedKeyEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::InterpretQueuedKeyEvent>(decoder, *replyEncoder, this, &WebPageProxy::interpretQueuedKeyEvent);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::ExecuteSavedCommandBySelector::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::ExecuteSavedCommandBySelector>(decoder, *replyEncoder, this, &WebPageProxy::executeSavedCommandBySelector);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::GetIsSpeaking::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::GetIsSpeaking>(decoder, *replyEncoder, this, &WebPageProxy::getIsSpeaking);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::SubstitutionsPanelIsShowing::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::SubstitutionsPanelIsShowing>(decoder, *replyEncoder, this, &WebPageProxy::substitutionsPanelIsShowing);
        return;
    }
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    if (decoder.messageName() == Messages::WebPageProxy::DismissCorrectionPanelSoon::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DismissCorrectionPanelSoon>(decoder, *replyEncoder, this, &WebPageProxy::dismissCorrectionPanelSoon);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::DictationAlternatives::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::DictationAlternatives>(decoder, *replyEncoder, this, &WebPageProxy::dictationAlternatives);
        return;
    }
#endif
#if PLUGIN_ARCHITECTURE(X11)
    if (decoder.messageName() == Messages::WebPageProxy::CreatePluginContainer::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::CreatePluginContainer>(decoder, *replyEncoder, this, &WebPageProxy::createPluginContainer);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::LoadRecentSearches::name()) {
        CoreIPC::handleMessage<Messages::WebPageProxy::LoadRecentSearches>(decoder, *replyEncoder, this, &WebPageProxy::loadRecentSearches);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
