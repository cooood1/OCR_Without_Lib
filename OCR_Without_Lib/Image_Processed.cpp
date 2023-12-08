#include "Image_Processed.h"

// RGBתGRAY
MyImage ImageProcessed::MyBGR2GRAY(MyImage img, MyImage& grey_img)
{
	for (size_t i = 0; i < img.height; i++)
	{
		Pixel* current_pixel = &img.pixels[i][0];
		for (size_t j = 0; j < img.width; j++)
		{
			// ����Ҷ�ֵ
			unsigned char gray_value = static_cast<unsigned char>(0.299 * current_pixel[j].red + 0.587 * current_pixel[j].green + 0.114 * current_pixel[j].blue);

			// ʹ��ָ������Ҷ�ͼ������
			grey_img.pixelsGRAY[i][j].grey = static_cast<int>(gray_value);
		}
	}
	return grey_img;
}


MyImage ImageProcessed::Mythreshold(const MyImage& grey_img, MyImage& threshold_img, int threshold)
{// threshold Ϊ�Լ��������ֵ
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
	// �������Vector����ֹԭ�����ݸ���
    if (!horizontal.empty())
    {
        horizontal.clear();
    }

    if (!vertical.empty())
    {
        vertical.clear();
    }

    int minCharacterWidth = 6;  // ��С�ַ���ȣ�
    //int gapThreshold = 5;      

	// ָ����ʼֵ�ͳ�ʼλ��
    int currentSegmentStartX = -1;
    int currentSegmentWidthX = 0;

    int currentSegmentStartY = -1;
    int currentSegmentWidthY = 0;

    // ˮƽͶӰ
    for (int y = 0; y < binaryImage.height; ++y)
    {
		const PixelGRAY* current_pixel = &binaryImage.pixelsGRAY[y][0];
        int rowSum = 0;
        for (int x = 0; x < binaryImage.width; ++x) 
        {
            rowSum += (int)current_pixel[x].grey;
        }

        // �����ǰ����Ϊ�ַ�
        if (rowSum > 0) 
        {
            if (currentSegmentStartX == -1) 
            {
                // ��ʼ�¼���
                currentSegmentStartX = y;
                currentSegmentWidthX = 1;
            }
            else 
            {
                // ���쵱ǰ����
                ++currentSegmentWidthX;
            }
        }
        else 
        {
            // ��ǰΪ����
            if (currentSegmentStartX != -1 && currentSegmentWidthX >= minCharacterWidth) 
            {
                horizontal.push_back(currentSegmentStartX);
                horizontal.push_back(currentSegmentWidthX + currentSegmentStartX);
                //std::cout << "Character at row " << currentSegmentStartX << ", width: " << currentSegmentWidthX << std::endl;
            }

            // ����
            currentSegmentStartX = -1;
            currentSegmentWidthX = 0;
        }
    }
	int temp = 0;
    // ��ֱͶӰ
	int colNum = 0; //�洢ÿһ���ַ�����
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
				//��ά���������еĴ洢�ռ䲻һ������
				colSum += binaryImage.pixelsGRAY[colLocate1 + y][x].grey;
			}

			// �����ǰ����Ϊ�ַ�
			if (colSum > 0)
			{
				if (currentSegmentStartY == -1)
				{
					// ��ʼ�µ��ַ���
					currentSegmentStartY = x;
					currentSegmentWidthY = 1;
				}
				else
				{
					// ���쵱ǰ�ַ���
					++currentSegmentWidthY;
				}
			}
			else
			{
				// ��ǰ����Ϊ����

				if (currentSegmentStartY != -1 && currentSegmentWidthY >= minCharacterWidth)
				{
					vertical.push_back(currentSegmentStartY);
					vertical.push_back(currentSegmentWidthY + currentSegmentStartY);
					//std::cout << "Character at column " << currentSegmentStartY << ", width: " << currentSegmentWidthY << std::endl;
				}

				// ���õ�ǰ�ַ���
				currentSegmentStartY = -1;
				currentSegmentWidthY = 0;
			}
		}
		colNum =((int)vertical.size() - temp) / 2 ;
		verticalSize.push_back(colNum);
	}
   
}


