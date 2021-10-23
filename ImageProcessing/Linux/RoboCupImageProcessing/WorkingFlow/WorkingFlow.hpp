#pragma once
#include "Configs.hpp"
#include <algorithm>

namespace RoboCup
{
	class WorkingFlow
	{
	public:
		WorkingFlow(const Configs &Configs) { };
		virtual ~WorkingFlow() { };
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		virtual int Run(void) = 0;

	protected:
		void sortRotatedRectMaxToMin(std::vector<cv::RotatedRect>& Rects)
		{
			std::sort(Rects.begin(), Rects.end(), [](const cv::RotatedRect&  Rect1, const cv::RotatedRect& Rect2) { return Rect1.size.area() > Rect2.size.area() ? true : false; });
		};

		void sortRotatedRectLeftToRight(std::vector<cv::RotatedRect>& Rects)
		{
			std::sort(Rects.begin(), Rects.end(), [](const cv::RotatedRect& Rect1, const cv::RotatedRect& Rect2) { return Rect1.center.x < Rect2.center.x ? true : false; });
		};

		void sortRotatedRectRightToLeft(std::vector<cv::RotatedRect>& Rects)
		{
			std::sort(Rects.begin(), Rects.end(), [](const cv::RotatedRect& Rect1, const cv::RotatedRect& Rect2) { return Rect1.center.x > Rect2.center.x ? true : false; });
		};
	};
}