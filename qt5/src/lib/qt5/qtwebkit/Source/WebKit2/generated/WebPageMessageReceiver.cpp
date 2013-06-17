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

#include "WebPage.h"

#include "ArgumentCoders.h"
#if PLATFORM(MAC)
#include "AttributedString.h"
#endif
#include "DataReference.h"
#if PLATFORM(MAC)
#include "EditorState.h"
#endif
#include "HandleMessage.h"
#if ENABLE(WEB_INTENTS)
#include "IntentData.h"
#endif
#include "MessageDecoder.h"
#include "PrintInfo.h"
#if PLATFORM(QT)
#include "QtNetworkReplyData.h"
#endif
#include "SandboxExtension.h"
#include "SessionState.h"
#if PLATFORM(MAC)
#include "SharedMemory.h"
#endif
#if ENABLE(CONTEXT_MENUS)
#include "WebContextMenuItemData.h"
#endif
#include "WebContextUserMessageCoders.h"
#include "WebCoreArgumentCoders.h"
#include "WebEvent.h"
#include "WebPageMessages.h"
#include "WebPreferencesStore.h"
#if ENABLE(INPUT_TYPE_COLOR)
#include <WebCore/Color.h>
#endif
#if PLATFORM(MAC)
#include <WebCore/DictationAlternative.h>
#endif
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
#include <WebCore/DragData.h>
#endif
#if PLATFORM(MAC) || PLATFORM(QT) || PLATFORM(EFL) || PLATFORM(WIN)
#include <WebCore/Editor.h>
#endif
#if PLATFORM(MAC)
#include <WebCore/FloatPoint.h>
#endif
#include <WebCore/IntPoint.h>
#if PLATFORM(MAC) || PLATFORM(MAC) || PLATFORM(WIN) || PLATFORM(WIN)
#include <WebCore/IntRect.h>
#endif
#include <WebCore/IntSize.h>
#if PLATFORM(MAC)
#include <WebCore/KeyboardEvent.h>
#endif
#include <WebCore/ResourceRequest.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

void WebPage::didReceiveWebPageMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebPage::SetActive::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetActive>(decoder, this, &WebPage::setActive);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetFocused::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetFocused>(decoder, this, &WebPage::setFocused);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetInitialFocus::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetInitialFocus>(decoder, this, &WebPage::setInitialFocus);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetIsInWindow::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetIsInWindow>(decoder, this, &WebPage::setIsInWindow);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetDrawsBackground::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetDrawsBackground>(decoder, this, &WebPage::setDrawsBackground);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetDrawsTransparentBackground::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetDrawsTransparentBackground>(decoder, this, &WebPage::setDrawsTransparentBackground);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ViewWillStartLiveResize::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ViewWillStartLiveResize>(decoder, this, &WebPage::viewWillStartLiveResize);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ViewWillEndLiveResize::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ViewWillEndLiveResize>(decoder, this, &WebPage::viewWillEndLiveResize);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::KeyEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPage::KeyEvent>(decoder, this, &WebPage::keyEvent);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::MouseEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPage::MouseEvent>(decoder, this, &WebPage::mouseEvent);
        return;
    }
#if ENABLE(TOUCH_EVENTS)
    if (decoder.messageName() == Messages::WebPage::TouchEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPage::TouchEvent>(decoder, this, &WebPage::touchEvent);
        return;
    }
#endif
#if ENABLE(TOUCH_EVENTS) && PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::HighlightPotentialActivation::name()) {
        CoreIPC::handleMessage<Messages::WebPage::HighlightPotentialActivation>(decoder, this, &WebPage::highlightPotentialActivation);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPage::DidEndColorChooser::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidEndColorChooser>(decoder, this, &WebPage::didEndColorChooser);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPage::DidChooseColor::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidChooseColor>(decoder, this, &WebPage::didChooseColor);
        return;
    }
