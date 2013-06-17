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

#ifndef AuthenticationManagerMessages_h
#define AuthenticationManagerMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebCore {
    class Credential;
}

namespace WebKit {
    class PlatformCertificateInfo;
}

namespace Messages {
namespace AuthenticationManager {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("AuthenticationManager");
}

enum Kind {
    UseCredentialForChallengeID,
    ContinueWithoutCredentialForChallengeID,
    CancelChallengeID,
};

struct UseCredentialForChallenge : CoreIPC::Arguments3<uint64_t, const WebCore::Credential&, const WebKit::PlatformCertificateInfo&> {
    static const Kind messageID = UseCredentialForChallengeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UseCredentialForChallenge"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint64_t, const WebCore::Credential&, const WebKit::PlatformCertificateInfo&> DecodeType;
    UseCredentialForChallenge(uint64_t challengeID, const WebCore::Credential& credential, const WebKit::PlatformCertificateInfo& certificate)
        : CoreIPC::Arguments3<uint64_t, const WebCore::Credential&, const WebKit::PlatformCertificateInfo&>(challengeID, credential, certificate)
    {
    }
};

struct ContinueWithoutCredentialForChallenge : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ContinueWithoutCredentialForChallengeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ContinueWithoutCredentialForChallenge"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ContinueWithoutCredentialForChallenge(uint64_t challengeID)
        : CoreIPC::Arguments1<uint64_t>(challengeID)
    {
    }
};

struct CancelChallenge : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = CancelChallengeID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CancelChallenge"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit CancelChallenge(uint64_t challengeID)
        : CoreIPC::Arguments1<uint64_t>(challengeID)
    {
    }
};

} // namespace AuthenticationManager
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::AuthenticationManager::Kind> {
    static const MessageClass messageClass = MessageClassAuthenticationManager;
};

} // namespace CoreIPC

#endif // AuthenticationManagerMessages_h
