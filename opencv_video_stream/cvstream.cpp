#include "cvstream.h"

using namespace std;
using namespace cv;

cvstream::cvstream(std::string source, std::string dist, int width, int height, int quality)
{
    this->videosource = source;
    this->stream_url = dist;
    this->height = height;
    this->width = width;
    this->quality = quality;
}

void cvstream::run()
{
    rtspThread = std::thread(&cvstream::read_source, this);

    svr.Get("/img", [this](const httplib::Request&, httplib::Response& res)
        {
            if (imgString.length() > 0)
            {
                res.set_content(imgString, "image/jpeg");
            }
            else
            {
                res.set_content("none", "text/plain");
            }
        });

    httpThread = std::thread([&]()
        {
            cout << "start http server" << endl;
            svr.listen("127.0.0.1", 9999);
        });

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

void cvstream::read_source()
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
                streamer.stop();
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        }
        // Release the video capture object
        vid_capture.release();

        return;

    } while (false);

}

void cvstream::stream_publish()
{
    std::vector<int> params = { cv::IMWRITE_JPEG_QUALITY, quality };

    while (isWorking)
    {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> guard(mutex);
            if (width > 0 && height > 0)
            {
                cv::resize(this->frame, frame, cv::Size(width, height));
            }
            else
            {
                frame = this->frame.clone();
            }

            if (!frame.empty())
            {
                std::vector<uchar> buff_bgr;
                cv::imencode(".jpg", frame, buff_bgr, params);
                imgString = std::string(buff_bgr.begin(), buff_bgr.end());
            }
        }

        streamer.publish("/" + stream_url, imgString);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
}