#endif
#if ENABLE(CONTEXT_MENUS)
    if (decoder.messageName() == Messages::WebPage::ContextMenuHidden::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ContextMenuHidden>(decoder, this, &WebPage::contextMenuHidden);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::ScrollBy::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ScrollBy>(decoder, this, &WebPage::scrollBy);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::CenterSelectionInVisibleArea::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CenterSelectionInVisibleArea>(decoder, this, &WebPage::centerSelectionInVisibleArea);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GoBack::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GoBack>(decoder, this, &WebPage::goBack);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GoForward::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GoForward>(decoder, this, &WebPage::goForward);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GoToBackForwardItem::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GoToBackForwardItem>(decoder, this, &WebPage::goToBackForwardItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::TryRestoreScrollPosition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::TryRestoreScrollPosition>(decoder, this, &WebPage::tryRestoreScrollPosition);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadHTMLString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadHTMLString>(decoder, this, &WebPage::loadHTMLString);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadAlternateHTMLString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadAlternateHTMLString>(decoder, this, &WebPage::loadAlternateHTMLString);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadPlainTextString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadPlainTextString>(decoder, this, &WebPage::loadPlainTextString);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadWebArchiveData::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadWebArchiveData>(decoder, this, &WebPage::loadWebArchiveData);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadURL::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadURL>(decoder, this, &WebPage::loadURL);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LoadURLRequest::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LoadURLRequest>(decoder, this, &WebPage::loadURLRequest);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::LinkClicked::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LinkClicked>(decoder, this, &WebPage::linkClicked);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::Reload::name()) {
        CoreIPC::handleMessage<Messages::WebPage::Reload>(decoder, this, &WebPage::reload);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::StopLoading::name()) {
        CoreIPC::handleMessage<Messages::WebPage::StopLoading>(decoder, this, &WebPage::stopLoading);
        return;
    }
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::ApplicationSchemeReply::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ApplicationSchemeReply>(decoder, this, &WebPage::applicationSchemeReply);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::RegisterApplicationScheme::name()) {
        CoreIPC::handleMessage<Messages::WebPage::RegisterApplicationScheme>(decoder, this, &WebPage::registerApplicationScheme);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::SetUserScripts::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetUserScripts>(decoder, this, &WebPage::setUserScripts);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::StopLoadingFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::StopLoadingFrame>(decoder, this, &WebPage::stopLoadingFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::RestoreSession::name()) {
        CoreIPC::handleMessage<Messages::WebPage::RestoreSession>(decoder, this, &WebPage::restoreSession);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::RestoreSessionAndNavigateToCurrentItem::name()) {
        CoreIPC::handleMessage<Messages::WebPage::RestoreSessionAndNavigateToCurrentItem>(decoder, this, &WebPage::restoreSessionAndNavigateToCurrentItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::DidRemoveBackForwardItem::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidRemoveBackForwardItem>(decoder, this, &WebPage::didRemoveBackForwardItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetWillGoToBackForwardItemCallbackEnabled::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetWillGoToBackForwardItemCallbackEnabled>(decoder, this, &WebPage::setWillGoToBackForwardItemCallbackEnabled);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::DidReceivePolicyDecision::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidReceivePolicyDecision>(decoder, this, &WebPage::didReceivePolicyDecision);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ClearSelection::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ClearSelection>(decoder, this, &WebPage::clearSelection);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetContentsAsString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetContentsAsString>(decoder, this, &WebPage::getContentsAsString);
        return;
    }
#if ENABLE(MHTML)
    if (decoder.messageName() == Messages::WebPage::GetContentsAsMHTMLData::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetContentsAsMHTMLData>(decoder, this, &WebPage::getContentsAsMHTMLData);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::GetMainResourceDataOfFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetMainResourceDataOfFrame>(decoder, this, &WebPage::getMainResourceDataOfFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetResourceDataFromFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetResourceDataFromFrame>(decoder, this, &WebPage::getResourceDataFromFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetRenderTreeExternalRepresentation::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetRenderTreeExternalRepresentation>(decoder, this, &WebPage::getRenderTreeExternalRepresentation);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetSelectionOrContentsAsString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetSelectionOrContentsAsString>(decoder, this, &WebPage::getSelectionOrContentsAsString);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetSourceForFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetSourceForFrame>(decoder, this, &WebPage::getSourceForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::GetWebArchiveOfFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetWebArchiveOfFrame>(decoder, this, &WebPage::getWebArchiveOfFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::RunJavaScriptInMainFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::RunJavaScriptInMainFrame>(decoder, this, &WebPage::runJavaScriptInMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ForceRepaint::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ForceRepaint>(decoder, this, &WebPage::forceRepaint);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::PerformDictionaryLookupAtLocation::name()) {
        CoreIPC::handleMessage<Messages::WebPage::PerformDictionaryLookupAtLocation>(decoder, this, &WebPage::performDictionaryLookupAtLocation);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::PreferencesDidChange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::PreferencesDidChange>(decoder, this, &WebPage::preferencesDidChange);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetUserAgent::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetUserAgent>(decoder, this, &WebPage::setUserAgent);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetCustomTextEncodingName::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetCustomTextEncodingName>(decoder, this, &WebPage::setCustomTextEncodingName);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SuspendActiveDOMObjectsAndAnimations::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SuspendActiveDOMObjectsAndAnimations>(decoder, this, &WebPage::suspendActiveDOMObjectsAndAnimations);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ResumeActiveDOMObjectsAndAnimations::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ResumeActiveDOMObjectsAndAnimations>(decoder, this, &WebPage::resumeActiveDOMObjectsAndAnimations);
        return;
    }
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPage::SetViewportSize::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetViewportSize>(decoder, this, &WebPage::setViewportSize);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::Close::name()) {
        CoreIPC::handleMessage<Messages::WebPage::Close>(decoder, this, &WebPage::close);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::TryClose::name()) {
        CoreIPC::handleMessage<Messages::WebPage::TryClose>(decoder, this, &WebPage::tryClose);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ValidateCommand::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ValidateCommand>(decoder, this, &WebPage::validateCommand);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ExecuteEditCommand::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ExecuteEditCommand>(decoder, this, &WebPage::executeEditCommand);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::DidRemoveEditCommand::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidRemoveEditCommand>(decoder, this, &WebPage::didRemoveEditCommand);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ReapplyEditCommand::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ReapplyEditCommand>(decoder, this, &WebPage::reapplyEditCommand);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::UnapplyEditCommand::name()) {
        CoreIPC::handleMessage<Messages::WebPage::UnapplyEditCommand>(decoder, this, &WebPage::unapplyEditCommand);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetPageAndTextZoomFactors::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetPageAndTextZoomFactors>(decoder, this, &WebPage::setPageAndTextZoomFactors);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetPageZoomFactor::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetPageZoomFactor>(decoder, this, &WebPage::setPageZoomFactor);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetTextZoomFactor::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetTextZoomFactor>(decoder, this, &WebPage::setTextZoomFactor);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::WindowScreenDidChange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::WindowScreenDidChange>(decoder, this, &WebPage::windowScreenDidChange);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ScalePage::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ScalePage>(decoder, this, &WebPage::scalePage);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetUseFixedLayout::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetUseFixedLayout>(decoder, this, &WebPage::setUseFixedLayout);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetFixedLayoutSize::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetFixedLayoutSize>(decoder, this, &WebPage::setFixedLayoutSize);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ListenForLayoutMilestones::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ListenForLayoutMilestones>(decoder, this, &WebPage::listenForLayoutMilestones);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetSuppressScrollbarAnimations::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetSuppressScrollbarAnimations>(decoder, this, &WebPage::setSuppressScrollbarAnimations);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetPaginationMode::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetPaginationMode>(decoder, this, &WebPage::setPaginationMode);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetPaginationBehavesLikeColumns::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetPaginationBehavesLikeColumns>(decoder, this, &WebPage::setPaginationBehavesLikeColumns);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetPageLength::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetPageLength>(decoder, this, &WebPage::setPageLength);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetGapBetweenPages::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetGapBetweenPages>(decoder, this, &WebPage::setGapBetweenPages);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::PostInjectedBundleMessage::name()) {
        CoreIPC::handleMessageVariadic<Messages::WebPage::PostInjectedBundleMessage>(decoder, this, &WebPage::postInjectedBundleMessage);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::FindString::name()) {
        CoreIPC::handleMessage<Messages::WebPage::FindString>(decoder, this, &WebPage::findString);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::HideFindUI::name()) {
        CoreIPC::handleMessage<Messages::WebPage::HideFindUI>(decoder, this, &WebPage::hideFindUI);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::CountStringMatches::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CountStringMatches>(decoder, this, &WebPage::countStringMatches);
        return;
    }
#if PLATFORM(WIN) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPage::PerformDragControllerAction::name()) {
        CoreIPC::handleMessage<Messages::WebPage::PerformDragControllerAction>(decoder, this, &WebPage::performDragControllerAction);
        return;
    }
#endif
#if (PLATFORM(QT) || PLATFORM(GTK)) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPage::PerformDragControllerAction::name()) {
        CoreIPC::handleMessage<Messages::WebPage::PerformDragControllerAction>(decoder, this, &WebPage::performDragControllerAction);
        return;
    }
#endif
#if !PLATFORM(WIN) && !PLATFORM(QT) && !PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPage::PerformDragControllerAction::name()) {
        CoreIPC::handleMessage<Messages::WebPage::PerformDragControllerAction>(decoder, this, &WebPage::performDragControllerAction);
        return;
    }
#endif
#if ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPage::DragEnded::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DragEnded>(decoder, this, &WebPage::dragEnded);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::DidChangeSelectedIndexForActivePopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidChangeSelectedIndexForActivePopupMenu>(decoder, this, &WebPage::didChangeSelectedIndexForActivePopupMenu);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetTextForActivePopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetTextForActivePopupMenu>(decoder, this, &WebPage::setTextForActivePopupMenu);
        return;
    }
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPage::FailedToShowPopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPage::FailedToShowPopupMenu>(decoder, this, &WebPage::failedToShowPopupMenu);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::HidePopupMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPage::HidePopupMenu>(decoder, this, &WebPage::hidePopupMenu);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::SelectedIndex::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SelectedIndex>(decoder, this, &WebPage::selectedIndex);
        return;
    }
#endif
#if ENABLE(CONTEXT_MENUS)
    if (decoder.messageName() == Messages::WebPage::DidSelectItemFromActiveContextMenu::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidSelectItemFromActiveContextMenu>(decoder, this, &WebPage::didSelectItemFromActiveContextMenu);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::DidChooseFilesForOpenPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidChooseFilesForOpenPanel>(decoder, this, &WebPage::didChooseFilesForOpenPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::DidCancelForOpenPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidCancelForOpenPanel>(decoder, this, &WebPage::didCancelForOpenPanel);
        return;
    }
#if ENABLE(WEB_PROCESS_SANDBOX)
    if (decoder.messageName() == Messages::WebPage::ExtendSandboxForFileFromOpenPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ExtendSandboxForFileFromOpenPanel>(decoder, this, &WebPage::extendSandboxForFileFromOpenPanel);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::AdvanceToNextMisspelling::name()) {
        CoreIPC::handleMessage<Messages::WebPage::AdvanceToNextMisspelling>(decoder, this, &WebPage::advanceToNextMisspelling);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ChangeSpellingToWord::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ChangeSpellingToWord>(decoder, this, &WebPage::changeSpellingToWord);
        return;
    }
#if USE(APPKIT)
    if (decoder.messageName() == Messages::WebPage::UppercaseWord::name()) {
        CoreIPC::handleMessage<Messages::WebPage::UppercaseWord>(decoder, this, &WebPage::uppercaseWord);
        return;
    }
#endif
#if USE(APPKIT)
    if (decoder.messageName() == Messages::WebPage::LowercaseWord::name()) {
        CoreIPC::handleMessage<Messages::WebPage::LowercaseWord>(decoder, this, &WebPage::lowercaseWord);
        return;
    }
#endif
#if USE(APPKIT)
    if (decoder.messageName() == Messages::WebPage::CapitalizeWord::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CapitalizeWord>(decoder, this, &WebPage::capitalizeWord);
        return;
    }
#endif
#if USE(APPKIT)
    if (decoder.messageName() == Messages::WebPage::SetSmartInsertDeleteEnabled::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetSmartInsertDeleteEnabled>(decoder, this, &WebPage::setSmartInsertDeleteEnabled);
        return;
    }
#endif
#if ENABLE(GEOLOCATION)
    if (decoder.messageName() == Messages::WebPage::DidReceiveGeolocationPermissionDecision::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidReceiveGeolocationPermissionDecision>(decoder, this, &WebPage::didReceiveGeolocationPermissionDecision);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::DidReceiveNotificationPermissionDecision::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DidReceiveNotificationPermissionDecision>(decoder, this, &WebPage::didReceiveNotificationPermissionDecision);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetWindowResizerSize::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetWindowResizerSize>(decoder, this, &WebPage::setWindowResizerSize);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::BeginPrinting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::BeginPrinting>(decoder, this, &WebPage::beginPrinting);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::EndPrinting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::EndPrinting>(decoder, this, &WebPage::endPrinting);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::ComputePagesForPrinting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ComputePagesForPrinting>(decoder, this, &WebPage::computePagesForPrinting);
        return;
    }
#if PLATFORM(MAC) || PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::DrawRectToImage::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DrawRectToImage>(decoder, this, &WebPage::drawRectToImage);
        return;
    }
#endif
#if PLATFORM(MAC) || PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::DrawPagesToPDF::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DrawPagesToPDF>(decoder, this, &WebPage::drawPagesToPDF);
        return;
    }
#endif
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPage::DrawPagesForPrinting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DrawPagesForPrinting>(decoder, this, &WebPage::drawPagesForPrinting);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::SetMediaVolume::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetMediaVolume>(decoder, this, &WebPage::setMediaVolume);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetMayStartMediaWhenInWindow::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetMayStartMediaWhenInWindow>(decoder, this, &WebPage::setMayStartMediaWhenInWindow);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetMemoryCacheMessagesEnabled::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetMemoryCacheMessagesEnabled>(decoder, this, &WebPage::setMemoryCacheMessagesEnabled);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetCanRunBeforeUnloadConfirmPanel::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetCanRunBeforeUnloadConfirmPanel>(decoder, this, &WebPage::setCanRunBeforeUnloadConfirmPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::SetCanRunModal::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetCanRunModal>(decoder, this, &WebPage::setCanRunModal);
        return;
    }
#if ENABLE(WEB_INTENTS)
    if (decoder.messageName() == Messages::WebPage::DeliverIntentToFrame::name()) {
        CoreIPC::handleMessage<Messages::WebPage::DeliverIntentToFrame>(decoder, this, &WebPage::deliverIntentToFrame);
        return;
    }
#endif
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPage::SetThemePath::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetThemePath>(decoder, this, &WebPage::setThemePath);
        return;
    }
