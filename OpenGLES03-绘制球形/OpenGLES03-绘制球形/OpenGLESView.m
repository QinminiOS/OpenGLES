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

typedef struct {
    GLfloat x,y,z;
    GLfloat r,g,b;
} Vertex;

@interface OpenGLESView ()
{
    CAEAGLLayer     *_eaglLayer;
    EAGLContext     *_context;
    GLuint          _colorRenderBuffer;
    GLuint          _frameBuffer;

    GLuint          _program;
    int             _vertCount;
}
@end

@implementation OpenGLESView

+ (Class)layerClass
{
    // 只有 [CAEAGLLayer class] 类型的 layer 才支持在其上描绘 OpenGL 内容。
    return [CAEAGLLayer class];
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setupLayer];
        [self setupContext];
        [self setupGLProgram];
    }
    return self;
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:_context];
    
    [self destoryRenderAndFrameBuffer];
    
    [self setupFrameAndRenderBuffer];
    
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

- (void)setupGLProgram
{
    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"frag.glsl" ofType:nil];
    _program = createGLProgramFromFile(vertFile.UTF8String, fragFile.UTF8String);
    
    glUseProgram(_program);
}

//- (void)setGLData
//{
//    _vertCount = 100;
//    Vertex *vertext = (Vertex *)malloc(sizeof(Vertex) * (_vertCount + 1));
//    memset(vertext, 0x00, sizeof(Vertex) * (_vertCount + 1));
//    
//    float a = 0.8;
//    float b = a * self.frame.size.width / self.frame.size.height;
//    
//    float delta = 2.0*M_PI/_vertCount;
//    for (int i = 0; i < _vertCount; i++) {
//        GLfloat x = a * cos(delta * i);
//        GLfloat y = b * sin(delta * i);
//        GLfloat z = 0.0;
//        vertext[i] = (Vertex){x, y, z, x, y, x+y};
//        
//        printf("%f , %f\n", x, y);
//    }
//    
//    GLuint vbo = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(Vertex) * (_vertCount + 1), vertext);
//    
//    free(vertext);
//    vertext = NULL;
//    
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(glGetAttribLocation(_program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
//    
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(glGetAttribLocation(_program, "color"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL+sizeof(GLfloat)*3);
//}

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
    glClearColor(1.0, 1.0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    _vertCount = 100; // 分割份数
    Vertex *vertext = (Vertex *)malloc(sizeof(Vertex) * _vertCount);
    memset(vertext, 0x00, sizeof(Vertex) * _vertCount);

    float a = 0.8; // 水平方向的半径
    float b = a * self.frame.size.width / self.frame.size.height;

    float delta = 2.0*M_PI/_vertCount;
    for (int i = 0; i < _vertCount; i++) {
        GLfloat x = a * cos(delta * i);
        GLfloat y = b * sin(delta * i);
        GLfloat z = 0.0;
        vertext[i] = (Vertex){x, y, z, x, y, x+y};

        printf("%f , %f\n", x, y);
    }

    glEnableVertexAttribArray(glGetAttribLocation(_program, "position"));
    glVertexAttribPointer(glGetAttribLocation(_program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), vertext);

    glEnableVertexAttribArray(glGetAttribLocation(_program, "color"));
    glVertexAttribPointer(glGetAttribLocation(_program, "color"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), vertext+sizeof(GLfloat)*3);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, _vertCount);
    
    //将指定 renderbuffer 呈现在屏幕上，在这里我们指定的是前面已经绑定为当前 renderbuffer 的那个，在 renderbuffer 可以被呈现之前，必须调用renderbufferStorage:fromDrawable: 为之分配存储空间。
    [_context presentRenderbuffer:GL_RENDERBUFFER];
    
    free(vertext);
    vertext = NULL;
}

@end
