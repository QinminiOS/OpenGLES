//
//  NSObject+GLES.h
//  OpenGLES04-缓存技术
//
//  Created by qinmin on 2017/2/11.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (GLES)

+ (UIImage *)imageFromRGBA:(unsigned char *)buffer
                     width:(int) width
                    height:(int) height;

@end
