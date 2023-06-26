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
    //mp4 file , rtsp url ... 
    std::string videosource;

    std::atomic_bool isWorking = true;

    cv::Mat frame;
    std::mutex mutex;
    nadjieb::MJPEGStreamer streamer;

private:
    std::thread decodeThread;
    std::thread encodeThread;

public:
    cvstream(std::string source);
    ~cvstream();
    void run();

private:
    void decoding();
    void encoding();
};