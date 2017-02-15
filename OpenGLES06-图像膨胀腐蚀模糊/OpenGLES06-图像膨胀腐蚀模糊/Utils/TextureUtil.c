//
//  GLTextureUtil.c
//  OpenGLES06-图像膨胀腐蚀模糊
//
//  Created by qinmin on 2017/2/13.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#include "TextureUtil.h"
#include <string.h>

#define TextureHeaderReadSize 12

int is_png(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 8)
    {
        return 0;
    }
    
    static const unsigned char PNG_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
    
    return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}

int is_jpeg(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 4)
    {
        return 0;
    }
    
    static const unsigned char JPG_SOI[] = {0xFF, 0xD8};
    
    return memcmp(data, JPG_SOI, 2) == 0;
}

int is_tiff(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 4)
    {
        return 0;
    }
    
    static const char* TIFF_II = "II";
    static const char* TIFF_MM = "MM";
    
    return (memcmp(data, TIFF_II, 2) == 0 && *((const unsigned char*)(data) + 2) == 42 && *((const unsigned char*)(data) + 3) == 0) ||
    (memcmp(data, TIFF_MM, 2) == 0 && *((const unsigned char*)(data) + 2) == 0 && *((const unsigned char*)(data) + 3) == 42);
}

int is_webp(const unsigned char * data, ssize_t dataLen)
{
    if (dataLen <= 12)
    {
        return 0;
    }
    
    static const char* WEBP_RIFF = "RIFF";
    static const char* WEBP_WEBP = "WEBP";
    
    return memcmp(data, WEBP_RIFF, 4) == 0
    && memcmp((const unsigned char*)(data) + 8, WEBP_WEBP, 4) == 0;
}

int read_texture_file(const char *file, TextureInfo *info)
{
    if (info == NULL) {
        return -1;
    }
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        return -1;
    }
    unsigned char buffer[TextureHeaderReadSize] = {0};
    fread(buffer, 1, TextureHeaderReadSize, fp);
    fclose(fp);
    
    if (is_png(buffer, sizeof(buffer))) {
        pic_data png_data;
        read_png_file(file, &png_data);
        info->width = png_data.width;
        info->height = png_data.height;
        info->bit_depth = png_data.bit_depth;
        info->flag = png_data.flag;
        int depth = png_data.flag == PNG_HAVE_ALPHA ? 4 : 3;
        info->rgba = (unsigned char*)malloc(sizeof(unsigned char)*png_data.width*png_data.height*depth);

        for (int i = 0, j = 0; i < png_data.width*png_data.height*depth; i++, j+= depth) {
            info->rgba[j] = png_data.rgba[i][0];
            info->rgba[j+1] = png_data.rgba[i][1];
            info->rgba[j+2] = png_data.rgba[i][2];
            if(png_data.flag == PNG_HAVE_ALPHA) {
                info->rgba[j+3] = png_data.rgba[i][3];
            }
        }
        //free
        if (png_data.rgba) {
            free(png_data.rgba);
            png_data.rgba = NULL;
        }
    }else if (is_jpeg(buffer, sizeof(buffer))) {
        int size = 0;
        info->flag = PNG_NO_ALPHA;
        info->bit_depth = sizeof(char);
        read_jpeg_file(file, &info->rgba, &size, &(info->width), &(info->height));
    }
    
    return 0;
}


