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

@interface OpenGLESView ()
{
    CAEAGLLayer     *_eaglLayer;
    EAGLContext     *_context;
    GLuint          _colorRenderBuffer;
    GLuint          _frameBuffer;

    GLRender        *_render;
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
    
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setupLayer];
        [self setupContext];
    }
    return self;
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:_context];
    
    [self destoryRenderAndFrameBuffer];
    
    [self setupFrameAndRenderBuffer];
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

#pragma mark - Clean
- (void)destoryRenderAndFrameBuffer
{
    glDeleteFramebuffers(1, &_frameBuffer);
    _frameBuffer = 0;
    glDeleteRenderbuffers(1, &_colorRenderBuffer);
    _colorRenderBuffer = 0;
}

#pragma mark - Render
- (void)draw
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    // 绘制
    [_render prepareRender];
    
    //将指定 renderbuffer 呈现在屏幕上，在这里我们指定的是前面已经绑定为当前 renderbuffer 的那个，在 renderbuffer 可以被呈现之前，必须调用renderbufferStorage:fromDrawable: 为之分配存储空间。
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

#pragma mark - PublicMethod
- (void)setRender:(GLRender *)render
{
    _render = render;
}

- (void)setTexture:(GLTexture *)texture
{
    [_render setTexture:texture];
}

- (void)setNeedDraw
{
    [self draw];
}

@end
