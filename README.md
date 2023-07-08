# opencv_video_stream

网络摄像头提供的RTSP视频流，无法直接在网页上播放，opencv_video_stream用于将RTSP流转换成MJPEG流在网页上播放。

### 优点：

1. 编解码速度快，资源占用低
2. 实时性比较好

### 缺点：

1. 网络带宽占用高

### 编译：

1. 安装opencv并设置OpenCV_DIR环境变量
2. CMake编译

### 使用：

终端使用：

```powershell

  Streaming a video source to MJPEG, then play on the web.

  OPTIONS:

      -h, --help                        Display this help menu
      -s[string], --source=[string]     The video source url. For example,
                                        rtsp://192.168.2.3:8554/live
      -d[string], --endpoint=[string]   The endpoint HTTP link suffix
      --width=[integer]                 The output/streaming video width.
      --height=[integer]                The output/streaming video height.
      -q[integer], --quality=[integer]  The jpeg image quality, 0-100.
```



在终端运行：

```powershell
.\opencv_video_stream.exe -s rtsp://192.168.2.3:8554/live -d tt -q 50 --width 320 --height 240
```

网页视频打开：http://127.0.0.1:8897/test

或者新建一个网页如下：

```html
<html>
<body>
  <img src="http://127.0.0.1:8897/test">
</body>
</html>
```

**注：**

如果没有可用的RTSP测试地址，可以使用ffmpeg将本地视频文件推流到RTSP服务器。具体步骤：

1. 下载RTSP服务器*[MediaMTX](https://github.com/bluenviron/mediamtx)* 并运行，默认监听8554端口
2. 使用ffmpeg将视频文件推流到MediaMTX

```powershell
ffmpeg.exe -re -i big_buck_bunny_720p_30mb.mp4 -c copy -rtsp_transport tcp -f rtsp rtsp://127.0.0.1:8554/stream
```

3. RTSP流`rtsp://127.0.0.1:8554/stream`可以通过VLC播放器播放，验证推流成功

### 其他方案：

1. [使用ffmpeg将RTSP转换成RTMP/FLV](https://blog.timetickme.com/archives/RTSP-Video-Streaming-Web-Playback-Solution-1)
2. [使用webrtc-stream将RTSP流转成webrtc](https://blog.timetickme.com/archives/RTSP%20Video%20Streaming%20Web%20Playback%20Solution%202)

