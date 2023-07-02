# opencv_video_stream

网络摄像头提供的RTSP视频流，无法直接在网页上播放，opencv_video_stream用于将RTSP流转换成HTTP流在网页上播放。



### 其他方案：

1. 使用ffmpeg将RTSP转换成RTMP/FLV
2. 使用webrtc-stream将RTSP流转成webrtc



### 本方案的优点：

1. 编解码速度快
2. 实时性比较好

### 本方案的缺点：

1. 网络带宽占用高



### 编译：

1. 安装opencv并设置OpenCV_DIR环境变量
2. CMake编译

### 测试：

在终端运行：

```powershell
 .\opencv_video_stream.exe rtsp://127.0.0.1:8554/stream /test
```

网页视频打开：http://127.0.0.1:8897/test



**注：**

如果没有可用的RTSP测试地址，可以使用ffmpeg将本地视频文件推流到RTSP服务器。具体步骤：

1. 下载RTSP服务器*[MediaMTX](https://github.com/bluenviron/mediamtx)* 并运行，默认监听8554端口
2. 使用ffmpeg将视频文件推流到MediaMTX

```powershell
ffmpeg.exe -re -i big_buck_bunny_720p_30mb.mp4 -c copy -rtsp_transport tcp -f rtsp rtsp://127.0.0.1:8554/stream
```

3. RTSP流`rtsp://127.0.0.1:8554/stream`可以通过VLC播放器播放，验证推流成功
