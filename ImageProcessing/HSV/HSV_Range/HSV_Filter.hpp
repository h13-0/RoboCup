#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <vector>

namespace h13
{
	/// <summary>
	/// typedef of rect.
	/// </summary>
	typedef struct 
	{
		unsigned int x;
		unsigned int y;
		unsigned int w;
		unsigned int h;
	} Rect_t;

	class HSV_Filter
	{
	public:
		HSV_Filter(unsigned int H_Min, unsigned int H_Max, unsigned int S_Min, unsigned int S_Max, unsigned int V_Min, unsigned int V_Max)
			: hMax(H_Max), hMin(H_Min), sMax(S_Max), sMin(S_Min), vMax(V_Max), vMin(V_Min) { }

		HSV_Filter() 
			: hMax(0), hMin(100), sMax(0), sMin(100), vMax(0), vMin(100) { }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="Input"></param>
		/// <param name="Output"></param>
		void ConvertToHSV(cv::InputArray Input, cv::OutputArray Output);

		/// <summary>
		/// Find the region in the image that meets the threshold and output it to the binary image.
		/// </summary>
		/// <param name="Input"> Input image in BGR. </param>
		/// <param name="Output"> Output image in Binary. </param>
		void Filter(cv::InputArray Input, cv::OutputArray Output);

		/// <summary>
		/// Find the region in the image that meets the threshold and return target box.
		/// </summary>
		/// <param name="Input"> Input image in BGR. </param>
		/// <returns></returns>
		std::vector<Rect_t> Detect(cv::InputArray Input);

		/// <summary>
		/// Set the threshold for channel.
		/// </summary>
		/// <param name="Minimum"></param>
		/// <param name="Maximum"></param>
		void SetHueRange(unsigned int Minimum, unsigned int Maximum);
		void SetSaturationRange(unsigned int Minimum, unsigned int Maximum);
		void SetValueRange(unsigned int Minimum, unsigned int Maximum);

		void SetHueMaximumValue(unsigned int Value);
		void SetHueMinimumValue(unsigned int Value);

		void SetSaturationMaximumValue(unsigned int Value);
		void SetSaturationMinimumValue(unsigned int Value);

		void SetValueMaximumValue(unsigned int Value);
		void SetValueMinimumValue(unsigned int Value);
		
		~HSV_Filter() { }

	private:
		unsigned int hMax;
		unsigned int hMin;
		unsigned int sMax;
		unsigned int sMin;
		unsigned int vMax;
		unsigned int vMin;
	};
}
