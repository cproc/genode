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
#include "JSMediaQueryList.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "KURL.h"
#include "MediaQueryList.h"
#include "MediaQueryListListener.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSMediaQueryListTableValues[] =
{
    { "media", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsMediaQueryListMedia), (intptr_t)0, NoIntrinsic },
    { "matches", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsMediaQueryListMatches), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsMediaQueryListConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSMediaQueryListTable = { 8, 7, JSMediaQueryListTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSMediaQueryListConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSMediaQueryListConstructorTable = { 1, 0, JSMediaQueryListConstructorTableValues, 0 };
const ClassInfo JSMediaQueryListConstructor::s_info = { "MediaQueryListConstructor", &Base::s_info, &JSMediaQueryListConstructorTable, 0, CREATE_METHOD_TABLE(JSMediaQueryListConstructor) };

JSMediaQueryListConstructor::JSMediaQueryListConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSMediaQueryListConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSMediaQueryListPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSMediaQueryListConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSMediaQueryListConstructor, JSDOMWrapper>(exec, &JSMediaQueryListConstructorTable, jsCast<JSMediaQueryListConstructor*>(cell), propertyName, slot);
}

bool JSMediaQueryListConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSMediaQueryListConstructor, JSDOMWrapper>(exec, &JSMediaQueryListConstructorTable, jsCast<JSMediaQueryListConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSMediaQueryListPrototypeTableValues[] =
{
    { "addListener", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsMediaQueryListPrototypeFunctionAddListener), (intptr_t)1, NoIntrinsic },
    { "removeListener", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsMediaQueryListPrototypeFunctionRemoveListener), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSMediaQueryListPrototypeTable = { 4, 3, JSMediaQueryListPrototypeTableValues, 0 };
const ClassInfo JSMediaQueryListPrototype::s_info = { "MediaQueryListPrototype", &Base::s_info, &JSMediaQueryListPrototypeTable, 0, CREATE_METHOD_TABLE(JSMediaQueryListPrototype) };

JSObject* JSMediaQueryListPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSMediaQueryList>(exec, globalObject);
}

bool JSMediaQueryListPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSMediaQueryListPrototype* thisObject = jsCast<JSMediaQueryListPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSMediaQueryListPrototypeTable, thisObject, propertyName, slot);
}

bool JSMediaQueryListPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSMediaQueryListPrototype* thisObject = jsCast<JSMediaQueryListPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSMediaQueryListPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSMediaQueryList::s_info = { "MediaQueryList", &Base::s_info, &JSMediaQueryListTable, 0 , CREATE_METHOD_TABLE(JSMediaQueryList) };

JSMediaQueryList::JSMediaQueryList(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<MediaQueryList> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSMediaQueryList::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSMediaQueryList::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSMediaQueryListPrototype::create(exec->globalData(), globalObject, JSMediaQueryListPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSMediaQueryList::destroy(JSC::JSCell* cell)
{
    JSMediaQueryList* thisObject = static_cast<JSMediaQueryList*>(cell);
    thisObject->JSMediaQueryList::~JSMediaQueryList();
}

JSMediaQueryList::~JSMediaQueryList()
{
    releaseImplIfNotNull();
}

bool JSMediaQueryList::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSMediaQueryList* thisObject = jsCast<JSMediaQueryList*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSMediaQueryList, Base>(exec, &JSMediaQueryListTable, thisObject, propertyName, slot);
}

bool JSMediaQueryList::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSMediaQueryList* thisObject = jsCast<JSMediaQueryList*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSMediaQueryList, Base>(exec, &JSMediaQueryListTable, thisObject, propertyName, descriptor);
}

JSValue jsMediaQueryListMedia(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSMediaQueryList* castedThis = jsCast<JSMediaQueryList*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    MediaQueryList* impl = static_cast<MediaQueryList*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->media());
    return result;
}


JSValue jsMediaQueryListMatches(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSMediaQueryList* castedThis = jsCast<JSMediaQueryList*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    MediaQueryList* impl = static_cast<MediaQueryList*>(castedThis->impl());
    JSValue result = jsBoolean(impl->matches());
    return result;
}


JSValue jsMediaQueryListConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSMediaQueryList* domObject = jsCast<JSMediaQueryList*>(asObject(slotBase));
    return JSMediaQueryList::getConstructor(exec, domObject->globalObject());
}

JSValue JSMediaQueryList::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSMediaQueryListConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsMediaQueryListPrototypeFunctionAddListener(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSMediaQueryList::s_info))
        return throwVMTypeError(exec);
    JSMediaQueryList* castedThis = jsCast<JSMediaQueryList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSMediaQueryList::s_info);
    MediaQueryList* impl = static_cast<MediaQueryList*>(castedThis->impl());
    RefPtr<MediaQueryListListener> listener(MediaQueryListListener::create(ScriptValue(exec->globalData(), MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined))));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->addListener(listener);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsMediaQueryListPrototypeFunctionRemoveListener(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSMediaQueryList::s_info))
        return throwVMTypeError(exec);
    JSMediaQueryList* castedThis = jsCast<JSMediaQueryList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSMediaQueryList::s_info);
    MediaQueryList* impl = static_cast<MediaQueryList*>(castedThis->impl());
    RefPtr<MediaQueryListListener> listener(MediaQueryListListener::create(ScriptValue(exec->globalData(), MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined))));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->removeListener(listener);
    return JSValue::encode(jsUndefined());
}

static inline bool isObservable(JSMediaQueryList* jsMediaQueryList)
{
    if (jsMediaQueryList->hasCustomProperties())
        return true;
    return false;
}

bool JSMediaQueryListOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSMediaQueryList* jsMediaQueryList = jsCast<JSMediaQueryList*>(handle.get().asCell());
    if (!isObservable(jsMediaQueryList))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSMediaQueryListOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSMediaQueryList* jsMediaQueryList = jsCast<JSMediaQueryList*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsMediaQueryList->impl(), jsMediaQueryList);
    jsMediaQueryList->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, MediaQueryList* impl)
{
    return wrap<JSMediaQueryList>(exec, globalObject, impl);
}

MediaQueryList* toMediaQueryList(JSC::JSValue value)
{
    return value.inherits(&JSMediaQueryList::s_info) ? jsCast<JSMediaQueryList*>(asObject(value))->impl() : 0;
}

}
