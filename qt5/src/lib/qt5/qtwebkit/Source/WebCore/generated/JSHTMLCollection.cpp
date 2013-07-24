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
#include "JSHTMLCollection.h"

#include "ExceptionCode.h"
#include "HTMLCollection.h"
#include "JSDOMBinding.h"
#include "JSNode.h"
#include "Node.h"
#include "wtf/text/AtomicString.h"
#include <runtime/Error.h>
#include <runtime/PropertyNameArray.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLCollectionTableValues[] =
{
    { "length", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLCollectionLength), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLCollectionConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLCollectionTable = { 5, 3, JSHTMLCollectionTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLCollectionConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLCollectionConstructorTable = { 1, 0, JSHTMLCollectionConstructorTableValues, 0 };
const ClassInfo JSHTMLCollectionConstructor::s_info = { "HTMLCollectionConstructor", &Base::s_info, &JSHTMLCollectionConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLCollectionConstructor) };

JSHTMLCollectionConstructor::JSHTMLCollectionConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLCollectionConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLCollectionPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLCollectionConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLCollectionConstructor, JSDOMWrapper>(exec, &JSHTMLCollectionConstructorTable, jsCast<JSHTMLCollectionConstructor*>(cell), propertyName, slot);
}

bool JSHTMLCollectionConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLCollectionConstructor, JSDOMWrapper>(exec, &JSHTMLCollectionConstructorTable, jsCast<JSHTMLCollectionConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLCollectionPrototypeTableValues[] =
{
    { "item", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsHTMLCollectionPrototypeFunctionItem), (intptr_t)1, NoIntrinsic },
    { "namedItem", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsHTMLCollectionPrototypeFunctionNamedItem), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLCollectionPrototypeTable = { 5, 3, JSHTMLCollectionPrototypeTableValues, 0 };
const ClassInfo JSHTMLCollectionPrototype::s_info = { "HTMLCollectionPrototype", &Base::s_info, &JSHTMLCollectionPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLCollectionPrototype) };

JSObject* JSHTMLCollectionPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLCollection>(exec, globalObject);
}

bool JSHTMLCollectionPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLCollectionPrototype* thisObject = jsCast<JSHTMLCollectionPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSHTMLCollectionPrototypeTable, thisObject, propertyName, slot);
}

bool JSHTMLCollectionPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLCollectionPrototype* thisObject = jsCast<JSHTMLCollectionPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSHTMLCollectionPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSHTMLCollection::s_info = { "HTMLCollection", &Base::s_info, &JSHTMLCollectionTable, 0 , CREATE_METHOD_TABLE(JSHTMLCollection) };

JSHTMLCollection::JSHTMLCollection(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLCollection> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSHTMLCollection::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLCollection::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLCollectionPrototype::create(exec->globalData(), globalObject, JSHTMLCollectionPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSHTMLCollection::destroy(JSC::JSCell* cell)
{
    JSHTMLCollection* thisObject = static_cast<JSHTMLCollection*>(cell);
    thisObject->JSHTMLCollection::~JSHTMLCollection();
}

JSHTMLCollection::~JSHTMLCollection()
{
    releaseImplIfNotNull();
}

bool JSHTMLCollection::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLCollection* thisObject = jsCast<JSHTMLCollection*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    JSValue proto = thisObject->prototype();
    if (proto.isObject() && jsCast<JSObject*>(asObject(proto))->hasProperty(exec, propertyName))
        return false;

    const HashEntry* entry = getStaticValueSlotEntryWithoutCaching<JSHTMLCollection>(exec, propertyName);
    if (entry) {
        slot.setCustom(thisObject, entry->propertyGetter());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<HTMLCollection*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, indexGetter);
        return true;
    }
    if (canGetItemsForName(exec, static_cast<HTMLCollection*>(thisObject->impl()), propertyName)) {
        slot.setCustom(thisObject, thisObject->nameGetter);
        return true;
    }
    return getStaticValueSlot<JSHTMLCollection, Base>(exec, &JSHTMLCollectionTable, thisObject, propertyName, slot);
}

