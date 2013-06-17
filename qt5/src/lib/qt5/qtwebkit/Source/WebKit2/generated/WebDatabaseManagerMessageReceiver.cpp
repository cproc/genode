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

#if ENABLE(SQL_DATABASE)

#include "WebDatabaseManager.h"

#include "ArgumentCoders.h"
#include "HandleMessage.h"
#include "MessageDecoder.h"
#include "WebDatabaseManagerMessages.h"
#include <wtf/text/WTFString.h>

namespace WebKit {

void WebDatabaseManager::didReceiveWebDatabaseManagerMessage(CoreIPC::Connection*, CoreIPC::MessageID, CoreIPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebDatabaseManager::GetDatabasesByOrigin::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::GetDatabasesByOrigin>(decoder, this, &WebDatabaseManager::getDatabasesByOrigin);
        return;
    }
    if (decoder.messageName() == Messages::WebDatabaseManager::GetDatabaseOrigins::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::GetDatabaseOrigins>(decoder, this, &WebDatabaseManager::getDatabaseOrigins);
        return;
    }
    if (decoder.messageName() == Messages::WebDatabaseManager::DeleteDatabaseWithNameForOrigin::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::DeleteDatabaseWithNameForOrigin>(decoder, this, &WebDatabaseManager::deleteDatabaseWithNameForOrigin);
        return;
    }
    if (decoder.messageName() == Messages::WebDatabaseManager::DeleteDatabasesForOrigin::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::DeleteDatabasesForOrigin>(decoder, this, &WebDatabaseManager::deleteDatabasesForOrigin);
        return;
    }
    if (decoder.messageName() == Messages::WebDatabaseManager::DeleteAllDatabases::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::DeleteAllDatabases>(decoder, this, &WebDatabaseManager::deleteAllDatabases);
        return;
    }
    if (decoder.messageName() == Messages::WebDatabaseManager::SetQuotaForOrigin::name()) {
        CoreIPC::handleMessage<Messages::WebDatabaseManager::SetQuotaForOrigin>(decoder, this, &WebDatabaseManager::setQuotaForOrigin);
        return;
    }
    ASSERT_NOT_REACHED();
}

} // namespace WebKit

#endif // ENABLE(SQL_DATABASE)
