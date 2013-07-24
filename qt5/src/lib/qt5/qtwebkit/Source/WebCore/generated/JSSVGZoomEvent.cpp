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

#include "JSSVGZoomEvent.h"

#include "JSSVGPoint.h"
#include "JSSVGRect.h"
#include "SVGRect.h"
#include "SVGZoomEvent.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGZoomEventTableValues[] =
{
    { "zoomRectScreen", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventZoomRectScreen), (intptr_t)0, NoIntrinsic },
    { "previousScale", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventPreviousScale), (intptr_t)0, NoIntrinsic },
    { "previousTranslate", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventPreviousTranslate), (intptr_t)0, NoIntrinsic },
    { "newScale", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventNewScale), (intptr_t)0, NoIntrinsic },
    { "newTranslate", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventNewTranslate), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGZoomEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGZoomEventTable = { 17, 15, JSSVGZoomEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGZoomEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGZoomEventConstructorTable = { 1, 0, JSSVGZoomEventConstructorTableValues, 0 };
const ClassInfo JSSVGZoomEventConstructor::s_info = { "SVGZoomEventConstructor", &Base::s_info, &JSSVGZoomEventConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGZoomEventConstructor) };

JSSVGZoomEventConstructor::JSSVGZoomEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGZoomEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGZoomEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGZoomEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGZoomEventConstructor, JSDOMWrapper>(exec, &JSSVGZoomEventConstructorTable, jsCast<JSSVGZoomEventConstructor*>(cell), propertyName, slot);
}

bool JSSVGZoomEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGZoomEventConstructor, JSDOMWrapper>(exec, &JSSVGZoomEventConstructorTable, jsCast<JSSVGZoomEventConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGZoomEventPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGZoomEventPrototypeTable = { 1, 0, JSSVGZoomEventPrototypeTableValues, 0 };
const ClassInfo JSSVGZoomEventPrototype::s_info = { "SVGZoomEventPrototype", &Base::s_info, &JSSVGZoomEventPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGZoomEventPrototype) };

JSObject* JSSVGZoomEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGZoomEvent>(exec, globalObject);
}

const ClassInfo JSSVGZoomEvent::s_info = { "SVGZoomEvent", &Base::s_info, &JSSVGZoomEventTable, 0 , CREATE_METHOD_TABLE(JSSVGZoomEvent) };

JSSVGZoomEvent::JSSVGZoomEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGZoomEvent> impl)
    : JSUIEvent(structure, globalObject, impl)
{
}

void JSSVGZoomEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGZoomEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGZoomEventPrototype::create(exec->globalData(), globalObject, JSSVGZoomEventPrototype::createStructure(exec->globalData(), globalObject, JSUIEventPrototype::self(exec, globalObject)));
}

bool JSSVGZoomEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGZoomEvent* thisObject = jsCast<JSSVGZoomEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGZoomEvent, Base>(exec, &JSSVGZoomEventTable, thisObject, propertyName, slot);
}

bool JSSVGZoomEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGZoomEvent* thisObject = jsCast<JSSVGZoomEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGZoomEvent, Base>(exec, &JSSVGZoomEventTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGZoomEventZoomRectScreen(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* castedThis = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGZoomEvent* impl = static_cast<SVGZoomEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<FloatRect>::create(impl->zoomRectScreen())));
    return result;
}


JSValue jsSVGZoomEventPreviousScale(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* castedThis = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGZoomEvent* impl = static_cast<SVGZoomEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->previousScale());
    return result;
}


JSValue jsSVGZoomEventPreviousTranslate(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* castedThis = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGZoomEvent* impl = static_cast<SVGZoomEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<FloatPoint>::create(impl->previousTranslate())));
    return result;
}


JSValue jsSVGZoomEventNewScale(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* castedThis = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGZoomEvent* impl = static_cast<SVGZoomEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->newScale());
    return result;
}


JSValue jsSVGZoomEventNewTranslate(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* castedThis = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGZoomEvent* impl = static_cast<SVGZoomEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<FloatPoint>::create(impl->newTranslate())));
    return result;
}


JSValue jsSVGZoomEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGZoomEvent* domObject = jsCast<JSSVGZoomEvent*>(asObject(slotBase));
    return JSSVGZoomEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGZoomEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGZoomEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG)
