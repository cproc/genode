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

#include "JSSVGPathSeg.h"

#include "KURL.h"
#include "SVGPathSeg.h"
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGPathSegTableValues[] =
{
    { "pathSegType", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPathSegType), (intptr_t)0, NoIntrinsic },
    { "pathSegTypeAsLetter", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPathSegTypeAsLetter), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegTable = { 8, 7, JSSVGPathSegTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGPathSegConstructorTableValues[] =
{
    { "PATHSEG_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_UNKNOWN), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CLOSEPATH", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CLOSEPATH), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_MOVETO_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_MOVETO_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_MOVETO_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_MOVETO_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_ARC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_ARC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_ARC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_ARC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_HORIZONTAL_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_HORIZONTAL_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_VERTICAL_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_VERTICAL_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_VERTICAL_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_VERTICAL_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_SMOOTH_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_SMOOTH_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_REL), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegConstructorTable = { 70, 63, JSSVGPathSegConstructorTableValues, 0 };

COMPILE_ASSERT(0 == SVGPathSeg::PATHSEG_UNKNOWN, SVGPathSegEnumPATHSEG_UNKNOWNIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(1 == SVGPathSeg::PATHSEG_CLOSEPATH, SVGPathSegEnumPATHSEG_CLOSEPATHIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(2 == SVGPathSeg::PATHSEG_MOVETO_ABS, SVGPathSegEnumPATHSEG_MOVETO_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(3 == SVGPathSeg::PATHSEG_MOVETO_REL, SVGPathSegEnumPATHSEG_MOVETO_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(4 == SVGPathSeg::PATHSEG_LINETO_ABS, SVGPathSegEnumPATHSEG_LINETO_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(5 == SVGPathSeg::PATHSEG_LINETO_REL, SVGPathSegEnumPATHSEG_LINETO_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(6 == SVGPathSeg::PATHSEG_CURVETO_CUBIC_ABS, SVGPathSegEnumPATHSEG_CURVETO_CUBIC_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(7 == SVGPathSeg::PATHSEG_CURVETO_CUBIC_REL, SVGPathSegEnumPATHSEG_CURVETO_CUBIC_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(8 == SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_ABS, SVGPathSegEnumPATHSEG_CURVETO_QUADRATIC_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(9 == SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_REL, SVGPathSegEnumPATHSEG_CURVETO_QUADRATIC_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(10 == SVGPathSeg::PATHSEG_ARC_ABS, SVGPathSegEnumPATHSEG_ARC_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(11 == SVGPathSeg::PATHSEG_ARC_REL, SVGPathSegEnumPATHSEG_ARC_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(12 == SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_ABS, SVGPathSegEnumPATHSEG_LINETO_HORIZONTAL_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(13 == SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_REL, SVGPathSegEnumPATHSEG_LINETO_HORIZONTAL_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(14 == SVGPathSeg::PATHSEG_LINETO_VERTICAL_ABS, SVGPathSegEnumPATHSEG_LINETO_VERTICAL_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(15 == SVGPathSeg::PATHSEG_LINETO_VERTICAL_REL, SVGPathSegEnumPATHSEG_LINETO_VERTICAL_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(16 == SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS, SVGPathSegEnumPATHSEG_CURVETO_CUBIC_SMOOTH_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(17 == SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL, SVGPathSegEnumPATHSEG_CURVETO_CUBIC_SMOOTH_RELIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(18 == SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS, SVGPathSegEnumPATHSEG_CURVETO_QUADRATIC_SMOOTH_ABSIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(19 == SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL, SVGPathSegEnumPATHSEG_CURVETO_QUADRATIC_SMOOTH_RELIsWrongUseDoNotCheckConstants);

const ClassInfo JSSVGPathSegConstructor::s_info = { "SVGPathSegConstructor", &Base::s_info, &JSSVGPathSegConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegConstructor) };

JSSVGPathSegConstructor::JSSVGPathSegConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGPathSegConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGPathSegPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGPathSegConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegConstructor, JSDOMWrapper>(exec, &JSSVGPathSegConstructorTable, jsCast<JSSVGPathSegConstructor*>(cell), propertyName, slot);
}

bool JSSVGPathSegConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegConstructor, JSDOMWrapper>(exec, &JSSVGPathSegConstructorTable, jsCast<JSSVGPathSegConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegPrototypeTableValues[] =
{
    { "PATHSEG_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_UNKNOWN), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CLOSEPATH", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CLOSEPATH), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_MOVETO_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_MOVETO_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_MOVETO_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_MOVETO_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_ARC_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_ARC_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_ARC_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_ARC_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_HORIZONTAL_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_HORIZONTAL_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_VERTICAL_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_VERTICAL_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_LINETO_VERTICAL_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_LINETO_VERTICAL_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_SMOOTH_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_CUBIC_SMOOTH_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_REL), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS), (intptr_t)0, NoIntrinsic },
    { "PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_REL), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGPathSegPrototypeTable = { 70, 63, JSSVGPathSegPrototypeTableValues, 0 };
const ClassInfo JSSVGPathSegPrototype::s_info = { "SVGPathSegPrototype", &Base::s_info, &JSSVGPathSegPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGPathSegPrototype) };

JSObject* JSSVGPathSegPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSeg>(exec, globalObject);
}

bool JSSVGPathSegPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGPathSegPrototype* thisObject = jsCast<JSSVGPathSegPrototype*>(cell);
    return getStaticValueSlot<JSSVGPathSegPrototype, JSObject>(exec, &JSSVGPathSegPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGPathSegPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGPathSegPrototype* thisObject = jsCast<JSSVGPathSegPrototype*>(object);
    return getStaticValueDescriptor<JSSVGPathSegPrototype, JSObject>(exec, &JSSVGPathSegPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGPathSeg::s_info = { "SVGPathSeg", &Base::s_info, &JSSVGPathSegTable, 0 , CREATE_METHOD_TABLE(JSSVGPathSeg) };

JSSVGPathSeg::JSSVGPathSeg(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPathSeg> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSSVGPathSeg::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGPathSeg::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGPathSegPrototype::create(exec->globalData(), globalObject, JSSVGPathSegPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSSVGPathSeg::destroy(JSC::JSCell* cell)
{
    JSSVGPathSeg* thisObject = static_cast<JSSVGPathSeg*>(cell);
    thisObject->JSSVGPathSeg::~JSSVGPathSeg();
}

JSSVGPathSeg::~JSSVGPathSeg()
{
    releaseImplIfNotNull();
}

bool JSSVGPathSeg::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGPathSeg* thisObject = jsCast<JSSVGPathSeg*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGPathSeg, Base>(exec, &JSSVGPathSegTable, thisObject, propertyName, slot);
}

bool JSSVGPathSeg::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGPathSeg* thisObject = jsCast<JSSVGPathSeg*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGPathSeg, Base>(exec, &JSSVGPathSegTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGPathSegPathSegType(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSeg* castedThis = jsCast<JSSVGPathSeg*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSeg* impl = static_cast<SVGPathSeg*>(castedThis->impl());
    JSValue result = jsNumber(impl->pathSegType());
    return result;
}


JSValue jsSVGPathSegPathSegTypeAsLetter(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSeg* castedThis = jsCast<JSSVGPathSeg*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSeg* impl = static_cast<SVGPathSeg*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->pathSegTypeAsLetter());
    return result;
}


JSValue jsSVGPathSegConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGPathSeg* domObject = jsCast<JSSVGPathSeg*>(asObject(slotBase));
    return JSSVGPathSeg::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGPathSeg::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPathSegConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

// Constant getters

JSValue jsSVGPathSegPATHSEG_UNKNOWN(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0));
}

JSValue jsSVGPathSegPATHSEG_CLOSEPATH(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsSVGPathSegPATHSEG_MOVETO_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsSVGPathSegPATHSEG_MOVETO_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSValue jsSVGPathSegPATHSEG_LINETO_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(4));
}

JSValue jsSVGPathSegPATHSEG_LINETO_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(5));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_CUBIC_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(6));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_CUBIC_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(7));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(8));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(9));
}

JSValue jsSVGPathSegPATHSEG_ARC_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(10));
}

JSValue jsSVGPathSegPATHSEG_ARC_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(11));
}

JSValue jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(12));
}

JSValue jsSVGPathSegPATHSEG_LINETO_HORIZONTAL_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(13));
}

JSValue jsSVGPathSegPATHSEG_LINETO_VERTICAL_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(14));
}

JSValue jsSVGPathSegPATHSEG_LINETO_VERTICAL_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(15));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(16));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_CUBIC_SMOOTH_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(17));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(18));
}

JSValue jsSVGPathSegPATHSEG_CURVETO_QUADRATIC_SMOOTH_REL(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(19));
}

static inline bool isObservable(JSSVGPathSeg* jsSVGPathSeg)
{
    if (jsSVGPathSeg->hasCustomProperties())
        return true;
    return false;
}

bool JSSVGPathSegOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSSVGPathSeg* jsSVGPathSeg = jsCast<JSSVGPathSeg*>(handle.get().asCell());
    if (!isObservable(jsSVGPathSeg))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSSVGPathSegOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSSVGPathSeg* jsSVGPathSeg = jsCast<JSSVGPathSeg*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsSVGPathSeg->impl(), jsSVGPathSeg);
    jsSVGPathSeg->releaseImpl();
}

SVGPathSeg* toSVGPathSeg(JSC::JSValue value)
{
    return value.inherits(&JSSVGPathSeg::s_info) ? jsCast<JSSVGPathSeg*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(SVG)