#endif
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPage::CommitPageTransitionViewport::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CommitPageTransitionViewport>(decoder, this, &WebPage::commitPageTransitionViewport);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::SetComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetComposition>(decoder, this, &WebPage::setComposition);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::ConfirmComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ConfirmComposition>(decoder, this, &WebPage::confirmComposition);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::CancelComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CancelComposition>(decoder, this, &WebPage::cancelComposition);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::SendComplexTextInputToPlugin::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SendComplexTextInputToPlugin>(decoder, this, &WebPage::sendComplexTextInputToPlugin);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::SetWindowIsVisible::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetWindowIsVisible>(decoder, this, &WebPage::setWindowIsVisible);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::WindowAndViewFramesChanged::name()) {
        CoreIPC::handleMessage<Messages::WebPage::WindowAndViewFramesChanged>(decoder, this, &WebPage::windowAndViewFramesChanged);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::RegisterUIProcessAccessibilityTokens::name()) {
        CoreIPC::handleMessage<Messages::WebPage::RegisterUIProcessAccessibilityTokens>(decoder, this, &WebPage::registerUIProcessAccessibilityTokens);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::ConfirmComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ConfirmComposition>(decoder, this, &WebPage::confirmComposition);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::SetComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetComposition>(decoder, this, &WebPage::setComposition);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::GestureDidScroll::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GestureDidScroll>(decoder, this, &WebPage::gestureDidScroll);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::GestureDidEnd::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GestureDidEnd>(decoder, this, &WebPage::gestureDidEnd);
        return;
    }
