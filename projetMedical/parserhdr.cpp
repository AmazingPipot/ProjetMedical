#include "parserhdr.h"
#include <iostream>
#include <fstream>
#include <string.h>
//#define cimg_display 0
//#include <CImg.h>
#include <limits>
#include <chrono>

//using namespace cimg_library;

ParserHDR::ParserHDR()
{

}

ParserHDR::~ParserHDR()
{

}

bool ParserHDR::readIMG(QString filename)
{
    auto start = std::chrono::high_resolution_clock::now();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() << std::endl;
        return false;
    }

    std::cout << ".img file size: " << file.size() << std::endl;

    QImage test(this->width, this->height, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(2 * this->width * this->height)];

    const int sliceToDisplay = 108;

    file.seek(2 * width * height * sliceToDisplay);

    int v;
    for (int j = 0; j < height; j++)
    {
        file.read(buffer, 2 * width);
        for (int i = 0; i < width; ++i)
        {
            v = static_cast<int>(((buffer[i*2]<<8)+buffer[i*2 + 1] + pow(2, 16) / 2) / pow(2, 16) * 255);
            test.setPixel(i, j, qRgb(v, v, v));
        }
    }
    delete[] buffer;
    test.save("slice.jpg");

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "slice " << sliceToDisplay << " generated in " << (elapsed.count() * 1000) << "ms" << std::endl;



    return true;
}

void ParserHDR::readHDR(QString filename)
{
    std::ifstream file;
    file.open(filename.toStdString().c_str(), std::ios::in | std::ios::binary);

    char buffer16[2];
    char buffer32[4];
    if (file.is_open())
    {
        file.read(buffer32, 4);

        std::cout << "sizeof_hdr: " << buffer32ToInt(buffer32) << std::endl;

        file.seekg(40, file.beg);

        file.read(buffer16, 2);
        std::cout << "dim: " << buffer16ToInt(buffer16) << std::endl;

        file.read(buffer16, 2);
        this->width = buffer16ToInt(buffer16);
        std::cout << "width(x): " << this->width << std::endl;
        file.read(buffer16, 2);
        this->height = buffer16ToInt(buffer16);
        std::cout << "height(y): " << this->height << std::endl;
        file.read(buffer16, 2);
        this->depth = buffer16ToInt(buffer16);
        std::cout << "depth(z): " << this->depth << std::endl;
        file.read(buffer16, 2);
        std::cout << "t: " << buffer16ToInt(buffer16) << std::endl;

        file.seekg(70, file.beg);

        file.read(buffer16, 2);
        this->dataType = static_cast<DataType>(buffer16ToInt(buffer16));
        std::cout << "datatype: " << this->dataType << std::endl;

        file.seekg(74, file.beg);

        file.read(buffer16, 2);
        this->sliceStart = buffer16ToInt(buffer16);
        std::cout << "slice_start: " << this->sliceStart << std::endl;

        file.read(buffer32, 4);
        std::cout << "voxel_0: " << buffer32ToFloat(buffer32) << std::endl;

        file.read(buffer32, 4);
        this->voxelsize[0] = buffer32ToFloat(buffer32);
        std::cout << "voxel_width: " << this->voxelsize[0] << std::endl;

        file.read(buffer32, 4);
        this->voxelsize[1] = buffer32ToFloat(buffer32);
        std::cout << "voxel_height: " << this->voxelsize[1] << std::endl;

        file.read(buffer32, 4);
        this->voxelsize[2] = buffer32ToFloat(buffer32);
        std::cout << "voxel_depth: " << this->voxelsize[2] << std::endl;

        file.close();
    }
    else
    {
        std::cerr<< "Unable to open file" << std::endl;
    }

}

int ParserHDR::buffer32ToInt(char* buffer)
{
    return static_cast<int>(
        static_cast<unsigned char>(buffer[0]) << 24 |
        static_cast<unsigned char>(buffer[1]) << 16 |
        static_cast<unsigned char>(buffer[2]) << 8  |
        static_cast<unsigned char>(buffer[3]));
}

int ParserHDR::buffer16ToInt(char* buffer)
{
    return static_cast<int>(
        static_cast<unsigned char>(buffer[0]) << 8 |
        static_cast<unsigned char>(buffer[1]));
}

float ParserHDR::buffer32ToFloat(char* buffer)
{
    float f;
    unsigned char b[] = {static_cast<unsigned char>(buffer[3]), static_cast<unsigned char>(buffer[2]), static_cast<unsigned char>(buffer[1]), static_cast<unsigned char>(buffer[0])};
    memcpy(&f, &b, sizeof(float));
    return f;
}