bool JSHTMLCollection::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLCollection* thisObject = jsCast<JSHTMLCollection*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    JSValue proto = thisObject->prototype();
    if (proto.isObject() && jsCast<JSObject*>(asObject(proto))->hasProperty(exec, propertyName))
        return false;

    const HashEntry* entry = JSHTMLCollectionTable.entry(exec, propertyName);
    if (entry) {
        PropertySlot slot;
        slot.setCustom(thisObject, entry->propertyGetter());
        descriptor.setDescriptor(slot.getValue(exec, propertyName), entry->attributes());
        return true;
    }
    unsigned index = propertyName.asIndex();
    if (index != PropertyName::NotAnIndex && index < static_cast<HTMLCollection*>(thisObject->impl())->length()) {
        PropertySlot slot;
        slot.setCustomIndex(thisObject, index, indexGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), DontDelete | ReadOnly);
        return true;
    }
    if (canGetItemsForName(exec, static_cast<HTMLCollection*>(thisObject->impl()), propertyName)) {
        PropertySlot slot;
        slot.setCustom(thisObject, nameGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), ReadOnly | DontDelete | DontEnum);
        return true;
    }
    return getStaticValueDescriptor<JSHTMLCollection, Base>(exec, &JSHTMLCollectionTable, thisObject, propertyName, descriptor);
}

bool JSHTMLCollection::getOwnPropertySlotByIndex(JSCell* cell, ExecState* exec, unsigned index, PropertySlot& slot)
{
    JSHTMLCollection* thisObject = jsCast<JSHTMLCollection*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (index < static_cast<HTMLCollection*>(thisObject->impl())->length()) {
        slot.setCustomIndex(thisObject, index, thisObject->indexGetter);
        return true;
    }
    PropertyName propertyName = Identifier::from(exec, index);
    if (canGetItemsForName(exec, static_cast<HTMLCollection*>(thisObject->impl()), propertyName)) {
        slot.setCustom(thisObject, thisObject->nameGetter);
        return true;
    }
    return Base::getOwnPropertySlotByIndex(thisObject, exec, index, slot);
}

JSValue jsHTMLCollectionLength(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLCollection* castedThis = jsCast<JSHTMLCollection*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLCollection* impl = static_cast<HTMLCollection*>(castedThis->impl());
    JSValue result = jsNumber(impl->length());
    return result;
}


JSValue jsHTMLCollectionConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLCollection* domObject = jsCast<JSHTMLCollection*>(asObject(slotBase));
    return JSHTMLCollection::getConstructor(exec, domObject->globalObject());
}

void JSHTMLCollection::getOwnPropertyNames(JSObject* object, ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    JSHTMLCollection* thisObject = jsCast<JSHTMLCollection*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    for (unsigned i = 0; i < static_cast<HTMLCollection*>(thisObject->impl())->length(); ++i)
        propertyNames.add(Identifier::from(exec, i));
     Base::getOwnPropertyNames(thisObject, exec, propertyNames, mode);
}

JSValue JSHTMLCollection::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLCollectionConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsHTMLCollectionPrototypeFunctionItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSHTMLCollection::s_info))
        return throwVMTypeError(exec);
    JSHTMLCollection* castedThis = jsCast<JSHTMLCollection*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSHTMLCollection::s_info);
    HTMLCollection* impl = static_cast<HTMLCollection*>(castedThis->impl());
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->item(index)));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsHTMLCollectionPrototypeFunctionNamedItem(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSHTMLCollection::s_info))
        return throwVMTypeError(exec);
    JSHTMLCollection* castedThis = jsCast<JSHTMLCollection*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSHTMLCollection::s_info);
    HTMLCollection* impl = static_cast<HTMLCollection*>(castedThis->impl());
    const String& name(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->namedItem(name)));
    return JSValue::encode(result);
}


JSValue JSHTMLCollection::indexGetter(ExecState* exec, JSValue slotBase, unsigned index)
{
    JSHTMLCollection* thisObj = jsCast<JSHTMLCollection*>(asObject(slotBase));
    ASSERT_GC_OBJECT_INHERITS(thisObj, &s_info);
    return toJS(exec, thisObj->globalObject(), static_cast<HTMLCollection*>(thisObj->impl())->item(index));
}

static inline bool isObservable(JSHTMLCollection* jsHTMLCollection)
{
    if (jsHTMLCollection->hasCustomProperties())
        return true;
    return false;
}

bool JSHTMLCollectionOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSHTMLCollection* jsHTMLCollection = jsCast<JSHTMLCollection*>(handle.get().asCell());
    if (!isObservable(jsHTMLCollection))
        return false;
    void* root = WebCore::root(jsHTMLCollection->impl()->base());
    return visitor.containsOpaqueRoot(root);
}

void JSHTMLCollectionOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSHTMLCollection* jsHTMLCollection = jsCast<JSHTMLCollection*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsHTMLCollection->impl(), jsHTMLCollection);
    jsHTMLCollection->releaseImpl();
}

HTMLCollection* toHTMLCollection(JSC::JSValue value)
{
    return value.inherits(&JSHTMLCollection::s_info) ? jsCast<JSHTMLCollection*>(asObject(value))->impl() : 0;
}

}
