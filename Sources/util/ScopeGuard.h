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

#ifndef SCOPEGUARD_H
#define SCOPEGUARD_H

#include <boost/function/function0.hpp>
#include <boost/noncopyable.hpp>

/**
 * useful tool for support RAII.
 * usage examples :
 *
 *  ScopeGuard fbCritSectGuard(
 *    bind(&CCriticalSection::Lock, ref(facebookDataCritSect)),
 *    bind(&CCriticalSection::Unlock, ref(facebookDataCritSect)));
 *  this locks critical section on guard construction and unlocks on  
 *
 *  fbCritSectGuard.dismiss() - cancels actions assigned on destruction
 *  
 *  another example:
 *
 *  SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
 *  ScopeGuard psaGuard(boost::bind(SafeArrayDestroyWrapper() , psa));
 *  this will  call SafeArrayDestroyWrapper(psa) when guard is 
 *  destructed
 *  
 *  Important Note: it is highly recommended to do not use API with boost::bind
 *  due to the differences in linkage -  pointer to extern "c" function
 *  is not always the same as pointer to 'usual' function.
 *  Always try to make a wrapper. e.g. 
 * {
 *     typedef void result_type;
 *     void operator()(SAFEARRAY* psa) const
 *    {
 *      SafeArrayDestroy(psa);
 *     }
 *  };
 * this is a preferable and portable way  of using boost::bind with API
 *
 */

namespace facebook {

   /**
    * class ScopeGuard
    *
    * Execute functions on start of declaration of ScopeGuard variable 
    * and at the end of it
    */
   class ScopeGuard : private boost::noncopyable{
   public:
      explicit ScopeGuard( const boost::function0<void>& doAtDestroyFunc )
         : m_dismissed(false)
         , m_doAtDestroyFunc(doAtDestroyFunc) {
      }

      explicit ScopeGuard(const boost::function0<void>& doAtCreateFunc,
         const boost::function0<void>& doAtDestroyFunc)
         : m_dismissed(false)
         , m_doAtDestroyFunc(doAtDestroyFunc) {
            doAtCreateFunc();
      }

      ~ScopeGuard() {
         if(!m_dismissed) {
            m_doAtDestroyFunc();
         }
      }

      void dismiss() {
         m_dismissed = true;
      }

   private:
      bool m_dismissed; // dissmissed flag
      const boost::function0<void> m_doAtDestroyFunc; 
      // pointer to function that executed on destroy
   };

} // !namespace facebook


#endif // #ifndef ScopeGuard_H
