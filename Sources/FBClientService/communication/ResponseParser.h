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

#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H

#include "../../data/ServiceData.h"
#include "../../data/NotificationsData.h"
#include "../../data/SessionInfo.h"

#include "../../util/Error.h"
#include "../../util/StringUtils.h"


/**
 * Set of functions used to parse response from Facebook server
 *
 * see http://wiki.developers.facebook.com/index.php/API
 *
 */

namespace facebook{ namespace ResponseParser{


/**
 * Parse response to create new token
 *
 * @param response to parse(by ref)
 *
 * @return token
 */
String parseCreateTokenResp(const String& response);

/**
 * Parse response to create new session
 *
 * @param response to parse(by ref)
 *
 * @return session info
 */
SessionInfo parseCreateSessionResp(const String& response);

/**
 * Parse response to get friends list
 *
 * @param response to parse(by ref)
 *
 * @return list of friends
 */
FriendsList parseFriendsListResp(const String& response);

/**
 * Parse response to create user info
 *
 * @param response to parse(by ref)
 *
 * @return user info
 */
FriendsList parseUsersInfoResp(const String& response);

/**
 * Parse response to get notification data
 *
 * @param response to parse(by ref)
 *
 * @return notification data
 */
NotificationsData parseGetNotificationsDataResponse(const String& response);

/**
 * Parse response to check operation permissions
 *
 * @param response to parse(by ref)
 *
 * @return true if application has permissions to perform operation
 */
bool parseHasPermissionResponse(const String& response);

/**
 * Parse response to get list of albums
 *
 * @param response to parse(by ref)
 *
 * @return list of albums
 */
AlbumsList parseGetAlbumsResponse(const String& response, bool initialResponse);


/**
 * class ErrorInResponse
 *
 * Instances of this exception throwed then
 * there are errors in parsing response
 */

class ErrorInResponse : public facebook::Error{
public:
   ErrorInResponse(const String& code, 
      const String& description);

   String getDescr() const;
   int getCode() const;
private:
   facebook::String description_;
   int code_;
};


}}

#endif
