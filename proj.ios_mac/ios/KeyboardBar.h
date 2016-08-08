//
//  KeyboardBar.h
//  KeyboardInputView
//
//  Created by Brian Mancini on 10/4/14.
//  Copyright (c) 2014 iOSExamples. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface KeyboardBar : UIView<UITextViewDelegate>

@property (strong, nonatomic) UITextView *textView;
@property (strong, nonatomic) UIButton *actionButton;

@end
