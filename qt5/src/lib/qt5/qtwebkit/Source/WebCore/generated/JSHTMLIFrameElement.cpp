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
#include "JSHTMLIFrameElement.h"

#include "DOMWindow.h"
#include "Document.h"
#include "ExceptionCode.h"
#include "HTMLIFrameElement.h"
#include "HTMLNames.h"
#include "JSDOMBinding.h"
#include "JSDOMWindow.h"
#include "JSDocument.h"
#include "JSSVGDocument.h"
#include "KURL.h"
#include "SVGDocument.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLIFrameElementTableValues[] =
{
    { "align", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementAlign), (intptr_t)setJSHTMLIFrameElementAlign, NoIntrinsic },
    { "frameBorder", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementFrameBorder), (intptr_t)setJSHTMLIFrameElementFrameBorder, NoIntrinsic },
    { "height", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementHeight), (intptr_t)setJSHTMLIFrameElementHeight, NoIntrinsic },
    { "longDesc", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementLongDesc), (intptr_t)setJSHTMLIFrameElementLongDesc, NoIntrinsic },
    { "marginHeight", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementMarginHeight), (intptr_t)setJSHTMLIFrameElementMarginHeight, NoIntrinsic },
    { "marginWidth", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementMarginWidth), (intptr_t)setJSHTMLIFrameElementMarginWidth, NoIntrinsic },
    { "name", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementName), (intptr_t)setJSHTMLIFrameElementName, NoIntrinsic },
    { "sandbox", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementSandbox), (intptr_t)setJSHTMLIFrameElementSandbox, NoIntrinsic },
#if ENABLE(IFRAME_SEAMLESS)
    { "seamless", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementSeamless), (intptr_t)setJSHTMLIFrameElementSeamless, NoIntrinsic },
#endif
    { "scrolling", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementScrolling), (intptr_t)setJSHTMLIFrameElementScrolling, NoIntrinsic },
    { "src", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementSrc), (intptr_t)setJSHTMLIFrameElementSrc, NoIntrinsic },
    { "srcdoc", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementSrcdoc), (intptr_t)setJSHTMLIFrameElementSrcdoc, NoIntrinsic },
    { "width", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementWidth), (intptr_t)setJSHTMLIFrameElementWidth, NoIntrinsic },
    { "contentDocument", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementContentDocument), (intptr_t)0, NoIntrinsic },
    { "contentWindow", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementContentWindow), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLIFrameElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLIFrameElementTable = { 37, 31, JSHTMLIFrameElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLIFrameElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLIFrameElementConstructorTable = { 1, 0, JSHTMLIFrameElementConstructorTableValues, 0 };
const ClassInfo JSHTMLIFrameElementConstructor::s_info = { "HTMLIFrameElementConstructor", &Base::s_info, &JSHTMLIFrameElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLIFrameElementConstructor) };

JSHTMLIFrameElementConstructor::JSHTMLIFrameElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLIFrameElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLIFrameElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLIFrameElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLIFrameElementConstructor, JSDOMWrapper>(exec, &JSHTMLIFrameElementConstructorTable, jsCast<JSHTMLIFrameElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLIFrameElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLIFrameElementConstructor, JSDOMWrapper>(exec, &JSHTMLIFrameElementConstructorTable, jsCast<JSHTMLIFrameElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLIFrameElementPrototypeTableValues[] =
{
    { "getSVGDocument", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsHTMLIFrameElementPrototypeFunctionGetSVGDocument), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLIFrameElementPrototypeTable = { 2, 1, JSHTMLIFrameElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLIFrameElementPrototype::s_info = { "HTMLIFrameElementPrototype", &Base::s_info, &JSHTMLIFrameElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLIFrameElementPrototype) };

JSObject* JSHTMLIFrameElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLIFrameElement>(exec, globalObject);
}

bool JSHTMLIFrameElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLIFrameElementPrototype* thisObject = jsCast<JSHTMLIFrameElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSHTMLIFrameElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSHTMLIFrameElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLIFrameElementPrototype* thisObject = jsCast<JSHTMLIFrameElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSHTMLIFrameElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSHTMLIFrameElement::s_info = { "HTMLIFrameElement", &Base::s_info, &JSHTMLIFrameElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLIFrameElement) };

JSHTMLIFrameElement::JSHTMLIFrameElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLIFrameElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLIFrameElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLIFrameElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLIFrameElementPrototype::create(exec->globalData(), globalObject, JSHTMLIFrameElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLIFrameElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLIFrameElement* thisObject = jsCast<JSHTMLIFrameElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSHTMLIFrameElement, Base>(exec, &JSHTMLIFrameElementTable, thisObject, propertyName, slot);
}

bool JSHTMLIFrameElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLIFrameElement* thisObject = jsCast<JSHTMLIFrameElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSHTMLIFrameElement, Base>(exec, &JSHTMLIFrameElementTable, thisObject, propertyName, descriptor);
}

