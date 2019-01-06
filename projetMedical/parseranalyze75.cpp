#include "parseranalyze75.h"
#include <iostream>
#include <fstream>
#include <string>
//#define cimg_display 0
//#include <CImg.h>
#include <limits>
#include <chrono>

//using namespace cimg_library;

ParserAnalyze75::ParserAnalyze75()
{
    dataSubsampledLoaded = false;
    headerLoaded = false;
}

ParserAnalyze75::~ParserAnalyze75()
{
    if (this->dataSubsampledLoaded)
        delete[] dataSubsampled;
}


bool ParserAnalyze75::load(QString filename)
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

        headerLoaded = true;

        return true;
    }
    else
    {
        std::cerr<< "Unable to open file" << std::endl;
        return false;
    }
}

bool ParserAnalyze75::loadSubsampled(int coef)
{
    this->coef = coef;

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    std::cout << ".img file size: " << file.size() << std::endl;

    const int widthSD = width / coef;
    const int heightSD = height / coef;
    const int depthSD = depth / coef;

    dataSubsampled = new short[static_cast<size_t>(widthSD * heightSD * depthSD)];

    char* buffer = new char[static_cast<size_t>(this->width * this->bytepix)];
    short*** dataTemp = new short**[static_cast<size_t>(coef)];
    for (int k = 0; k < coef; k++)
    {
        dataTemp[k] = new short*[static_cast<size_t>(this->height)];
        for (int j = 0; j < this->height; j++)
        {
            dataTemp[k][j] = new short[static_cast<size_t>(this->width)];
        }
    }

    int sum;
    for (int d = 0; d < depthSD; d++)
    {
        //std::cout << d / (depthSD - 1.0f) * 100.0f << "%" << std::endl;

        for (int k = 0; k < coef; k++)
        {
            for (int j = 0; j < this->height; j++)
            {
                file.read(buffer, this->width * this->bytepix);
                for (int i = 0; i < this->width; i++)
                {
                    if (this->bytepix == 1)
                    {
                        dataTemp[k][j][i] = static_cast<short>(buffer[i]);
                    }
                    else
                    {
                        dataTemp[k][j][i] = this->getPixelValue255(buffer[i*bytepix], buffer[(i*bytepix)+1]);
                    }
                }
            }
        }

        for (int y = 0; y < heightSD; y++)
        {
            for (int x = 0; x < widthSD; x++)
            {
                sum = 0;
                for (int k = 0; k < coef; k++)
                {
                    for (int j = 0; j < coef; j++)
                    {
                        for (int i = 0; i < coef; i++)
                        {
                            sum += dataTemp[k][y*coef+j][x*coef+i];
                        }
                    }
                }
                short result = static_cast<short>(sum / (coef * coef * coef));
                //std::cerr << "result: " << result << std::endl;
                dataSubsampled[d * heightSD * widthSD + y * widthSD + x] = result;
            }
        }
    }

    /*
    QImage image = QImage(widthSD, heightSD, QImage::Format::Format_RGB888);

    for (int j = 0; j < heightSD; j++)
    {
        for (int i = 0; i < widthSD; ++i)
        {
            short v = dataSD[100 * widthSD * heightSD + j * widthSD + i];
            image.setPixel(i, j, qRgb(v, v, v));
        }
    }
    image.save("imageSD.jpg");
    */

    // free
    for (int k = 0; k < coef; k++)
    {
        for (int j = 0; j < this->height; j++)
        {
            delete[] dataTemp[k][j];
        }
        delete[] dataTemp[k];
    }
    delete[] dataTemp;

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "image with subsampled coef " << coef << " generated in " << (elapsed.count() * 1000) << "ms (w:" << widthSD << " h:" << heightSD << " d: " << depthSD << ", " << (widthSD * heightSD * depthSD) << " pixels)" << std::endl;

    dataSubsampledLoaded = true;
    return true;
}

