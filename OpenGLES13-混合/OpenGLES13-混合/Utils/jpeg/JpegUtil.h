//
//  JpegUtil.h
//  OpenGLES05-纹理贴图
//
//  Created by qinmin on 2017/2/13.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#ifndef JpegUtil_h
#define JpegUtil_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

/**
 read jpeg file

 @param jpegfile jpeg file path
 @param data jpeg color data
 @param size jpeg color data size
 @param width jpeg file width
 @param height jpeg file height
 @return decode success or not
 */
int read_jpeg_file(const char* jpegfile,
                   unsigned char** data,
                   int* size,
                   int* width,
                   int* height);


/**
 write jpeg file

 @param jpegfile write file path
 @param data rgb data
 @param width file width
 @param height file height
 @param quality image quality
 @return write success or not
 */
int write_jpeg_file(const char* jpegfile,
                    unsigned char* data,
                    int width,
                    int height,
                    int quality);
    
#ifdef __cplusplus
}
#endif
    
#endif /* JpegUtil_h */
