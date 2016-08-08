#import <UIKit/UIKit.h>

@class RootViewController;
@interface AppController : NSObject <UIApplicationDelegate,UITextFieldDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

- (void)showKeyboard;
- (void)dismissKeyboard;

@end

