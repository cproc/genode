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

#ifndef LayerTreeCoordinatorMessages_h
#define LayerTreeCoordinatorMessages_h

#if USE(COORDINATED_GRAPHICS)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "StringReference.h"

namespace WebCore {
    class FloatPoint;
    class IntRect;
}

namespace Messages {
namespace LayerTreeCoordinator {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("LayerTreeCoordinator");
}

enum Kind {
    SetVisibleContentsRectID,
    RenderNextFrameID,
    PurgeBackingStoresID,
#if ENABLE(REQUEST_ANIMATION_FRAME)
    AnimationFrameReadyID,
#endif
};

struct SetVisibleContentsRect : CoreIPC::Arguments3<const WebCore::IntRect&, float, const WebCore::FloatPoint&> {
    static const Kind messageID = SetVisibleContentsRectID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetVisibleContentsRect"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<const WebCore::IntRect&, float, const WebCore::FloatPoint&> DecodeType;
    SetVisibleContentsRect(const WebCore::IntRect& visibleContentsRect, float scale, const WebCore::FloatPoint& trajectoryVectory)
        : CoreIPC::Arguments3<const WebCore::IntRect&, float, const WebCore::FloatPoint&>(visibleContentsRect, scale, trajectoryVectory)
    {
    }
};

struct RenderNextFrame : CoreIPC::Arguments0 {
    static const Kind messageID = RenderNextFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RenderNextFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

struct PurgeBackingStores : CoreIPC::Arguments0 {
    static const Kind messageID = PurgeBackingStoresID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("PurgeBackingStores"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};

#if ENABLE(REQUEST_ANIMATION_FRAME)
struct AnimationFrameReady : CoreIPC::Arguments0 {
    static const Kind messageID = AnimationFrameReadyID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("AnimationFrameReady"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

} // namespace LayerTreeCoordinator
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::LayerTreeCoordinator::Kind> {
    static const MessageClass messageClass = MessageClassLayerTreeCoordinator;
};

} // namespace CoreIPC

#endif // USE(COORDINATED_GRAPHICS)

#endif // LayerTreeCoordinatorMessages_h
