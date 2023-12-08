#pragma once
#include "MyImage.h"

#define MODE_SWITCH 36 //10数字，36数字加字母

class ImageProcessed{

public:

	static MyImage grey_img;

	//灰度化
	static MyImage MyBGR2GRAY(MyImage img, MyImage& grey_img);

	//二值化
	static MyImage Mythreshold(const MyImage& grey_img, MyImage& threshold_img, int threshold);

	//分割区域
	static void CharacterSegmentation(const MyImage& binaryImage, std::vector<int>& vertical, std::vector<int>& verticalSize, std::vector<int>& horizontal);
	static void GetROI(const MyImage& src, const std::vector<int>& vertical, const std::vector<int>& verticalSize, const std::vector<int>& horizontal, std::vector<MyImage>& dst);
	
	//模板匹配
	static void Mymatchtemplate(const MyImage& img, const MyImage& temp, std::vector <int>& dst);
	
	//图片转文字
	static std::vector<int> OCR_ImgToTxt(std::vector<MyImage> ROI);
	
	//显示结果
	static void OCR_ShowResult(std::vector<int> result, std::vector<int>& verticalSize);
};