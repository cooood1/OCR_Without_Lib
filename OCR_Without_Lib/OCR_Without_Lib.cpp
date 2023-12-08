#include "Image_Processed.h"


int main()
{
    //std::wstring  filename = L"./template/0.png";
    std::wstring  filename = L"./image/4.png";
    //std::wstring  filename = L"./test/01.png";
    MyImage image_read(filename);
    std::cout << "行：" << image_read.height;
    std::cout << "      列：" << image_read.width << std::endl;
    //image_read.displayPixelsRGB();
    //std::cout << std::endl;

    // 转灰度图
    MyImage grey_img(image_read.height, image_read.width, 0);
    ImageProcessed::MyBGR2GRAY(image_read, grey_img);
    //grey_img.displayPixelsGRAY();
    //std::cout << std::endl;

    // 二值化
    MyImage threshold_img(grey_img.height, grey_img.width, 0);
    ImageProcessed::Mythreshold(grey_img, threshold_img, 75);
    //threshold_img.displayPixelsGRAY();
    //std::cout << std::endl;

    // 分割并存储区域
    std::vector<int> vertical;  //存储划分信息，格式为：开始点 + 长度
    std::vector<int> horizontal;
    std::vector<int> verticalSize; //存储每一行字符个数
    ImageProcessed::CharacterSegmentation(threshold_img, vertical, verticalSize, horizontal);

    //for (size_t i = 0; i < vertical.size(); i++)
    //{
    //    std::cout << "vertical:" << vertical[i] << std::endl;
    //}
    //
    //for (size_t i = 0; i < horizontal.size(); i++)
    //{
    //    std::cout << "horizontal:" << horizontal[i] << std::endl;
    //}

    // 存储模板匹配区域 ,同时resize 待识别文字区域，让其与模板大小统一
    std::vector<MyImage> ROI;
    ImageProcessed::GetROI(threshold_img, vertical, verticalSize, horizontal, ROI);
    std::cout << std::endl;

    //for (size_t i = 0; i < ROI.size(); i++)
    //{
    //    if (i == 0)
    //    {
    //        ROI[i].displayPixelsGRAY();
    //    }
    //}

    // 读入模板图片并进行模板匹配
    std::vector<int> result;
    result = ImageProcessed::OCR_ImgToTxt(ROI);
    // 显示匹配结果
    ImageProcessed::OCR_ShowResult(result, verticalSize);
 
     return 0;
}

