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

#include "JSOESVertexArrayObject.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSWebGLVertexArrayObjectOES.h"
#include "OESVertexArrayObject.h"
#include "WebGLVertexArrayObjectOES.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table for prototype */

static const HashTableValue JSOESVertexArrayObjectPrototypeTableValues[] =
{
    { "VERTEX_ARRAY_BINDING_OES", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsOESVertexArrayObjectVERTEX_ARRAY_BINDING_OES), (intptr_t)0, NoIntrinsic },
    { "createVertexArrayOES", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsOESVertexArrayObjectPrototypeFunctionCreateVertexArrayOES), (intptr_t)0, NoIntrinsic },
    { "deleteVertexArrayOES", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsOESVertexArrayObjectPrototypeFunctionDeleteVertexArrayOES), (intptr_t)1, NoIntrinsic },
    { "isVertexArrayOES", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsOESVertexArrayObjectPrototypeFunctionIsVertexArrayOES), (intptr_t)1, NoIntrinsic },
    { "bindVertexArrayOES", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsOESVertexArrayObjectPrototypeFunctionBindVertexArrayOES), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSOESVertexArrayObjectPrototypeTable = { 16, 15, JSOESVertexArrayObjectPrototypeTableValues, 0 };
const ClassInfo JSOESVertexArrayObjectPrototype::s_info = { "OESVertexArrayObjectPrototype", &Base::s_info, &JSOESVertexArrayObjectPrototypeTable, 0, CREATE_METHOD_TABLE(JSOESVertexArrayObjectPrototype) };

JSObject* JSOESVertexArrayObjectPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSOESVertexArrayObject>(exec, globalObject);
}

bool JSOESVertexArrayObjectPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSOESVertexArrayObjectPrototype* thisObject = jsCast<JSOESVertexArrayObjectPrototype*>(cell);
    return getStaticPropertySlot<JSOESVertexArrayObjectPrototype, JSObject>(exec, &JSOESVertexArrayObjectPrototypeTable, thisObject, propertyName, slot);
}

bool JSOESVertexArrayObjectPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSOESVertexArrayObjectPrototype* thisObject = jsCast<JSOESVertexArrayObjectPrototype*>(object);
    return getStaticPropertyDescriptor<JSOESVertexArrayObjectPrototype, JSObject>(exec, &JSOESVertexArrayObjectPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSOESVertexArrayObject::s_info = { "OESVertexArrayObject", &Base::s_info, 0, 0 , CREATE_METHOD_TABLE(JSOESVertexArrayObject) };

JSOESVertexArrayObject::JSOESVertexArrayObject(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<OESVertexArrayObject> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSOESVertexArrayObject::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSOESVertexArrayObject::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSOESVertexArrayObjectPrototype::create(exec->globalData(), globalObject, JSOESVertexArrayObjectPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSOESVertexArrayObject::destroy(JSC::JSCell* cell)
{
    JSOESVertexArrayObject* thisObject = static_cast<JSOESVertexArrayObject*>(cell);
    thisObject->JSOESVertexArrayObject::~JSOESVertexArrayObject();
}

JSOESVertexArrayObject::~JSOESVertexArrayObject()
{
    releaseImplIfNotNull();
}

EncodedJSValue JSC_HOST_CALL jsOESVertexArrayObjectPrototypeFunctionCreateVertexArrayOES(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSOESVertexArrayObject::s_info))
        return throwVMTypeError(exec);
    JSOESVertexArrayObject* castedThis = jsCast<JSOESVertexArrayObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSOESVertexArrayObject::s_info);
    OESVertexArrayObject* impl = static_cast<OESVertexArrayObject*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->createVertexArrayOES()));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsOESVertexArrayObjectPrototypeFunctionDeleteVertexArrayOES(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSOESVertexArrayObject::s_info))
        return throwVMTypeError(exec);
    JSOESVertexArrayObject* castedThis = jsCast<JSOESVertexArrayObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSOESVertexArrayObject::s_info);
    OESVertexArrayObject* impl = static_cast<OESVertexArrayObject*>(castedThis->impl());
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSWebGLVertexArrayObjectOES::s_info))
        return throwVMTypeError(exec);
    WebGLVertexArrayObjectOES* arrayObject(toWebGLVertexArrayObjectOES(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->deleteVertexArrayOES(arrayObject);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsOESVertexArrayObjectPrototypeFunctionIsVertexArrayOES(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSOESVertexArrayObject::s_info))
        return throwVMTypeError(exec);
    JSOESVertexArrayObject* castedThis = jsCast<JSOESVertexArrayObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSOESVertexArrayObject::s_info);
    OESVertexArrayObject* impl = static_cast<OESVertexArrayObject*>(castedThis->impl());
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSWebGLVertexArrayObjectOES::s_info))
        return throwVMTypeError(exec);
    WebGLVertexArrayObjectOES* arrayObject(toWebGLVertexArrayObjectOES(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsBoolean(impl->isVertexArrayOES(arrayObject));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsOESVertexArrayObjectPrototypeFunctionBindVertexArrayOES(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSOESVertexArrayObject::s_info))
        return throwVMTypeError(exec);
    JSOESVertexArrayObject* castedThis = jsCast<JSOESVertexArrayObject*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSOESVertexArrayObject::s_info);
    OESVertexArrayObject* impl = static_cast<OESVertexArrayObject*>(castedThis->impl());
    ExceptionCode ec = 0;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSWebGLVertexArrayObjectOES::s_info))
        return throwVMTypeError(exec);
    WebGLVertexArrayObjectOES* arrayObject(toWebGLVertexArrayObjectOES(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->bindVertexArrayOES(arrayObject, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

// Constant getters

JSValue jsOESVertexArrayObjectVERTEX_ARRAY_BINDING_OES(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0x85B5));
}

static inline bool isObservable(JSOESVertexArrayObject* jsOESVertexArrayObject)
{
    if (jsOESVertexArrayObject->hasCustomProperties())
        return true;
    return false;
}

bool JSOESVertexArrayObjectOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSOESVertexArrayObject* jsOESVertexArrayObject = jsCast<JSOESVertexArrayObject*>(handle.get().asCell());
    if (!isObservable(jsOESVertexArrayObject))
        return false;
    WebGLRenderingContext* root = jsOESVertexArrayObject->impl()->context();
    return visitor.containsOpaqueRoot(root);
}

void JSOESVertexArrayObjectOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSOESVertexArrayObject* jsOESVertexArrayObject = jsCast<JSOESVertexArrayObject*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsOESVertexArrayObject->impl(), jsOESVertexArrayObject);
    jsOESVertexArrayObject->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, OESVertexArrayObject* impl)
{
    return wrap<JSOESVertexArrayObject>(exec, globalObject, impl);
}

OESVertexArrayObject* toOESVertexArrayObject(JSC::JSValue value)
{
    return value.inherits(&JSOESVertexArrayObject::s_info) ? jsCast<JSOESVertexArrayObject*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(WEBGL)
