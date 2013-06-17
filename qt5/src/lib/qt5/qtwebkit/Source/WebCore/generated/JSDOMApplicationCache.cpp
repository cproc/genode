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
#include "JSDOMApplicationCache.h"

#include "DOMApplicationCache.h"
#include "Event.h"
#include "EventListener.h"
#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSEvent.h"
#include "JSEventListener.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSDOMApplicationCacheTableValues[] =
{
    { "status", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheStatus), (intptr_t)0, NoIntrinsic },
    { "onchecking", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnchecking), (intptr_t)setJSDOMApplicationCacheOnchecking, NoIntrinsic },
    { "onerror", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnerror), (intptr_t)setJSDOMApplicationCacheOnerror, NoIntrinsic },
    { "onnoupdate", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnnoupdate), (intptr_t)setJSDOMApplicationCacheOnnoupdate, NoIntrinsic },
    { "ondownloading", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOndownloading), (intptr_t)setJSDOMApplicationCacheOndownloading, NoIntrinsic },
    { "onprogress", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnprogress), (intptr_t)setJSDOMApplicationCacheOnprogress, NoIntrinsic },
    { "onupdateready", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnupdateready), (intptr_t)setJSDOMApplicationCacheOnupdateready, NoIntrinsic },
    { "oncached", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOncached), (intptr_t)setJSDOMApplicationCacheOncached, NoIntrinsic },
    { "onobsolete", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOnobsolete), (intptr_t)setJSDOMApplicationCacheOnobsolete, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSDOMApplicationCacheTable = { 33, 31, JSDOMApplicationCacheTableValues, 0 };
/* Hash table for prototype */

static const HashTableValue JSDOMApplicationCachePrototypeTableValues[] =
{
    { "UNCACHED", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheUNCACHED), (intptr_t)0, NoIntrinsic },
    { "IDLE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheIDLE), (intptr_t)0, NoIntrinsic },
    { "CHECKING", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheCHECKING), (intptr_t)0, NoIntrinsic },
    { "DOWNLOADING", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheDOWNLOADING), (intptr_t)0, NoIntrinsic },
    { "UPDATEREADY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheUPDATEREADY), (intptr_t)0, NoIntrinsic },
    { "OBSOLETE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMApplicationCacheOBSOLETE), (intptr_t)0, NoIntrinsic },
    { "update", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionUpdate), (intptr_t)0, NoIntrinsic },
    { "swapCache", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionSwapCache), (intptr_t)0, NoIntrinsic },
    { "abort", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionAbort), (intptr_t)0, NoIntrinsic },
    { "addEventListener", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionAddEventListener), (intptr_t)3, NoIntrinsic },
    { "removeEventListener", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionRemoveEventListener), (intptr_t)3, NoIntrinsic },
    { "dispatchEvent", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMApplicationCachePrototypeFunctionDispatchEvent), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSDOMApplicationCachePrototypeTable = { 33, 31, JSDOMApplicationCachePrototypeTableValues, 0 };
const ClassInfo JSDOMApplicationCachePrototype::s_info = { "DOMApplicationCachePrototype", &Base::s_info, &JSDOMApplicationCachePrototypeTable, 0, CREATE_METHOD_TABLE(JSDOMApplicationCachePrototype) };

JSObject* JSDOMApplicationCachePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSDOMApplicationCache>(exec, globalObject);
}

bool JSDOMApplicationCachePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSDOMApplicationCachePrototype* thisObject = jsCast<JSDOMApplicationCachePrototype*>(cell);
    return getStaticPropertySlot<JSDOMApplicationCachePrototype, JSObject>(exec, &JSDOMApplicationCachePrototypeTable, thisObject, propertyName, slot);
}

