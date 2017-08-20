//
//  ViewController.m
//  XcodeZhMatcherDemo
//
//  Created by KyleWong on 26/10/2016.
//  Copyright © 2016 KyleWong. All rights reserved.
//

#import "ViewController.h"

NSString *feStr =
@"您需要在［系统设置->隐私->"
@"照片］里面设置允许APP访问照片才行哦～";

@interface XXViewController ()
@end

@implementation XXViewController
- (instancetype)init{
    if(self = [super init]){
        [self func1];
    }
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    [self.view setBackgroundColor:[UIColor redColor]];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)dealloc{
    [self func2];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)func1{
    NSLog(XLocalized(@"驗碼"));
}

- (void)func2{
    char *abc="char哈哈哈哈..."; //好吧1234
    NSString *ocstr=@"ocstr恩";
    NSArray *ary = @[@"abcd"];
    NSDictionary *dict = @{@"abc":@"哈哈哈哈"};
    NSLog(@"系统忙，请稍后再试");
    NSString *str = [NSString stringWithFormat:@"ABCD:%@-%@",@"EFG",@"中国"];
    str = [NSString stringWithFormat:@"哈哈:%d",1];
    [NSObject superclass];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"您需要～" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
}
@end
