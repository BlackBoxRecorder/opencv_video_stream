#pragma once

#pragma warning(disable:4996)

#include <string>
#include <atomic>
#include <thread>
#include <nadjieb/mjpeg_streamer.hpp>

#include <opencv2/opencv.hpp>

class cvstream
{
private:
  //rtsp url
  std::string videosource;

  //http stream url
  std::string stream_url;

  std::atomic_bool isWorking = true;

  cv::Mat frame;
  std::mutex mutex;
  nadjieb::MJPEGStreamer streamer;

private:
  std::thread rtspThread;
  std::thread streamThread;

public:
  cvstream(std::string source, std::string dist);
  ~cvstream();
  void run();

private:
  void read_rtsp();
  void stream_publish();
};