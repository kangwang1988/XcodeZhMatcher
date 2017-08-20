//
//  ViewController.h
//  XcodeZhMatcherDemo
//
//  Created by KyleWong on 26/10/2016.
//  Copyright © 2016 KyleWong. All rights reserved.
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT NSString *feStr;

@class XXViewController;
@protocol XXViewControllerDelegate <NSObject>
- (void)didLoadOfViewController:(XXViewController *)vc;
@end

@interface XXViewController : UIViewController
@end
