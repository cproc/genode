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

#if ENABLE(CSS_FILTERS)

#include "JSWebKitCSSFilterValue.h"

#include "WebKitCSSFilterValue.h"
#include <runtime/PropertyNameArray.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSWebKitCSSFilterValueTableValues[] =
{
    { "operationType", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueOperationType), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSFilterValueTable = { 5, 3, JSWebKitCSSFilterValueTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSWebKitCSSFilterValueConstructorTableValues[] =
{
    { "CSS_FILTER_REFERENCE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_REFERENCE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_GRAYSCALE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_GRAYSCALE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_SEPIA", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_SEPIA), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_SATURATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_SATURATE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_HUE_ROTATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_HUE_ROTATE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_INVERT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_INVERT), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_OPACITY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_OPACITY), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_BRIGHTNESS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_BRIGHTNESS), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_CONTRAST", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_CONTRAST), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_BLUR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_BLUR), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_DROP_SHADOW", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_DROP_SHADOW), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSFilterValueConstructorTable = { 33, 31, JSWebKitCSSFilterValueConstructorTableValues, 0 };
const ClassInfo JSWebKitCSSFilterValueConstructor::s_info = { "WebKitCSSFilterValueConstructor", &Base::s_info, &JSWebKitCSSFilterValueConstructorTable, 0, CREATE_METHOD_TABLE(JSWebKitCSSFilterValueConstructor) };

JSWebKitCSSFilterValueConstructor::JSWebKitCSSFilterValueConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSWebKitCSSFilterValueConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSWebKitCSSFilterValuePrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSWebKitCSSFilterValueConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSWebKitCSSFilterValueConstructor, JSDOMWrapper>(exec, &JSWebKitCSSFilterValueConstructorTable, jsCast<JSWebKitCSSFilterValueConstructor*>(cell), propertyName, slot);
}

bool JSWebKitCSSFilterValueConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSWebKitCSSFilterValueConstructor, JSDOMWrapper>(exec, &JSWebKitCSSFilterValueConstructorTable, jsCast<JSWebKitCSSFilterValueConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSWebKitCSSFilterValuePrototypeTableValues[] =
{
    { "CSS_FILTER_REFERENCE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_REFERENCE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_GRAYSCALE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_GRAYSCALE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_SEPIA", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_SEPIA), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_SATURATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_SATURATE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_HUE_ROTATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_HUE_ROTATE), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_INVERT", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_INVERT), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_OPACITY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_OPACITY), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_BRIGHTNESS", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_BRIGHTNESS), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_CONTRAST", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_CONTRAST), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_BLUR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_BLUR), (intptr_t)0, NoIntrinsic },
    { "CSS_FILTER_DROP_SHADOW", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWebKitCSSFilterValueCSS_FILTER_DROP_SHADOW), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWebKitCSSFilterValuePrototypeTable = { 33, 31, JSWebKitCSSFilterValuePrototypeTableValues, 0 };
const ClassInfo JSWebKitCSSFilterValuePrototype::s_info = { "WebKitCSSFilterValuePrototype", &Base::s_info, &JSWebKitCSSFilterValuePrototypeTable, 0, CREATE_METHOD_TABLE(JSWebKitCSSFilterValuePrototype) };

JSObject* JSWebKitCSSFilterValuePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSWebKitCSSFilterValue>(exec, globalObject);
}

bool JSWebKitCSSFilterValuePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWebKitCSSFilterValuePrototype* thisObject = jsCast<JSWebKitCSSFilterValuePrototype*>(cell);
    return getStaticValueSlot<JSWebKitCSSFilterValuePrototype, JSObject>(exec, &JSWebKitCSSFilterValuePrototypeTable, thisObject, propertyName, slot);
}

bool JSWebKitCSSFilterValuePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWebKitCSSFilterValuePrototype* thisObject = jsCast<JSWebKitCSSFilterValuePrototype*>(object);
    return getStaticValueDescriptor<JSWebKitCSSFilterValuePrototype, JSObject>(exec, &JSWebKitCSSFilterValuePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSWebKitCSSFilterValue::s_info = { "WebKitCSSFilterValue", &Base::s_info, &JSWebKitCSSFilterValueTable, 0 , CREATE_METHOD_TABLE(JSWebKitCSSFilterValue) };

JSWebKitCSSFilterValue::JSWebKitCSSFilterValue(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<WebKitCSSFilterValue> impl)
    : JSCSSValueList(structure, globalObject, impl)
{
}

void JSWebKitCSSFilterValue::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSWebKitCSSFilterValue::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSWebKitCSSFilterValuePrototype::create(exec->globalData(), globalObject, JSWebKitCSSFilterValuePrototype::createStructure(exec->globalData(), globalObject, JSCSSValueListPrototype::self(exec, globalObject)));
}

