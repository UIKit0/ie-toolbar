/**
* Facebook Internet Explorer Toolbar Software License 
* Copyright (c) 2009 Facebook, Inc. 
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (which, together with any graphical images included with such
* software, are collectively referred to below as the "Software") to (a) use,
* reproduce, display, distribute, execute, and transmit the Software, (b)
* prepare derivative works of the Software (excluding any graphical images
* included with the Software, which may not be modified or altered), and (c)
* permit third-parties to whom the Software is furnished to do so, all
* subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.  
*
* Facebook, Inc. retains ownership of the Software and all associated
* intellectual property rights.  All rights not expressly granted in this
* license are reserved by Facebook, Inc.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/ 


#ifndef PROTOCOLCF_H
#define PROTOCOLCF_H

#include "ProtocolImpl.h"
#include "SessionSink.h"

namespace facebook {

namespace Detail {

// A bit of template meta programming allows to avoid requiring
// the protocol class to either derive from CComCoClass or explicitly
// specify DECLARE_*AGGREGATABLE macro. If the class does it, the declared
// behavior is used, otherwise the default behavior is as if
// DECLARE_NOT_AGGREGATABLE is specified

// If T has a typedef _CreatorClass, Derived would pick it from its base.
// If T does not define _CreatorClass, the definition is picked from the next
// enclosing scope, which is ChooseCreatorClass::_CreatorClass, or Default
template <typename T, typename Default>
struct ChooseCreatorClass {

  typedef Default _CreatorClass;

  struct Derived : private T {
    typedef _CreatorClass CreatorClass;
  };

  typedef typename Derived::CreatorClass CreatorClass;

};

} // !namespace facebook::Detail

/**
 * class ProtocolClassFactory
 *
 * Used to produce our custom protocol handler
 * 
 */
class ATL_NO_VTABLE ProtocolClassFactory :
  public CComClassFactory {

  typedef CComClassFactory BaseClass;

public:

  STDMETHODIMP CreateInstance(IUnknown* resultInterface, REFIID riid,
      void** resultObject);

  /**
   * Create the real tprotocol handler 
   * that will be used inside our custom
   *
   * @param targetProtocol - return pointer to the created handler
   */
  HRESULT CreateInstanceTarget(IUnknown** targetProtocol);

  HRESULT GetTargetClassFactory(IClassFactory** targetFactory);
  HRESULT SetTargetClassFactory(IClassFactory* factory);
  HRESULT SetTargetCLSID(REFCLSID classId, DWORD classContext = CLSCTX_ALL);

  HRESULT setTargetWindow(HWND window) {
     targetWindow_ = window;
     return S_OK;
  }

  void FinalRelease();

private:

  CComPtr<IClassFactory> targetFactory_;
  HWND targetWindow_;

};


/**
 * struct MetaFactory
 *
 * Used to produce our custom protocol handler factory
 * 
 */
template <class Protocol>
struct MetaFactory {

  typedef
    CComCreator2<CComCreator<CComObject<Protocol> >,
      CComCreator<CComAggObject<Protocol> > >
  DefaultCreatorClass;

  typedef typename
    Detail::ChooseCreatorClass<Protocol, DefaultCreatorClass>::CreatorClass
      CreatorClass;

  static HRESULT CreateInstance(ProtocolClassFactory** resultObject, HWND window);

  static HRESULT CreateInstance(IClassFactory* targetInterface, IClassFactory** targetFactory,
      HWND window);

  static HRESULT CreateInstance(REFCLSID clsidTarget, IClassFactory** targetFactory,
      HWND window);

};

typedef MetaFactory<InternetProtocol<SessionPolicy>> SessionMetaFactory;

} // !namespace facebook

#include "ProtocolFactory.inl"

#endif // PROTOCOLCF_H
