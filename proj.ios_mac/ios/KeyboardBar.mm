//
//  KeyboardBar.m
//  KeyboardInputView
//
//  Created by Brian Mancini on 10/4/14.
//  Copyright (c) 2014 iOSExamples. All rights reserved.
//

#import "KeyboardBar.h"
#import "AppController.h"
#include "ChatController.hpp"
#include "BasisUtil.h"

@implementation KeyboardBar

- (id)init {
    CGRect screen = [[UIScreen mainScreen] bounds];
    CGRect frame = CGRectMake(0,0, CGRectGetWidth(screen), 40);
    self = [self initWithFrame:frame];
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if(self) {
        
        self.backgroundColor = [UIColor colorWithWhite:0.75f alpha:1.0f];
    
        self.textView = [[UITextView alloc]initWithFrame:CGRectMake(5, 5, frame.size.width - 70, frame.size.height - 10)];
        self.textView.backgroundColor = [UIColor colorWithRed:1.f green:1.f blue:1.f alpha:1.f];
        self.textView.font = [UIFont systemFontOfSize:30];
        [self.textView setDelegate:self];
        [self addSubview:self.textView];
        
        self.actionButton = [[UIButton alloc]initWithFrame:CGRectMake(frame.size.width - 60, 5, 55, frame.size.height - 10)];
        self.actionButton.backgroundColor = [UIColor colorWithWhite:0.5f alpha:1.0f];
        self.actionButton.layer.cornerRadius = 2.0;
        self.actionButton.layer.borderWidth = 1.0;
        self.actionButton.layer.borderColor = [[UIColor colorWithWhite:0.45 alpha:1.0f] CGColor];
        [self.actionButton setTitle:@"Send" forState:UIControlStateNormal];
        [self.actionButton addTarget:self action:@selector(didTouchAction) forControlEvents:UIControlEventTouchUpInside];
        
        [self addSubview:self.actionButton];
        
    }
    return self;
}

- (void) didTouchAction
{
    NSString *result = self.textView.text;
    NSLog(@"didTouchAction %@",result);
    codechiev::control::ChatController::chat([result UTF8String], nullptr);
    codechiev::onDismissKeyboard();
    
    [self.textView setText:@""];
    [self.textView resignFirstResponder];
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app dismissKeyboard];
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
    codechiev::onDismissKeyboard();
    
    [self.textView setText:@""];
    [self.textView resignFirstResponder];
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app dismissKeyboard];
    
    NSLog(@"textViewDidEndEditing ");
}

@end
