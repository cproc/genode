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
#include "JSWebKitCSSTransformValue.h"

#include "WebKitCSSTransformValue.h"
#include <runtime/PropertyNameArray.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSWebKitCSSTransformValueTableValues[] =
{
    { "operationType", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueOperationType), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSTransformValueTable = { 5, 3, JSWebKitCSSTransformValueTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSWebKitCSSTransformValueConstructorTableValues[] =
{
    { "CSS_TRANSLATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATE), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEX), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEY), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATE), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALE), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEX), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEY), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEW", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEW), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEWX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEWX), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEWY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEWY), (intptr_t)0, NoIntrinsic },
    { "CSS_MATRIX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_MATRIX), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEX), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEY), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_PERSPECTIVE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_PERSPECTIVE), (intptr_t)0, NoIntrinsic },
    { "CSS_MATRIX3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_MATRIX3D), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSTransformValueConstructorTable = { 68, 63, JSWebKitCSSTransformValueConstructorTableValues, 0 };
const ClassInfo JSWebKitCSSTransformValueConstructor::s_info = { "WebKitCSSTransformValueConstructor", &Base::s_info, &JSWebKitCSSTransformValueConstructorTable, 0, CREATE_METHOD_TABLE(JSWebKitCSSTransformValueConstructor) };

JSWebKitCSSTransformValueConstructor::JSWebKitCSSTransformValueConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSWebKitCSSTransformValueConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSWebKitCSSTransformValuePrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSWebKitCSSTransformValueConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSWebKitCSSTransformValueConstructor, JSDOMWrapper>(exec, &JSWebKitCSSTransformValueConstructorTable, jsCast<JSWebKitCSSTransformValueConstructor*>(cell), propertyName, slot);
}

bool JSWebKitCSSTransformValueConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSWebKitCSSTransformValueConstructor, JSDOMWrapper>(exec, &JSWebKitCSSTransformValueConstructorTable, jsCast<JSWebKitCSSTransformValueConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSWebKitCSSTransformValuePrototypeTableValues[] =
{
    { "CSS_TRANSLATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATE), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEX), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEY), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATE), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALE), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEX), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEY), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEW", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEW), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEWX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEWX), (intptr_t)0, NoIntrinsic },
    { "CSS_SKEWY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SKEWY), (intptr_t)0, NoIntrinsic },
    { "CSS_MATRIX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_MATRIX), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_TRANSLATE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_TRANSLATE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEX), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEY), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_ROTATE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_ROTATE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALEZ", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALEZ), (intptr_t)0, NoIntrinsic },
    { "CSS_SCALE3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_SCALE3D), (intptr_t)0, NoIntrinsic },
    { "CSS_PERSPECTIVE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_PERSPECTIVE), (intptr_t)0, NoIntrinsic },
    { "CSS_MATRIX3D", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSTransformValueCSS_MATRIX3D), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSTransformValuePrototypeTable = { 68, 63, JSWebKitCSSTransformValuePrototypeTableValues, 0 };
const ClassInfo JSWebKitCSSTransformValuePrototype::s_info = { "WebKitCSSTransformValuePrototype", &Base::s_info, &JSWebKitCSSTransformValuePrototypeTable, 0, CREATE_METHOD_TABLE(JSWebKitCSSTransformValuePrototype) };

JSObject* JSWebKitCSSTransformValuePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSWebKitCSSTransformValue>(exec, globalObject);
}

bool JSWebKitCSSTransformValuePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWebKitCSSTransformValuePrototype* thisObject = jsCast<JSWebKitCSSTransformValuePrototype*>(cell);
    return getStaticValueSlot<JSWebKitCSSTransformValuePrototype, JSObject>(exec, &JSWebKitCSSTransformValuePrototypeTable, thisObject, propertyName, slot);
}

bool JSWebKitCSSTransformValuePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWebKitCSSTransformValuePrototype* thisObject = jsCast<JSWebKitCSSTransformValuePrototype*>(object);
    return getStaticValueDescriptor<JSWebKitCSSTransformValuePrototype, JSObject>(exec, &JSWebKitCSSTransformValuePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSWebKitCSSTransformValue::s_info = { "WebKitCSSTransformValue", &Base::s_info, &JSWebKitCSSTransformValueTable, 0 , CREATE_METHOD_TABLE(JSWebKitCSSTransformValue) };

JSWebKitCSSTransformValue::JSWebKitCSSTransformValue(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<WebKitCSSTransformValue> impl)
    : JSCSSValueList(structure, globalObject, impl)
{
}

void JSWebKitCSSTransformValue::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSWebKitCSSTransformValue::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSWebKitCSSTransformValuePrototype::create(exec->globalData(), globalObject, JSWebKitCSSTransformValuePrototype::createStructure(exec->globalData(), globalObject, JSCSSValueListPrototype::self(exec, globalObject)));
}

bool JSWebKitCSSTransformValue::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWebKitCSSTransformValue* thisObject = jsCast<JSWebKitCSSTransformValue*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    const HashEntry* entry = getStaticValueSlotEntryWithoutCaching<JSWebKitCSSTransformValue>(exec, propertyName);
    if (entry) {
        slot.setCustom(thisObject, entry->propertyGetter());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<WebKitCSSTransformValue*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, indexGetter);
        return true;
    }
    return getStaticValueSlot<JSWebKitCSSTransformValue, Base>(exec, &JSWebKitCSSTransformValueTable, thisObject, propertyName, slot);
}