bool JSDOMApplicationCachePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSDOMApplicationCachePrototype* thisObject = jsCast<JSDOMApplicationCachePrototype*>(object);
    return getStaticPropertyDescriptor<JSDOMApplicationCachePrototype, JSObject>(exec, &JSDOMApplicationCachePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSDOMApplicationCache::s_info = { "DOMApplicationCache", &Base::s_info, &JSDOMApplicationCacheTable, 0 , CREATE_METHOD_TABLE(JSDOMApplicationCache) };

JSDOMApplicationCache::JSDOMApplicationCache(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<DOMApplicationCache> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSDOMApplicationCache::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSDOMApplicationCache::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSDOMApplicationCachePrototype::create(exec->globalData(), globalObject, JSDOMApplicationCachePrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSDOMApplicationCache::destroy(JSC::JSCell* cell)
{
    JSDOMApplicationCache* thisObject = static_cast<JSDOMApplicationCache*>(cell);
    thisObject->JSDOMApplicationCache::~JSDOMApplicationCache();
}

JSDOMApplicationCache::~JSDOMApplicationCache()
{
    releaseImplIfNotNull();
}

bool JSDOMApplicationCache::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSDOMApplicationCache* thisObject = jsCast<JSDOMApplicationCache*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSDOMApplicationCache, Base>(exec, &JSDOMApplicationCacheTable, thisObject, propertyName, slot);
}

bool JSDOMApplicationCache::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSDOMApplicationCache* thisObject = jsCast<JSDOMApplicationCache*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSDOMApplicationCache, Base>(exec, &JSDOMApplicationCacheTable, thisObject, propertyName, descriptor);
}

JSValue jsDOMApplicationCacheStatus(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    JSValue result = jsNumber(impl->status());
    return result;
}


JSValue jsDOMApplicationCacheOnchecking(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onchecking()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOnerror(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onerror()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOnnoupdate(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onnoupdate()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOndownloading(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->ondownloading()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOnprogress(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onprogress()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOnupdateready(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onupdateready()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOncached(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->oncached()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


JSValue jsDOMApplicationCacheOnobsolete(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (EventListener* listener = impl->onobsolete()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(impl->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}


void JSDOMApplicationCache::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSDOMApplicationCache* thisObject = jsCast<JSDOMApplicationCache*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSDOMApplicationCache, Base>(exec, propertyName, value, &JSDOMApplicationCacheTable, thisObject, slot);
}

void setJSDOMApplicationCacheOnchecking(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnchecking(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOnerror(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnerror(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOnnoupdate(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnnoupdate(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOndownloading(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOndownloading(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOnprogress(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnprogress(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOnupdateready(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnupdateready(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOncached(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOncached(createJSAttributeEventListener(exec, value, thisObject));
}


void setJSDOMApplicationCacheOnobsolete(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    UNUSED_PARAM(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(thisObject);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->setOnobsolete(createJSAttributeEventListener(exec, value, thisObject));
}


EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionUpdate(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->update(ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionSwapCache(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->swapCache(ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionAbort(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    impl->abort();
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionAddEventListener(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    JSValue listener = exec->argument(1);
    if (!listener.isObject())
        return JSValue::encode(jsUndefined());
    impl->addEventListener(exec->argument(0).toString(exec)->value(exec), JSEventListener::create(asObject(listener), castedThis, false, currentWorld(exec)), exec->argument(2).toBoolean(exec));
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionRemoveEventListener(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    JSValue listener = exec->argument(1);
    if (!listener.isObject())
        return JSValue::encode(jsUndefined());
    impl->removeEventListener(exec->argument(0).toString(exec)->value(exec), JSEventListener::create(asObject(listener), castedThis, false, currentWorld(exec)).get(), exec->argument(2).toBoolean(exec));
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsDOMApplicationCachePrototypeFunctionDispatchEvent(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMApplicationCache::s_info))
        return throwVMTypeError(exec);
    JSDOMApplicationCache* castedThis = jsCast<JSDOMApplicationCache*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMApplicationCache::s_info);
    DOMApplicationCache* impl = static_cast<DOMApplicationCache*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    Event* evt(toEvent(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsBoolean(impl->dispatchEvent(evt, ec));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

void JSDOMApplicationCache::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    JSDOMApplicationCache* thisObject = jsCast<JSDOMApplicationCache*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    COMPILE_ASSERT(StructureFlags & OverridesVisitChildren, OverridesVisitChildrenWithoutSettingFlag);
    ASSERT(thisObject->structure()->typeInfo().overridesVisitChildren());
    Base::visitChildren(thisObject, visitor);
    thisObject->impl()->visitJSEventListeners(visitor);
}

// Constant getters

JSValue jsDOMApplicationCacheUNCACHED(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0));
}

JSValue jsDOMApplicationCacheIDLE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsDOMApplicationCacheCHECKING(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsDOMApplicationCacheDOWNLOADING(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSValue jsDOMApplicationCacheUPDATEREADY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(4));
}

JSValue jsDOMApplicationCacheOBSOLETE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(5));
}

static inline bool isObservable(JSDOMApplicationCache* jsDOMApplicationCache)
{
    if (jsDOMApplicationCache->hasCustomProperties())
        return true;
    if (jsDOMApplicationCache->impl()->hasEventListeners())
        return true;
    return false;
}

bool JSDOMApplicationCacheOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSDOMApplicationCache* jsDOMApplicationCache = jsCast<JSDOMApplicationCache*>(handle.get().asCell());
    if (!isObservable(jsDOMApplicationCache))
        return false;
    Frame* root = jsDOMApplicationCache->impl()->frame();
    if (!root)
        return false;
    return visitor.containsOpaqueRoot(root);
}

void JSDOMApplicationCacheOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSDOMApplicationCache* jsDOMApplicationCache = jsCast<JSDOMApplicationCache*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsDOMApplicationCache->impl(), jsDOMApplicationCache);
    jsDOMApplicationCache->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, DOMApplicationCache* impl)
{
    return wrap<JSDOMApplicationCache>(exec, globalObject, impl);
}

DOMApplicationCache* toDOMApplicationCache(JSC::JSValue value)
{
    return value.inherits(&JSDOMApplicationCache::s_info) ? jsCast<JSDOMApplicationCache*>(asObject(value))->impl() : 0;
}

}
