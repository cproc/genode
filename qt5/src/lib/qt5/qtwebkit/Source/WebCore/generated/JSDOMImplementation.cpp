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
#include "JSDOMImplementation.h"

#include "CSSStyleSheet.h"
#include "DOMImplementation.h"
#include "Document.h"
#include "DocumentType.h"
#include "ExceptionCode.h"
#include "HTMLDocument.h"
#include "JSCSSStyleSheet.h"
#include "JSDOMBinding.h"
#include "JSDocument.h"
#include "JSDocumentType.h"
#include "JSHTMLDocument.h"
#include "NodeFilter.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSDOMImplementationTableValues[] =
{
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsDOMImplementationConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSDOMImplementationTable = { 2, 1, JSDOMImplementationTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSDOMImplementationConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSDOMImplementationConstructorTable = { 1, 0, JSDOMImplementationConstructorTableValues, 0 };
const ClassInfo JSDOMImplementationConstructor::s_info = { "DOMImplementationConstructor", &Base::s_info, &JSDOMImplementationConstructorTable, 0, CREATE_METHOD_TABLE(JSDOMImplementationConstructor) };

JSDOMImplementationConstructor::JSDOMImplementationConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSDOMImplementationConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSDOMImplementationPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSDOMImplementationConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSDOMImplementationConstructor, JSDOMWrapper>(exec, &JSDOMImplementationConstructorTable, jsCast<JSDOMImplementationConstructor*>(cell), propertyName, slot);
}

bool JSDOMImplementationConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSDOMImplementationConstructor, JSDOMWrapper>(exec, &JSDOMImplementationConstructorTable, jsCast<JSDOMImplementationConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSDOMImplementationPrototypeTableValues[] =
{
    { "hasFeature", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMImplementationPrototypeFunctionHasFeature), (intptr_t)2, NoIntrinsic },
    { "createDocumentType", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMImplementationPrototypeFunctionCreateDocumentType), (intptr_t)3, NoIntrinsic },
    { "createDocument", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMImplementationPrototypeFunctionCreateDocument), (intptr_t)3, NoIntrinsic },
    { "createCSSStyleSheet", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMImplementationPrototypeFunctionCreateCSSStyleSheet), (intptr_t)2, NoIntrinsic },
    { "createHTMLDocument", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsDOMImplementationPrototypeFunctionCreateHTMLDocument), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSDOMImplementationPrototypeTable = { 17, 15, JSDOMImplementationPrototypeTableValues, 0 };
const ClassInfo JSDOMImplementationPrototype::s_info = { "DOMImplementationPrototype", &Base::s_info, &JSDOMImplementationPrototypeTable, 0, CREATE_METHOD_TABLE(JSDOMImplementationPrototype) };

JSObject* JSDOMImplementationPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSDOMImplementation>(exec, globalObject);
}

bool JSDOMImplementationPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSDOMImplementationPrototype* thisObject = jsCast<JSDOMImplementationPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSDOMImplementationPrototypeTable, thisObject, propertyName, slot);
}

bool JSDOMImplementationPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSDOMImplementationPrototype* thisObject = jsCast<JSDOMImplementationPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSDOMImplementationPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSDOMImplementation::s_info = { "DOMImplementation", &Base::s_info, &JSDOMImplementationTable, 0 , CREATE_METHOD_TABLE(JSDOMImplementation) };

JSDOMImplementation::JSDOMImplementation(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<DOMImplementation> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSDOMImplementation::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSDOMImplementation::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSDOMImplementationPrototype::create(exec->globalData(), globalObject, JSDOMImplementationPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSDOMImplementation::destroy(JSC::JSCell* cell)
{
    JSDOMImplementation* thisObject = static_cast<JSDOMImplementation*>(cell);
    thisObject->JSDOMImplementation::~JSDOMImplementation();
}

JSDOMImplementation::~JSDOMImplementation()
{
    releaseImplIfNotNull();
}

bool JSDOMImplementation::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSDOMImplementation* thisObject = jsCast<JSDOMImplementation*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSDOMImplementation, Base>(exec, &JSDOMImplementationTable, thisObject, propertyName, slot);
}

bool JSDOMImplementation::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSDOMImplementation* thisObject = jsCast<JSDOMImplementation*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSDOMImplementation, Base>(exec, &JSDOMImplementationTable, thisObject, propertyName, descriptor);
}

