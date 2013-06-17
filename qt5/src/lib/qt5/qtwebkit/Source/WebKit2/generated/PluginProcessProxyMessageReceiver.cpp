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

#if ENABLE(PLUGIN_PROCESS)

#include "PluginProcessProxy.h"

#include "ArgumentCoders.h"
#include "Attachment.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "PluginProcessProxyMessages.h"
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

void PluginProcessProxy::didReceivePluginProcessProxyMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::PluginProcessProxy::DidCreateWebProcessConnection::name()) {
        CoreIPC::handleMessage<Messages::PluginProcessProxy::DidCreateWebProcessConnection>(decoder, this, &PluginProcessProxy::didCreateWebProcessConnection);
        return;
    }
    if (decoder.messageName() == Messages::PluginProcessProxy::DidGetSitesWithData::name()) {
        CoreIPC::handleMessage<Messages::PluginProcessProxy::DidGetSitesWithData>(decoder, this, &PluginProcessProxy::didGetSitesWithData);
        return;
    }
    if (decoder.messageName() == Messages::PluginProcessProxy::DidClearSiteData::name()) {
        CoreIPC::handleMessage<Messages::PluginProcessProxy::DidClearSiteData>(decoder, this, &PluginProcessProxy::didClearSiteData);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::PluginProcessProxy::SetModalWindowIsShowing::name()) {
        CoreIPC::handleMessage<Messages::PluginProcessProxy::SetModalWindowIsShowing>(decoder, this, &PluginProcessProxy::setModalWindowIsShowing);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::PluginProcessProxy::SetFullscreenWindowIsShowing::name()) {
        CoreIPC::handleMessage<Messages::PluginProcessProxy::SetFullscreenWindowIsShowing>(decoder, this, &PluginProcessProxy::setFullscreenWindowIsShowing);
        return;
    }
#endif
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(PLUGIN_PROCESS)
