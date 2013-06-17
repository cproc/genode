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

#include "JSSVGAnimatedNumber.h"

#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGAnimatedNumberTableValues[] =
{
    { "baseVal", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGAnimatedNumberBaseVal), (intptr_t)setJSSVGAnimatedNumberBaseVal, NoIntrinsic },
    { "animVal", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGAnimatedNumberAnimVal), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGAnimatedNumberConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGAnimatedNumberTable = { 8, 7, JSSVGAnimatedNumberTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGAnimatedNumberConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGAnimatedNumberConstructorTable = { 1, 0, JSSVGAnimatedNumberConstructorTableValues, 0 };
const ClassInfo JSSVGAnimatedNumberConstructor::s_info = { "SVGAnimatedNumberConstructor", &Base::s_info, &JSSVGAnimatedNumberConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGAnimatedNumberConstructor) };

JSSVGAnimatedNumberConstructor::JSSVGAnimatedNumberConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGAnimatedNumberConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGAnimatedNumberPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGAnimatedNumberConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGAnimatedNumberConstructor, JSDOMWrapper>(exec, &JSSVGAnimatedNumberConstructorTable, jsCast<JSSVGAnimatedNumberConstructor*>(cell), propertyName, slot);
}

bool JSSVGAnimatedNumberConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGAnimatedNumberConstructor, JSDOMWrapper>(exec, &JSSVGAnimatedNumberConstructorTable, jsCast<JSSVGAnimatedNumberConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGAnimatedNumberPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGAnimatedNumberPrototypeTable = { 1, 0, JSSVGAnimatedNumberPrototypeTableValues, 0 };
const ClassInfo JSSVGAnimatedNumberPrototype::s_info = { "SVGAnimatedNumberPrototype", &Base::s_info, &JSSVGAnimatedNumberPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGAnimatedNumberPrototype) };

JSObject* JSSVGAnimatedNumberPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGAnimatedNumber>(exec, globalObject);
}

const ClassInfo JSSVGAnimatedNumber::s_info = { "SVGAnimatedNumber", &Base::s_info, &JSSVGAnimatedNumberTable, 0 , CREATE_METHOD_TABLE(JSSVGAnimatedNumber) };

JSSVGAnimatedNumber::JSSVGAnimatedNumber(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGAnimatedNumber> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSSVGAnimatedNumber::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGAnimatedNumber::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGAnimatedNumberPrototype::create(exec->globalData(), globalObject, JSSVGAnimatedNumberPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSSVGAnimatedNumber::destroy(JSC::JSCell* cell)
{
    JSSVGAnimatedNumber* thisObject = static_cast<JSSVGAnimatedNumber*>(cell);
    thisObject->JSSVGAnimatedNumber::~JSSVGAnimatedNumber();
}

JSSVGAnimatedNumber::~JSSVGAnimatedNumber()
{
    releaseImplIfNotNull();
}

bool JSSVGAnimatedNumber::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGAnimatedNumber* thisObject = jsCast<JSSVGAnimatedNumber*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGAnimatedNumber, Base>(exec, &JSSVGAnimatedNumberTable, thisObject, propertyName, slot);
}

bool JSSVGAnimatedNumber::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGAnimatedNumber* thisObject = jsCast<JSSVGAnimatedNumber*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGAnimatedNumber, Base>(exec, &JSSVGAnimatedNumberTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGAnimatedNumberBaseVal(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGAnimatedNumber* castedThis = jsCast<JSSVGAnimatedNumber*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGAnimatedNumber* impl = static_cast<SVGAnimatedNumber*>(castedThis->impl());
    JSValue result = jsNumber(impl->baseVal());
    return result;
}


JSValue jsSVGAnimatedNumberAnimVal(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGAnimatedNumber* castedThis = jsCast<JSSVGAnimatedNumber*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGAnimatedNumber* impl = static_cast<SVGAnimatedNumber*>(castedThis->impl());
    JSValue result = jsNumber(impl->animVal());
    return result;
}


JSValue jsSVGAnimatedNumberConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGAnimatedNumber* domObject = jsCast<JSSVGAnimatedNumber*>(asObject(slotBase));
    return JSSVGAnimatedNumber::getConstructor(exec, domObject->globalObject());
}

void JSSVGAnimatedNumber::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGAnimatedNumber* thisObject = jsCast<JSSVGAnimatedNumber*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGAnimatedNumber, Base>(exec, propertyName, value, &JSSVGAnimatedNumberTable, thisObject, slot);
}

void setJSSVGAnimatedNumberBaseVal(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGAnimatedNumber* castedThis = jsCast<JSSVGAnimatedNumber*>(thisObject);
    SVGAnimatedNumber* impl = static_cast<SVGAnimatedNumber*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setBaseVal(value.toFloat(exec), ec);
    setDOMException(exec, ec);
}


JSValue JSSVGAnimatedNumber::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGAnimatedNumberConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

static inline bool isObservable(JSSVGAnimatedNumber* jsSVGAnimatedNumber)
{
    if (jsSVGAnimatedNumber->hasCustomProperties())
        return true;
    return false;
}

bool JSSVGAnimatedNumberOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSSVGAnimatedNumber* jsSVGAnimatedNumber = jsCast<JSSVGAnimatedNumber*>(handle.get().asCell());
    if (!isObservable(jsSVGAnimatedNumber))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSSVGAnimatedNumberOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSSVGAnimatedNumber* jsSVGAnimatedNumber = jsCast<JSSVGAnimatedNumber*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsSVGAnimatedNumber->impl(), jsSVGAnimatedNumber);
    jsSVGAnimatedNumber->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, SVGAnimatedNumber* impl)
{
    return wrap<JSSVGAnimatedNumber>(exec, globalObject, impl);
}

SVGAnimatedNumber* toSVGAnimatedNumber(JSC::JSValue value)
{
    return value.inherits(&JSSVGAnimatedNumber::s_info) ? jsCast<JSSVGAnimatedNumber*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(SVG)
