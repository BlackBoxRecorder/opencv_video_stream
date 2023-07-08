#include<opencv2/opencv.hpp>
#include<iostream>
#include <nadjieb/mjpeg_streamer.hpp>
#include "cvstream.h"
#include "3rdparty/args/args.hxx"

int main(int argc, char* argv[])
{
  args::ArgumentParser parser("Streaming a video source to MJPEG, then play on the web.");
  args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
  args::ValueFlag<std::string> source(parser, "string", "The video source url. For example, rtsp://192.168.2.3:8554/live", { 's',"source" });
  args::ValueFlag<std::string> endpoint(parser, "string", "The endpoint HTTP link suffix", { 'd',"endpoint" });
  args::ValueFlag<int> width(parser, "integer", "The output/streaming video width.", { "width" });
  args::ValueFlag<int> height(parser, "integer", "The output/streaming video height.", { "height" });
  args::ValueFlag<int> quality(parser, "integer", "The jpeg image quality, 0-100.", { 'q' ,"quality" });

  try
  {
    parser.ParseCLI(argc, argv);
  }
  catch (args::Help)
  {
    std::cout << parser;
    return 0;
  }

  std::string m_source;
  std::string m_endpoint = "live";
  int m_width = 1280;
  int m_height = 720;
  int m_quality = 100;

  if (source) {
    m_source = args::get(source);
    std::cout << "video source: " << m_source << std::endl;
  }
  else {
    std::cout << "missing -s param" << std::endl;
    return -1;
  }
  if (endpoint) {
    m_endpoint = args::get(endpoint);
    std::cout << "streaming url: " << m_endpoint << std::endl;
  }
  if (width) {
    m_width = args::get(width);
    std::cout << "streaming video width: " << m_width << std::endl;
  }
  if (height) {
    m_height = args::get(height);
    std::cout << "streaming video height: " << m_height << std::endl;
  }
  if (quality) {
    m_quality = args::get(quality);
    std::cout << "streaming video quality: " << m_quality << std::endl;
  }

  if (argc < 2) {
    std::cout << "use -h flag to get help." << std::endl;
    return -1;
  }

  cvstream svc(m_source, m_endpoint, m_width, m_height, m_quality);
  svc.run();
  return 0;
}
