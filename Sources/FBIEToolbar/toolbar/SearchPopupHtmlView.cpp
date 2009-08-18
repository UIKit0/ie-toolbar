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
#include "SearchPopupHtmlView.h"

#pragma warning(disable : 4146)
#pragma warning(disable : 4278) 

#include <boost/algorithm/string/replace.hpp>

#include "../resource.h"

#include "../FriendListXmlBuilder.h"

#include "../system/RuntimeContext.h"
#include "../system/UserDataObserver.h"
#include "../../common/CommonConstants.h"
#include "../../common/UrlUtils.h"
#include "../../common/ToolbarMessages.h"

#include "../../util/JavaScriptUtils.h"
#include "../../common/ResourceMessages.h"
#include "../../util/LogUtils.h"
#include "../../util/ResourceUtils.h"
#include "../../util/XSLTUtils.h"


namespace facebook{

// ---------------------------------------------------------------------
// class SearchPopupHtmlView
// ---------------------------------------------------------------------


SearchPopupHtmlView::SearchPopupHtmlView() {
  actionHandler_ = &(SearchPopupHtmlView::handleHtmlLoad);
  messagesHook_ = SetWindowsHookEx(WH_GETMESSAGE, 
       (HOOKPROC)getMessageHook, AfxGetApp()->m_hInstance,
       GetCurrentThreadId());
}

SearchPopupHtmlView::~SearchPopupHtmlView() {
  if (messagesHook_) {
    UnhookWindowsHookEx(messagesHook_);
  }
}

int SearchPopupHtmlView::getFriendsListXslResource() {
   return IDR_POPUP_FRIEND_LIST_XSLT;
}

LRESULT CALLBACK SearchPopupHtmlView::getMessageHook(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode < 0) {
    return CallNextHookEx(NULL, nCode, wParam, lParam);
  }
  MSG* msg = (MSG*)lParam;
  if (msg->message == WM_KEYDOWN) {
    CWnd *friendPopup = CWnd::FindWindow(NULL, sFriendsSearchPopupWindowName.c_str());
    if (friendPopup) {
      BOOL result = friendPopup->SendMessage(TBM_CUSTOM_KEY_PRESSED, msg->wParam, (LPARAM)msg->hwnd);
      if (result) {
        //if we handle it then don't pass it to other windows
        return TRUE;
      }
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool SearchPopupHtmlView::handleHtmlLoad(LPCTSTR url, 
                                         BOOL* cancelAction, 
                                         CWnd* notifyWindow) {
  
  LOG4CPLUS_DEBUG(LogUtils::getLogger(), _T("SearchPopupHtmlView::handleHtmlLoad url = ") << url);
  // check if url contain command prefix "app:"
  // and exit if not
  if (!isCustomAction(url)) {
    return false;
  }

  *cancelAction = TRUE;
  const LPCTSTR APP_ALLFRIENDS = _T("app:allFriends@");
  
  if (!UserDataObserver::getInstance().isLoggedIn(false)) {
    // show login dialog if not logged in
    UserDataObserver::getInstance().login();
  } else if (_tcsnicmp(url, APP_ALLFRIENDS, _tcslen(APP_ALLFRIENDS)) == 0) {
    // oped sidebar if action=allFriends
    if (FALSE == RuntimeContext::isSidebarOpened()) {
      if (RuntimeContext::getToolbar()) {
        RuntimeContext::getToolbar()->postMessage(WM_COMMAND, 
          IDC_TBI_SIDEBAR, 0);
      }
    }
  } else {
    //process default commands, url
    processBrowserCommand(url);
  }
  // close popup after click on link
  CWnd *friendPopup = notifyWindow->GetParent();
  if (friendPopup) {
     friendPopup->SendMessage(WM_LBUTTONUP);
  }  
  return true;

}

bool SearchPopupHtmlView::isCustomHtmlView(const FriendsList &friendsToShow, 
                                           String &result) {
  using boost::replace_all;
  String messageHtml;
  // handle if there would be all friends
  if (friendsToShow.size() == fullFriendsList_.size()) {

    messageHtml = loadStringFromResources(IDR_MESSAGE_VIEW, 
      RT_HTML);
    // message to put into html template
    String message = ResourceMessages::getMessage(kHtmlSeeAllFriends);
    //convert friends To Show count to string
    String friendsCountString;
    StringStream friendsCountStream;
    friendsCountStream << friendsToShow.size();
    friendsCountStream >> friendsCountString;
    replace_all(message, _T("{1}"), friendsCountString);
    // action related to message to put into html template
    String action = kSeeAllFriends;

    replace_all(messageHtml, _T("@message"), message);
    replace_all(messageHtml, _T("@action"), action);
    result = messageHtml;
  } else if (friendsToShow.size() == 1) {
    LOG4CPLUS_DEBUG(LogUtils::getLogger(), 
      _T("SearchPopupHtmlView::isCustomHtmlView show 1 friend"));
  // if there is only one friend then show specific html
    messageHtml = loadStringFromResources(IDR_ONE_FRIEND_VIEW, 
      RT_HTML);
    const String friendsXML = FriendsListXmlBuilder::friendsToXml(friendsToShow); 
    result = XSLTUtils::generateHtml(messageHtml, friendsXML);
    longListLoaded_ = true;
  } else {
    // handle it in the usual way
    return false;
  }
  return true;
}

}  //!namespace facebook








