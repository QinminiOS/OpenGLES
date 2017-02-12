//
//  GLUtil.h
//  GLKit
//
//  Created by qinmin on 2017/1/4.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#ifndef GLUtil_h
#define GLUtil_h

#ifdef __cplusplus
extern "C" {
#endif

#define GLES_PLATFORM_IOS           1
#define GLES_PLATFORM_ANDROID       2
    
// iOS
#if defined(__APPLE__) && !defined(ANDROID)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#undef  GLES_TARGET_PLATFORM
#define GLES_TARGET_PLATFORM         GLES_PLATFORM_IOS
#endif
#endif
    
// android
#if defined(ANDROID)
#undef  GLES_TARGET_PLATFORM
#define GLES_TARGET_PLATFORM         GLES_PLATFORM_ANDROID
#endif
 
#if GLES_TARGET_PLATFORM == GLES_PLATFORM_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <stdio.h>
#define GLlog(format,...)           printf(format,__VA_ARGS__)
#endif


/**
 get the content of a file

 @param buffer buffer to receive file data
 @param len buffer alloc size
 @param filePath file path
 @return the size of file content
 */
long getFileContent(char *buffer, long len, const char *filePath);
    
/**
 create opengles program

 @param vertext vertext string
 @param frag frag string
 @return opengles program id
 */
GLuint createGLProgram(const char *vertext, const char *frag);
 

/**
 create opengles program

 @param vertextPath vertext path
 @param fragPath frag path
 @return opengles program id
 */
GLuint createGLProgramFromFile(const char *vertextPath, const char *fragPath);
  

/**
 create VBO object

 @param target GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
 @param usage GL_***_DRAW
 @param datSize data len
 @param data data ptr
 @return vbo id
 */
GLuint createVBO(GLenum target, int usage, int datSize, void *data);


/**
 create Texture2D Buffer

 @param format RGB GRBA ...
 @param width texture width
 @param height texture height
 @param data texture data
 @return texture buffer id
 */
GLuint createTexture2D(GLenum format, int width, int height, void *data);
  

/**
 create VAO object

 @param setting your setting
 @return vao id
 */
GLuint createVAO(void(*setting)());

    
#ifdef __cplusplus
}
#endif
#endif /* GLUtil_h */
