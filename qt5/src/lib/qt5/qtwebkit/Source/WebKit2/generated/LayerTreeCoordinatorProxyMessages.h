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

#ifndef LayerTreeCoordinatorProxyMessages_h
#define LayerTreeCoordinatorProxyMessages_h

#if USE(COORDINATED_GRAPHICS)

#include "Arguments.h"
#include "MessageEncoder.h"
#include "MessageID.h"
#include "ShareableSurface.h"
#include "StringReference.h"
#include <wtf/Vector.h>

namespace WebCore {
    class CustomFilterProgramInfo;
    class FilterOperations;
    class GraphicsLayerAnimations;
    class IntSize;
    class GraphicsSurfaceToken;
    class Color;
    class IntRect;
    class IntPoint;
}

namespace WebKit {
    class WebLayerInfo;
    class SurfaceUpdateInfo;
}

namespace Messages {
namespace LayerTreeCoordinatorProxy {

static inline CoreIPC::StringReference messageReceiverName()
{
    return CoreIPC::StringReference("LayerTreeCoordinatorProxy");
}

enum Kind {
    SetCompositingLayerStateID,
    SetCompositingLayerChildrenID,
#if ENABLE(CSS_FILTERS)
    SetCompositingLayerFiltersID,
#endif
#if ENABLE(CSS_SHADERS)
    CreateCustomFilterProgramID,
#endif
#if ENABLE(CSS_SHADERS)
    RemoveCustomFilterProgramID,
#endif
    SetRootCompositingLayerID,
    DeleteCompositingLayerID,
    CreateTileForLayerID,
    UpdateTileForLayerID,
    RemoveTileForLayerID,
    CreateUpdateAtlasID,
    RemoveUpdateAtlasID,
    CreateImageBackingID,
    UpdateImageBackingID,
    ClearImageBackingContentsID,
    RemoveImageBackingID,
    DidRenderFrameID,
    DidChangeScrollPositionID,
    SetLayerAnimationsID,
    SetAnimationsLockedID,
#if ENABLE(REQUEST_ANIMATION_FRAME)
    RequestAnimationFrameID,
#endif
#if USE(GRAPHICS_SURFACE)
    CreateCanvasID,
#endif
#if USE(GRAPHICS_SURFACE)
    SyncCanvasID,
#endif
#if USE(GRAPHICS_SURFACE)
    DestroyCanvasID,
#endif
    SetBackgroundColorID,
};

struct SetCompositingLayerState : CoreIPC::Arguments2<uint32_t, const WebKit::WebLayerInfo&> {
    static const Kind messageID = SetCompositingLayerStateID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCompositingLayerState"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, const WebKit::WebLayerInfo&> DecodeType;
    SetCompositingLayerState(uint32_t id, const WebKit::WebLayerInfo& layerInfo)
        : CoreIPC::Arguments2<uint32_t, const WebKit::WebLayerInfo&>(id, layerInfo)
    {
    }
};

struct SetCompositingLayerChildren : CoreIPC::Arguments2<uint32_t, const Vector<uint32_t>&> {
    static const Kind messageID = SetCompositingLayerChildrenID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCompositingLayerChildren"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, const Vector<uint32_t>&> DecodeType;
    SetCompositingLayerChildren(uint32_t id, const Vector<uint32_t>& children)
        : CoreIPC::Arguments2<uint32_t, const Vector<uint32_t>&>(id, children)
    {
    }
};

#if ENABLE(CSS_FILTERS)
struct SetCompositingLayerFilters : CoreIPC::Arguments2<uint32_t, const WebCore::FilterOperations&> {
    static const Kind messageID = SetCompositingLayerFiltersID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetCompositingLayerFilters"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, const WebCore::FilterOperations&> DecodeType;
    SetCompositingLayerFilters(uint32_t id, const WebCore::FilterOperations& filters)
        : CoreIPC::Arguments2<uint32_t, const WebCore::FilterOperations&>(id, filters)
    {
    }
};
#endif

#if ENABLE(CSS_SHADERS)
struct CreateCustomFilterProgram : CoreIPC::Arguments2<const int&, const WebCore::CustomFilterProgramInfo&> {
    static const Kind messageID = CreateCustomFilterProgramID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateCustomFilterProgram"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const int&, const WebCore::CustomFilterProgramInfo&> DecodeType;
    CreateCustomFilterProgram(const int& id, const WebCore::CustomFilterProgramInfo& programInfo)
        : CoreIPC::Arguments2<const int&, const WebCore::CustomFilterProgramInfo&>(id, programInfo)
    {
    }
};
#endif

#if ENABLE(CSS_SHADERS)
struct RemoveCustomFilterProgram : CoreIPC::Arguments1<const int&> {
    static const Kind messageID = RemoveCustomFilterProgramID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveCustomFilterProgram"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const int&> DecodeType;
    explicit RemoveCustomFilterProgram(const int& id)
        : CoreIPC::Arguments1<const int&>(id)
    {
    }
};
#endif

struct SetRootCompositingLayer : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = SetRootCompositingLayerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetRootCompositingLayer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit SetRootCompositingLayer(uint32_t id)
        : CoreIPC::Arguments1<uint32_t>(id)
    {
    }
};

struct DeleteCompositingLayer : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = DeleteCompositingLayerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DeleteCompositingLayer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit DeleteCompositingLayer(uint32_t id)
        : CoreIPC::Arguments1<uint32_t>(id)
    {
    }
};

