#include "MainWorkingFlow.hpp"

#include <chrono>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "FruitDetector.hpp"
void RoboCup::MainWorkingFlow::Run()
{
    using namespace cv;
    using namespace std;
    using namespace h13;
    using namespace RoboCup;

#if defined(__linux__)
    FrameBufferMonitor monitor("/dev/fb0");
    uint16_t xResolution = 0;
    uint16_t yResolution = 0;

    monitor.GetFrameBufferResolution(xResolution, yResolution);
    cout << xResolution << " " << yResolution << endl;
#endif

    FruitDetector detector;
    //AppleDetector detector;

    VideoCapture cap(0);

    if (cap.isOpened())
    {
        Mat frame;
        while (cap.read(frame))
        {
            auto start = std::chrono::high_resolution_clock::now();
            Mat mask;
            //auto results = detector.Detect(frame, noArray(), mask);
            auto results = detector.Detect(frame);
            for (auto result : results)
            {
                Point2f point[4];
                //result.points(point);
                result.Rect.points(point);
                Rect rect = result.Rect.boundingRect();
                rectangle(frame, rect, Scalar(0, 255, 0), 2);
                putText(frame, GetLabelString(result.FruitType), Point(rect.x, rect.y - 16), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
                for (int index = 0; index < 4; index++)
                {
                    line(frame, point[index], point[(index + 1) % 4], cv::Scalar(255, 0, 0), 2);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> tm = end - start;
            putText(frame, "fps: " + to_string(1000 / tm.count()), Point(0, 24), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
            monitor.Display("Final", frame);
        }
    }
}
