//
//  GLTextureUtil.h
//  OpenGLES06-图像膨胀腐蚀模糊
//
//  Created by qinmin on 2017/2/13.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#ifndef TextureUtil_h
#define TextureUtil_h

#include "PngUtil.h"
#include "JpegUtil.h"

typedef struct {
    int width, height; /* 尺寸 */
    int bit_depth;  /* 位深 */
    int flag;   /* 一个标志，表示是否有alpha通道 */
    unsigned char *rgba; /* 图片数组;r=rgba[0];g=rgba[1];b=rgba[2] */
} TextureInfo;


/**
 read texture file

 @param file file path
 @param info file info

 @return success or not
 */
int read_texture_file(const char *file, TextureInfo *info);

/**
 is png file

 @param data    file data
 @param dataLen file length

 @return 1 true 0 not true
 */
int is_png(const unsigned char * data, ssize_t dataLen);


/**
 is jpeg file

 @param data    file data
 @param dataLen file length

 @return 1 true 0 not true
 */
int is_jpeg(const unsigned char * data, ssize_t dataLen);


/**
 is tiff file

 @param data    file data
 @param dataLen file length

 @return 1 true 0 not true
 */
int is_tiff(const unsigned char * data, ssize_t dataLen);


/**
 is webp file

 @param data    file data
 @param dataLen file length

 @return 1 true 0 not true
 */
int is_webp(const unsigned char * data, ssize_t dataLen);

#endif /* GLTextureUtil_h */
