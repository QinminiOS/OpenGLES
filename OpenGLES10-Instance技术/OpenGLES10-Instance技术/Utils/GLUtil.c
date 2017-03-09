//
//  GLUtil.c
//  GLKit
//
//  Created by qinmin on 2017/1/4.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#include "GLUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long getFileContent(char *buffer, long len, const char *filePath)
{
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    
    if (len < size) {
        GLlog("file is large than the size(%ld) you give\n", len);
        return -1;
    }
    
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    
    fclose(file);
    
    return size;
}

static GLuint createGLShader(const char *shaderText, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);
    
    int compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            if (infoLog) {
                glGetShaderInfoLog (shader, infoLen, NULL, infoLog);
                GLlog("Error compiling shader: %s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLuint createGLProgram(const char *vertext, const char *frag)
{
    GLuint program = glCreateProgram();
    
    GLuint vertShader = createGLShader(vertext, GL_VERTEX_SHADER);
    GLuint fragShader = createGLShader(frag, GL_FRAGMENT_SHADER);
    
    if (vertShader == 0 || fragShader == 0) {
        return 0;
    }
    
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            GLchar *infoText = (GLchar *)malloc(sizeof(GLchar)*infoLen + 1);
            if (infoText) {
                memset(infoText, 0x00, sizeof(GLchar)*infoLen + 1);
                glGetProgramInfoLog(program, infoLen, NULL, infoText);
                GLlog("%s", infoText);
                free(infoText);
            }
        }
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
    }
    
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    return program;
}

GLuint createGLProgramFromFile(const char *vertextPath, const char *fragPath)
{
    char vBuffer[2048] = {0};
    char fBuffer[2048] = {0};
    
    if (getFileContent(vBuffer, sizeof(vBuffer), vertextPath) < 0) {
        return 0;
    }
    
    if (getFileContent(fBuffer, sizeof(fBuffer), fragPath) < 0) {
        return 0;
    }
    
    return createGLProgram(vBuffer, fBuffer);
}

GLuint createVBO(GLenum target, int usage, int datSize, void *data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(target, vbo);
    glBufferData(target, datSize, data, usage);
    return vbo;
}

GLuint createTexture2D(GLenum format, int width, int height, void *data)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint createVAO(void(*setting)())
{
#ifdef GL_ES_VERSION_3_0
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    if (setting) {
        setting();
    }
    glBindVertexArray(0);
    return vao;
#endif

#if GL_OES_vertex_array_object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    if (setting) {
        setting();
    }
    glBindVertexArray(0);
    return vao;
#endif
    
    return 0;
}
