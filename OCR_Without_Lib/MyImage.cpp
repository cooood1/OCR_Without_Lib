#include "MyImage.h"

//RGB图读取
void MyImage::readImageRGB(const std::wstring fname)
{
    std::vector<unsigned char> image;  // 用于存储PNG图像的像素数据

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 从本地文件加载图片
    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(fname.c_str());

    // 获取图像信息
    int imgWidth  = bitmap->GetWidth();
    int imgHeight = bitmap->GetHeight();

    // 遍历图像的像素
    for (int y = 0; y < imgHeight; ++y) {
        for (int x = 0; x < imgWidth; ++x) {
            Gdiplus::Color pixelColor;
            bitmap->GetPixel(x, y, &pixelColor);

            // pixelColor 中包含了当前像素的颜色信息
            BYTE red = GetRValue(pixelColor.GetValue());
            BYTE green = GetGValue(pixelColor.GetValue());
            BYTE blue = GetBValue(pixelColor.GetValue());
            image.push_back((int)red);
            image.push_back((int)green);
            image.push_back((int)blue);
        }
    }

    // 关闭 GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    unsigned char* current_image = &image[0];
    pixels.resize(imgHeight, std::vector<Pixel>(imgWidth));
    this->height = imgHeight;
    this->width = imgWidth;
    for (size_t i = 0; i < imgHeight; ++i)
    {
        for (size_t j = 0; j < imgWidth; ++j)
        {
            int index = (j + i * imgWidth) * 3;

            // 从 image 中读取像素值
            BYTE red = image.at(index);
            BYTE green = image.at(index + 1);
            BYTE blue = image.at(index + 2);
            //Pixel pixel(*(current_image + j * 3 + i * pixels[0].size() * 3), *(current_image + j * 3 + i * pixels[0].size() * 3), *(current_image + j * 3 + i * pixels[0].size() * 3));
            Pixel pixel(red, green, blue);
            pixels[i][j] = pixel;
        }
    }
    //std::vector<unsigned char> image2;
    //std::string ff = "./test/01.png";
    //unsigned width,height;
    //unsigned error = lodepng::decode(image2, width, height, ff);
    //if (error) 
    //{
    //    std::cerr << "Error " << error << ": " << lodepng_error_text(error) << std::endl;
    //    return;
    //}
    //// 获取图像宽度、高度
    //this->width = static_cast<int>(width);
    //this->height = static_cast<int>(height);
    //// 将图像数据转换为像素向量
    //pixels.resize(this->height, std::vector<Pixel>(this->width));
    //unsigned char* current_image2 = &image[0];
    //for (size_t i = 0; i < pixels.size(); ++i)
    //{
    //    for (size_t j = 0; j < pixels[0].size(); ++j)
    //    {
    //        Pixel pixel(*(current_image2 + j * 4 + i * pixels[0].size() * 4), *(current_image2 + j * 4 + i * pixels[0].size() * 4), *(current_image2 + j * 4 + i * pixels[0].size() * 4));
    //        //Pixel pixel(image[j * 4 + i * pixels[0].size() * 4], image[j * 4 + i * pixels[0].size() * 4], image[j * 4 + i * pixels[0].size() * 4]);
    //        pixels[i][j] = pixel;
    //    }
    //}
}

void MyImage::displayPixelsRGB() const
{
    for (const auto& row : pixels)
    {
        for (const auto& pixel : row) 
        {
            std::cout << "(" << static_cast<int>(pixel.red) << "," << static_cast<int>(pixel.green)
                << "," << static_cast<int>(pixel.blue) << ") \t";
        }
        std::cout << std::endl;
    }
}

