//
//  OpenGLESView.m
//  OpenGLES01-环境搭建
//
//  Created by qinmin on 2017/2/9.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#import "OpenGLESView.h"
#import <OpenGLES/ES2/gl.h>
#import "GLUtil.h"
#include "JpegUtil.h"

@interface OpenGLESView ()
{
    CAEAGLLayer     *_eaglLayer;
    EAGLContext     *_context;
    GLuint          _colorRenderBuffer;
    GLuint          _frameBuffer;
    GLuint          _frameBuffer1;

    GLuint          _program;
    GLuint          _program1;
    GLuint          _vbo;
    GLuint          _vbo1;
    GLuint          _texture;
    GLuint          _texture1;
    int             _vertCount;
}
@end

@implementation OpenGLESView

+ (Class)layerClass
{
    // 只有 [CAEAGLLayer class] 类型的 layer 才支持在其上描绘 OpenGL 内容。
    return [CAEAGLLayer class];
}

- (void)dealloc
{
    glDeleteBuffers(1, &_vbo);
    glDeleteTextures(1, &_texture);
    glDeleteProgram(_program);
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setupLayer];
        [self setupContext];
        [self setupGLProgram];
        [self setupGLProgram1];
    }
    return self;
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:_context];
    
    [self destoryRenderAndFrameBuffer];
    
    [self setupFrameAndRenderBuffer];
    
    [self setupFrameBuffer1];
    
    [self render];
}


#pragma mark - Setup
- (void)setupLayer
{
    _eaglLayer = (CAEAGLLayer*) self.layer;
    
    // CALayer 默认是透明的，必须将它设为不透明才能让其可见
    _eaglLayer.opaque = YES;
    
    // 设置描绘属性，在这里设置不维持渲染内容以及颜色格式为 RGBA8
    _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}

- (void)setupContext
{
    // 设置OpenGLES的版本为2.0 当然还可以选择1.0和最新的3.0的版本，以后我们会讲到2.0与3.0的差异，目前为了兼容性选择2.0的版本
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    // 将当前上下文设置为我们创建的上下文
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}

- (void)setupFrameAndRenderBuffer
{
    // 窗口默认帧缓存
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    // 为 color renderbuffer 分配存储空间
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    
    glGenFramebuffers(1, &_frameBuffer);
    // 设置为当前 framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    // 将 _colorRenderBuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
}

- (void)setupFrameBuffer1
{
    _texture1 = createTexture2D(GL_RGBA, self.frame.size.width, self.frame.size.height, NULL);
    glGenFramebuffers(1, &_frameBuffer1);
    // 设置为当前 framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture1, 0);
}

- (void)setupGLProgram
{
    NSString *vertFile1 = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile1 = [[NSBundle mainBundle] pathForResource:@"mosaic.glsl" ofType:nil];
    _program = createGLProgramFromFile(vertFile1.UTF8String, fragFile1.UTF8String);
}

- (void)setupGLProgram1
{
    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"frag.glsl" ofType:nil];
    _program1 = createGLProgramFromFile(vertFile.UTF8String, fragFile.UTF8String);
}

- (void)setupVBO
{
    _vertCount = 6;
    
    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   // 右上
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f   // 左上
    };
    
    // 创建VBO
    _vbo = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
    
    glEnableVertexAttribArray(glGetAttribLocation(_program, "position"));
    glVertexAttribPointer(glGetAttribLocation(_program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    
    glEnableVertexAttribArray(glGetAttribLocation(_program, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(_program, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
}

- (void)setupVBO1
{
    _vertCount = 6;
    
    GLfloat vertices[] = {
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   // 右上
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // 右下
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // 左下
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    
    // 创建VBO
    _vbo1 = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
    
    glEnableVertexAttribArray(glGetAttribLocation(_program1, "position"));
    glVertexAttribPointer(glGetAttribLocation(_program1, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    
    glEnableVertexAttribArray(glGetAttribLocation(_program1, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(_program1, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
}

- (void)setupTexure
{
    NSString *path = [[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"];
    
    unsigned char *data;
    int size;
    int width;
    int height;
    
    // 加载纹理
    if (read_jpeg_file(path.UTF8String, &data, &size, &width, &height) < 0) {
        printf("%s\n", "decode fail");
    }
    
    // 创建纹理
    _texture = createTexture2D(GL_RGB, width, height, data);
    
    if (data) {
        free(data);
        data = NULL;
    }
}

#pragma mark - Clean
- (void)destoryRenderAndFrameBuffer
{
    glDeleteFramebuffers(1, &_frameBuffer);
    _frameBuffer = 0;
    glDeleteRenderbuffers(1, &_colorRenderBuffer);
    _colorRenderBuffer = 0;
}

#pragma mark - Render
- (void)render
{
    /************************离屏渲染********************************************/
    glUseProgram(_program1);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer1);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    [self setupVBO1];
    [self setupTexure];
    
    // 激活纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(glGetUniformLocation(_program1, "image"), 0);
    
    // 索引数组
    unsigned int indices1[] = {0,1,2,3,2,0};
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices1);
    
    /************************当前屏幕渲染********************************************/
    glUseProgram(_program);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    [self setupVBO];
    
    // 激活纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture1);
    glUniform1i(glGetUniformLocation(_program, "image"), 1);
    
    // 索引数组
    unsigned int indices[] = {0,1,2,3,2,0};
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
    
    //将指定 renderbuffer 呈现在屏幕上，在这里我们指定的是前面已经绑定为当前 renderbuffer 的那个，在 renderbuffer 可以被呈现之前，必须调用renderbufferStorage:fromDrawable: 为之分配存储空间。
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
