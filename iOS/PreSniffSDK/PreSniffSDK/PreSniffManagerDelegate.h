/*
 * Author: Andreas Linde <mail@andreaslinde.de>
 *
 * Copyright (c) 2012-2014 HockeyApp, Bit Stadium GmbH.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#import <Foundation/Foundation.h>
#import "PreSniffSDKFeatureConfig.h"

#if HOCKEYSDK_FEATURE_CRASH_REPORTER
#import "PRESCrashManagerDelegate.h"
#endif

#if HOCKEYSDK_FEATURE_UPDATES
#import "BITUpdateManagerDelegate.h"
#endif

#if HOCKEYSDK_FEATURE_FEEDBACK
#import "BITFeedbackManagerDelegate.h"
#endif

#if HOCKEYSDK_FEATURE_AUTHENTICATOR
#import "BITAuthenticator.h"
#endif

@class PreSniffManager;
@class PRESBaseManager;

/**
 The `PreSniffManagerDelegate` formal protocol defines methods further configuring
  the behaviour of `PreSniffManager`, as well as the delegate of the modules it manages.
 */

@protocol PreSniffManagerDelegate <NSObject
#if HOCKEYSDK_FEATURE_CRASH_REPORTER
  , PRESCrashManagerDelegate
#endif
#if HOCKEYSDK_FEATURE_UPDATES
  , BITUpdateManagerDelegate
#endif
#if HOCKEYSDK_FEATURE_FEEDBACK
  , BITFeedbackManagerDelegate
#endif
#if HOCKEYSDK_FEATURE_AUTHENTICATOR
  , BITAuthenticatorDelegate
#endif
  >

@optional


///-----------------------------------------------------------------------------
/// @name App Identifier usage
///-----------------------------------------------------------------------------

/**
 Implement to force the usage of the live identifier
 
 This is useful if you are e.g. distributing an enterprise app inside your company
 and want to use the `liveIdentifier` for that even though it is not running from
 the App Store.
 
 Example:
 
     - (BOOL)shouldUseLiveIdentifierForHockeyManager:(PreSniffManager *)hockeyManager {
     #ifdef (CONFIGURATION_AppStore)
       return YES;
     #endif
       return NO;
     }
 
 @param hockeyManager PreSniffManager instance
 */
- (BOOL)shouldUseLiveIdentifierForHockeyManager:(PreSniffManager *)hockeyManager;


///-----------------------------------------------------------------------------
/// @name UI presentation
///-----------------------------------------------------------------------------


// optional parent view controller for the feedback screen when invoked via the alert view, default is the root UIWindow instance
/**
 Return a custom parent view controller for presenting modal sheets
 
 By default the SDK is using the root UIWindow instance to present any required
 view controllers. Overwrite this if this doesn't result in a satisfying
 behavior or if you want to define any other parent view controller.
 
 @param hockeyManager The `PreSniffManager` HockeyManager instance invoking this delegate
 @param componentManager The `PRESBaseManager` component instance invoking this delegate, can be `PRESCrashManager` or `BITFeedbackManager`
 */
- (UIViewController *)viewControllerForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager;


///-----------------------------------------------------------------------------
/// @name Additional meta data
///-----------------------------------------------------------------------------


/** Return the userid that should used in the SDK components
 
 Right now this is used by the `PRESCrashManager` to attach to a crash report.
 `BITFeedbackManager` uses it too for assigning the user to a discussion thread.
 
 In addition, if this returns not nil for `BITFeedbackManager` the user will
 not be asked for any user details by the component, including userName or userEmail.
 
 You can find out the component requesting the userID like this:
 
    - (NSString *)userIDForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager {
      if (componentManager == hockeyManager.feedbackManager) {
        return UserIDForFeedback;
      } else if (componentManager == hockeyManager.crashManager) {
        return UserIDForCrashReports;
      } else {
        return nil;
      }
    }
 
 For crash reports, this delegate is invoked on the startup after the crash!
 
 Alternatively you can also use `[PreSniffManager userID]` which will cache the value in the keychain.

 @warning When returning a non nil value for the `PRESCrashManager` component, crash reports
 are not anonymous any more and the crash alerts will not show the word "anonymous"!
 
 @param hockeyManager The `PreSniffManager` HockeyManager instance invoking this delegate
 @param componentManager The `PRESBaseManager` component instance invoking this delegate, can be `PRESCrashManager` or `BITFeedbackManager`
 @see userNameForHockeyManager:componentManager:
 @see userEmailForHockeyManager:componentManager:
 @see [PreSniffManager userID]
 */
- (NSString *)userIDForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager;


/** Return the user name that should used in the SDK components
 
 Right now this is used by the `PRESCrashManager` to attach to a crash report.
 `BITFeedbackManager` uses it too for assigning the user to a discussion thread.
 
 In addition, if this returns not nil for `BITFeedbackManager` the user will
 not be asked for any user details by the component, including userName or userEmail.
 
 You can find out the component requesting the user name like this:
 
    - (NSString *)userNameForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager {
      if (componentManager == hockeyManager.feedbackManager) {
        return UserNameForFeedback;
      } else if (componentManager == hockeyManager.crashManager) {
        return UserNameForCrashReports;
      } else {
        return nil;
      }
 }

 For crash reports, this delegate is invoked on the startup after the crash!
 
 Alternatively you can also use `[PreSniffManager userName]` which will cache the value in the keychain.
 
 @warning When returning a non nil value for the `PRESCrashManager` component, crash reports
 are not anonymous any more and the crash alerts will not show the word "anonymous"!

 @param hockeyManager The `PreSniffManager` HockeyManager instance invoking this delegate
 @param componentManager The `PRESBaseManager` component instance invoking this delegate, can be `PRESCrashManager` or `BITFeedbackManager`
 @see userIDForHockeyManager:componentManager:
 @see userEmailForHockeyManager:componentManager:
 @see [PreSniffManager userName]
 */
- (NSString *)userNameForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager;


/** Return the users email address that should used in the SDK components
 
 Right now this is used by the `PRESCrashManager` to attach to a crash report.
 `BITFeedbackManager` uses it too for assigning the user to a discussion thread.
 
 In addition, if this returns not nil for `BITFeedbackManager` the user will
 not be asked for any user details by the component, including userName or userEmail.
 
 You can find out the component requesting the user email like this:
 
    - (NSString *)userEmailForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager {
      if (componentManager == hockeyManager.feedbackManager) {
        return UserEmailForFeedback;
      } else if (componentManager == hockeyManager.crashManager) {
        return UserEmailForCrashReports;
       } else {
        return nil;
       }
    }
 
 For crash reports, this delegate is invoked on the startup after the crash!
 
 Alternatively you can also use `[PreSniffManager userEmail]` which will cache the value in the keychain.
 
 @warning When returning a non nil value for the `PRESCrashManager` component, crash reports
 are not anonymous any more and the crash alerts will not show the word "anonymous"!

 @param hockeyManager The `PreSniffManager` HockeyManager instance invoking this delegate
 @param componentManager The `PRESBaseManager` component instance invoking this delegate, can be `PRESCrashManager` or `BITFeedbackManager`
 @see userIDForHockeyManager:componentManager:
 @see userNameForHockeyManager:componentManager:
 @see [PreSniffManager userEmail]
 */
- (NSString *)userEmailForHockeyManager:(PreSniffManager *)hockeyManager componentManager:(PRESBaseManager *)componentManager;

@end
