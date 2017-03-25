//
//  ViewController.m
//  OpenGLES11-相机视频渲染
//
//  Created by mac on 17/3/24.
//  Copyright © 2017年 Qinmin. All rights reserved.
//

#import "ViewController.h"
#import <AVFoundation/AVFoundation.h>
#import "libyuv.h"
#import "OpenGLESView.h"

@interface ViewController () <AVCaptureVideoDataOutputSampleBufferDelegate>

@property (nonatomic, strong) AVCaptureSession *captureSession;
@property (nonatomic, strong) AVCaptureDeviceInput *videoInput;
@property (nonatomic, strong) AVCaptureVideoDataOutput *videoOutput;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    OpenGLESView *glView = [[OpenGLESView alloc] initWithFrame:self.view.frame];
    GLRender *render = [[GLRenderRGB alloc] init];
    [glView setRender:render];
    self.view = glView;
    
    UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(0, 30, 120, 30)];
    [btn addTarget:self action:@selector(startBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [btn setTitle:@"开始" forState:UIControlStateNormal];
    [btn setBackgroundColor:[UIColor greenColor]];
    [self.view addSubview:btn];
    
    [self setupSession];
}

- (void)setupSession
{
    _captureSession = [[AVCaptureSession alloc] init];
    [_captureSession beginConfiguration];
    
    // 设置换面尺寸
    [_captureSession setSessionPreset:AVCaptureSessionPreset640x480];
    
    // 设置输入设备
    AVCaptureDevice *inputCamera = nil;
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == AVCaptureDevicePositionBack)
        {
            inputCamera = device;
        }
    }
    
    if (!inputCamera) {
        return;
    }
    
    NSError *error = nil;
    _videoInput = [[AVCaptureDeviceInput alloc] initWithDevice:inputCamera error:&error];
    if ([_captureSession canAddInput:_videoInput])
    {
        [_captureSession addInput:_videoInput];
    }
    
    // 设置输出数据
    _videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    [_videoOutput setAlwaysDiscardsLateVideoFrames:NO];
    
    [_videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    [_videoOutput setSampleBufferDelegate:self queue:dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)];
    
    
    if ([_captureSession canAddOutput:_videoOutput]) {
        [_captureSession addOutput:_videoOutput];
    }
    [_captureSession commitConfiguration];
}

// 视频格式为：kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange或kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
- (void)processVideoSampleBufferToYUV:(CMSampleBufferRef)sampleBuffer
{
    //CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    
    //表示开始操作数据
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    int pixelWidth = (int) CVPixelBufferGetWidth(pixelBuffer);
    int pixelHeight = (int) CVPixelBufferGetHeight(pixelBuffer);
    
    GLTextureYUV *yuv = [[GLTextureYUV alloc] init];
    yuv.width = pixelWidth;
    yuv.height = pixelHeight;
    
    //size_t count = CVPixelBufferGetPlaneCount(pixelBuffer);
    //获取CVImageBufferRef中的y数据
    size_t y_size = pixelWidth * pixelHeight;
    uint8_t *yuv_frame = malloc(y_size);
    uint8_t *y_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    memcpy(yuv_frame, y_frame, y_size);
    yuv.Y = yuv_frame;
    
    // UV数据
    uint8_t *uv_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    size_t uv_size = y_size/2;
    
    //获取CMVImageBufferRef中的u数据
    size_t u_size = y_size/4;
    uint8_t *u_frame = malloc(u_size);
    for (int i = 0, j = 0; i < uv_size; i += 2, j++) {
        u_frame[j] = uv_frame[i];
    }
    yuv.U = u_frame;
    
    //获取CMVImageBufferRef中的v数据
    size_t v_size = y_size/4;
    uint8_t *v_frame = malloc(v_size);
    for (int i = 1, j = 0; i < uv_size; i += 2, j++) {
        v_frame[j] = uv_frame[i];
    }
    yuv.V = v_frame;
    
    // Unlock
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        OpenGLESView *glView = (OpenGLESView *)self.view;
        [glView setTexture:yuv];
        [glView setNeedDraw];
    });
}

// 视频格式为：kCVPixelFormatType_32BGRA
- (void)processVideoSampleBufferToRGB:(CMSampleBufferRef)sampleBuffer
{
    //CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    //size_t count = CVPixelBufferGetPlaneCount(pixelBuffer);
    //printf("%zud\n", count);
    
    //表示开始操作数据
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    int pixelWidth = (int) CVPixelBufferGetWidth(pixelBuffer);
    int pixelHeight = (int) CVPixelBufferGetHeight(pixelBuffer);
    
    GLTextureRGB *rgb = [[GLTextureRGB alloc] init];
    rgb.width = pixelWidth;
    rgb.height = pixelHeight;
    
    // BGRA数据
    //size_t y_size = pixelWidth * pixelHeight;
    uint8_t *frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    
    uint8_t *bgra = malloc(pixelHeight * pixelWidth * 4);
    memcpy(bgra, frame, pixelHeight * pixelWidth * 4);
    
    rgb.RGBA = bgra;
    
    // Unlock
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        OpenGLESView *glView = (OpenGLESView *)self.view;
        [glView setTexture:rgb];
        [glView setNeedDraw];
    });
}

// 视频格式为：kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange或kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
- (void)processVideoSampleBufferToRGB1:(CMSampleBufferRef)sampleBuffer
{
    //CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    //size_t count = CVPixelBufferGetPlaneCount(pixelBuffer);
    //printf("%zud\n", count);
    
    //表示开始操作数据
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    int pixelWidth = (int) CVPixelBufferGetWidth(pixelBuffer);
    int pixelHeight = (int) CVPixelBufferGetHeight(pixelBuffer);
    
    GLTextureRGB *rgb = [[GLTextureRGB alloc] init];
    rgb.width = pixelWidth;
    rgb.height = pixelHeight;
    
    // Y数据
    //size_t y_size = pixelWidth * pixelHeight;
    uint8_t *y_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    
    // UV数据
    uint8_t *uv_frame = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    //size_t uv_size = y_size/2;
    
    // ARGB = BGRA 大小端问题 转换出来的数据是BGRA
    uint8_t *bgra = malloc(pixelHeight * pixelWidth * 4);
    NV12ToARGB(y_frame, pixelWidth, uv_frame, pixelWidth, bgra, pixelWidth * 4, pixelWidth, pixelHeight);
    
    rgb.RGBA = bgra;
    
    // Unlock
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        OpenGLESView *glView = (OpenGLESView *)self.view;
        [glView setTexture:rgb];
        [glView setNeedDraw];
    });
}

#pragma mark - Action
- (IBAction)startBtnClick:(UIButton *)sender
{
    if (![_captureSession isRunning]) {
        [_captureSession startRunning];
    }
}

#pragma mark - <AVCaptureVideoDataOutputSampleBufferDelegate>
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if (!self.captureSession.isRunning) {
        return;
    }else if (captureOutput == _videoOutput) {
        OpenGLESView *glView = (OpenGLESView *)self.view;
        if ([glView.render isMemberOfClass:[GLRenderRGB class]]) {
             [self processVideoSampleBufferToRGB1:sampleBuffer];
        }else {
            [self processVideoSampleBufferToYUV:sampleBuffer];
        }
    }
}

@end
