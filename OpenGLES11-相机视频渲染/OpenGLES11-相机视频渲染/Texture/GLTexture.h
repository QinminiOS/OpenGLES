//
//  GLTexture.h
//  OpenGLES11-相机视频渲染
//
//  Created by mac on 17/3/24.
//  Copyright © 2017年 Qinmin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GLTexture : NSObject
@property (assign, nonatomic) int width;
@property (assign, nonatomic) int height;
@end

@interface GLTextureRGB : GLTexture
@property (nonatomic, assign) uint8_t *RGBA;
@end

@interface GLTextureYUV : GLTexture
@property (nonatomic, assign) uint8_t *Y;
@property (nonatomic, assign) uint8_t *U;
@property (nonatomic, assign) uint8_t *V;
@end
