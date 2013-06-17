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
#include "JSUIEvent.h"

#include "DOMWindow.h"
#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSDOMWindow.h"
#include "UIEvent.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSUIEventTableValues[] =
{
    { "view", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventView), (intptr_t)0, NoIntrinsic },
    { "detail", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventDetail), (intptr_t)0, NoIntrinsic },
    { "keyCode", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventKeyCode), (intptr_t)0, NoIntrinsic },
    { "charCode", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventCharCode), (intptr_t)0, NoIntrinsic },
    { "layerX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventLayerX), (intptr_t)0, NoIntrinsic },
    { "layerY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventLayerY), (intptr_t)0, NoIntrinsic },
    { "pageX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventPageX), (intptr_t)0, NoIntrinsic },
    { "pageY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventPageY), (intptr_t)0, NoIntrinsic },
    { "which", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventWhich), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSUIEventTable = { 34, 31, JSUIEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSUIEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSUIEventConstructorTable = { 1, 0, JSUIEventConstructorTableValues, 0 };
const ClassInfo JSUIEventConstructor::s_info = { "UIEventConstructor", &Base::s_info, &JSUIEventConstructorTable, 0, CREATE_METHOD_TABLE(JSUIEventConstructor) };

JSUIEventConstructor::JSUIEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSUIEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSUIEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSUIEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSUIEventConstructor, JSDOMWrapper>(exec, &JSUIEventConstructorTable, jsCast<JSUIEventConstructor*>(cell), propertyName, slot);
}

bool JSUIEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSUIEventConstructor, JSDOMWrapper>(exec, &JSUIEventConstructorTable, jsCast<JSUIEventConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSUIEventPrototypeTableValues[] =
{
    { "initUIEvent", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsUIEventPrototypeFunctionInitUIEvent), (intptr_t)5, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSUIEventPrototypeTable = { 2, 1, JSUIEventPrototypeTableValues, 0 };
const ClassInfo JSUIEventPrototype::s_info = { "UIEventPrototype", &Base::s_info, &JSUIEventPrototypeTable, 0, CREATE_METHOD_TABLE(JSUIEventPrototype) };

JSObject* JSUIEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSUIEvent>(exec, globalObject);
}

bool JSUIEventPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSUIEventPrototype* thisObject = jsCast<JSUIEventPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSUIEventPrototypeTable, thisObject, propertyName, slot);
}

bool JSUIEventPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSUIEventPrototype* thisObject = jsCast<JSUIEventPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSUIEventPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSUIEvent::s_info = { "UIEvent", &Base::s_info, &JSUIEventTable, 0 , CREATE_METHOD_TABLE(JSUIEvent) };

JSUIEvent::JSUIEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<UIEvent> impl)
    : JSEvent(structure, globalObject, impl)
{
}

void JSUIEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSUIEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSUIEventPrototype::create(exec->globalData(), globalObject, JSUIEventPrototype::createStructure(exec->globalData(), globalObject, JSEventPrototype::self(exec, globalObject)));
}

bool JSUIEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSUIEvent* thisObject = jsCast<JSUIEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSUIEvent, Base>(exec, &JSUIEventTable, thisObject, propertyName, slot);
}

bool JSUIEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSUIEvent* thisObject = jsCast<JSUIEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSUIEvent, Base>(exec, &JSUIEventTable, thisObject, propertyName, descriptor);
}

JSValue jsUIEventView(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->view()));
    return result;
}


JSValue jsUIEventDetail(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->detail());
    return result;
}


JSValue jsUIEventKeyCode(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->keyCode());
    return result;
}


JSValue jsUIEventCharCode(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->charCode());
    return result;
}


JSValue jsUIEventLayerX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->layerX());
    return result;
}


JSValue jsUIEventLayerY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->layerY());
    return result;
}


JSValue jsUIEventPageX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->pageX());
    return result;
}


JSValue jsUIEventPageY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->pageY());
    return result;
}


JSValue jsUIEventWhich(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->which());
    return result;
}


JSValue jsUIEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSUIEvent* domObject = jsCast<JSUIEvent*>(asObject(slotBase));
    return JSUIEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSUIEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSUIEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsUIEventPrototypeFunctionInitUIEvent(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSUIEvent::s_info))
        return throwVMTypeError(exec);
    JSUIEvent* castedThis = jsCast<JSUIEvent*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSUIEvent::s_info);
    UIEvent* impl = static_cast<UIEvent*>(castedThis->impl());
    const String& type(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    bool canBubble(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toBoolean(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    bool cancelable(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined).toBoolean(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    DOMWindow* view(toDOMWindow(MAYBE_MISSING_PARAMETER(exec, 3, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    int detail(MAYBE_MISSING_PARAMETER(exec, 4, DefaultIsUndefined).toInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->initUIEvent(type, canBubble, cancelable, view, detail);
    return JSValue::encode(jsUndefined());
}


}