JSValue jsHTMLIFrameElementAlign(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::alignAttr));
    return result;
}


JSValue jsHTMLIFrameElementFrameBorder(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::frameborderAttr));
    return result;
}


JSValue jsHTMLIFrameElementHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::heightAttr));
    return result;
}


JSValue jsHTMLIFrameElementLongDesc(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::longdescAttr));
    return result;
}


JSValue jsHTMLIFrameElementMarginHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::marginheightAttr));
    return result;
}


JSValue jsHTMLIFrameElementMarginWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::marginwidthAttr));
    return result;
}


JSValue jsHTMLIFrameElementName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::nameAttr));
    return result;
}


JSValue jsHTMLIFrameElementSandbox(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::sandboxAttr));
    return result;
}


#if ENABLE(IFRAME_SEAMLESS)
JSValue jsHTMLIFrameElementSeamless(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsBoolean(impl->hasAttribute(WebCore::HTMLNames::seamlessAttr));
    return result;
}

#endif

JSValue jsHTMLIFrameElementScrolling(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::scrollingAttr));
    return result;
}


JSValue jsHTMLIFrameElementSrc(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getURLAttribute(WebCore::HTMLNames::srcAttr));
    return result;
}


JSValue jsHTMLIFrameElementSrcdoc(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::srcdocAttr));
    return result;
}


JSValue jsHTMLIFrameElementWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::widthAttr));
    return result;
}


JSValue jsHTMLIFrameElementContentDocument(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    return shouldAllowAccessToNode(exec, impl->contentDocument()) ? toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->contentDocument())) : jsNull();
}


JSValue jsHTMLIFrameElementContentWindow(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->contentWindow()));
    return result;
}


JSValue jsHTMLIFrameElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLIFrameElement* domObject = jsCast<JSHTMLIFrameElement*>(asObject(slotBase));
    return JSHTMLIFrameElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLIFrameElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLIFrameElement* thisObject = jsCast<JSHTMLIFrameElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSHTMLIFrameElement, Base>(exec, propertyName, value, &JSHTMLIFrameElementTable, thisObject, slot);
}

void setJSHTMLIFrameElementAlign(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::alignAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementFrameBorder(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::frameborderAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementHeight(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::heightAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementLongDesc(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::longdescAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementMarginHeight(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::marginheightAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementMarginWidth(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::marginwidthAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementName(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::nameAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementSandbox(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::sandboxAttr, valueToStringWithNullCheck(exec, value));
}


#if ENABLE(IFRAME_SEAMLESS)
void setJSHTMLIFrameElementSeamless(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setBooleanAttribute(WebCore::HTMLNames::seamlessAttr, value.toBoolean(exec));
}

#endif

void setJSHTMLIFrameElementScrolling(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::scrollingAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementSrc(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::srcAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementSrcdoc(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::srcdocAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLIFrameElementWidth(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(thisObject);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::widthAttr, valueToStringWithNullCheck(exec, value));
}


JSValue JSHTMLIFrameElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLIFrameElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsHTMLIFrameElementPrototypeFunctionGetSVGDocument(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSHTMLIFrameElement::s_info))
        return throwVMTypeError(exec);
    JSHTMLIFrameElement* castedThis = jsCast<JSHTMLIFrameElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSHTMLIFrameElement::s_info);
    HTMLIFrameElement* impl = static_cast<HTMLIFrameElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    if (!shouldAllowAccessToNode(exec, impl->getSVGDocument(ec)))
        return JSValue::encode(jsNull());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getSVGDocument(ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}


}