#endif
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPage::ConfirmComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ConfirmComposition>(decoder, this, &WebPage::confirmComposition);
        return;
    }
#endif
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPage::SetComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetComposition>(decoder, this, &WebPage::setComposition);
        return;
    }
#endif
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPage::CancelComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CancelComposition>(decoder, this, &WebPage::cancelComposition);
        return;
    }
#endif
#if PLATFORM(QT)
    if (decoder.messageName() == Messages::WebPage::FindZoomableAreaForPoint::name()) {
        CoreIPC::handleMessage<Messages::WebPage::FindZoomableAreaForPoint>(decoder, this, &WebPage::findZoomableAreaForPoint);
        return;
    }
#endif
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
    if (decoder.messageName() == Messages::WebPage::HandleAlternativeTextUIResult::name()) {
        CoreIPC::handleMessage<Messages::WebPage::HandleAlternativeTextUIResult>(decoder, this, &WebPage::handleAlternativeTextUIResult);
        return;
    }
#endif
#if ENABLE(PAGE_VISIBILITY_API) || ENABLE(HIDDEN_PAGE_DOM_TIMER_THROTTLING)
    if (decoder.messageName() == Messages::WebPage::SetVisibilityState::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetVisibilityState>(decoder, this, &WebPage::setVisibilityState);
        return;
    }
