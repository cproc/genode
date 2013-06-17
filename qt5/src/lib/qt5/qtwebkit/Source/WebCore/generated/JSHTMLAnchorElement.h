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

#ifndef JSHTMLAnchorElement_h
#define JSHTMLAnchorElement_h

#include "HTMLAnchorElement.h"
#include "JSDOMBinding.h"
#include "JSHTMLElement.h"
#include <runtime/JSObject.h>

namespace WebCore {

class JSHTMLAnchorElement : public JSHTMLElement {
public:
    typedef JSHTMLElement Base;
    static JSHTMLAnchorElement* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLAnchorElement> impl)
    {
        JSHTMLAnchorElement* ptr = new (NotNull, JSC::allocateCell<JSHTMLAnchorElement>(globalObject->globalData().heap)) JSHTMLAnchorElement(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
protected:
    JSHTMLAnchorElement(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<HTMLAnchorElement>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};


class JSHTMLAnchorElementPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSHTMLAnchorElementPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSHTMLAnchorElementPrototype* ptr = new (NotNull, JSC::allocateCell<JSHTMLAnchorElementPrototype>(globalData.heap)) JSHTMLAnchorElementPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSHTMLAnchorElementPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSHTMLAnchorElementConstructor : public DOMConstructorObject {
private:
    JSHTMLAnchorElementConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSHTMLAnchorElementConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSHTMLAnchorElementConstructor* ptr = new (NotNull, JSC::allocateCell<JSHTMLAnchorElementConstructor>(*exec->heap())) JSHTMLAnchorElementConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsHTMLAnchorElementPrototypeFunctionToString(JSC::ExecState*);
// Attributes

JSC::JSValue jsHTMLAnchorElementCharset(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementCharset(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementCoords(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementCoords(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#if ENABLE(DOWNLOAD_ATTRIBUTE)
JSC::JSValue jsHTMLAnchorElementDownload(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementDownload(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
JSC::JSValue jsHTMLAnchorElementHref(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementHref(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementHreflang(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementHreflang(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementName(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementName(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementPing(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementPing(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementRel(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementRel(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementRev(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementRev(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementShape(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementShape(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementTarget(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementTarget(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementType(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementType(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementHash(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementHash(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementHost(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementHost(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementHostname(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementHostname(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementPathname(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementPathname(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementPort(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementPort(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementProtocol(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementProtocol(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementSearch(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSHTMLAnchorElementSearch(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsHTMLAnchorElementOrigin(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsHTMLAnchorElementText(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsHTMLAnchorElementConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif
