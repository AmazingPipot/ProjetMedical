#ifndef PARSERHDR_H
#define PARSERHDR_H

//#include<QString>
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QtGui>

enum DataType
{
    DT_NONE = 0,
    DT_UNKNOWN = 0,
    DT_BINARY = 1,
    DT_UNSIGNED_CHAR = 2,
    DT_SIGNED_SHORT = 4,
    DT_SIGNED_INT = 8,
    DT_FLOAT = 16,
    DT_COMPLEX = 32,
    DT_DOUBLE = 64,
    DT_RGB = 128,
    DT_ALL = 255
};

class ParserAnalyze75
{
public:
    int width;
    int height;
    int depth;
    DataType dataType;
    int bytepix; // byte(s) per voxel/pixel
    int sliceStart;
    float voxelsize[3];

    QString filename;

    short* dataSubsampled;
    int coef;
    bool headerLoaded;
    bool dataSubsampledLoaded;

    explicit ParserAnalyze75();

    ~ParserAnalyze75();
    bool load(QString filename);
    bool loadSubsampled(int coef);

    bool getImageXY(int sliceXY, QImage& image);
    bool getImageYZ(int sliceYZ, QImage& image);
    bool getImageXZ(int sliceXZ, QImage& image);

    bool getImageXY(int sliceXY, QRect rect, QImage& image);
    bool getImageYZ(int sliceYZ, QRect rect, QImage& image);
    bool getImageXZ(int sliceXZ, QRect rect, QImage& image);

private:
    int buffer32ToInt(char* buffer32);
    int buffer16ToInt(char* buffer16);
    float buffer32ToFloat(char* buffer32);

    inline short getPixelValue255(short b1, short b2)
    {
        return static_cast<short>(((b1<<8)+b2 + static_cast<double>(0x8000)) / static_cast<double>(0x10000) * 255);
    }
};

#endif // PARSERHDR_H
