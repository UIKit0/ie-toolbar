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

#ifndef BASEHTMLLISTVIEW_H
#define BASEHTMLLISTVIEW_H

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

#include <mshtml.h>

#include "../../common/GenericHtmlView.h"
#include "../../data/ServiceData.h"
#include "../../util/StringUtils.h"

namespace facebook{

/**
 * class BaseHtmlListView
 *
 * This is ABSTRACT class whici holds base 
 * functionality used to display the list of friends.
 *
 * Basically friends list is displayed using html view.
 * Additionally this class supports several view states
 * and list sorting modes.
 *
 * Also added support for module html resources 
 * @see loadResource()
 *
 * Friend list is visualised using Xml/Xslt transformation
 * which makes flexible UI changes without C++ code changes.
 *
 * To support external CSS added html content preprocessing
 * which replaces predefined variabled with corresponding 
 * resource @see processContent()
 * 
 */

class BaseHtmlListView : public GenericHtmlView{
   // types
public:
   enum ViewState{
      VS_INITIAL,
      VS_WAIT_LOGIN,
      VS_LOADING_FRIENDS,
      VS_FRIENDS_LOADED
   };

   // construction/destruction
public:
   BaseHtmlListView();
   BaseHtmlListView(ActionHandler actionHandler);
  virtual ~BaseHtmlListView();

   // public interface
public:
   
  void loaded(const FriendsList& friendsList, const bool needRefresh);

  /**
   * Redraws friends list html according
   * to the specified sort mode and current filter
   * (if any )
   * 
   */
  void changeSortMode(const SortMode& sortMode);

  /**
   * Redraws friends list html according
   * to the specified filter mode and current sort
   * mode (if any )
   * 
   */
  void changeFilter(const String& filter, bool needReload = true);


  /**
   * Most important method, used to generate view html
   * 
   * This method analyse current view state and 
   * generate html using different html resources
   * see implementation for mode details
   */
  String getListHtml();

  // state methods
  void loggedOut();
  void loading();

  ViewState getViewState() const;

  bool isLongListLoaded() const;

  /**
   * loads html resource, prepare it for display and write it to the browser
   */
  BOOL LoadFromResource(int resourceId);

  /**
   * Script in html view is responsible for navigation
   * through the list of friends using keyboard. This method
   * is used to translate keyboard events to the html page
   */
  bool translateKeyCodeToJS(const UINT keyCode);

  /**
   * Checks whether specified window is our chield
   *
   * @param descendantCandidate - child candidate to check
   * @return true if yes
   */
  bool isWindowOurDescendant(const HWND descendantCandidate) const;

  /**
   * Does all the necessary steps to produces 
   * html to be displayed in friends list html view
   * Eventually it calls putHtml
   */
  void renderHtml();

  // overrides
protected:

  static String getViewAction(String url);

  virtual bool isCustomHtmlView(const FriendsList &friendsToShow, String &result);


  // message handlers
protected:
  DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnFriendsListUpdated(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnChangeFriendsFilter(WPARAM, LPARAM lParam);

protected:

  /**
   * Return XSL for rendering HTML view
   *
   */
  String getXsl();


  /**
   * MUST be overriden, should return
   * module resouce id associated with 
   * Xsl transformation used to generate
   * friends list html 
   */
  virtual int getFriendsListXslResource() = 0;

  /**
   * Process events sent by actions performed in browser  
   * 
   * @param commandURL - contains all the information
   *                     about command to be executed
   * @param needEncode - check whether need to encode url
   */
  static void processBrowserCommand(const String& commandURL, bool needEncode = false);

  // private methods
private: 

  /**
   * Process html content before write it into the 
   * html document
   */
  void processContent(String& content);

  // members
protected:

   bool longListLoaded_;
   String friendsXSL_;  // XSLT template 
   SortMode sortMode_; // current sort mode

   FriendsList fullFriendsList_;  /**< the source list of friends
   to which filter and sorting is applied */
   String filter_; // current filter
   ViewState viewState_;
};

} //!namespace facebook

#endif 