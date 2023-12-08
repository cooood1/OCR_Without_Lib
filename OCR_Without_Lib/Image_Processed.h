#pragma once
#include "MyImage.h"

#define MODE_SWITCH 36 //10���֣�36���ּ���ĸ

class ImageProcessed{

public:

	static MyImage grey_img;

	//�ҶȻ�
	static MyImage MyBGR2GRAY(MyImage img, MyImage& grey_img);

	//��ֵ��
	static MyImage Mythreshold(const MyImage& grey_img, MyImage& threshold_img, int threshold);

	//�ָ�����
	static void CharacterSegmentation(const MyImage& binaryImage, std::vector<int>& vertical, std::vector<int>& verticalSize, std::vector<int>& horizontal);
	static void GetROI(const MyImage& src, const std::vector<int>& vertical, const std::vector<int>& verticalSize, const std::vector<int>& horizontal, std::vector<MyImage>& dst);
	
	//ģ��ƥ��
	static void Mymatchtemplate(const MyImage& img, const MyImage& temp, std::vector <int>& dst);
	
	//ͼƬת����
	static std::vector<int> OCR_ImgToTxt(std::vector<MyImage> ROI);
	
	//��ʾ���
	static void OCR_ShowResult(std::vector<int> result, std::vector<int>& verticalSize);
};