JSValue jsDOMImplementationConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSDOMImplementation* domObject = jsCast<JSDOMImplementation*>(asObject(slotBase));
    return JSDOMImplementation::getConstructor(exec, domObject->globalObject());
}

JSValue JSDOMImplementation::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSDOMImplementationConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsDOMImplementationPrototypeFunctionHasFeature(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMImplementation::s_info))
        return throwVMTypeError(exec);
    JSDOMImplementation* castedThis = jsCast<JSDOMImplementation*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMImplementation::s_info);
    DOMImplementation* impl = static_cast<DOMImplementation*>(castedThis->impl());
    const String& feature(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& version(valueToStringWithNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsBoolean(impl->hasFeature(feature, version));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsDOMImplementationPrototypeFunctionCreateDocumentType(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMImplementation::s_info))
        return throwVMTypeError(exec);
    JSDOMImplementation* castedThis = jsCast<JSDOMImplementation*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMImplementation::s_info);
    DOMImplementation* impl = static_cast<DOMImplementation*>(castedThis->impl());
    ExceptionCode ec = 0;
    const String& qualifiedName(valueToStringWithUndefinedOrNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& publicId(valueToStringWithUndefinedOrNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& systemId(valueToStringWithUndefinedOrNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->createDocumentType(qualifiedName, publicId, systemId, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsDOMImplementationPrototypeFunctionCreateDocument(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMImplementation::s_info))
        return throwVMTypeError(exec);
    JSDOMImplementation* castedThis = jsCast<JSDOMImplementation*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMImplementation::s_info);
    DOMImplementation* impl = static_cast<DOMImplementation*>(castedThis->impl());
    ExceptionCode ec = 0;
    const String& namespaceURI(valueToStringWithNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& qualifiedName(valueToStringWithNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    DocumentType* doctype(toDocumentType(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->createDocument(namespaceURI, qualifiedName, doctype, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsDOMImplementationPrototypeFunctionCreateCSSStyleSheet(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMImplementation::s_info))
        return throwVMTypeError(exec);
    JSDOMImplementation* castedThis = jsCast<JSDOMImplementation*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMImplementation::s_info);
    DOMImplementation* impl = static_cast<DOMImplementation*>(castedThis->impl());
    ExceptionCode ec = 0;
    const String& title(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& media(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->createCSSStyleSheet(title, media, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsDOMImplementationPrototypeFunctionCreateHTMLDocument(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSDOMImplementation::s_info))
        return throwVMTypeError(exec);
    JSDOMImplementation* castedThis = jsCast<JSDOMImplementation*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSDOMImplementation::s_info);
    DOMImplementation* impl = static_cast<DOMImplementation*>(castedThis->impl());
    const String& title(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsNullString).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsNullString).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->createHTMLDocument(title)));
    return JSValue::encode(result);
}

static inline bool isObservable(JSDOMImplementation* jsDOMImplementation)
{
    if (jsDOMImplementation->hasCustomProperties())
        return true;
    return false;
}

bool JSDOMImplementationOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSDOMImplementation* jsDOMImplementation = jsCast<JSDOMImplementation*>(handle.get().asCell());
    if (!isObservable(jsDOMImplementation))
        return false;
    Document* root = jsDOMImplementation->impl()->document();
    if (!root)
        return false;
    return visitor.containsOpaqueRoot(root);
}

void JSDOMImplementationOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSDOMImplementation* jsDOMImplementation = jsCast<JSDOMImplementation*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsDOMImplementation->impl(), jsDOMImplementation);
    jsDOMImplementation->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, DOMImplementation* impl)
{
    return wrap<JSDOMImplementation>(exec, globalObject, impl);
}

DOMImplementation* toDOMImplementation(JSC::JSValue value)
{
    return value.inherits(&JSDOMImplementation::s_info) ? jsCast<JSDOMImplementation*>(asObject(value))->impl() : 0;
}

}
