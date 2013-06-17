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

#if ENABLE(SVG)

#include "JSSVGLengthList.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSSVGLength.h"
#include "SVGLength.h"
#include "SVGLengthList.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGLengthListTableValues[] =
{
    { "numberOfItems", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGLengthListNumberOfItems), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGLengthListConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGLengthListTable = { 4, 3, JSSVGLengthListTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGLengthListConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGLengthListConstructorTable = { 1, 0, JSSVGLengthListConstructorTableValues, 0 };
const ClassInfo JSSVGLengthListConstructor::s_info = { "SVGLengthListConstructor", &Base::s_info, &JSSVGLengthListConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGLengthListConstructor) };

JSSVGLengthListConstructor::JSSVGLengthListConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGLengthListConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGLengthListPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGLengthListConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGLengthListConstructor, JSDOMWrapper>(exec, &JSSVGLengthListConstructorTable, jsCast<JSSVGLengthListConstructor*>(cell), propertyName, slot);
}

bool JSSVGLengthListConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGLengthListConstructor, JSDOMWrapper>(exec, &JSSVGLengthListConstructorTable, jsCast<JSSVGLengthListConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGLengthListPrototypeTableValues[] =
{
    { "clear", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionClear), (intptr_t)0, NoIntrinsic },
    { "initialize", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionInitialize), (intptr_t)1, NoIntrinsic },
    { "getItem", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionGetItem), (intptr_t)1, NoIntrinsic },
    { "insertItemBefore", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionInsertItemBefore), (intptr_t)2, NoIntrinsic },
    { "replaceItem", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionReplaceItem), (intptr_t)2, NoIntrinsic },
    { "removeItem", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionRemoveItem), (intptr_t)1, NoIntrinsic },
    { "appendItem", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGLengthListPrototypeFunctionAppendItem), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGLengthListPrototypeTable = { 18, 15, JSSVGLengthListPrototypeTableValues, 0 };
const ClassInfo JSSVGLengthListPrototype::s_info = { "SVGLengthListPrototype", &Base::s_info, &JSSVGLengthListPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGLengthListPrototype) };

JSObject* JSSVGLengthListPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGLengthList>(exec, globalObject);
}

bool JSSVGLengthListPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGLengthListPrototype* thisObject = jsCast<JSSVGLengthListPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGLengthListPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGLengthListPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGLengthListPrototype* thisObject = jsCast<JSSVGLengthListPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSSVGLengthListPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGLengthList::s_info = { "SVGLengthList", &Base::s_info, &JSSVGLengthListTable, 0 , CREATE_METHOD_TABLE(JSSVGLengthList) };

JSSVGLengthList::JSSVGLengthList(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGListPropertyTearOff<SVGLengthList> > impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSSVGLengthList::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGLengthList::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGLengthListPrototype::create(exec->globalData(), globalObject, JSSVGLengthListPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSSVGLengthList::destroy(JSC::JSCell* cell)
{
    JSSVGLengthList* thisObject = static_cast<JSSVGLengthList*>(cell);
    thisObject->JSSVGLengthList::~JSSVGLengthList();
}

JSSVGLengthList::~JSSVGLengthList()
{
    releaseImplIfNotNull();
}

bool JSSVGLengthList::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGLengthList* thisObject = jsCast<JSSVGLengthList*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGLengthList, Base>(exec, &JSSVGLengthListTable, thisObject, propertyName, slot);
}

bool JSSVGLengthList::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGLengthList* thisObject = jsCast<JSSVGLengthList*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGLengthList, Base>(exec, &JSSVGLengthListTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGLengthListNumberOfItems(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    JSValue result =  jsNumber(castedThis->impl()->numberOfItems());
    return result;
}


JSValue jsSVGLengthListConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGLengthList* domObject = jsCast<JSSVGLengthList*>(asObject(slotBase));
    return JSSVGLengthList::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGLengthList::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGLengthListConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionClear(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->clear(ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionInitialize(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSSVGLength::s_info))
        return throwVMTypeError(exec);
    SVGPropertyTearOff<SVGLength>* item(toSVGLength(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->initialize(item, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionGetItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getItem(index, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionInsertItemBefore(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 2)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSSVGLength::s_info))
        return throwVMTypeError(exec);
    SVGPropertyTearOff<SVGLength>* item(toSVGLength(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->insertItemBefore(item, index, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionReplaceItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 2)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSSVGLength::s_info))
        return throwVMTypeError(exec);
    SVGPropertyTearOff<SVGLength>* item(toSVGLength(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->replaceItem(item, index, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionRemoveItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->removeItem(index, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGLengthListPrototypeFunctionAppendItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGLengthList::s_info))
        return throwVMTypeError(exec);
    JSSVGLengthList* castedThis = jsCast<JSSVGLengthList*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGLengthList::s_info);
    SVGListPropertyTearOff<SVGLengthList> * impl = static_cast<SVGListPropertyTearOff<SVGLengthList> *>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    if (exec->argumentCount() > 0 && !exec->argument(0).isUndefinedOrNull() && !exec->argument(0).inherits(&JSSVGLength::s_info))
        return throwVMTypeError(exec);
    SVGPropertyTearOff<SVGLength>* item(toSVGLength(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->appendItem(item, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

static inline bool isObservable(JSSVGLengthList* jsSVGLengthList)
{
    if (jsSVGLengthList->hasCustomProperties())
        return true;
    return false;
}

bool JSSVGLengthListOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSSVGLengthList* jsSVGLengthList = jsCast<JSSVGLengthList*>(handle.get().asCell());
    if (!isObservable(jsSVGLengthList))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSSVGLengthListOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSSVGLengthList* jsSVGLengthList = jsCast<JSSVGLengthList*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsSVGLengthList->impl(), jsSVGLengthList);
    jsSVGLengthList->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, SVGListPropertyTearOff<SVGLengthList> * impl)
{
    return wrap<JSSVGLengthList>(exec, globalObject, impl);
}

SVGListPropertyTearOff<SVGLengthList> * toSVGLengthList(JSC::JSValue value)
{
    return value.inherits(&JSSVGLengthList::s_info) ? jsCast<JSSVGLengthList*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(SVG)
