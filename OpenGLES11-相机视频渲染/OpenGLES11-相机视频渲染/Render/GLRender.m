//
//  GLRGBRender.m
//  OpenGLES11-相机视频渲染
//
//  Created by mac on 17/3/24.
//  Copyright © 2017年 Qinmin. All rights reserved.
//

#import "GLRender.h"

////////////////GLRender//////////////////////////
@implementation GLRender
- (void)setupGLProgram
{
}

- (void)setTexture:(GLTexture *)texture
{
}

- (void)prepareRender
{
}
@end

////////////////GLRenderRGB//////////////////////////
@implementation GLRenderRGB

- (instancetype)init
{
    if (self = [super init]) {
        [self setupGLProgram];
        [self setupVBO];
        
        // 这里宽高设置死了，但是可以动态设置
        _rgb = createTexture2D(GL_RGBA, 640, 480, NULL);
    }
    return self;
}

- (void)setupGLProgram
{
    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"frag_rgb.glsl" ofType:nil];
    
    self.program = createGLProgramFromFile(vertFile.UTF8String, fragFile.UTF8String);
    glUseProgram(self.program);
}

- (void)setupVBO
{
    self.vertCount = 6;
    
    GLfloat vertices[] = {
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
        0.8f, -0.6f, 0.0f, 1.0f, 1.0f,   // 右下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f,  0.6f, 0.0f, 0.0f, 0.0f,  // 左上
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
    };
    
    // 创建VBO
    self.vertexVBO = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
}

- (void)setTexture:(GLTexture *)texture
{
    if ([texture isMemberOfClass:[GLTextureRGB class]]) {
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        GLTextureRGB *rgbTexture = (GLTextureRGB *)texture;
        glBindTexture(GL_TEXTURE_2D, _rgb);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, rgbTexture.RGBA);
    }
}

- (void)prepareRender
{
    glBindBuffer(GL_ARRAY_BUFFER, self.vertexVBO);
    glEnableVertexAttribArray(glGetAttribLocation(self.program, "position"));
    glVertexAttribPointer(glGetAttribLocation(self.program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    
    glEnableVertexAttribArray(glGetAttribLocation(self.program, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(self.program, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _rgb);
    glUniform1i(glGetUniformLocation(self.program, "image0"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, self.vertCount);
}

@end

////////////////GLRenderYUV//////////////////////////
@implementation GLRenderYUV
- (instancetype)init
{
    if (self = [super init]) {
        [self setupGLProgram];
        [self setupVBO];
        
        // 这里宽高设置死了，但是可以动态设置
        _y = createTexture2D(GL_LUMINANCE, 640, 480, NULL);
        _u = createTexture2D(GL_LUMINANCE, 640/2, 480/2, NULL);
        _v = createTexture2D(GL_LUMINANCE, 640/2, 480/2, NULL);
    }
    return self;
}

- (void)setupGLProgram
{
    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"frag.glsl" ofType:nil];
    
    self.program = createGLProgramFromFile(vertFile.UTF8String, fragFile.UTF8String);
    glUseProgram(self.program);
}

- (void)setupVBO
{
    self.vertCount = 6;
    
    GLfloat vertices[] = {
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
        0.8f, -0.6f, 0.0f, 1.0f, 1.0f,   // 右下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f,  0.6f, 0.0f, 0.0f, 0.0f,  // 左上
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
    };
    
    // 创建VBO
    self.vertexVBO = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
}

- (void)setTexture:(GLTexture *)texture
{
    if ([texture isMemberOfClass:[GLTextureYUV class]]) {
        GLTextureYUV *rgbTexture = (GLTextureYUV *)texture;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glBindTexture(GL_TEXTURE_2D, _y);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width, texture.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgbTexture.Y);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glBindTexture(GL_TEXTURE_2D, _u);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width/2, texture.height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgbTexture.U);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glBindTexture(GL_TEXTURE_2D, _v);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width/2, texture.height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgbTexture.V);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

- (void)prepareRender
{
    glBindBuffer(GL_ARRAY_BUFFER, self.vertexVBO);
    glEnableVertexAttribArray(glGetAttribLocation(self.program, "position"));
    glVertexAttribPointer(glGetAttribLocation(self.program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    
    glEnableVertexAttribArray(glGetAttribLocation(self.program, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(self.program, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _y);
    glUniform1i(glGetUniformLocation(self.program, "image0"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _u);
    glUniform1i(glGetUniformLocation(self.program, "image1"), 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _v);
    glUniform1i(glGetUniformLocation(self.program, "image2"), 2);
    
    glDrawArrays(GL_TRIANGLES, 0, self.vertCount);
}
@end
