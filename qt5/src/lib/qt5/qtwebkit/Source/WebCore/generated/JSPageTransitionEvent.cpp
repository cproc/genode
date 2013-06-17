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
#include "JSPageTransitionEvent.h"

#include "JSDictionary.h"
#include "PageTransitionEvent.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSPageTransitionEventTableValues[] =
{
    { "persisted", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsPageTransitionEventPersisted), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsPageTransitionEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSPageTransitionEventTable = { 4, 3, JSPageTransitionEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSPageTransitionEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSPageTransitionEventConstructorTable = { 1, 0, JSPageTransitionEventConstructorTableValues, 0 };
const ClassInfo JSPageTransitionEventConstructor::s_info = { "PageTransitionEventConstructor", &Base::s_info, &JSPageTransitionEventConstructorTable, 0, CREATE_METHOD_TABLE(JSPageTransitionEventConstructor) };

JSPageTransitionEventConstructor::JSPageTransitionEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSPageTransitionEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSPageTransitionEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
    putDirect(exec->globalData(), exec->propertyNames().length, jsNumber(2), ReadOnly | DontDelete | DontEnum);
}

bool JSPageTransitionEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSPageTransitionEventConstructor, JSDOMWrapper>(exec, &JSPageTransitionEventConstructorTable, jsCast<JSPageTransitionEventConstructor*>(cell), propertyName, slot);
}

bool JSPageTransitionEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSPageTransitionEventConstructor, JSDOMWrapper>(exec, &JSPageTransitionEventConstructorTable, jsCast<JSPageTransitionEventConstructor*>(object), propertyName, descriptor);
}

EncodedJSValue JSC_HOST_CALL JSPageTransitionEventConstructor::constructJSPageTransitionEvent(ExecState* exec)
{
    JSPageTransitionEventConstructor* jsConstructor = jsCast<JSPageTransitionEventConstructor*>(exec->callee());

    ScriptExecutionContext* executionContext = jsConstructor->scriptExecutionContext();
    if (!executionContext)
        return throwVMError(exec, createReferenceError(exec, "Constructor associated execution context is unavailable"));

    AtomicString eventType = exec->argument(0).toString(exec)->value(exec);
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    PageTransitionEventInit eventInit;

    JSValue initializerValue = exec->argument(1);
    if (!initializerValue.isUndefinedOrNull()) {
        // Given the above test, this will always yield an object.
        JSObject* initializerObject = initializerValue.toObject(exec);

        // Create the dictionary wrapper from the initializer object.
        JSDictionary dictionary(exec, initializerObject);

        // Attempt to fill in the EventInit.
        if (!fillPageTransitionEventInit(eventInit, dictionary))
            return JSValue::encode(jsUndefined());
    }

    RefPtr<PageTransitionEvent> event = PageTransitionEvent::create(eventType, eventInit);
    return JSValue::encode(toJS(exec, jsConstructor->globalObject(), event.get()));
}

bool fillPageTransitionEventInit(PageTransitionEventInit& eventInit, JSDictionary& dictionary)
{
    if (!fillEventInit(eventInit, dictionary))
        return false;

    if (!dictionary.tryGetProperty("persisted", eventInit.persisted))
        return false;
    return true;
}

ConstructType JSPageTransitionEventConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSPageTransitionEvent;
    return ConstructTypeHost;
}

/* Hash table for prototype */

static const HashTableValue JSPageTransitionEventPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSPageTransitionEventPrototypeTable = { 1, 0, JSPageTransitionEventPrototypeTableValues, 0 };
const ClassInfo JSPageTransitionEventPrototype::s_info = { "PageTransitionEventPrototype", &Base::s_info, &JSPageTransitionEventPrototypeTable, 0, CREATE_METHOD_TABLE(JSPageTransitionEventPrototype) };

JSObject* JSPageTransitionEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSPageTransitionEvent>(exec, globalObject);
}

const ClassInfo JSPageTransitionEvent::s_info = { "PageTransitionEvent", &Base::s_info, &JSPageTransitionEventTable, 0 , CREATE_METHOD_TABLE(JSPageTransitionEvent) };

JSPageTransitionEvent::JSPageTransitionEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<PageTransitionEvent> impl)
    : JSEvent(structure, globalObject, impl)
{
}

void JSPageTransitionEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSPageTransitionEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSPageTransitionEventPrototype::create(exec->globalData(), globalObject, JSPageTransitionEventPrototype::createStructure(exec->globalData(), globalObject, JSEventPrototype::self(exec, globalObject)));
}

bool JSPageTransitionEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSPageTransitionEvent* thisObject = jsCast<JSPageTransitionEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSPageTransitionEvent, Base>(exec, &JSPageTransitionEventTable, thisObject, propertyName, slot);
}

bool JSPageTransitionEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSPageTransitionEvent* thisObject = jsCast<JSPageTransitionEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSPageTransitionEvent, Base>(exec, &JSPageTransitionEventTable, thisObject, propertyName, descriptor);
}

JSValue jsPageTransitionEventPersisted(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSPageTransitionEvent* castedThis = jsCast<JSPageTransitionEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    PageTransitionEvent* impl = static_cast<PageTransitionEvent*>(castedThis->impl());
    JSValue result = jsBoolean(impl->persisted());
    return result;
}


JSValue jsPageTransitionEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSPageTransitionEvent* domObject = jsCast<JSPageTransitionEvent*>(asObject(slotBase));
    return JSPageTransitionEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSPageTransitionEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSPageTransitionEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}
