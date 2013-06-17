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

#include "JSSVGPathSegCurvetoQuadraticAbs.h"

#include "SVGPathSegCurvetoQuadratic.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGPathSegCurvetoQuadraticAbsTableValues[] =
{
    { "x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoQuadraticAbsX), (intptr_t)setJSSVGPathSegCurvetoQuadraticAbsX, NoIntrinsic },
    { "y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoQuadraticAbsY), (intptr_t)setJSSVGPathSegCurvetoQuadraticAbsY, NoIntrinsic },
    { "x1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoQuadraticAbsX1), (intptr_t)setJSSVGPathSegCurvetoQuadraticAbsX1, NoIntrinsic },
    { "y1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoQuadraticAbsY1), (intptr_t)setJSSVGPathSegCurvetoQuadraticAbsY1, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoQuadraticAbsConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegCurvetoQuadraticAbsTable = { 16, 15, JSSVGPathSegCurvetoQuadraticAbsTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGPathSegCurvetoQuadraticAbsConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegCurvetoQuadraticAbsConstructorTable = { 1, 0, JSSVGPathSegCurvetoQuadraticAbsConstructorTableValues, 0 };
const ClassInfo JSSVGPathSegCurvetoQuadraticAbsConstructor::s_info = { "SVGPathSegCurvetoQuadraticAbsConstructor", &Base::s_info, &JSSVGPathSegCurvetoQuadraticAbsConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegCurvetoQuadraticAbsConstructor) };

JSSVGPathSegCurvetoQuadraticAbsConstructor::JSSVGPathSegCurvetoQuadraticAbsConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGPathSegCurvetoQuadraticAbsConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGPathSegCurvetoQuadraticAbsPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGPathSegCurvetoQuadraticAbsConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoQuadraticAbsConstructor, JSDOMWrapper>(exec, &JSSVGPathSegCurvetoQuadraticAbsConstructorTable, jsCast<JSSVGPathSegCurvetoQuadraticAbsConstructor*>(cell), propertyName, slot);
}

bool JSSVGPathSegCurvetoQuadraticAbsConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegCurvetoQuadraticAbsConstructor, JSDOMWrapper>(exec, &JSSVGPathSegCurvetoQuadraticAbsConstructorTable, jsCast<JSSVGPathSegCurvetoQuadraticAbsConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegCurvetoQuadraticAbsPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegCurvetoQuadraticAbsPrototypeTable = { 1, 0, JSSVGPathSegCurvetoQuadraticAbsPrototypeTableValues, 0 };
const ClassInfo JSSVGPathSegCurvetoQuadraticAbsPrototype::s_info = { "SVGPathSegCurvetoQuadraticAbsPrototype", &Base::s_info, &JSSVGPathSegCurvetoQuadraticAbsPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegCurvetoQuadraticAbsPrototype) };

JSObject* JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSegCurvetoQuadraticAbs>(exec, globalObject);
}

const ClassInfo JSSVGPathSegCurvetoQuadraticAbs::s_info = { "SVGPathSegCurvetoQuadraticAbs", &Base::s_info, &JSSVGPathSegCurvetoQuadraticAbsTable, 0 , CREATE_METHOD_TABLE(JSSVGPathSegCurvetoQuadraticAbs) };

JSSVGPathSegCurvetoQuadraticAbs::JSSVGPathSegCurvetoQuadraticAbs(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPathSegCurvetoQuadraticAbs> impl)
    : JSSVGPathSeg(structure, globalObject, impl)
{
}

void JSSVGPathSegCurvetoQuadraticAbs::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGPathSegCurvetoQuadraticAbs::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGPathSegCurvetoQuadraticAbsPrototype::create(exec->globalData(), globalObject, JSSVGPathSegCurvetoQuadraticAbsPrototype::createStructure(exec->globalData(), globalObject, JSSVGPathSegPrototype::self(exec, globalObject)));
}

bool JSSVGPathSegCurvetoQuadraticAbs::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGPathSegCurvetoQuadraticAbs* thisObject = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGPathSegCurvetoQuadraticAbs, Base>(exec, &JSSVGPathSegCurvetoQuadraticAbsTable, thisObject, propertyName, slot);
}

bool JSSVGPathSegCurvetoQuadraticAbs::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGPathSegCurvetoQuadraticAbs* thisObject = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGPathSegCurvetoQuadraticAbs, Base>(exec, &JSSVGPathSegCurvetoQuadraticAbsTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGPathSegCurvetoQuadraticAbsX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    JSValue result = jsNumber(impl->x());
    return result;
}


JSValue jsSVGPathSegCurvetoQuadraticAbsY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    JSValue result = jsNumber(impl->y());
    return result;
}


JSValue jsSVGPathSegCurvetoQuadraticAbsX1(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    JSValue result = jsNumber(impl->x1());
    return result;
}


JSValue jsSVGPathSegCurvetoQuadraticAbsY1(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    JSValue result = jsNumber(impl->y1());
    return result;
}


JSValue jsSVGPathSegCurvetoQuadraticAbsConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSegCurvetoQuadraticAbs* domObject = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(asObject(slotBase));
    return JSSVGPathSegCurvetoQuadraticAbs::getConstructor(exec, domObject->globalObject());
}

void JSSVGPathSegCurvetoQuadraticAbs::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGPathSegCurvetoQuadraticAbs* thisObject = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGPathSegCurvetoQuadraticAbs, Base>(exec, propertyName, value, &JSSVGPathSegCurvetoQuadraticAbsTable, thisObject, slot);
}

void setJSSVGPathSegCurvetoQuadraticAbsX(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(thisObject);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    impl->setX(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoQuadraticAbsY(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(thisObject);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    impl->setY(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoQuadraticAbsX1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(thisObject);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    impl->setX1(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoQuadraticAbsY1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGPathSegCurvetoQuadraticAbs* castedThis = jsCast<JSSVGPathSegCurvetoQuadraticAbs*>(thisObject);
    SVGPathSegCurvetoQuadraticAbs* impl = static_cast<SVGPathSegCurvetoQuadraticAbs*>(castedThis->impl());
    impl->setY1(value.toFloat(exec));
}


JSValue JSSVGPathSegCurvetoQuadraticAbs::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPathSegCurvetoQuadraticAbsConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG)
