/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(WEBGL)

#include "JSWebGLCompressedTextureS3TC.h"

#include "WebGLCompressedTextureS3TC.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table for prototype */

static const HashTableValue JSWebGLCompressedTextureS3TCPrototypeTableValues[] =
{
    { "COMPRESSED_RGB_S3TC_DXT1_EXT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebGLCompressedTextureS3TCCOMPRESSED_RGB_S3TC_DXT1_EXT), (intptr_t)0, NoIntrinsic },
    { "COMPRESSED_RGBA_S3TC_DXT1_EXT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT1_EXT), (intptr_t)0, NoIntrinsic },
    { "COMPRESSED_RGBA_S3TC_DXT3_EXT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT3_EXT), (intptr_t)0, NoIntrinsic },
    { "COMPRESSED_RGBA_S3TC_DXT5_EXT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT5_EXT), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebGLCompressedTextureS3TCPrototypeTable = { 8, 7, JSWebGLCompressedTextureS3TCPrototypeTableValues, 0 };
const ClassInfo JSWebGLCompressedTextureS3TCPrototype::s_info = { "WebGLCompressedTextureS3TCPrototype", &Base::s_info, &JSWebGLCompressedTextureS3TCPrototypeTable, 0, CREATE_METHOD_TABLE(JSWebGLCompressedTextureS3TCPrototype) };

JSObject* JSWebGLCompressedTextureS3TCPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSWebGLCompressedTextureS3TC>(exec, globalObject);
}

bool JSWebGLCompressedTextureS3TCPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWebGLCompressedTextureS3TCPrototype* thisObject = jsCast<JSWebGLCompressedTextureS3TCPrototype*>(cell);
    return getStaticValueSlot<JSWebGLCompressedTextureS3TCPrototype, JSObject>(exec, &JSWebGLCompressedTextureS3TCPrototypeTable, thisObject, propertyName, slot);
}

bool JSWebGLCompressedTextureS3TCPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWebGLCompressedTextureS3TCPrototype* thisObject = jsCast<JSWebGLCompressedTextureS3TCPrototype*>(object);
    return getStaticValueDescriptor<JSWebGLCompressedTextureS3TCPrototype, JSObject>(exec, &JSWebGLCompressedTextureS3TCPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSWebGLCompressedTextureS3TC::s_info = { "WebGLCompressedTextureS3TC", &Base::s_info, 0, 0 , CREATE_METHOD_TABLE(JSWebGLCompressedTextureS3TC) };

JSWebGLCompressedTextureS3TC::JSWebGLCompressedTextureS3TC(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<WebGLCompressedTextureS3TC> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSWebGLCompressedTextureS3TC::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSWebGLCompressedTextureS3TC::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSWebGLCompressedTextureS3TCPrototype::create(exec->globalData(), globalObject, JSWebGLCompressedTextureS3TCPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSWebGLCompressedTextureS3TC::destroy(JSC::JSCell* cell)
{
    JSWebGLCompressedTextureS3TC* thisObject = static_cast<JSWebGLCompressedTextureS3TC*>(cell);
    thisObject->JSWebGLCompressedTextureS3TC::~JSWebGLCompressedTextureS3TC();
}

JSWebGLCompressedTextureS3TC::~JSWebGLCompressedTextureS3TC()
{
    releaseImplIfNotNull();
}

// Constant getters

JSValue jsWebGLCompressedTextureS3TCCOMPRESSED_RGB_S3TC_DXT1_EXT(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0x83F0));
}

JSValue jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT1_EXT(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0x83F1));
}

JSValue jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT3_EXT(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0x83F2));
}

JSValue jsWebGLCompressedTextureS3TCCOMPRESSED_RGBA_S3TC_DXT5_EXT(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0x83F3));
}

static inline bool isObservable(JSWebGLCompressedTextureS3TC* jsWebGLCompressedTextureS3TC)
{
    if (jsWebGLCompressedTextureS3TC->hasCustomProperties())
        return true;
    return false;
}

bool JSWebGLCompressedTextureS3TCOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSWebGLCompressedTextureS3TC* jsWebGLCompressedTextureS3TC = jsCast<JSWebGLCompressedTextureS3TC*>(handle.get().asCell());
    if (!isObservable(jsWebGLCompressedTextureS3TC))
        return false;
    WebGLRenderingContext* root = jsWebGLCompressedTextureS3TC->impl()->context();
    return visitor.containsOpaqueRoot(root);
}

void JSWebGLCompressedTextureS3TCOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSWebGLCompressedTextureS3TC* jsWebGLCompressedTextureS3TC = jsCast<JSWebGLCompressedTextureS3TC*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsWebGLCompressedTextureS3TC->impl(), jsWebGLCompressedTextureS3TC);
    jsWebGLCompressedTextureS3TC->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, WebGLCompressedTextureS3TC* impl)
{
    return wrap<JSWebGLCompressedTextureS3TC>(exec, globalObject, impl);
}

WebGLCompressedTextureS3TC* toWebGLCompressedTextureS3TC(JSC::JSValue value)
{
    return value.inherits(&JSWebGLCompressedTextureS3TC::s_info) ? jsCast<JSWebGLCompressedTextureS3TC*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(WEBGL)