struct CreateTileForLayer : CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&> {
    static const Kind messageID = CreateTileForLayerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateTileForLayer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&> DecodeType;
    CreateTileForLayer(uint32_t layerID, const int& tileID, const WebCore::IntRect& tileRect, const WebKit::SurfaceUpdateInfo& updateInfo)
        : CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&>(layerID, tileID, tileRect, updateInfo)
    {
    }
};

struct UpdateTileForLayer : CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&> {
    static const Kind messageID = UpdateTileForLayerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UpdateTileForLayer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&> DecodeType;
    UpdateTileForLayer(uint32_t layerID, const int& tileID, const WebCore::IntRect& tileRect, const WebKit::SurfaceUpdateInfo& updateInfo)
        : CoreIPC::Arguments4<uint32_t, const int&, const WebCore::IntRect&, const WebKit::SurfaceUpdateInfo&>(layerID, tileID, tileRect, updateInfo)
    {
    }
};

struct RemoveTileForLayer : CoreIPC::Arguments2<uint32_t, const int&> {
    static const Kind messageID = RemoveTileForLayerID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveTileForLayer"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, const int&> DecodeType;
    RemoveTileForLayer(uint32_t layerID, const int& tileID)
        : CoreIPC::Arguments2<uint32_t, const int&>(layerID, tileID)
    {
    }
};

struct CreateUpdateAtlas : CoreIPC::Arguments2<const int&, const WebKit::ShareableSurface::Handle&> {
    static const Kind messageID = CreateUpdateAtlasID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateUpdateAtlas"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const int&, const WebKit::ShareableSurface::Handle&> DecodeType;
    CreateUpdateAtlas(const int& atlasID, const WebKit::ShareableSurface::Handle& handle)
        : CoreIPC::Arguments2<const int&, const WebKit::ShareableSurface::Handle&>(atlasID, handle)
    {
    }
};

struct RemoveUpdateAtlas : CoreIPC::Arguments1<const int&> {
    static const Kind messageID = RemoveUpdateAtlasID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveUpdateAtlas"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const int&> DecodeType;
    explicit RemoveUpdateAtlas(const int& atlasID)
        : CoreIPC::Arguments1<const int&>(atlasID)
    {
    }
};

struct CreateImageBacking : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = CreateImageBackingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateImageBacking"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit CreateImageBacking(uint64_t imageID)
        : CoreIPC::Arguments1<uint64_t>(imageID)
    {
    }
};

