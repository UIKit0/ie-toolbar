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

#include "StdAfx.h"

#include "../data/ToolbarSettings.h"
#include "../common/ResourceMessages.h"

#include "system/BandFactory.h"
#include "system/ClassIds.h"
#include "system/UserDataObserver.h"
#include "system/ObjectsServer.h"

#include "../util/LogUtils.h"

#include <afxwinappex.h>


namespace facebook{


STDAPI DllCanUnloadNow() {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return ObjectsServer::isLocked() ? S_FALSE : S_OK;
}


STDAPI DllGetClassObject(REFCLSID classId, REFIID interfaceId,
      LPVOID* interfacePointer) {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   *interfacePointer = 0;

   if (!IsEqualCLSID(classId, CLSID_IEToolbar) &&
     !IsEqualCLSID(classId, CLSID_IESidebar)) {
      return CLASS_E_CLASSNOTAVAILABLE;
   }

   std::auto_ptr<IClassFactory> classFactory(new BandFactory(classId));

   const HRESULT queryInterfaceResult =
         classFactory->QueryInterface(interfaceId, interfacePointer);

   if (FAILED(queryInterfaceResult)) {
      return queryInterfaceResult;
   }

   classFactory.release();

   return S_OK;
}


STDAPI DllRegisterServer() {
  LOG4CPLUS_DEBUG(LogUtils::getLogger(), "DLL RegisterServer");
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  try {
    ObjectsServer::registerServer();

    return S_OK;
  }catch (...) {
    return E_FAIL;
  }
}


STDAPI DllUnregisterServer() {
   LOG4CPLUS_DEBUG(LogUtils::getLogger(), "DLL UnregisterServer");
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   try {
      ObjectsServer::unregisterServer();

      return S_OK;
   } catch (...) {
      return E_FAIL;
   }
}

typedef CWinAppEx WinApp;


class FBIEToolsApp : public WinApp {

// Construction

public:

   FBIEToolsApp() {
   }

// Overrides

private:

  BOOL InitInstance() {
    INITCOMMONCONTROLSEX initCtrls;
    initCtrls.dwSize = sizeof(initCtrls);
    initCtrls.dwICC = ICC_WIN95_CLASSES | ICC_BAR_CLASSES | ICC_COOL_CLASSES;
    const BOOL initResult = InitCommonControlsEx(&initCtrls);
    
    if (FALSE == initResult) {
         return initResult;
    }
    LOG4CPLUS_DEBUG(LogUtils::getLogger(), "DLL Init Instance pre loading...");
    // load the cultures. need this here because automatic 
    // loading is made too early and we have no acees to the resources
    ResourceMessages::getInstance().loadCurrentCulture();
    ResourceMessages::getInstance().load();
    LOG4CPLUS_DEBUG(LogUtils::getLogger(), "DLL Init Instance loading");
    return WinApp::InitInstance();
}

  int ExitInstance() {
    LOG4CPLUS_DEBUG(LogUtils::getLogger(), "DLL Exit Instance");
    UserDataObserver::clearInstances();
    return 0;
  }

   DECLARE_MESSAGE_MAP()

};


BEGIN_MESSAGE_MAP(FBIEToolsApp, WinApp)
END_MESSAGE_MAP()





CAtlMfcModule atlModule;

FBIEToolsApp winApp;

} //!namespace facebook