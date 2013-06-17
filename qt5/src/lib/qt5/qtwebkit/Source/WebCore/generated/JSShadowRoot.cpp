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

#if ENABLE(SHADOW_DOM)

#include "JSShadowRoot.h"

#include "DOMSelection.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSDOMSelection.h"
#include "JSElement.h"
#include "JSNode.h"
#include "JSNodeList.h"
#include "KURL.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeList.h"
#include "ShadowRoot.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSShadowRootTableValues[] =
{
    { "activeElement", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsShadowRootActiveElement), (intptr_t)0, NoIntrinsic },
    { "applyAuthorStyles", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsShadowRootApplyAuthorStyles), (intptr_t)setJSShadowRootApplyAuthorStyles, NoIntrinsic },
    { "resetStyleInheritance", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsShadowRootResetStyleInheritance), (intptr_t)setJSShadowRootResetStyleInheritance, NoIntrinsic },
    { "innerHTML", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsShadowRootInnerHTML), (intptr_t)setJSShadowRootInnerHTML, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsShadowRootConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSShadowRootTable = { 16, 15, JSShadowRootTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSShadowRootConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSShadowRootConstructorTable = { 1, 0, JSShadowRootConstructorTableValues, 0 };
const ClassInfo JSShadowRootConstructor::s_info = { "ShadowRootConstructor", &Base::s_info, &JSShadowRootConstructorTable, 0, CREATE_METHOD_TABLE(JSShadowRootConstructor) };

JSShadowRootConstructor::JSShadowRootConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSShadowRootConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSShadowRootPrototype::self(exec, globalObject), DontDelete | ReadOnly);
    putDirect(exec->globalData(), exec->propertyNames().length, jsNumber(1), ReadOnly | DontDelete | DontEnum);
}

bool JSShadowRootConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSShadowRootConstructor, JSDOMWrapper>(exec, &JSShadowRootConstructorTable, jsCast<JSShadowRootConstructor*>(cell), propertyName, slot);
}

bool JSShadowRootConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSShadowRootConstructor, JSDOMWrapper>(exec, &JSShadowRootConstructorTable, jsCast<JSShadowRootConstructor*>(object), propertyName, descriptor);
}

EncodedJSValue JSC_HOST_CALL JSShadowRootConstructor::constructJSShadowRoot(ExecState* exec)
{
    JSShadowRootConstructor* castedThis = jsCast<JSShadowRootConstructor*>(exec->callee());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    Element* host(toElement(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    RefPtr<ShadowRoot> object = ShadowRoot::create(host, ec);
    if (ec) {
        setDOMException(exec, ec);
        return JSValue::encode(JSValue());
    }
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

ConstructType JSShadowRootConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSShadowRoot;
    return ConstructTypeHost;
}

/* Hash table for prototype */

static const HashTableValue JSShadowRootPrototypeTableValues[] =
{
    { "cloneNode", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionCloneNode), (intptr_t)1, NoIntrinsic },
    { "getSelection", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionGetSelection), (intptr_t)0, NoIntrinsic },
    { "getElementById", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionGetElementById), (intptr_t)1, NoIntrinsic },
    { "getElementsByClassName", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionGetElementsByClassName), (intptr_t)1, NoIntrinsic },
    { "getElementsByTagName", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionGetElementsByTagName), (intptr_t)1, NoIntrinsic },
    { "getElementsByTagNameNS", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsShadowRootPrototypeFunctionGetElementsByTagNameNS), (intptr_t)2, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSShadowRootPrototypeTable = { 17, 15, JSShadowRootPrototypeTableValues, 0 };
const ClassInfo JSShadowRootPrototype::s_info = { "ShadowRootPrototype", &Base::s_info, &JSShadowRootPrototypeTable, 0, CREATE_METHOD_TABLE(JSShadowRootPrototype) };

JSObject* JSShadowRootPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSShadowRoot>(exec, globalObject);
}

bool JSShadowRootPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSShadowRootPrototype* thisObject = jsCast<JSShadowRootPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSShadowRootPrototypeTable, thisObject, propertyName, slot);
}

bool JSShadowRootPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSShadowRootPrototype* thisObject = jsCast<JSShadowRootPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSShadowRootPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSShadowRoot::s_info = { "ShadowRoot", &Base::s_info, &JSShadowRootTable, 0 , CREATE_METHOD_TABLE(JSShadowRoot) };

JSShadowRoot::JSShadowRoot(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<ShadowRoot> impl)
    : JSDocumentFragment(structure, globalObject, impl)
{
}

void JSShadowRoot::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSShadowRoot::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSShadowRootPrototype::create(exec->globalData(), globalObject, JSShadowRootPrototype::createStructure(exec->globalData(), globalObject, JSDocumentFragmentPrototype::self(exec, globalObject)));
}

bool JSShadowRoot::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSShadowRoot* thisObject = jsCast<JSShadowRoot*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSShadowRoot, Base>(exec, &JSShadowRootTable, thisObject, propertyName, slot);
}

bool JSShadowRoot::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSShadowRoot* thisObject = jsCast<JSShadowRoot*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSShadowRoot, Base>(exec, &JSShadowRootTable, thisObject, propertyName, descriptor);
}

JSValue jsShadowRootActiveElement(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->activeElement()));
    return result;
}


JSValue jsShadowRootApplyAuthorStyles(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    JSValue result = jsBoolean(impl->applyAuthorStyles());
    return result;
}


JSValue jsShadowRootResetStyleInheritance(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    JSValue result = jsBoolean(impl->resetStyleInheritance());
    return result;
}


JSValue jsShadowRootInnerHTML(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->innerHTML());
    return result;
}


JSValue jsShadowRootConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSShadowRoot* domObject = jsCast<JSShadowRoot*>(asObject(slotBase));
    return JSShadowRoot::getConstructor(exec, domObject->globalObject());
}

void JSShadowRoot::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSShadowRoot* thisObject = jsCast<JSShadowRoot*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSShadowRoot, Base>(exec, propertyName, value, &JSShadowRootTable, thisObject, slot);
}

void setJSShadowRootApplyAuthorStyles(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(thisObject);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    impl->setApplyAuthorStyles(value.toBoolean(exec));
}


void setJSShadowRootResetStyleInheritance(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(thisObject);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    impl->setResetStyleInheritance(value.toBoolean(exec));
}


void setJSShadowRootInnerHTML(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(thisObject);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setInnerHTML(valueToStringWithNullCheck(exec, value), ec);
    setDOMException(exec, ec);
}


JSValue JSShadowRoot::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSShadowRootConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionCloneNode(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    ExceptionCode ec = 0;
    bool deep(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toBoolean(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->cloneNode(deep, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionGetSelection(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getSelection()));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionGetElementById(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    const String& elementId(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getElementById(elementId)));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionGetElementsByClassName(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    const String& className(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getElementsByClassName(className)));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionGetElementsByTagName(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    const String& tagName(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getElementsByTagName(tagName)));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsShadowRootPrototypeFunctionGetElementsByTagNameNS(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSShadowRoot::s_info))
        return throwVMTypeError(exec);
    JSShadowRoot* castedThis = jsCast<JSShadowRoot*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSShadowRoot::s_info);
    ShadowRoot* impl = static_cast<ShadowRoot*>(castedThis->impl());
    const String& namespaceURI(valueToStringWithNullCheck(exec, MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& localName(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getElementsByTagNameNS(namespaceURI, localName)));
    return JSValue::encode(result);
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, ShadowRoot* impl)
{
    return wrap<JSShadowRoot>(exec, globalObject, impl);
}


}

#endif // ENABLE(SHADOW_DOM)