#endif
#if PLATFORM(GTK) && USE(TEXTURE_MAPPER_GL)
    if (decoder.messageName() == Messages::WebPage::SetAcceleratedCompositingWindowId::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetAcceleratedCompositingWindowId>(decoder, this, &WebPage::setAcceleratedCompositingWindowId);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

void WebPage::didReceiveSyncWebPageMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder, OwnPtr<CoreIPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebPage::KeyEventSyncForTesting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::KeyEventSyncForTesting>(decoder, *replyEncoder, this, &WebPage::keyEventSyncForTesting);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::MouseEventSyncForTesting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::MouseEventSyncForTesting>(decoder, *replyEncoder, this, &WebPage::mouseEventSyncForTesting);
        return;
    }
    if (decoder.messageName() == Messages::WebPage::WheelEventSyncForTesting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::WheelEventSyncForTesting>(decoder, *replyEncoder, this, &WebPage::wheelEventSyncForTesting);
        return;
    }
#if ENABLE(TOUCH_EVENTS)
    if (decoder.messageName() == Messages::WebPage::TouchEventSyncForTesting::name()) {
        CoreIPC::handleMessage<Messages::WebPage::TouchEventSyncForTesting>(decoder, *replyEncoder, this, &WebPage::touchEventSyncForTesting);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPage::Dummy::name()) {
        CoreIPC::handleMessage<Messages::WebPage::Dummy>(decoder, *replyEncoder, this, &WebPage::dummy);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::GetStringSelectionForPasteboard::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetStringSelectionForPasteboard>(decoder, *replyEncoder, this, &WebPage::getStringSelectionForPasteboard);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::GetDataSelectionForPasteboard::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetDataSelectionForPasteboard>(decoder, *replyEncoder, this, &WebPage::getDataSelectionForPasteboard);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::ReadSelectionFromPasteboard::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ReadSelectionFromPasteboard>(decoder, *replyEncoder, this, &WebPage::readSelectionFromPasteboard);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::SetComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::SetComposition>(decoder, *replyEncoder, this, &WebPage::setComposition);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::ConfirmComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ConfirmComposition>(decoder, *replyEncoder, this, &WebPage::confirmComposition);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::CancelComposition::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CancelComposition>(decoder, *replyEncoder, this, &WebPage::cancelComposition);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::InsertText::name()) {
        CoreIPC::handleMessage<Messages::WebPage::InsertText>(decoder, *replyEncoder, this, &WebPage::insertText);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::GetMarkedRange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetMarkedRange>(decoder, *replyEncoder, this, &WebPage::getMarkedRange);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::GetSelectedRange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetSelectedRange>(decoder, *replyEncoder, this, &WebPage::getSelectedRange);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::GetAttributedSubstringFromRange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetAttributedSubstringFromRange>(decoder, *replyEncoder, this, &WebPage::getAttributedSubstringFromRange);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::CharacterIndexForPoint::name()) {
        CoreIPC::handleMessage<Messages::WebPage::CharacterIndexForPoint>(decoder, *replyEncoder, this, &WebPage::characterIndexForPoint);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::FirstRectForCharacterRange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::FirstRectForCharacterRange>(decoder, *replyEncoder, this, &WebPage::firstRectForCharacterRange);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::ExecuteKeypressCommands::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ExecuteKeypressCommands>(decoder, *replyEncoder, this, &WebPage::executeKeypressCommands);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::ShouldDelayWindowOrderingEvent::name()) {
        CoreIPC::handleMessage<Messages::WebPage::ShouldDelayWindowOrderingEvent>(decoder, *replyEncoder, this, &WebPage::shouldDelayWindowOrderingEvent);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::AcceptsFirstMouse::name()) {
        CoreIPC::handleMessage<Messages::WebPage::AcceptsFirstMouse>(decoder, *replyEncoder, this, &WebPage::acceptsFirstMouse);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPage::InsertDictatedText::name()) {
        CoreIPC::handleMessage<Messages::WebPage::InsertDictatedText>(decoder, *replyEncoder, this, &WebPage::insertDictatedText);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::FirstRectForCharacterInSelectedRange::name()) {
        CoreIPC::handleMessage<Messages::WebPage::FirstRectForCharacterInSelectedRange>(decoder, *replyEncoder, this, &WebPage::firstRectForCharacterInSelectedRange);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::GetSelectedText::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GetSelectedText>(decoder, *replyEncoder, this, &WebPage::getSelectedText);
        return;
    }
#endif
#if PLATFORM(WIN)
    if (decoder.messageName() == Messages::WebPage::GestureWillBegin::name()) {
        CoreIPC::handleMessage<Messages::WebPage::GestureWillBegin>(decoder, *replyEncoder, this, &WebPage::gestureWillBegin);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
