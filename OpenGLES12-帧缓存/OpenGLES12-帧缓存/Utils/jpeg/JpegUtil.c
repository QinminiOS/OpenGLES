//
//  JpegUtil.c
//  OpenGLES05-纹理贴图
//
//  Created by qinmin on 2017/2/13.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#include "JpegUtil.h"
#include "jpeglib.h"
#include "jerror.h"

struct my_error_mgr
{
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;  /* for return to caller */
};

typedef struct my_error_mgr* my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    
    (*cinfo->err->output_message) (cinfo);
    
    longjmp(myerr->setjmp_buffer, 1);
}

int read_jpeg_file(const char* jpegfile,
                   unsigned char** data,
                   int* size,
                   int* width,
                   int* height)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE* fp;
    
    JSAMPARRAY buffer;
    int row_stride = 0;
    unsigned char* tmp_buffer = NULL;
    int rgb_size;
    
    fp = fopen(jpegfile, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpegfile);
        return -1;
    }
    
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return -1;
    }
    
    jpeg_create_decompress(&cinfo);
    
    jpeg_stdio_src(&cinfo, fp);
    
    jpeg_read_header(&cinfo, TRUE);
    
    // we only support RGB or grayscale
    if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
        cinfo.out_color_space = JCS_GRAYSCALE;
    }else {
        cinfo.out_color_space = JCS_RGB;
    }
    
    jpeg_start_decompress(&cinfo);
    
    row_stride = cinfo.output_width * cinfo.output_components;
    *width = cinfo.output_width;
    *height = cinfo.output_height;
    
    rgb_size = row_stride * cinfo.output_height; // 总大小
    *size = rgb_size;
    
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    *data = (unsigned char *)malloc(sizeof(char) * rgb_size);    // 分配总内存
    
    printf("jpeg debug:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n",
           rgb_size,
           cinfo.image_width*cinfo.image_height*3,
           cinfo.image_width,
           cinfo.image_height,
           row_stride);
    
    tmp_buffer = *data;
    while (cinfo.output_scanline < cinfo.output_height) // 解压每一行
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // 复制到内存
        memcpy(tmp_buffer, buffer[0], row_stride);
        tmp_buffer += row_stride;
    }
    
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    fclose(fp);
    
    return 0;
}

int write_jpeg_file(const char* jpegfile,
                    unsigned char* data,
                    int width,
                    int height,
                    int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int row_stride = 0;
    FILE* fp = NULL;
    JSAMPROW row_pointer[1];
    
    cinfo.err = jpeg_std_error(&jerr);
    
    jpeg_create_compress(&cinfo);
    fp = fopen(jpegfile, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpegfile);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, 1);  // todo 1 == true
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = width * cinfo.input_components;
    
    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &data[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);
    
    return 0;
}
