//
//  GLRGBRender.h
//  OpenGLES11-相机视频渲染
//
//  Created by mac on 17/3/24.
//  Copyright © 2017年 Qinmin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLTexture.h"
#import "GLUtil.h"

@interface GLRender : NSObject
@property (nonatomic, assign) GLuint program;
@property (nonatomic, assign) GLuint vertexVBO;
@property (nonatomic, assign) int vertCount;
- (void)setTexture:(GLTexture *)texture;
- (void)prepareRender;
@end

@interface GLRenderRGB : GLRender
@property(nonatomic, assign, readonly) GLuint rgb;
@end

@interface GLRenderYUV : GLRender
@property(nonatomic, assign, readonly) GLuint y;
@property(nonatomic, assign, readonly) GLuint u;
@property(nonatomic, assign, readonly) GLuint v;
@end
