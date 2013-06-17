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

#include "WebKeyValueStorageManager.h"

#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "SecurityOriginData.h"
#include "WebKeyValueStorageManagerMessages.h"

namespace WebKit {

void WebKeyValueStorageManager::didReceiveWebKeyValueStorageManagerMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebKeyValueStorageManager::GetKeyValueStorageOrigins::name()) {
        CoreIPC::handleMessage<Messages::WebKeyValueStorageManager::GetKeyValueStorageOrigins>(decoder, this, &WebKeyValueStorageManager::getKeyValueStorageOrigins);
        return;
    }
    if (decoder.messageName() == Messages::WebKeyValueStorageManager::DeleteEntriesForOrigin::name()) {
        CoreIPC::handleMessage<Messages::WebKeyValueStorageManager::DeleteEntriesForOrigin>(decoder, this, &WebKeyValueStorageManager::deleteEntriesForOrigin);
        return;
    }
    if (decoder.messageName() == Messages::WebKeyValueStorageManager::DeleteAllEntries::name()) {
        CoreIPC::handleMessage<Messages::WebKeyValueStorageManager::DeleteAllEntries>(decoder, this, &WebKeyValueStorageManager::deleteAllEntries);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