bool JSWebKitCSSFilterValue::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWebKitCSSFilterValue* thisObject = jsCast<JSWebKitCSSFilterValue*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    const HashEntry* entry = getStaticValueSlotEntryWithoutCaching<JSWebKitCSSFilterValue>(exec, propertyName);
    if (entry) {
        slot.setCustom(thisObject, entry->propertyGetter());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<WebKitCSSFilterValue*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, indexGetter);
        return true;
    }
    return getStaticValueSlot<JSWebKitCSSFilterValue, Base>(exec, &JSWebKitCSSFilterValueTable, thisObject, propertyName, slot);
}

bool JSWebKitCSSFilterValue::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWebKitCSSFilterValue* thisObject = jsCast<JSWebKitCSSFilterValue*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    const HashEntry* entry = JSWebKitCSSFilterValueTable.entry(exec, propertyName);
    if (entry) {
        PropertySlot slot;
        slot.setCustom(thisObject, entry->propertyGetter());
        descriptor.setDescriptor(slot.getValue(exec, propertyName), entry->attributes());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<WebKitCSSFilterValue*>(thisObject->impl())->length()) {
        PropertySlot slot;
        slot.setCustomIndex(thisObject, index, indexGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), DontDelete | ReadOnly);
        return true;
    }
    return getStaticValueDescriptor<JSWebKitCSSFilterValue, Base>(exec, &JSWebKitCSSFilterValueTable, thisObject, propertyName, descriptor);
}

bool JSWebKitCSSFilterValue::getOwnPropertySlotByIndex(JSCell* cell, ExecState* exec, unsigned index, PropertySlot& slot)
{
    JSWebKitCSSFilterValue* thisObject = jsCast<JSWebKitCSSFilterValue*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (index < static_cast<WebKitCSSFilterValue*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, thisObject->indexGetter);
        return true;
    }
    return Base::getOwnPropertySlotByIndex(thisObject, exec, index, slot);
}

JSValue jsWebKitCSSFilterValueOperationType(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWebKitCSSFilterValue* castedThis = jsCast<JSWebKitCSSFilterValue*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    WebKitCSSFilterValue* impl = static_cast<WebKitCSSFilterValue*>(castedThis->impl());
    JSValue result = jsNumber(impl->operationType());
    return result;
}


JSValue jsWebKitCSSFilterValueConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWebKitCSSFilterValue* domObject = jsCast<JSWebKitCSSFilterValue*>(asObject(slotBase));
    return JSWebKitCSSFilterValue::getConstructor(exec, domObject->globalObject());
}

void JSWebKitCSSFilterValue::getOwnPropertyNames(JSObject* object, ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    JSWebKitCSSFilterValue* thisObject = jsCast<JSWebKitCSSFilterValue*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    for (unsigned i = 0; i < static_cast<WebKitCSSFilterValue*>(thisObject->impl())->length(); ++i)
        propertyNames.add(Identifier::from(exec, i));
     Base::getOwnPropertyNames(thisObject, exec, propertyNames, mode);
}

JSValue JSWebKitCSSFilterValue::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSWebKitCSSFilterValueConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

// Constant getters

JSValue jsWebKitCSSFilterValueCSS_FILTER_REFERENCE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_GRAYSCALE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_SEPIA(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_SATURATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(4));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_HUE_ROTATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(5));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_INVERT(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(6));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_OPACITY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(7));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_BRIGHTNESS(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(8));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_CONTRAST(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(9));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_BLUR(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(10));
}

JSValue jsWebKitCSSFilterValueCSS_FILTER_DROP_SHADOW(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(11));
}


JSValue JSWebKitCSSFilterValue::indexGetter(ExecState* exec, JSValue slotBase, unsigned index)
{
    JSWebKitCSSFilterValue* thisObj = jsCast<JSWebKitCSSFilterValue*>(asObject(slotBase));
    ASSERT_GC_OBJECT_INHERITS(thisObj, &s_info);
    return toJS(exec, thisObj->globalObject(), static_cast<WebKitCSSFilterValue*>(thisObj->impl())->item(index));
}


}

#endif // ENABLE(CSS_FILTERS)