// ��ԭͼƬ�ֳ����ɸ������򲢴���
void ImageProcessed::GetROI(const MyImage& src, const std::vector<int>& vertical, const std::vector<int>& verticalSize, const std::vector<int>& horizontal, std::vector<MyImage>& dst)
{
	// ���Vector
	if (!dst.empty())
	{
		dst.clear();
	}
	// ÿ��Ԫ�ظ���������
	int num_x = horizontal.size() / 2;
	int num_y = vertical.size() / 2;

	// �����ж�Ŀǰ���浽�ڼ���
	int number_x = 0;
	int number_y = 0;
	std::cout << "����: " << num_x;
	std::cout << "		�ַ���: " << num_y << std::endl;
	//for (size_t i = 0; i < verticalSize.size(); i++)
	//{
	//	std::cout << "size:  " << verticalSize[i];
	//}

	// ����ÿ�����ֻ�����ĸ
	for (size_t i = 0; i < num_x; i++)
	{
		int height = (int)horizontal[number_x + 1] - (int)horizontal[number_x];
		for (size_t j = 0; j < verticalSize[i]; j++)
		{
			// ��ȡ��¼�����ַ��ĸ߶ȺͿ��
			//std::cout << "I :" << i<<"   J: "<<j<<std::endl;

			int width = (int)vertical[number_y + 1] - (int)vertical[number_y];

			// �洢ÿ���ַ����صĶ�ά����
			std::vector<std::vector<PixelGRAY>> pixelsGRAY1;
			pixelsGRAY1.resize(height, std::vector<PixelGRAY>(width));
			// ��ʼ�洢ÿ��������Ϣ
			for (size_t m = 0; m < height; m++)
			{
				const PixelGRAY* current_pixel = &src.pixelsGRAY[(horizontal[number_x] + m)][(vertical[number_y])];
				PixelGRAY* current_pixel_temp = &pixelsGRAY1[m][0];
				for (size_t n = 0; n < width; n++)
				{
					current_pixel_temp[n].grey = current_pixel[n].grey;
				}
			}

			//������Ϣ��Image���󣬲��Ѹö������vector��
			MyImage temp_img;
			temp_img.pixelsGRAY = pixelsGRAY1;
			temp_img.width = width;
			temp_img.height = height;
			temp_img.resize(48, 60);  // (width,height)�ĸ�ʽ
			ImageProcessed::Mythreshold(temp_img, temp_img, 70);
			number_y = number_y + 2;
			dst.push_back(temp_img);
		}
		number_x = number_x + 2;
	}
}

// ģ��ƥ��
void ImageProcessed::Mymatchtemplate(const MyImage& img, const MyImage& temp, std::vector <int>& dst)
{
	// ƽ�������ģ��ƥ��
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
			// R�д洢ÿ�����ص��ֵ���ۼӺ�
            R += pow((int)current_pixel1[n].grey - (int)current_pixel2[n].grey, 2);
        }
    }
    dst.push_back(R);
}

std::vector<int> ImageProcessed::OCR_ImgToTxt(std::vector<MyImage> ROI)
{
	// �洢ģ��ͼƬ
	std::vector<MyImage> Mytemplate;

	// ����10�����ֵ�ģ��
	for (int i = 0; i < 10; i++)	
	{
		//��ģ����лҶ�ת���Ͷ�ֵ��
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
	// ����26����ĸ��ģ��
	if (MODE_SWITCH == 36)
	{
		for (char i = 'A'; i <= 'Z'; i++)
		{
			//��ģ����лҶ�ת���Ͷ�ֵ��
			std::string alpha(1, i);
			std::string filename_string = "./template/" + alpha + ".png";  // ģ����Ϊ ��ĸ.png	
			std::wstring filename_Wstring = to_wide_string(filename_string);
			MyImage img(filename_Wstring);	
			MyImage grey_img(img.height, img.width, 0);
			ImageProcessed::MyBGR2GRAY(img, grey_img);
			MyImage threshold_img(grey_img.height, grey_img.width, 0);
			ImageProcessed::Mythreshold(grey_img, threshold_img, 70);
			Mytemplate.push_back(threshold_img);
		}
	}

	// �洢���մ����������Ϊ����
	std::vector<int> real_result;
	for (int j = 0; j < ROI.size(); j++)
	{
		// �洢��ʱ�����������Ϊ����
		std::vector<int> result;

		// ƥ��ÿ��ͼƬ
		for (int i = 0; i < MODE_SWITCH; i++)
		{
			Mymatchtemplate(ROI[j], Mytemplate[i], result);
		}

		int temp = result[0];
		int label = 0;

		// �ҵ�ƽ�������Сֵ����ֵ��Ӧ��ͼƬ��Ϊģ��ƥ�����ѽ��
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

// �����ֽ��ת��Ϊ�ַ�����������
void ImageProcessed::OCR_ShowResult(std::vector<int> result, std::vector<int>& verticalSize)
{
	std::string output;

	//ģ��˳����ƥ�䣬Ŀǰ�ǰ���0��9��A��Z��˳��
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
	std::cout << "ʶ������" << std::endl;
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
