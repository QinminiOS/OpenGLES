//
//  GLTextureUtil.c
//  OpenGLES06-图像膨胀腐蚀模糊
//
//  Created by qinmin on 2017/2/13.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#include "TextureUtil.h"
#include <string.h>

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


