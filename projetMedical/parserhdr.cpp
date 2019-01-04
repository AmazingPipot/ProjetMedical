#include "parserhdr.h"
#include <iostream>
#include <fstream>
#include <string>

#include <limits>
#include <chrono>

//using namespace cimg_library;

ParserHDR::ParserHDR()
{

}

ParserHDR::~ParserHDR()
{

}


bool ParserHDR::load(QString filename)
{
    this->filename = filename;

    std::ifstream file;
    file.open(filename.toStdString() + ".hdr", std::ios::in | std::ios::binary);

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

        file.seekg(72, file.beg);

        file.read(buffer16, 2);
        this->bytepix = buffer16ToInt(buffer16) >> 3;
        std::cout << "bytepix: " << this->bytepix << std::endl;
        if (this->bytepix != 1 && this->bytepix != 2)
        {
            std::cerr << "format not supported (bytepix= " << this->bytepix << std::endl;
            return false;
        }

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
        return true;
    }
    else
    {
        std::cerr<< "Unable to open file" << std::endl;
        return false;
    }
}

bool ParserHDR::getImageXY(int sliceXY, QImage& image)
{
    if (sliceXY < 0 || sliceXY > depth - 1)
    {
        std::cerr << "sliceXY not in range(" << 0 << ", " << (depth - 1) << ")" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    // std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(this->width, this->height, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(bytepix * this->width)];

    file.seek(width * height * bytepix * sliceXY);
    int v;
    for (int j = 0; j < height; j++)
    {
        file.read(buffer, bytepix * width);
        for (int i = 0; i < width; ++i)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[i]);
            }
            else
            {
                v = static_cast<int>(((buffer[i*bytepix]<<8)+buffer[i*bytepix + 1] + pow(2, 16) / 2) / pow(2, 16) * 255);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXY " << sliceXY << " generated in " << (elapsed.count() * 1000) << "ms (" << (this->width * this->height) << " pixels)" << std::endl;

    return true;
}

bool ParserHDR::getImageYZ(int sliceYZ, QImage& image)
{
    if (sliceYZ < 0 || sliceYZ > width - 1)
    {
        std::cerr << "sliceZY not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    // std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(this->depth, this->height, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(bytepix)];

    file.seek(sliceYZ * bytepix);
    int v;
    for (int i = 0; i < depth; ++i)
    {
        for (int j = 0; j < height; j++)
        {
            //file.QIODevice::seek(i * width * height * 2 + j * width * 2 + sliceYZ * 2);
            file.read(buffer, bytepix);
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[0]);
            }
            else
            {
                v = static_cast<int>(((buffer[0]<<8)+buffer[1] + pow(2, 16) / 2) / pow(2, 16) * 255);
            }
            image.setPixel(i, j, qRgb(v, v, v));
            file.QFileDevice::seek(file.pos() + width * bytepix - bytepix);
        }
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceYZ " << sliceYZ << " generated in " << (elapsed.count() * 1000) << "ms (" << std::to_string(this->depth * this->height) << " pixels)" << std::endl;

    return true;
}

bool ParserHDR::getImageXZ(int sliceXZ, QImage& image)
{
    if (sliceXZ < 0 || sliceXZ > height - 1)
    {
        std::cerr << "sliceZY not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    // std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(this->width, this->depth, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(this->width * bytepix)];

    file.seek(width * bytepix * sliceXZ);
    int v;
    for (int j = 0; j < depth; ++j)
    {
        file.read(buffer, this->width * bytepix);
        for (int i = 0; i < width; ++i)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[i]);
            }
            else
            {
                v = static_cast<int>(((buffer[i*bytepix]<<8)+buffer[(i*bytepix)+1] + pow(2, 16) / 2) / pow(2, 16) * 255);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
        file.QFileDevice::seek(file.pos() + (height-1) * width * bytepix);
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXZ " << sliceXZ << " generated in " << (elapsed.count() * 1000) << "ms (" << std::to_string(this->depth * this->width) << " pixels)" << std::endl;

    return true;
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
