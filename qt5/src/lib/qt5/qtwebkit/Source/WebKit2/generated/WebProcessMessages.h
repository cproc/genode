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

#ifndef WebProcessMessages_h
#define WebProcessMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "SandboxExtension.h"
#include "SharedMemory.h"
#include "StringReference.h"
#include <WebCore/LinkHash.h>
#include <WebCore/PluginData.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace CoreIPC {
    class DataReference;
}

namespace WTF {
    class String;
}

namespace WebCore {
    class ResourceRequest;
}

namespace WebKit {
    class WebContextUserMessageEncoder;
    struct WebProcessCreationParameters;
    struct TextCheckerState;
    class SecItemResponseData;
    class SecKeychainItemResponseData;
    struct WebPageCreationParameters;
}

namespace Messages {
namespace WebProcess {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("WebProcess");
}

enum Kind {
    InitializeWebProcessID,
    CreateWebPageID,
#if ENABLE(WEB_INTENTS)
    RemoveMessagePortChannelID,
#endif
    SetVisitedLinkTableID,
    VisitedLinkStateChangedID,
    AllVisitedLinkStateChangedID,
    SetShouldTrackVisitedLinksID,
    SetCacheModelID,
    RegisterURLSchemeAsEmptyDocumentID,
    RegisterURLSchemeAsSecureID,
    SetDomainRelaxationForbiddenForURLSchemeID,
    RegisterURLSchemeAsLocalID,
    RegisterURLSchemeAsNoAccessID,
    RegisterURLSchemeAsDisplayIsolatedID,
    RegisterURLSchemeAsCORSEnabledID,
    SetDefaultRequestTimeoutIntervalID,
    SetAlwaysUsesComplexTextCodePathID,
    SetShouldUseFontSmoothingID,
    UserPreferredLanguagesChangedID,
    FullKeyboardAccessModeChangedID,
#if PLATFORM(WIN)
    SetShouldPaintNativeControlsID,
#endif
#if ENABLE(NETSCAPE_PLUGIN_API)
    DidGetPluginsID,
#endif
#if ENABLE(NETSCAPE_PLUGIN_API) && !ENABLE(PLUGIN_PROCESS)
    GetSitesWithPluginDataID,
#endif
#if ENABLE(NETSCAPE_PLUGIN_API) && !ENABLE(PLUGIN_PROCESS)
    ClearPluginSiteDataID,
#endif
#if ENABLE(NETWORK_PROCESS)
    NetworkProcessCrashedID,
#endif
#if ENABLE(PLUGIN_PROCESS)
    PluginProcessCrashedID,
#endif
    StartMemorySamplerID,
    StopMemorySamplerID,
    DownloadRequestID,
    CancelDownloadID,
#if PLATFORM(QT)
    StartTransferID,
#endif
    SetTextCheckerStateID,
    SetEnhancedAccessibilityID,
    GetWebCoreStatisticsID,
    GarbageCollectJavaScriptObjectsID,
    SetJavaScriptGarbageCollectorTimerEnabledID,
    PostInjectedBundleMessageID,
#if USE(SECURITY_FRAMEWORK)
    SecItemResponseID,
#endif
#if USE(SECURITY_FRAMEWORK)
    SecKeychainItemResponseID,
#endif
#if PLATFORM(MAC)
    SetApplicationIsOccludedID,
#endif
#if ENABLE(CUSTOM_PROTOCOLS)
    RegisterSchemeForCustomProtocolID,
#endif
#if ENABLE(CUSTOM_PROTOCOLS)
    UnregisterSchemeForCustomProtocolID,
#endif
};

struct InitializeWebProcess : CoreIPC::Arguments2<const WebKit::WebProcessCreationParameters&, const WebKit::WebContextUserMessageEncoder&> {
    static const Kind messageID = InitializeWebProcessID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("InitializeWebProcess"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<WebKit::WebProcessCreationParameters&> DecodeType;
    InitializeWebProcess(const WebKit::WebProcessCreationParameters& processCreationParameters, const WebKit::WebContextUserMessageEncoder& initializationUserData)
        : CoreIPC::Arguments2<const WebKit::WebProcessCreationParameters&, const WebKit::WebContextUserMessageEncoder&>(processCreationParameters, initializationUserData)
    {
    }
};

struct CreateWebPage : CoreIPC::Arguments2<uint64_t, const WebKit::WebPageCreationParameters&> {
    static const Kind messageID = CreateWebPageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateWebPage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::WebPageCreationParameters&> DecodeType;
    CreateWebPage(uint64_t newPageID, const WebKit::WebPageCreationParameters& pageCreationParameters)
        : CoreIPC::Arguments2<uint64_t, const WebKit::WebPageCreationParameters&>(newPageID, pageCreationParameters)
    {
    }
};

#if ENABLE(WEB_INTENTS)
struct RemoveMessagePortChannel : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = RemoveMessagePortChannelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveMessagePortChannel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit RemoveMessagePortChannel(uint64_t channelID)
        : CoreIPC::Arguments1<uint64_t>(channelID)
    {
    }
};
#endif

struct SetVisitedLinkTable : CoreIPC::Arguments1<const WebKit::SharedMemory::Handle&> {
    static const Kind messageID = SetVisitedLinkTableID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetVisitedLinkTable"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::SharedMemory::Handle&> DecodeType;
    explicit SetVisitedLinkTable(const WebKit::SharedMemory::Handle& handle)
        : CoreIPC::Arguments1<const WebKit::SharedMemory::Handle&>(handle)
    {
    }
};

struct VisitedLinkStateChanged : CoreIPC::Arguments1<const Vector<WebCore::LinkHash>&> {
    static const Kind messageID = VisitedLinkStateChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("VisitedLinkStateChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<WebCore::LinkHash>&> DecodeType;
    explicit VisitedLinkStateChanged(const Vector<WebCore::LinkHash>& linkHashes)
        : CoreIPC::Arguments1<const Vector<WebCore::LinkHash>&>(linkHashes)
    {
    }
};

struct AllVisitedLinkStateChanged : CoreIPC::Arguments0 {
    static const Kind messageID = AllVisitedLinkStateChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AllVisitedLinkStateChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetShouldTrackVisitedLinks : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetShouldTrackVisitedLinksID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetShouldTrackVisitedLinks"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetShouldTrackVisitedLinks(bool shouldTrackVisitedLinks)
        : CoreIPC::Arguments1<bool>(shouldTrackVisitedLinks)
    {
    }
};

struct SetCacheModel : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = SetCacheModelID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCacheModel"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit SetCacheModel(uint32_t cacheModel)
        : CoreIPC::Arguments1<uint32_t>(cacheModel)
    {
    }
};

struct RegisterURLSchemeAsEmptyDocument : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsEmptyDocumentID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsEmptyDocument"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsEmptyDocument(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct RegisterURLSchemeAsSecure : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsSecureID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsSecure"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsSecure(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct SetDomainRelaxationForbiddenForURLScheme : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = SetDomainRelaxationForbiddenForURLSchemeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetDomainRelaxationForbiddenForURLScheme"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit SetDomainRelaxationForbiddenForURLScheme(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct RegisterURLSchemeAsLocal : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsLocalID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsLocal"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsLocal(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct RegisterURLSchemeAsNoAccess : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsNoAccessID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsNoAccess"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsNoAccess(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct RegisterURLSchemeAsDisplayIsolated : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsDisplayIsolatedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsDisplayIsolated"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsDisplayIsolated(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct RegisterURLSchemeAsCORSEnabled : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterURLSchemeAsCORSEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterURLSchemeAsCORSEnabled"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterURLSchemeAsCORSEnabled(const WTF::String& scheme)
        : CoreIPC::Arguments1<const WTF::String&>(scheme)
    {
    }
};

struct SetDefaultRequestTimeoutInterval : CoreIPC::Arguments1<double> {
    static const Kind messageID = SetDefaultRequestTimeoutIntervalID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetDefaultRequestTimeoutInterval"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<double> DecodeType;
    explicit SetDefaultRequestTimeoutInterval(double timeoutInterval)
        : CoreIPC::Arguments1<double>(timeoutInterval)
    {
    }
};

struct SetAlwaysUsesComplexTextCodePath : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetAlwaysUsesComplexTextCodePathID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetAlwaysUsesComplexTextCodePath"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetAlwaysUsesComplexTextCodePath(bool alwaysUseComplexText)
        : CoreIPC::Arguments1<bool>(alwaysUseComplexText)
    {
    }
};

struct SetShouldUseFontSmoothing : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetShouldUseFontSmoothingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetShouldUseFontSmoothing"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetShouldUseFontSmoothing(bool useFontSmoothing)
        : CoreIPC::Arguments1<bool>(useFontSmoothing)
    {
    }
};

struct UserPreferredLanguagesChanged : CoreIPC::Arguments1<const Vector<WTF::String>&> {
    static const Kind messageID = UserPreferredLanguagesChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UserPreferredLanguagesChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const Vector<WTF::String>&> DecodeType;
    explicit UserPreferredLanguagesChanged(const Vector<WTF::String>& languages)
        : CoreIPC::Arguments1<const Vector<WTF::String>&>(languages)
    {
    }
};

struct FullKeyboardAccessModeChanged : CoreIPC::Arguments1<bool> {
    static const Kind messageID = FullKeyboardAccessModeChangedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("FullKeyboardAccessModeChanged"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit FullKeyboardAccessModeChanged(bool fullKeyboardAccessEnabled)
        : CoreIPC::Arguments1<bool>(fullKeyboardAccessEnabled)
    {
    }
};

#if PLATFORM(WIN)
struct SetShouldPaintNativeControls : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetShouldPaintNativeControlsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetShouldPaintNativeControls"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetShouldPaintNativeControls(bool shouldPaintNativeControls)
        : CoreIPC::Arguments1<bool>(shouldPaintNativeControls)
    {
    }
};
#endif

#if ENABLE(NETSCAPE_PLUGIN_API)
struct DidGetPlugins : CoreIPC::Arguments2<uint64_t, const Vector<WebCore::PluginInfo>&> {
    static const Kind messageID = DidGetPluginsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidGetPlugins"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const Vector<WebCore::PluginInfo>&> DecodeType;
    DidGetPlugins(uint64_t requestID, const Vector<WebCore::PluginInfo>& plugins)
        : CoreIPC::Arguments2<uint64_t, const Vector<WebCore::PluginInfo>&>(requestID, plugins)
    {
    }
};
#endif

#if ENABLE(NETSCAPE_PLUGIN_API) && !ENABLE(PLUGIN_PROCESS)
struct GetSitesWithPluginData : CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t> {
    static const Kind messageID = GetSitesWithPluginDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetSitesWithPluginData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t> DecodeType;
    GetSitesWithPluginData(const Vector<WTF::String>& pluginPaths, uint64_t callbackID)
        : CoreIPC::Arguments2<const Vector<WTF::String>&, uint64_t>(pluginPaths, callbackID)
    {
    }
};
#endif

#if ENABLE(NETSCAPE_PLUGIN_API) && !ENABLE(PLUGIN_PROCESS)
struct ClearPluginSiteData : CoreIPC::Arguments5<const Vector<WTF::String>&, const Vector<WTF::String>&, uint64_t, uint64_t, uint64_t> {
    static const Kind messageID = ClearPluginSiteDataID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearPluginSiteData"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments5<const Vector<WTF::String>&, const Vector<WTF::String>&, uint64_t, uint64_t, uint64_t> DecodeType;
    ClearPluginSiteData(const Vector<WTF::String>& pluginPaths, const Vector<WTF::String>& sites, uint64_t flags, uint64_t maxAgeInSeconds, uint64_t callbackID)
        : CoreIPC::Arguments5<const Vector<WTF::String>&, const Vector<WTF::String>&, uint64_t, uint64_t, uint64_t>(pluginPaths, sites, flags, maxAgeInSeconds, callbackID)
    {
    }
};
#endif

#if ENABLE(NETWORK_PROCESS)
struct NetworkProcessCrashed : CoreIPC::Arguments0 {
    static const Kind messageID = NetworkProcessCrashedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("NetworkProcessCrashed"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if ENABLE(PLUGIN_PROCESS)
struct PluginProcessCrashed : CoreIPC::Arguments2<const String&, uint32_t> {
    static const Kind messageID = PluginProcessCrashedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PluginProcessCrashed"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const String&, uint32_t> DecodeType;
    PluginProcessCrashed(const String& pluginProcess, uint32_t processType)
        : CoreIPC::Arguments2<const String&, uint32_t>(pluginProcess, processType)
    {
    }
};
#endif

struct StartMemorySampler : CoreIPC::Arguments3<const WebKit::SandboxExtension::Handle&, const WTF::String&, double> {
    static const Kind messageID = StartMemorySamplerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartMemorySampler"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebKit::SandboxExtension::Handle&, const WTF::String&, double> DecodeType;
    StartMemorySampler(const WebKit::SandboxExtension::Handle& sampleLogFileHandle, const WTF::String& sampleLogFilePath, double interval)
        : CoreIPC::Arguments3<const WebKit::SandboxExtension::Handle&, const WTF::String&, double>(sampleLogFileHandle, sampleLogFilePath, interval)
    {
    }
};

struct StopMemorySampler : CoreIPC::Arguments0 {
    static const Kind messageID = StopMemorySamplerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StopMemorySampler"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct DownloadRequest : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceRequest&> {
    static const Kind messageID = DownloadRequestID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DownloadRequest"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceRequest&> DecodeType;
    DownloadRequest(uint64_t downloadID, uint64_t initiatingPageID, const WebCore::ResourceRequest& request)
        : CoreIPC::Arguments3<uint64_t, uint64_t, const WebCore::ResourceRequest&>(downloadID, initiatingPageID, request)
    {
    }
};

struct CancelDownload : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = CancelDownloadID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CancelDownload"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit CancelDownload(uint64_t downloadID)
        : CoreIPC::Arguments1<uint64_t>(downloadID)
    {
    }
};

#if PLATFORM(QT)
struct StartTransfer : CoreIPC::Arguments2<uint64_t, const WTF::String&> {
    static const Kind messageID = StartTransferID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("StartTransfer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WTF::String&> DecodeType;
    StartTransfer(uint64_t downloadID, const WTF::String& destination)
        : CoreIPC::Arguments2<uint64_t, const WTF::String&>(downloadID, destination)
    {
    }
};
#endif

struct SetTextCheckerState : CoreIPC::Arguments1<const WebKit::TextCheckerState&> {
    static const Kind messageID = SetTextCheckerStateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetTextCheckerState"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebKit::TextCheckerState&> DecodeType;
    explicit SetTextCheckerState(const WebKit::TextCheckerState& textCheckerState)
        : CoreIPC::Arguments1<const WebKit::TextCheckerState&>(textCheckerState)
    {
    }
};

struct SetEnhancedAccessibility : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetEnhancedAccessibilityID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetEnhancedAccessibility"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetEnhancedAccessibility(bool flag)
        : CoreIPC::Arguments1<bool>(flag)
    {
    }
};

struct GetWebCoreStatistics : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = GetWebCoreStatisticsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GetWebCoreStatistics"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit GetWebCoreStatistics(uint64_t callbackID)
        : CoreIPC::Arguments1<uint64_t>(callbackID)
    {
    }
};

struct GarbageCollectJavaScriptObjects : CoreIPC::Arguments0 {
    static const Kind messageID = GarbageCollectJavaScriptObjectsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("GarbageCollectJavaScriptObjects"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct SetJavaScriptGarbageCollectorTimerEnabled : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetJavaScriptGarbageCollectorTimerEnabledID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetJavaScriptGarbageCollectorTimerEnabled"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetJavaScriptGarbageCollectorTimerEnabled(bool enable)
        : CoreIPC::Arguments1<bool>(enable)
    {
    }
};

struct PostInjectedBundleMessage : CoreIPC::Arguments1<const CoreIPC::DataReference&> {
    static const Kind messageID = PostInjectedBundleMessageID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PostInjectedBundleMessage"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const CoreIPC::DataReference&> DecodeType;
    explicit PostInjectedBundleMessage(const CoreIPC::DataReference& messageData)
        : CoreIPC::Arguments1<const CoreIPC::DataReference&>(messageData)
    {
    }
};

#if USE(SECURITY_FRAMEWORK)
struct SecItemResponse : CoreIPC::Arguments2<uint64_t, const WebKit::SecItemResponseData&> {
    static const Kind messageID = SecItemResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SecItemResponse"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::SecItemResponseData&> DecodeType;
    SecItemResponse(uint64_t requestID, const WebKit::SecItemResponseData& response)
        : CoreIPC::Arguments2<uint64_t, const WebKit::SecItemResponseData&>(requestID, response)
    {
    }
};
#endif

#if USE(SECURITY_FRAMEWORK)
struct SecKeychainItemResponse : CoreIPC::Arguments2<uint64_t, const WebKit::SecKeychainItemResponseData&> {
    static const Kind messageID = SecKeychainItemResponseID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SecKeychainItemResponse"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::SecKeychainItemResponseData&> DecodeType;
    SecKeychainItemResponse(uint64_t requestID, const WebKit::SecKeychainItemResponseData& response)
        : CoreIPC::Arguments2<uint64_t, const WebKit::SecKeychainItemResponseData&>(requestID, response)
    {
    }
};
#endif

#if PLATFORM(MAC)
struct SetApplicationIsOccluded : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetApplicationIsOccludedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetApplicationIsOccluded"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetApplicationIsOccluded(bool flag)
        : CoreIPC::Arguments1<bool>(flag)
    {
    }
};
#endif

#if ENABLE(CUSTOM_PROTOCOLS)
struct RegisterSchemeForCustomProtocol : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = RegisterSchemeForCustomProtocolID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RegisterSchemeForCustomProtocol"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit RegisterSchemeForCustomProtocol(const WTF::String& name)
        : CoreIPC::Arguments1<const WTF::String&>(name)
    {
    }
};
#endif

#if ENABLE(CUSTOM_PROTOCOLS)
struct UnregisterSchemeForCustomProtocol : CoreIPC::Arguments1<const WTF::String&> {
    static const Kind messageID = UnregisterSchemeForCustomProtocolID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UnregisterSchemeForCustomProtocol"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WTF::String&> DecodeType;
    explicit UnregisterSchemeForCustomProtocol(const WTF::String& name)
        : CoreIPC::Arguments1<const WTF::String&>(name)
    {
    }
};
#endif

} // namespace WebProcess
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::WebProcess::Kind> {
    static const MessageClass messageClass = MessageClassWebProcess;
};

} // namespace CoreIPC

#endif // WebProcessMessages_h
