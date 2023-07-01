#include<opencv2/opencv.hpp>
#include<iostream>
#include <nadjieb/mjpeg_streamer.hpp>
#include "cvstream.h"

using namespace std;
using namespace cv;
using MJPEGStreamer = nadjieb::MJPEGStreamer;

MJPEGStreamer streamer;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "missing argc. please specify a mp4 file or rtsp url" << std::endl;
        return -1;
    }

    cvstream svc(argv[1], argv[2]);
    svc.run();
    return 0;
}
