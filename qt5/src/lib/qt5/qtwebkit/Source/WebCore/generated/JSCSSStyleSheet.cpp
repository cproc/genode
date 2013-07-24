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
#include "JSCSSStyleSheet.h"

#include "CSSRule.h"
#include "CSSRuleList.h"
#include "CSSStyleSheet.h"
#include "ExceptionCode.h"
#include "JSCSSRule.h"
#include "JSCSSRuleList.h"
#include "JSDOMBinding.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSCSSStyleSheetTableValues[] =
{
    { "ownerRule", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetOwnerRule), (intptr_t)0, NoIntrinsic },
    { "cssRules", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetCssRules), (intptr_t)0, NoIntrinsic },
    { "rules", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetRules), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSCSSStyleSheetTable = { 9, 7, JSCSSStyleSheetTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSCSSStyleSheetConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSCSSStyleSheetConstructorTable = { 1, 0, JSCSSStyleSheetConstructorTableValues, 0 };
const ClassInfo JSCSSStyleSheetConstructor::s_info = { "CSSStyleSheetConstructor", &Base::s_info, &JSCSSStyleSheetConstructorTable, 0, CREATE_METHOD_TABLE(JSCSSStyleSheetConstructor) };

JSCSSStyleSheetConstructor::JSCSSStyleSheetConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSCSSStyleSheetConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSCSSStyleSheetPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSCSSStyleSheetConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSStyleSheetConstructor, JSDOMWrapper>(exec, &JSCSSStyleSheetConstructorTable, jsCast<JSCSSStyleSheetConstructor*>(cell), propertyName, slot);
}

bool JSCSSStyleSheetConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSCSSStyleSheetConstructor, JSDOMWrapper>(exec, &JSCSSStyleSheetConstructorTable, jsCast<JSCSSStyleSheetConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSCSSStyleSheetPrototypeTableValues[] =
{
    { "insertRule", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionInsertRule), (intptr_t)2, NoIntrinsic },
    { "deleteRule", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionDeleteRule), (intptr_t)1, NoIntrinsic },
    { "addRule", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionAddRule), (intptr_t)3, NoIntrinsic },
    { "removeRule", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionRemoveRule), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSCSSStyleSheetPrototypeTable = { 8, 7, JSCSSStyleSheetPrototypeTableValues, 0 };
const ClassInfo JSCSSStyleSheetPrototype::s_info = { "CSSStyleSheetPrototype", &Base::s_info, &JSCSSStyleSheetPrototypeTable, 0, CREATE_METHOD_TABLE(JSCSSStyleSheetPrototype) };

JSObject* JSCSSStyleSheetPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSCSSStyleSheet>(exec, globalObject);
}

bool JSCSSStyleSheetPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSCSSStyleSheetPrototype* thisObject = jsCast<JSCSSStyleSheetPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSCSSStyleSheetPrototypeTable, thisObject, propertyName, slot);
}

bool JSCSSStyleSheetPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSCSSStyleSheetPrototype* thisObject = jsCast<JSCSSStyleSheetPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSCSSStyleSheetPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSCSSStyleSheet::s_info = { "CSSStyleSheet", &Base::s_info, &JSCSSStyleSheetTable, 0 , CREATE_METHOD_TABLE(JSCSSStyleSheet) };

JSCSSStyleSheet::JSCSSStyleSheet(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<CSSStyleSheet> impl)
    : JSStyleSheet(structure, globalObject, impl)
{
}

void JSCSSStyleSheet::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSCSSStyleSheet::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSCSSStyleSheetPrototype::create(exec->globalData(), globalObject, JSCSSStyleSheetPrototype::createStructure(exec->globalData(), globalObject, JSStyleSheetPrototype::self(exec, globalObject)));
}

bool JSCSSStyleSheet::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSCSSStyleSheet* thisObject = jsCast<JSCSSStyleSheet*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSCSSStyleSheet, Base>(exec, &JSCSSStyleSheetTable, thisObject, propertyName, slot);
}

bool JSCSSStyleSheet::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSCSSStyleSheet* thisObject = jsCast<JSCSSStyleSheet*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSCSSStyleSheet, Base>(exec, &JSCSSStyleSheetTable, thisObject, propertyName, descriptor);
}

JSValue jsCSSStyleSheetOwnerRule(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->ownerRule()));
    return result;
}


JSValue jsCSSStyleSheetCssRules(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->cssRules()));
    return result;
}


JSValue jsCSSStyleSheetRules(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->rules()));
    return result;
}


JSValue jsCSSStyleSheetConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSCSSStyleSheet* domObject = jsCast<JSCSSStyleSheet*>(asObject(slotBase));
    return JSCSSStyleSheet::getConstructor(exec, domObject->globalObject());
}

JSValue JSCSSStyleSheet::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSCSSStyleSheetConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionInsertRule(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwVMTypeError(exec);
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSCSSStyleSheet::s_info);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    ExceptionCode ec = 0;
    const String& rule(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsNumber(impl->insertRule(rule, index, ec));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionDeleteRule(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwVMTypeError(exec);
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSCSSStyleSheet::s_info);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    ExceptionCode ec = 0;
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->deleteRule(index, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionAddRule(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwVMTypeError(exec);
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSCSSStyleSheet::s_info);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    ExceptionCode ec = 0;
    const String& selector(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& style(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    size_t argsCount = exec->argumentCount();
    if (argsCount <= 2) {

        JSC::JSValue result = jsNumber(impl->addRule(selector, style, ec));
        setDOMException(exec, ec);
        return JSValue::encode(result);
    }

    unsigned index(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsNumber(impl->addRule(selector, style, index, ec));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionRemoveRule(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwVMTypeError(exec);
    JSCSSStyleSheet* castedThis = jsCast<JSCSSStyleSheet*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSCSSStyleSheet::s_info);
    CSSStyleSheet* impl = static_cast<CSSStyleSheet*>(castedThis->impl());
    ExceptionCode ec = 0;
    unsigned index(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->removeRule(index, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}


}
