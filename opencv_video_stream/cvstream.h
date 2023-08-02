#pragma once

#pragma warning(disable:4996)

#include <string>
#include <atomic>
#include <thread>
#include <nadjieb/mjpeg_streamer.hpp>
#include "3rdparty/cpp-httplib/httplib.h"
#include <opencv2/opencv.hpp>

class cvstream
{
private:
    //rtsp url
    std::string videosource;

    //http stream url
    std::string stream_url;

    int height = 720;
    int width = 1280;
    int quality = 100;

    std::atomic_bool isWorking = true;

    cv::Mat frame;
    std::mutex mutex;
    nadjieb::MJPEGStreamer streamer;

private:
    std::thread rtspThread;
    std::thread streamThread;

    std::thread httpThread;

    httplib::Server svr;
    std::string imgString;

public:
    cvstream(std::string source, std::string dist, int width, int height, int quality);
    ~cvstream();
    void run();

private:
    void read_source();
    void stream_publish();
};