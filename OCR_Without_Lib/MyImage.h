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
    std::vector<std::vector<Pixel>> pixels;  // ��ά�����洢��������
    std::vector<std::vector<PixelGRAY>> pixelsGRAY;  // ��ά�����洢��������
    //std::string filename;  // ͼ���ļ���
    const std::wstring filename;
    int width;  // ͼ����
    int height;  // ͼ��߶�

    // ���캯��
    MyImage() {
        this->height = 0;
        this->width = 0;
    }
    // ��ͨ����Ĭ�Ϲ��캯��
    MyImage(int h, int w, unsigned char grey) : height(h),width(w) {
        pixelsGRAY.resize(height, std::vector<PixelGRAY>(width, PixelGRAY(grey)));
    }

    MyImage(const std::wstring fname) : filename(fname)
    {
        // ��ȡͼ��
        readImageRGB(fname);
    }

    //MyImage(const std::string& fname) : filename(fname)
    //{
    //    // ��ȡͼ��
    //    readImageRGB(fname);
    //}

    // �������캯��
    MyImage(const MyImage& other)
    {
        width = other.width;
        height = other.height;
        pixels = other.pixels;
        pixelsGRAY = other.pixelsGRAY;
    }

    // ��ֵ���������
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


    // ��ȡͼ�����ݵĺ���
    void readImageRGB(const std::wstring fname);
    void readImageGRAY(const std::wstring fname);

    //void readImageRGB(const std::string fname);

    // ��ʾͼ������ֵ
    void displayPixelsRGB() const;
    void displayPixelsGRAY() const;

    // �ı�ͼ���С
    void resize(int newWidth, int newHeight);
};
