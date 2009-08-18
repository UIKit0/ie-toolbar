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

#ifndef USERSTATUSEDIT_H
#define USERSTATUSEDIT_H

#include "../gui/ToolbarEdit.h"

namespace facebook {

/**
 * class UserStatusEdit
 *
 * Implements status edit look & feel.
 * It also includes functionality to load and save current status
 */
class UserStatusEdit : public ToolbarEdit {

// Types

private:

   typedef UserStatusEdit ClassType;
   typedef ToolbarEdit BaseType;

// MFC declarations

private:

   DECLARE_DYNAMIC(UserStatusEdit)

   DECLARE_MESSAGE_MAP()

// Construction

public:

   UserStatusEdit();

// Destruction

public:

   ~UserStatusEdit();

   virtual void updateView(int changeId = 0);

// Window messages handlers

protected:

   afx_msg int OnCreate(LPCREATESTRUCT createStructure);

   afx_msg void OnKeyUp(UINT character, UINT repeatCount, UINT flags);

   afx_msg void OnKillFocus(CWnd* newWindow);

// Methods

private:

  /**
   * Loads the user active status
   *
   */
  void loadUserStatus();

  /**
   * Save the user status to the facebook
   *
   */
  void saveUserStatus();

  /**
   * Ask user to autorize the status changing
   *
   */
  void authorizeSetStatus();
};


} //!namespace facebook


#endif // USERSTATUSEDIT_H