//GREY图读取
void MyImage::readImageGRAY(const std::wstring fname)
{
    std::vector<unsigned char> image;  // 用于存储PNG图像的像素数据
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 从本地文件加载图片
    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(fname.c_str());

    // 获取图像信息
    int imgWidth = bitmap->GetWidth();
    int imgHeight = bitmap->GetHeight();

    // 遍历图像的像素
    for (int y = 0; y < imgHeight; ++y) {
        for (int x = 0; x < imgWidth; ++x) {
            Gdiplus::Color pixelColor;
            bitmap->GetPixel(x, y, &pixelColor);

            // pixelColor 中包含了当前像素的颜色信息
            BYTE grey = GetRValue(pixelColor.GetValue());
            image.push_back((int)grey);
        }
    }

    // 关闭 GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    unsigned char* current_image = &image[0];
    pixels.resize(imgHeight, std::vector<Pixel>(imgWidth));
    this->height = imgHeight;
    this->width = imgWidth;
    for (size_t i = 0; i < image.size(); i += 1)
    {
        PixelGRAY pixelGRAY(*(current_image + i));
        pixelsGRAY[i / this->width].push_back(pixelGRAY);
    }
    //std::vector<unsigned char> image;  // 用于存储PNG图像的像素数据
    //unsigned width, height;
    //unsigned error = lodepng::decode(image, width, height, fname);
    //if (error) 
    //{
    //    std::cerr << "Error " << error << ": " << lodepng_error_text(error) << std::endl;
    //    return;
    //}
    //// 获取图像宽度、高度
    //this->width = static_cast<int>(width);
    //this->height = static_cast<int>(height);
    //// 将图像数据转换为像素向量
    //pixelsGRAY.resize(this->height, std::vector<PixelGRAY>(this->width));
    //unsigned char* current_image = &image[0];
    //for (size_t i = 0; i < image.size(); i += 1) 
    //{
    //    //PixelGRAY pixelGRAY(image[i]);
    //    PixelGRAY pixelGRAY(*(current_image + i));
    //    pixelsGRAY[i /this->width].push_back(pixelGRAY);
    //}
}

void MyImage::displayPixelsGRAY() const
{
    for (const auto& row : pixelsGRAY) 
    {
        for (const auto& pixelGRAY : row) 
        {
            std::cout << "(" << static_cast<int>(pixelGRAY.grey) << ") \t";
        }
        std::cout << std::endl;
    }
}

void MyImage::resize(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0) 
    {
        std::cout << "Error in Resize" << std::endl;
        return;
    }

    // 创建新的像素矩阵
    std::vector<std::vector<PixelGRAY>> newPixelsGRAY(newHeight, std::vector<PixelGRAY>(newWidth));

    // 计算缩放因子
    double scaleX = static_cast<double>(newWidth) / static_cast<double>(width);
    double scaleY = static_cast<double>(newHeight) / static_cast<double>(height);

    // 执行双线性插值
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = x / scaleX;
            double srcY = y / scaleY;

            int x1 = static_cast<int>(std::floor(srcX));
            int x2 = static_cast<int>(std::ceil(srcX));
            int y1 = static_cast<int>(std::floor(srcY));
            int y2 = static_cast<int>(std::ceil(srcY));

            // 防止越界
            x1 = std::max<int>(0, std::min<int>(x1, width - 1));
            x2 = std::max<int>(0, std::min<int>(x2, width - 1));
            y1 = std::max<int>(0, std::min<int>(y1, height - 1));
            y2 = std::max<int>(0, std::min<int>(y2, height - 1));

            double weightX1 = 0.5;
            double weightX2 = 0.5;
            double weightY1 = 0.5;
            double weightY2 = 0.5;

            if (scaleX == 1 && scaleY == 1)
            {
                return;
            }
            else if (scaleX == 1)
            {
                weightX1 = 0.5;
                weightX2 = 0.5;
                weightY1 = y2 - srcY;
                weightY2 = srcY - y1;
            }
            else if (scaleY == 1)
            {
                weightX1 = x2 - srcX;
                weightX2 = srcX - x1;
                weightY1 = 0.5;
                weightY2 = 0.5;
            }
            else 
            {
                weightX1 = x2 - srcX;
                weightX2 = srcX - x1;
                weightY1 = y2 - srcY;
                weightY2 = srcY - y1;
            }

            // 开始双线性插值
            double interpolatedValue = weightX1 * weightY1 * pixelsGRAY[y1][x1].grey + weightX2 * weightY1 * pixelsGRAY[y1][x2].grey +
                                       weightX1 * weightY2 * pixelsGRAY[y2][x1].grey + weightX2 * weightY2 * pixelsGRAY[y2][x2].grey;

            // 四舍五入并存储
            newPixelsGRAY[y][x].grey = static_cast<unsigned char>(std::round(interpolatedValue));
        }
    }

    // 更新图像尺寸和像素矩阵
    width = newWidth;
    height = newHeight;
    pixelsGRAY = std::move(newPixelsGRAY);
}