bool JSWebKitCSSTransformValue::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWebKitCSSTransformValue* thisObject = jsCast<JSWebKitCSSTransformValue*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    const HashEntry* entry = JSWebKitCSSTransformValueTable.entry(exec, propertyName);
    if (entry) {
        PropertySlot slot;
        slot.setCustom(thisObject, entry->propertyGetter());
        descriptor.setDescriptor(slot.getValue(exec, propertyName), entry->attributes());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<WebKitCSSTransformValue*>(thisObject->impl())->length()) {
        PropertySlot slot;
        slot.setCustomIndex(thisObject, index, indexGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), DontDelete | ReadOnly);
        return true;
    }
    return getStaticValueDescriptor<JSWebKitCSSTransformValue, Base>(exec, &JSWebKitCSSTransformValueTable, thisObject, propertyName, descriptor);
}

bool JSWebKitCSSTransformValue::getOwnPropertySlotByIndex(JSCell* cell, ExecState* exec, unsigned index, PropertySlot& slot)
{
    JSWebKitCSSTransformValue* thisObject = jsCast<JSWebKitCSSTransformValue*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (index < static_cast<WebKitCSSTransformValue*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, thisObject->indexGetter);
        return true;
    }
    return Base::getOwnPropertySlotByIndex(thisObject, exec, index, slot);
}

JSValue jsWebKitCSSTransformValueOperationType(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWebKitCSSTransformValue* castedThis = jsCast<JSWebKitCSSTransformValue*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    WebKitCSSTransformValue* impl = static_cast<WebKitCSSTransformValue*>(castedThis->impl());
    JSValue result = jsNumber(impl->operationType());
    return result;
}


JSValue jsWebKitCSSTransformValueConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWebKitCSSTransformValue* domObject = jsCast<JSWebKitCSSTransformValue*>(asObject(slotBase));
    return JSWebKitCSSTransformValue::getConstructor(exec, domObject->globalObject());
}

void JSWebKitCSSTransformValue::getOwnPropertyNames(JSObject* object, ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    JSWebKitCSSTransformValue* thisObject = jsCast<JSWebKitCSSTransformValue*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    for (unsigned i = 0; i < static_cast<WebKitCSSTransformValue*>(thisObject->impl())->length(); ++i)
        propertyNames.add(Identifier::from(exec, i));
     Base::getOwnPropertyNames(thisObject, exec, propertyNames, mode);
}

JSValue JSWebKitCSSTransformValue::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSWebKitCSSTransformValueConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

// Constant getters

JSValue jsWebKitCSSTransformValueCSS_TRANSLATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsWebKitCSSTransformValueCSS_TRANSLATEX(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsWebKitCSSTransformValueCSS_TRANSLATEY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSValue jsWebKitCSSTransformValueCSS_ROTATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(4));
}

JSValue jsWebKitCSSTransformValueCSS_SCALE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(5));
}

JSValue jsWebKitCSSTransformValueCSS_SCALEX(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(6));
}

JSValue jsWebKitCSSTransformValueCSS_SCALEY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(7));
}

JSValue jsWebKitCSSTransformValueCSS_SKEW(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(8));
}

JSValue jsWebKitCSSTransformValueCSS_SKEWX(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(9));
}

JSValue jsWebKitCSSTransformValueCSS_SKEWY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(10));
}

JSValue jsWebKitCSSTransformValueCSS_MATRIX(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(11));
}

JSValue jsWebKitCSSTransformValueCSS_TRANSLATEZ(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(12));
}

JSValue jsWebKitCSSTransformValueCSS_TRANSLATE3D(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(13));
}

JSValue jsWebKitCSSTransformValueCSS_ROTATEX(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(14));
}

JSValue jsWebKitCSSTransformValueCSS_ROTATEY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(15));
}

JSValue jsWebKitCSSTransformValueCSS_ROTATEZ(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(16));
}

JSValue jsWebKitCSSTransformValueCSS_ROTATE3D(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(17));
}

JSValue jsWebKitCSSTransformValueCSS_SCALEZ(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(18));
}

JSValue jsWebKitCSSTransformValueCSS_SCALE3D(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(19));
}

JSValue jsWebKitCSSTransformValueCSS_PERSPECTIVE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(20));
}

JSValue jsWebKitCSSTransformValueCSS_MATRIX3D(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(21));
}


JSValue JSWebKitCSSTransformValue::indexGetter(ExecState* exec, JSValue slotBase, unsigned index)
{
    JSWebKitCSSTransformValue* thisObj = jsCast<JSWebKitCSSTransformValue*>(asObject(slotBase));
    ASSERT_GC_OBJECT_INHERITS(thisObj, &s_info);
    return toJS(exec, thisObj->globalObject(), static_cast<WebKitCSSTransformValue*>(thisObj->impl())->item(index));
}


}
