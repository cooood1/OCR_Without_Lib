#pragma once
#include "OCR_Without_Lib.h"
#include "lodepng.h"
#include <comdef.h>
#include <gdiplus.h>
#include <windows.h>
#pragma comment (lib, "gdiplus.lib")




class Pixel {
public:
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    Pixel() : red(0), green(0), blue(0) {}
    Pixel(unsigned char r, unsigned char g, unsigned char b) : red(r), green(g), blue(b) {}
};

class PixelGRAY {
public:
    unsigned char grey;

    PixelGRAY() : grey(0) {}
    PixelGRAY(unsigned char g) :grey(g) {}
};

class MyImage {

public:
    std::vector<std::vector<Pixel>> pixels;  // 二维向量存储像素数据
    std::vector<std::vector<PixelGRAY>> pixelsGRAY;  // 二维向量存储像素数据
    //std::string filename;  // 图像文件名
    const std::wstring filename;
    int width;  // 图像宽度
    int height;  // 图像高度

    // 构造函数
    MyImage() {
        this->height = 0;
        this->width = 0;
    }
    // 单通道的默认构造函数
    MyImage(int h, int w, unsigned char grey) : height(h),width(w) {
        pixelsGRAY.resize(height, std::vector<PixelGRAY>(width, PixelGRAY(grey)));
    }

    MyImage(const std::wstring fname) : filename(fname)
    {
        // 读取图像
        readImageRGB(fname);
    }

    //MyImage(const std::string& fname) : filename(fname)
    //{
    //    // 读取图像
    //    readImageRGB(fname);
    //}

    // 拷贝构造函数
    MyImage(const MyImage& other)
    {
        width = other.width;
        height = other.height;
        pixels = other.pixels;
        pixelsGRAY = other.pixelsGRAY;
    }

    // 赋值运算符重载
    MyImage& operator=(const MyImage& other)
    {
        if (this != &other) 
        {
            width = other.width;
            height = other.height;
            pixels = other.pixels;
            pixelsGRAY = other.pixelsGRAY;
        }
        return *this;
    }


    // 读取图像数据的函数
    void readImageRGB(const std::wstring fname);
    void readImageGRAY(const std::wstring fname);

    //void readImageRGB(const std::string fname);

    // 显示图像像素值
    void displayPixelsRGB() const;
    void displayPixelsGRAY() const;

    // 改变图像大小
    void resize(int newWidth, int newHeight);
};
