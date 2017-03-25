//
//  OpenGLESView.h
//  OpenGLES01-环境搭建
//
//  Created by qinmin on 2017/2/9.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLTexture.h"
#import "GLRender.h"

@interface OpenGLESView : UIView
@property (nonatomic, strong) GLRender *render;
- (void)setTexture:(GLTexture *)texture;
- (void)setNeedDraw;
@end