struct UpdateImageBacking : CoreIPC::Arguments2<uint64_t, const WebKit::ShareableSurface::Handle&> {
    static const Kind messageID = UpdateImageBackingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("UpdateImageBacking"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint64_t, const WebKit::ShareableSurface::Handle&> DecodeType;
    UpdateImageBacking(uint64_t imageID, const WebKit::ShareableSurface::Handle& handle)
        : CoreIPC::Arguments2<uint64_t, const WebKit::ShareableSurface::Handle&>(imageID, handle)
    {
    }
};

struct ClearImageBackingContents : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = ClearImageBackingContentsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("ClearImageBackingContents"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit ClearImageBackingContents(uint64_t imageID)
        : CoreIPC::Arguments1<uint64_t>(imageID)
    {
    }
};

struct RemoveImageBacking : CoreIPC::Arguments1<uint64_t> {
    static const Kind messageID = RemoveImageBackingID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RemoveImageBacking"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint64_t> DecodeType;
    explicit RemoveImageBacking(uint64_t imageID)
        : CoreIPC::Arguments1<uint64_t>(imageID)
    {
    }
};

struct DidRenderFrame : CoreIPC::Arguments2<const WebCore::IntSize&, const WebCore::IntRect&> {
    static const Kind messageID = DidRenderFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidRenderFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<const WebCore::IntSize&, const WebCore::IntRect&> DecodeType;
    DidRenderFrame(const WebCore::IntSize& contentsSize, const WebCore::IntRect& coveredRect)
        : CoreIPC::Arguments2<const WebCore::IntSize&, const WebCore::IntRect&>(contentsSize, coveredRect)
    {
    }
};

struct DidChangeScrollPosition : CoreIPC::Arguments1<const WebCore::IntPoint&> {
    static const Kind messageID = DidChangeScrollPositionID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DidChangeScrollPosition"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::IntPoint&> DecodeType;
    explicit DidChangeScrollPosition(const WebCore::IntPoint& position)
        : CoreIPC::Arguments1<const WebCore::IntPoint&>(position)
    {
    }
};

struct SetLayerAnimations : CoreIPC::Arguments2<uint32_t, const WebCore::GraphicsLayerAnimations&> {
    static const Kind messageID = SetLayerAnimationsID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetLayerAnimations"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, const WebCore::GraphicsLayerAnimations&> DecodeType;
    SetLayerAnimations(uint32_t id, const WebCore::GraphicsLayerAnimations& animations)
        : CoreIPC::Arguments2<uint32_t, const WebCore::GraphicsLayerAnimations&>(id, animations)
    {
    }
};

struct SetAnimationsLocked : CoreIPC::Arguments1<bool> {
    static const Kind messageID = SetAnimationsLockedID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetAnimationsLocked"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<bool> DecodeType;
    explicit SetAnimationsLocked(bool locked)
        : CoreIPC::Arguments1<bool>(locked)
    {
    }
};

#if ENABLE(REQUEST_ANIMATION_FRAME)
struct RequestAnimationFrame : CoreIPC::Arguments0 {
    static const Kind messageID = RequestAnimationFrameID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("RequestAnimationFrame"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments0 DecodeType;
};
#endif

#if USE(GRAPHICS_SURFACE)
struct CreateCanvas : CoreIPC::Arguments3<uint32_t, const WebCore::IntSize&, const WebCore::GraphicsSurfaceToken&> {
    static const Kind messageID = CreateCanvasID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("CreateCanvas"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments3<uint32_t, const WebCore::IntSize&, const WebCore::GraphicsSurfaceToken&> DecodeType;
    CreateCanvas(uint32_t id, const WebCore::IntSize& canvasSize, const WebCore::GraphicsSurfaceToken& token)
        : CoreIPC::Arguments3<uint32_t, const WebCore::IntSize&, const WebCore::GraphicsSurfaceToken&>(id, canvasSize, token)
    {
    }
};
#endif

#if USE(GRAPHICS_SURFACE)
struct SyncCanvas : CoreIPC::Arguments2<uint32_t, uint32_t> {
    static const Kind messageID = SyncCanvasID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SyncCanvas"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments2<uint32_t, uint32_t> DecodeType;
    SyncCanvas(uint32_t id, uint32_t frontBuffer)
        : CoreIPC::Arguments2<uint32_t, uint32_t>(id, frontBuffer)
    {
    }
};
#endif

#if USE(GRAPHICS_SURFACE)
struct DestroyCanvas : CoreIPC::Arguments1<uint32_t> {
    static const Kind messageID = DestroyCanvasID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("DestroyCanvas"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<uint32_t> DecodeType;
    explicit DestroyCanvas(uint32_t id)
        : CoreIPC::Arguments1<uint32_t>(id)
    {
    }
};
#endif

struct SetBackgroundColor : CoreIPC::Arguments1<const WebCore::Color&> {
    static const Kind messageID = SetBackgroundColorID;
    static CoreIPC::StringReference receiverName() { return messageReceiverName(); }
    static CoreIPC::StringReference name() { return CoreIPC::StringReference("SetBackgroundColor"); }
    static const bool isSync = false;

    typedef CoreIPC::Arguments1<const WebCore::Color&> DecodeType;
    explicit SetBackgroundColor(const WebCore::Color& color)
        : CoreIPC::Arguments1<const WebCore::Color&>(color)
    {
    }
};

} // namespace LayerTreeCoordinatorProxy
} // namespace Messages

namespace CoreIPC {

template<> struct MessageKindTraits<Messages::LayerTreeCoordinatorProxy::Kind> {
    static const MessageClass messageClass = MessageClassLayerTreeCoordinatorProxy;
};

} // namespace CoreIPC

#endif // USE(COORDINATED_GRAPHICS)

#endif // LayerTreeCoordinatorProxyMessages_h
