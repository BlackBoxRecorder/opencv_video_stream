#include "cvstream.h"
#include <cstdlib>
#include <fstream>


using namespace std;
using namespace cv;

cvstream::cvstream(std::string source)
{
    this->videosource = source;
}

void cvstream::run()
{
    decodeThread = std::thread(&cvstream::decoding, this);

    if (decodeThread.joinable())
    {
        decodeThread.join();
    }
}

cvstream::~cvstream()
{
    isWorking = false;

    if (decodeThread.joinable())
    {
        decodeThread.join();
    }

    if (encodeThread.joinable())
    {
        encodeThread.join();
    }
}

void cvstream::decoding()
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
            // You can replace 5 with CAP_PROP_FPS as well, they are enumerations
            int fps = vid_capture.get(CAP_PROP_FPS);
            cout << "Frames per second :" << fps;

            // Obtain frame_count using opencv built in frame count reading method
            // You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
            int frame_count = vid_capture.get(7);
            cout << "  Frame count :" << frame_count;
        }

        streamer.start(8897);

        // Read the frames to the last frame
        while (vid_capture.isOpened())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            // Initialise frame matrix
            Mat frame;

            // Initialize a boolean to check if frames are there or not
            bool isSuccess = vid_capture.read(frame);

            // If frames are present, show it
            if (isSuccess == true)
            {
                std::lock_guard<std::mutex> guard(mutex);

                //display frames
                //imshow("Frame", frame);

                // 启动解码线程
                bool flag = this->frame.empty();

                this->frame = frame.clone();
                if (flag) {
                    this->encodeThread = std::thread(&cvstream::encoding, this);
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
        destroyAllWindows();
        return;

    } while (false);

}

void cvstream::encoding()
{
    while (isWorking)
    {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> guard(mutex);
            //frame = this->frame.clone();
            cv::resize(this->frame, frame, cv::Size(1280, 720));
        }

        if (!frame.empty())
        {
            std::vector<uchar> buff_bgr;
            cv::imencode(".jpg", frame, buff_bgr);
            streamer.publish("/bgr", std::string(buff_bgr.begin(), buff_bgr.end()));
        }
    }

}
