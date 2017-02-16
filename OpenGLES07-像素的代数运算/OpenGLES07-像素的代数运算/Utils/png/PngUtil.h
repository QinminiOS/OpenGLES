//
//  PngUtil.h
//  OpenGLES06-图像膨胀腐蚀模糊
//
//  Created by mac on 17/2/14.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#ifndef PngUtil_h
#define PngUtil_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 PNG ALPHA
 */
#define PNG_HAVE_ALPHA 1
#define PNG_NO_ALPHA 0

/**
 PNG info
 */
typedef struct {
    int width, height; /* 尺寸 */
    int bit_depth;  /* 位深 */
    int flag;   /* 一个标志，表示是否有alpha通道 */
    unsigned char **rgba; /* 图片数组;r=rgba[0];g=rgba[1];b=rgba[2] */
} pic_data;

/**
 read png file

 @param filepath png file path
 @param out      png info

 @return success or not
 */
int read_png_file(const char *filepath, pic_data *out);

    
/**
 write png file

 @param file_name png file name
 @param graph     png data to write

 @return success or not
 */
int write_png_file(const char *filename , pic_data *graph);
    
#ifdef __cplusplus
}
#endif
#endif /* PngUtil_h */
