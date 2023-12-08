#include "Image_Processed.h"

// RGB转GRAY
MyImage ImageProcessed::MyBGR2GRAY(MyImage img, MyImage& grey_img)
{
	for (size_t i = 0; i < img.height; i++)
	{
		Pixel* current_pixel = &img.pixels[i][0];
		for (size_t j = 0; j < img.width; j++)
		{
			// 计算灰度值
			unsigned char gray_value = static_cast<unsigned char>(0.299 * current_pixel[j].red + 0.587 * current_pixel[j].green + 0.114 * current_pixel[j].blue);

			// 使用指针操作灰度图像像素
			grey_img.pixelsGRAY[i][j].grey = static_cast<int>(gray_value);
		}
	}
	return grey_img;
}


MyImage ImageProcessed::Mythreshold(const MyImage& grey_img, MyImage& threshold_img, int threshold)
{// threshold 为自己所需的阈值
	for (size_t i = 0; i < grey_img.height; i++)
	{
		const PixelGRAY* current_pixel = &grey_img.pixelsGRAY[i][0];
		for (size_t j = 0; j < grey_img.width; j++)
		{
			unsigned char gray_value = static_cast<int>(current_pixel[j].grey);
			//std::cout << (int)gray_value << std::endl;
			if ((int)gray_value <= threshold)
			{
				threshold_img.pixelsGRAY[i][j].grey = 255;
			}
			else if ((int)gray_value > threshold)
			{
				threshold_img.pixelsGRAY[i][j].grey = 0;
			}
			else
			{
				std::cout << "ERROR to threshold" << std::endl;
			}
		}
	}

	/*for (auto& row : grey_img.pixelsGRAY)
	{
		for (auto& pixelsGRAY : row)
		{
			if (static_cast<int>(pixelsGRAY.grey) <= 100)
			{
				pixelsGRAY.grey = 0;
			}
			else if (static_cast<int>(pixelsGRAY.grey) > 100)
			{
				pixelsGRAY.grey = 255;
			}
			else
			{
				std::cout << "ERROR to threshold" << std::endl;
			}
		}
	}*/
	return grey_img;
}


void ImageProcessed::CharacterSegmentation(const MyImage& binaryImage, std::vector<int>& vertical, std::vector<int>& verticalSize, std::vector<int>& horizontal)
{
	// 清空两个Vector，防止原有数据干扰
    if (!horizontal.empty())
    {
        horizontal.clear();
    }

    if (!vertical.empty())
    {
        vertical.clear();
    }

    int minCharacterWidth = 6;  // 最小字符宽度，
    //int gapThreshold = 5;      

	// 指定初始值和初始位置
    int currentSegmentStartX = -1;
    int currentSegmentWidthX = 0;

    int currentSegmentStartY = -1;
    int currentSegmentWidthY = 0;

    // 水平投影
    for (int y = 0; y < binaryImage.height; ++y)
    {
		const PixelGRAY* current_pixel = &binaryImage.pixelsGRAY[y][0];
        int rowSum = 0;
        for (int x = 0; x < binaryImage.width; ++x) 
        {
            rowSum += (int)current_pixel[x].grey;
        }

        // 如果当前像素为字符
        if (rowSum > 0) 
        {
            if (currentSegmentStartX == -1) 
            {
                // 开始新计算
                currentSegmentStartX = y;
                currentSegmentWidthX = 1;
            }
            else 
            {
                // 延伸当前计算
                ++currentSegmentWidthX;
            }
        }
        else 
        {
            // 当前为背景
            if (currentSegmentStartX != -1 && currentSegmentWidthX >= minCharacterWidth) 
            {
                horizontal.push_back(currentSegmentStartX);
                horizontal.push_back(currentSegmentWidthX + currentSegmentStartX);
                //std::cout << "Character at row " << currentSegmentStartX << ", width: " << currentSegmentWidthX << std::endl;
            }

            // 重置
            currentSegmentStartX = -1;
            currentSegmentWidthX = 0;
        }
    }
	int temp = 0;
    // 垂直投影
	int colNum = 0; //存储每一行字符个数
	for (size_t i = 0; i < horizontal.size() / 2; i++)
	{
		int temp = vertical.size();
		for (int x = 0; x < binaryImage.width; ++x)
		{
			int colSum = 0;
			int colHeight = horizontal[i * 2 + 1] - horizontal[i * 2];
			int colLocate1 = horizontal[i * 2];
			for (int y = 0; y < colHeight; ++y)
			{
				//二维数组行与行的存储空间不一定连续
				colSum += binaryImage.pixelsGRAY[colLocate1 + y][x].grey;
			}

			// 如果当前像素为字符
			if (colSum > 0)
			{
				if (currentSegmentStartY == -1)
				{
					// 开始新的字符段
					currentSegmentStartY = x;
					currentSegmentWidthY = 1;
				}
				else
				{
					// 延伸当前字符段
					++currentSegmentWidthY;
				}
			}
			else
			{
				// 当前像素为背景

				if (currentSegmentStartY != -1 && currentSegmentWidthY >= minCharacterWidth)
				{
					vertical.push_back(currentSegmentStartY);
					vertical.push_back(currentSegmentWidthY + currentSegmentStartY);
					//std::cout << "Character at column " << currentSegmentStartY << ", width: " << currentSegmentWidthY << std::endl;
				}

				// 重置当前字符段
				currentSegmentStartY = -1;
				currentSegmentWidthY = 0;
			}
		}
		colNum =((int)vertical.size() - temp) / 2 ;
		verticalSize.push_back(colNum);
	}
   
}


