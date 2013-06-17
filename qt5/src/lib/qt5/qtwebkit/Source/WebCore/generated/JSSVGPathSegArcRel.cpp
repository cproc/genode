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

#include "JSSVGPathSegArcRel.h"

#include "SVGPathSegArc.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGPathSegArcRelTableValues[] =
{
    { "x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelX), (intptr_t)setJSSVGPathSegArcRelX, NoIntrinsic },
    { "y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelY), (intptr_t)setJSSVGPathSegArcRelY, NoIntrinsic },
    { "r1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelR1), (intptr_t)setJSSVGPathSegArcRelR1, NoIntrinsic },
    { "r2", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelR2), (intptr_t)setJSSVGPathSegArcRelR2, NoIntrinsic },
    { "angle", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelAngle), (intptr_t)setJSSVGPathSegArcRelAngle, NoIntrinsic },
    { "largeArcFlag", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelLargeArcFlag), (intptr_t)setJSSVGPathSegArcRelLargeArcFlag, NoIntrinsic },
    { "sweepFlag", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelSweepFlag), (intptr_t)setJSSVGPathSegArcRelSweepFlag, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegArcRelConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegArcRelTable = { 17, 15, JSSVGPathSegArcRelTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGPathSegArcRelConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegArcRelConstructorTable = { 1, 0, JSSVGPathSegArcRelConstructorTableValues, 0 };
const ClassInfo JSSVGPathSegArcRelConstructor::s_info = { "SVGPathSegArcRelConstructor", &Base::s_info, &JSSVGPathSegArcRelConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegArcRelConstructor) };

JSSVGPathSegArcRelConstructor::JSSVGPathSegArcRelConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGPathSegArcRelConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGPathSegArcRelPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGPathSegArcRelConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegArcRelConstructor, JSDOMWrapper>(exec, &JSSVGPathSegArcRelConstructorTable, jsCast<JSSVGPathSegArcRelConstructor*>(cell), propertyName, slot);
}

bool JSSVGPathSegArcRelConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegArcRelConstructor, JSDOMWrapper>(exec, &JSSVGPathSegArcRelConstructorTable, jsCast<JSSVGPathSegArcRelConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegArcRelPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegArcRelPrototypeTable = { 1, 0, JSSVGPathSegArcRelPrototypeTableValues, 0 };
const ClassInfo JSSVGPathSegArcRelPrototype::s_info = { "SVGPathSegArcRelPrototype", &Base::s_info, &JSSVGPathSegArcRelPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegArcRelPrototype) };

JSObject* JSSVGPathSegArcRelPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSegArcRel>(exec, globalObject);
}

const ClassInfo JSSVGPathSegArcRel::s_info = { "SVGPathSegArcRel", &Base::s_info, &JSSVGPathSegArcRelTable, 0 , CREATE_METHOD_TABLE(JSSVGPathSegArcRel) };

JSSVGPathSegArcRel::JSSVGPathSegArcRel(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPathSegArcRel> impl)
    : JSSVGPathSeg(structure, globalObject, impl)
{
}

void JSSVGPathSegArcRel::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGPathSegArcRel::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGPathSegArcRelPrototype::create(exec->globalData(), globalObject, JSSVGPathSegArcRelPrototype::createStructure(exec->globalData(), globalObject, JSSVGPathSegPrototype::self(exec, globalObject)));
}

bool JSSVGPathSegArcRel::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGPathSegArcRel* thisObject = jsCast<JSSVGPathSegArcRel*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGPathSegArcRel, Base>(exec, &JSSVGPathSegArcRelTable, thisObject, propertyName, slot);
}

bool JSSVGPathSegArcRel::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGPathSegArcRel* thisObject = jsCast<JSSVGPathSegArcRel*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGPathSegArcRel, Base>(exec, &JSSVGPathSegArcRelTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGPathSegArcRelX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsNumber(impl->x());
    return result;
}


JSValue jsSVGPathSegArcRelY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsNumber(impl->y());
    return result;
}


JSValue jsSVGPathSegArcRelR1(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsNumber(impl->r1());
    return result;
}


JSValue jsSVGPathSegArcRelR2(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsNumber(impl->r2());
    return result;
}


JSValue jsSVGPathSegArcRelAngle(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsNumber(impl->angle());
    return result;
}


JSValue jsSVGPathSegArcRelLargeArcFlag(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsBoolean(impl->largeArcFlag());
    return result;
}


JSValue jsSVGPathSegArcRelSweepFlag(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    JSValue result = jsBoolean(impl->sweepFlag());
    return result;
}


JSValue jsSVGPathSegArcRelConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegArcRel* domObject = jsCast<JSSVGPathSegArcRel*>(asObject(slotBase));
    return JSSVGPathSegArcRel::getConstructor(exec, domObject->globalObject());
}

void JSSVGPathSegArcRel::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGPathSegArcRel* thisObject = jsCast<JSSVGPathSegArcRel*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGPathSegArcRel, Base>(exec, propertyName, value, &JSSVGPathSegArcRelTable, thisObject, slot);
}

void setJSSVGPathSegArcRelX(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setX(value.toFloat(exec));
}


void setJSSVGPathSegArcRelY(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setY(value.toFloat(exec));
}


void setJSSVGPathSegArcRelR1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setR1(value.toFloat(exec));
}


void setJSSVGPathSegArcRelR2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setR2(value.toFloat(exec));
}


void setJSSVGPathSegArcRelAngle(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setAngle(value.toFloat(exec));
}


void setJSSVGPathSegArcRelLargeArcFlag(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setLargeArcFlag(value.toBoolean(exec));
}


void setJSSVGPathSegArcRelSweepFlag(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegArcRel* castedThis = jsCast<JSSVGPathSegArcRel*>(thisObject);
    SVGPathSegArcRel* impl = static_cast<SVGPathSegArcRel*>(castedThis->impl());
    impl->setSweepFlag(value.toBoolean(exec));
}


JSValue JSSVGPathSegArcRel::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPathSegArcRelConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG)
