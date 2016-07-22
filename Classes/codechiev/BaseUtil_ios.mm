//
//  BaseUtil_ios.cpp
//  empire
//
//  Created by metasoft on 16/4/6.
//
//

#include "BasisUtil.h"
#include "../layer/MenuLayer.h"
#include "../model/MessageLayer.hpp"
#import "AppController.h"

std::string
codechiev::getDeviceID()
{
    NSString *idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    //NSString *idfv = @"";
    return [idfv UTF8String];
}

void
codechiev::showKeyboard()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app showKeyboard];
}

void
codechiev::onDismissKeyboard()
{
    gMenuLayer->msgLayer->msgBtn->setEnabled(true);
}