// 把原图片分成若干个子区域并储存
void ImageProcessed::GetROI(const MyImage& src, const std::vector<int>& vertical, const std::vector<int>& verticalSize, const std::vector<int>& horizontal, std::vector<MyImage>& dst)
{
	// 清空Vector
	if (!dst.empty())
	{
		dst.clear();
	}
	// 每行元素个数和行数
	int num_x = horizontal.size() / 2;
	int num_y = vertical.size() / 2;

	// 用于判断目前储存到第几个
	int number_x = 0;
	int number_y = 0;
	std::cout << "行数: " << num_x;
	std::cout << "		字符数: " << num_y << std::endl;
	//for (size_t i = 0; i < verticalSize.size(); i++)
	//{
	//	std::cout << "size:  " << verticalSize[i];
	//}

	// 遍历每个数字或者字母
	for (size_t i = 0; i < num_x; i++)
	{
		int height = (int)horizontal[number_x + 1] - (int)horizontal[number_x];
		for (size_t j = 0; j < verticalSize[i]; j++)
		{
			// 读取记录单个字符的高度和宽度
			//std::cout << "I :" << i<<"   J: "<<j<<std::endl;

			int width = (int)vertical[number_y + 1] - (int)vertical[number_y];

			// 存储每个字符像素的二维向量
			std::vector<std::vector<PixelGRAY>> pixelsGRAY1;
			pixelsGRAY1.resize(height, std::vector<PixelGRAY>(width));
			// 开始存储每个像素信息
			for (size_t m = 0; m < height; m++)
			{
				const PixelGRAY* current_pixel = &src.pixelsGRAY[(horizontal[number_x] + m)][(vertical[number_y])];
				PixelGRAY* current_pixel_temp = &pixelsGRAY1[m][0];
				for (size_t n = 0; n < width; n++)
				{
					current_pixel_temp[n].grey = current_pixel[n].grey;
				}
			}

			//像素信息给Image对象，并把该对象存入vector中
			MyImage temp_img;
			temp_img.pixelsGRAY = pixelsGRAY1;
			temp_img.width = width;
			temp_img.height = height;
			temp_img.resize(48, 60);  // (width,height)的格式
			ImageProcessed::Mythreshold(temp_img, temp_img, 70);
			number_y = number_y + 2;
			dst.push_back(temp_img);
		}
		number_x = number_x + 2;
	}
}

// 模板匹配
void ImageProcessed::Mymatchtemplate(const MyImage& img, const MyImage& temp, std::vector <int>& dst)
{
	// 平方差法进行模板匹配
    int R = 0;
	//std::cout << "img_height:" << img.height << " img_width:" << img.width << std::endl;
	//std::cout << "temp_height:" << temp.height << " temp_width:" << temp.width << std::endl;
    for (int m = 0; m < img.height; ++m)
    {
		const PixelGRAY* current_pixel1 = &img.pixelsGRAY[m][0];
		const PixelGRAY* current_pixel2 = &temp.pixelsGRAY[m][0];
        for (int n = 0; n < img.width; ++n)
        {
			//std::cout << "m:" << m<< " n:" << n << std::endl;
			// R中存储每个像素点插值的累加和
            R += pow((int)current_pixel1[n].grey - (int)current_pixel2[n].grey, 2);
        }
    }
    dst.push_back(R);
}

