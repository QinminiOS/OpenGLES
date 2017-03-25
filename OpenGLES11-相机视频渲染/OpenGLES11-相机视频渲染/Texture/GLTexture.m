//
//  GLTexture.m
//  OpenGLES11-相机视频渲染
//
//  Created by mac on 17/3/24.
//  Copyright © 2017年 Qinmin. All rights reserved.
//

#import "GLTexture.h"

@implementation GLTexture
@end

@implementation GLTextureRGB
- (void)dealloc
{
    if (_RGBA) {
        free(_RGBA);
        _RGBA = NULL;
    }
}
@end

@implementation GLTextureYUV
- (void)dealloc
{
    if (_Y) {
        free(_Y);
        _Y = NULL;
    }
    
    if (_U) {
        free(_U);
        _U = NULL;
    }
    
    if (_V) {
        free(_V);
        _V = NULL;
    }
}
@end
