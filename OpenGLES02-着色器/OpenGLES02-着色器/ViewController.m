//
//  ViewController.m
//  OpenGLES02-着色器
//
//  Created by qinmin on 2017/2/9.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLESView.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view = [[OpenGLESView alloc] initWithFrame:self.view.bounds];
}

@end