bool ParserAnalyze75::getImageXY(int sliceXY, QImage& image)
{
    if (sliceXY < 0 || sliceXY > depth - 1)
    {
        std::cerr << "SliceXY not in range(" << 0 << ", " << (depth - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    //std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(this->width, this->height, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(bytepix * this->width)];

    if(!file.skip(static_cast<int64_t>(this->width) * this->height * this->bytepix * sliceXY))
    {
        std::cout <<"ko" << std::endl;
    }
    int v;

    for (int j = 0; j < this->height; j++)
    {
        if(file.read(buffer, this->bytepix * this->width) != this->bytepix * this->width)
        {
            std::cout <<"ko" << std::endl;
        }
        for (int i = 0; i < this->width; ++i)
        {
            if (this->bytepix == 1)
            {
                v = static_cast<int>(buffer[i]);
            }
            else
            {
                v = this->getPixelValue255(buffer[i*bytepix], buffer[(i*bytepix)+1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXY " << sliceXY << " generated in " << (elapsed.count() * 1000) << "ms (w:" << width << " h:" << height << ", " << (this->width * this->height) << " pixels)" << std::endl;

    return true;
}

bool ParserAnalyze75::getImageYZ(int sliceYZ, QImage& image)
{
    if (sliceYZ < 0 || sliceYZ > width - 1)
    {
        std::cerr << "sliceYZ not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    //std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(this->depth, this->height, QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(width * bytepix * height)];

    file.seek(sliceYZ * bytepix);
    int v;
    for (int i = 0; i < depth; ++i)
    {
        file.read(buffer, width * bytepix * height);
        for (int j = 0; j < height; j++)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[width * j]);
            }
            else
            {
                v = this->getPixelValue255(buffer[width * bytepix * j], buffer[width * bytepix * j + 1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceYZ " << sliceYZ << " generated in " << (elapsed.count() * 1000) << "ms (w:" << depth << " h:" << height << ", " << std::to_string(this->depth * this->height) << " pixels)" << std::endl;

    return true;
}

bool ParserAnalyze75::getImageXZ(int sliceXZ, QImage& image)
{
    if (sliceXZ < 0 || sliceXZ > height - 1)
    {
        std::cerr << "sliceZY not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
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
                v = this->getPixelValue255(buffer[i*bytepix], buffer[(i*bytepix)+1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
        file.QFileDevice::skip((height-1) * width * bytepix);
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXZ " << sliceXZ << " generated in " << (elapsed.count() * 1000) << "ms (w:" << width << " h:" << depth << ", " << std::to_string(this->depth * this->width) << " pixels)" << std::endl;

    return true;
}

bool ParserAnalyze75::getImageXY(int sliceXY, QRect rect, QImage& image)
{
    if (sliceXY < 0 || sliceXY > depth - 1)
    {
        std::cerr << "sliceXY not in range(" << 0 << ", " << (depth - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
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

    image = QImage(rect.width(), rect.height(), QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(bytepix * rect.width())];

    file.seek(width * height * bytepix * sliceXY + rect.y() * width * bytepix + rect.x() * bytepix);
    int v;
    for (int j = 0; j < rect.height(); j++)
    {
        file.read(buffer, bytepix * rect.width());
        for (int i = 0; i < rect.width(); ++i)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[i]);
            }
            else
            {
                v = this->getPixelValue255(buffer[i*bytepix], buffer[i*bytepix + 1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
        file.skip((width - rect.width()) * bytepix);
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXY " << sliceXY << " generated in " << (elapsed.count() * 1000) << "ms (rect x:" << rect.x() << " y:" << rect.y() << " w:" << rect.width() << " h:" << rect.height() << ", " << (rect.width() * rect.height()) << " pixels)" << std::endl;

    return true;
}

bool ParserAnalyze75::getImageYZ(int sliceYZ, QRect rect, QImage& image)
{
    if (sliceYZ < 0 || sliceYZ > width - 1)
    {
        std::cerr << "sliceYZ not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    QFile file(this->filename + ".img");
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file: " << filename.toStdString() + ".img" << std::endl;
        return false;
    }

    //std::cout << ".img file size: " << file.size() << std::endl;

    image = QImage(rect.width(), rect.height(), QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(width * bytepix * height)];

    file.seek(sliceYZ * bytepix + rect.x() * bytepix * width * height);
    int v;
    for (int i = 0; i < rect.width(); ++i)
    {
        file.skip(rect.y() * width * bytepix);
        file.read(buffer, width * bytepix * rect.height());
        for (int j = 0; j < rect.height(); j++)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[width * j]);
            }
            else
            {
                v = this->getPixelValue255(buffer[width * bytepix * j], buffer[width * bytepix * j + 1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
        file.skip((height - rect.height() - rect.y()) * width * bytepix);
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceYZ " << sliceYZ << " generated in " << (elapsed.count() * 1000) << "ms (rect x:" << rect.x() << " y: " << rect.y() << " w:" << rect.width() << " h:" << rect.height() << ", " << std::to_string(rect.width() * rect.height()) << " pixels)" << std::endl;

    return true;
}

bool ParserAnalyze75::getImageXZ(int sliceXZ, QRect rect, QImage& image)
{
    if (sliceXZ < 0 || sliceXZ > height - 1)
    {
        std::cerr << "sliceZY not in range(" << 0 << ", " << (width - 1) << ")" << std::endl;
        return false;
    }

    if (!this->headerLoaded)
    {
        std::cerr << "Header is not loaded" << std::endl;
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

    image = QImage(rect.width(), rect.height(), QImage::Format::Format_RGB888);

    char* buffer = new char[static_cast<size_t>(rect.width() * bytepix)];

    file.seek(width * bytepix * sliceXZ + rect.y() * width * height * bytepix + rect.x() * bytepix);
    int v;
    for (int j = 0; j < rect.height(); ++j)
    {
        file.read(buffer, rect.width() * bytepix);
        for (int i = 0; i < rect.width(); ++i)
        {
            if (bytepix == 1)
            {
                v = static_cast<int>(buffer[i]);
            }
            else
            {
                v = this->getPixelValue255(buffer[i*bytepix], buffer[(i*bytepix)+1]);
            }
            image.setPixel(i, j, qRgb(v, v, v));
        }
        file.QFileDevice::skip((height-1) * width * bytepix + (width - rect.width()) * bytepix);
    }

    delete[] buffer;
    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "sliceXZ " << sliceXZ << " generated in " << (elapsed.count() * 1000) << "ms (rect x: " << rect.x() << " y:" << rect.y() << " w:" << rect.width() << " h:" << rect.height() << ", " << std::to_string(rect.width() * rect.height()) << " pixels)" << std::endl;

    return true;
}

int ParserAnalyze75::buffer32ToInt(char* buffer)
{
    return static_cast<int>(
        static_cast<unsigned char>(buffer[0]) << 24 |
        static_cast<unsigned char>(buffer[1]) << 16 |
        static_cast<unsigned char>(buffer[2]) << 8  |
        static_cast<unsigned char>(buffer[3]));
}

int ParserAnalyze75::buffer16ToInt(char* buffer)
{
    return static_cast<int>(
        static_cast<unsigned char>(buffer[0]) << 8 |
        static_cast<unsigned char>(buffer[1]));
}

float ParserAnalyze75::buffer32ToFloat(char* buffer)
{
    float f;
    unsigned char b[] = {static_cast<unsigned char>(buffer[3]), static_cast<unsigned char>(buffer[2]), static_cast<unsigned char>(buffer[1]), static_cast<unsigned char>(buffer[0])};
    memcpy(&f, &b, sizeof(float));
    return f;
}
