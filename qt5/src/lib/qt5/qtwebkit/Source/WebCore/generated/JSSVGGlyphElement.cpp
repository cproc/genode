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

#if ENABLE(SVG) && ENABLE(SVG_FONTS)

#include "JSSVGGlyphElement.h"

#include "SVGGlyphElement.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGGlyphElementTableValues[] =
{
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphElementTable = { 2, 1, JSSVGGlyphElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGGlyphElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphElementConstructorTable = { 1, 0, JSSVGGlyphElementConstructorTableValues, 0 };
const ClassInfo JSSVGGlyphElementConstructor::s_info = { "SVGGlyphElementConstructor", &Base::s_info, &JSSVGGlyphElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGGlyphElementConstructor) };

JSSVGGlyphElementConstructor::JSSVGGlyphElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGGlyphElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGGlyphElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGGlyphElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGGlyphElementConstructor, JSDOMWrapper>(exec, &JSSVGGlyphElementConstructorTable, jsCast<JSSVGGlyphElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGGlyphElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGGlyphElementConstructor, JSDOMWrapper>(exec, &JSSVGGlyphElementConstructorTable, jsCast<JSSVGGlyphElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGGlyphElementPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphElementPrototypeTable = { 1, 0, JSSVGGlyphElementPrototypeTableValues, 0 };
const ClassInfo JSSVGGlyphElementPrototype::s_info = { "SVGGlyphElementPrototype", &Base::s_info, &JSSVGGlyphElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGGlyphElementPrototype) };

JSObject* JSSVGGlyphElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGGlyphElement>(exec, globalObject);
}

const ClassInfo JSSVGGlyphElement::s_info = { "SVGGlyphElement", &Base::s_info, &JSSVGGlyphElementTable, 0 , CREATE_METHOD_TABLE(JSSVGGlyphElement) };

JSSVGGlyphElement::JSSVGGlyphElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGGlyphElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGGlyphElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGGlyphElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGGlyphElementPrototype::create(exec->globalData(), globalObject, JSSVGGlyphElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGGlyphElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGGlyphElement* thisObject = jsCast<JSSVGGlyphElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGGlyphElement, Base>(exec, &JSSVGGlyphElementTable, thisObject, propertyName, slot);
}

bool JSSVGGlyphElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGGlyphElement* thisObject = jsCast<JSSVGGlyphElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGGlyphElement, Base>(exec, &JSSVGGlyphElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGGlyphElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphElement* domObject = jsCast<JSSVGGlyphElement*>(asObject(slotBase));
    return JSSVGGlyphElement::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGGlyphElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGGlyphElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FONTS)
