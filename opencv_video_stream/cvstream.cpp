#include "cvstream.h"


using namespace std;
using namespace cv;

cvstream::cvstream(std::string source, std::string dist)
{
    this->videosource = source;
    this->stream_url = dist;
}

void cvstream::run()
{
    rtspThread = std::thread(&cvstream::read_rtsp, this);

    if (rtspThread.joinable())
    {
        rtspThread.join();
    }
}

cvstream::~cvstream()
{
    isWorking = false;

    if (rtspThread.joinable())
    {
        rtspThread.join();
    }

    if (streamThread.joinable())
    {
        streamThread.join();
    }
}

void cvstream::read_rtsp()
{
    do
    {
        // initialize a video capture object
        cv::VideoCapture vid_capture(videosource);

        // Print error message if the stream is invalid
        if (!vid_capture.isOpened())
        {
            cout << "Error opening video stream or file" << endl;
        }
        else
        {
            // Obtain fps and frame count by get() method and print
            int fps = vid_capture.get(CAP_PROP_FPS);
            cout << "Frames per second :" << fps;
        }

        streamer.start(8897);

        // Read the frames to the last frame
        while (vid_capture.isOpened())
        {
            // Initialise frame matrix
            Mat frame;

            // Initialize a boolean to check if frames are there or not
            bool isSuccess = vid_capture.read(frame);

            // If frames are present, show it
            if (isSuccess == true)
            {
                std::lock_guard<std::mutex> guard(mutex);

                bool flag = this->frame.empty();

                this->frame = frame.clone();
                if (flag) {
                    this->streamThread = std::thread(&cvstream::stream_publish, this);
                }
            }

            if (isSuccess == false)
            {
                cout << "video end" << endl;
                isWorking = false;
                break;
            }

        }
        // Release the video capture object
        vid_capture.release();

        return;

    } while (false);

}

void cvstream::stream_publish()
{
    while (isWorking)
    {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> guard(mutex);
            frame = this->frame.clone();
            //cv::resize(this->frame, frame, cv::Size(1280, 720));
        }

        if (!frame.empty())
        {
            std::vector<uchar> buff_bgr;
            cv::imencode(".jpg", frame, buff_bgr);
            streamer.publish("/" + stream_url, std::string(buff_bgr.begin(), buff_bgr.end()));
        }
    }
}