std::vector<int> ImageProcessed::OCR_ImgToTxt(std::vector<MyImage> ROI)
{
	// 存储模板图片
	std::vector<MyImage> Mytemplate;

	// 导入10个数字的模板
	for (int i = 0; i < 10; i++)	
	{
		//对模板进行灰度转化和二值化
		std::string filename_string = "./template/" + std::to_string(i) + ".png";
		std::wstring filename_Wstring = to_wide_string(filename_string);
		MyImage img(filename_Wstring);
		MyImage grey_img(img.height, img.width, 0);
		ImageProcessed::MyBGR2GRAY(img, grey_img);
		MyImage threshold_img(grey_img.height, grey_img.width, 0);
		ImageProcessed::Mythreshold(grey_img, threshold_img, 70);
		//threshold_img.displayPixelsGRAY();
		//std::cout << std::endl;
		Mytemplate.push_back(threshold_img);
	}
	// 导入26个字母的模板
	if (MODE_SWITCH == 36)
	{
		for (char i = 'A'; i <= 'Z'; i++)
		{
			//对模板进行灰度转化和二值化
			std::string alpha(1, i);
			std::string filename_string = "./template/" + alpha + ".png";  // 模板名为 字母.png	
			std::wstring filename_Wstring = to_wide_string(filename_string);
			MyImage img(filename_Wstring);	
			MyImage grey_img(img.height, img.width, 0);
			ImageProcessed::MyBGR2GRAY(img, grey_img);
			MyImage threshold_img(grey_img.height, grey_img.width, 0);
			ImageProcessed::Mythreshold(grey_img, threshold_img, 70);
			Mytemplate.push_back(threshold_img);
		}
	}

	// 存储最终处理结果，结果为数字
	std::vector<int> real_result;
	for (int j = 0; j < ROI.size(); j++)
	{
		// 存储临时处理结果，结果为数字
		std::vector<int> result;

		// 匹配每个图片
		for (int i = 0; i < MODE_SWITCH; i++)
		{
			Mymatchtemplate(ROI[j], Mytemplate[i], result);
		}

		int temp = result[0];
		int label = 0;

		// 找到平方差的最小值，该值对应的图片则为模板匹配的最佳结果
		for (int n = 0; n < MODE_SWITCH - 1; n++)
		{
			if (temp > result[n + 1])
			{
				temp = result[n + 1];
				label = n + 1;
			}
		}
		real_result.push_back(label);
	}
	return real_result;
}

// 将数字结果转化为字符串结果并输出
void ImageProcessed::OCR_ShowResult(std::vector<int> result, std::vector<int>& verticalSize)
{
	std::string output;

	//模板顺序需匹配，目前是按从0—9—A—Z的顺序
	for (int i = 0; i < result.size(); i++)
	{
		switch (result[i])
		{
		case 0:
			output += "0";
			break;
		case 1:
			output += "1";
			break;
		case 2:
			output += "2";
			break;
		case 3:
			output += "3";
			break;
		case 4:
			output += "4";
			break;
		case 5:
			output += "5";
			break;
		case 6:
			output += "6";
			break;
		case 7:
			output += "7";
			break;
		case 8:
			output += "8";
			break;
		case 9:
			output += "9";
			break;
		case 10:
			output += "A";
			break;
		case 11:
			output += "B";
			break;
		case 12:
			output += "C";
			break;
		case 13:
			output += "D";
			break;
		case 14:
			output += "E";
			break;
		case 15:
			output += "F";
			break;
		case 16:
			output += "G";
			break;
		case 17:
			output += "H";
			break;
		case 18:
			output += "I";
			break;
		case 19:
			output += "J";
			break;
		case 20:
			output += "K";
			break;
		case 21:
			output += "L";
			break;
		case 22:
			output += "M";
			break;
		case 23:
			output += "N";
			break;
		case 24:
			output += "O";
			break;
		case 25:
			output += "P";
			break;
		case 26:
			output += "Q";
			break;
		case 27:
			output += "R";
			break;
		case 28:
			output += "S";
			break;
		case 29:
			output += "T";
			break;
		case 30:
			output += "U";
			break;
		case 31:
			output += "V";
			break;
		case 32:
			output += "W";
			break;
		case 33:
			output += "X";
			break;
		case 34:
			output += "Y";
			break;
		case 35:
			output += "Z";
			break;
		default:
			std::cout << "Error" << std::endl;
			break;
		}
	}
	std::cout << "识别结果：" << std::endl;
	std::cout << "          ";
	int sum = 0;
	for (size_t i = 0; i < verticalSize.size(); i++)
	{
		for (size_t j = 0; j < verticalSize[i]; j++)
		{
			std::cout << output[sum] << "  ";
			sum++;
		}
		std::cout << std::endl;
		std::cout << "          ";
	}
}
