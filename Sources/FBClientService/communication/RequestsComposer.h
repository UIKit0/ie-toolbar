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

#ifndef REQUESTSCOMPOSER_H
#define REQUESTSCOMPOSER_H

#include "XMLRequest.h"
#include "../../data/ChangesTypes.h"

/**
 * Set of functions used to build request to Facebook server
 *
 * see http://wiki.developers.facebook.com/index.php/API
 *
 */

namespace facebook{ namespace RequestsComposer{

/**
 * Builds request to create new token
 *
 */
XMLRequest composeCreateTokenRequest();

/**
 * Builds request to create new session
 *
 * @param token used to create session (by ref)
 */
XMLRequest composeCreateSessionRequest(const String& token);

/**
 * Builds request to get list of friends
 *
 * @param logged user id (by ref)
 */
XMLRequest composeGetFriendsListRequest(const String& loggedInUserID);

/**
 * Builds request to get notifications
 *
 */
XMLRequest composeGetNotificationsRequest();

/**
 * Builds request to check permissions
 *
 * @param operation to check permissions (by ref)
 */
XMLRequest composeHasPermissionRequest(const String& operation);

/**
 * Builds request to set user status
 *
 * @param status to set (by ref)
 */
XMLRequest composeSetStatusRequest(const String& statusString);

/**
 * Builds request to get albums
 *
 * @param logged user id (by ref)
 * @param interval (by value)
 */
XMLRequest composeGetAlbumsRequest(const String& loggedInUserID, 
  const size_t interval);

/**
 * Builds request to get user info
 *
 * @param logged user id (by ref)
 */
XMLRequest composeGetUsersInfoRequest(const UsersIDs& usersIDs);

}}

#endif