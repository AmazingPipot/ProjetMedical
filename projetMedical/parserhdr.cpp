#include "parserhdr.h"

ParserHDR::ParserHDR()
{

}
ParserHDR::~ParserHDR()
{

}

void ParserHDR::lectureHDR(QString filename)
{
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly))
    {
        fprintf(stderr,"le fichier est ouvert \n");
        QByteArray array = file.readAll();

        QString str;
        QString str2;

        foreach (char c, array)
        {
            str += QString("%1").arg(c,8, 2, QChar('0'));

            if (str.length() == 8)
            {
                str2.clear();
                for (int i = 1; i< 8; i++)
                {
                    str2 += str.at(i);
                }
                fprintf(stderr,"Valeur binaire2 %c \n",str2.fromLatin1(array.constData()).toStdString().c_str());
                fprintf(stderr,"Valeur binaire3 %s \n",str2.toStdString().c_str());
                str.clear();
            }
        }
    }
    else
    {
        fprintf(stderr,"le fichier n'est pas ouvert");
    }
    //(Img[i]%256)*256+Img[i]/256

    /*bool Mesh::loadFromObjFile(QString filename)
    {

        if(!VBO.create())

        {

            std::cerr << "Error creating VBO" << std::endl;

        }


        if(!VBO.bind())

        {

            std::cerr << "Error binding array buffer" << std::endl;

        }


        auto start = std::chrono::high_resolution_clock::now();

        std::vector<VertexData> vertices;


        std::string line;

        std::cout << "Trying to open file: " << filename.toStdString() << std::endl;

        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            return false;

        }


        QTextStream in(&file);


        unsigned int verticesCount = 0,

                     normalsCount = 0,

                     textCoordsCount = 0;


        trianglesCount = 0;


        QString type;

        char separator;

        unsigned int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;


        while(!in.atEnd())

        {

            in >> type;

            if (type == "v")

            {

                verticesCount++;

            }

            else if (type == "vn")

            {

                normalsCount++;

            }

            else if (type == "vt")

            {

                textCoordsCount++;

            }

            else if (type == "f")

            {

                trianglesCount++;

            }

            in.readLine();

        }


        in.seek(0);


        float* verticesData = new float[verticesCount * 3];

        float* normalsData = new float[normalsCount * 3];

        float* textCoordsData = new float[textCoordsCount * 2];


        verticesCount = 0;

        textCoordsCount = 0;

        normalsCount = 0;


        while(!in.atEnd())

        {

            in >> type;

            if (type == "v")

            {

                in >> verticesData[verticesCount] >> verticesData[verticesCount+1] >> verticesData[verticesCount+2];

                verticesCount += 3;

            }

            else if (type == "vn")

            {

                in >> normalsData[normalsCount] >> normalsData[normalsCount+1] >> normalsData[normalsCount+2];

                normalsCount += 3;

            }

            else if (type == "vt")

            {

                in >> textCoordsData[textCoordsCount] >> textCoordsData[textCoordsCount+1];

                textCoordsCount += 2;

                in.readLine();

            }

            else if (type == "f")

            {

                in >> v1 >> separator >> vt1 >> separator >> vn1

                   >> v2 >> separator >> vt2 >> separator >> vn2

                   >> v3 >> separator >> vt3 >> separator >> vn3;


                v1--; v2--; v3--;

                vt1--; vt2--; vt3--;

                vn1--; vn2--; vn3--;


                vertices.push_back({

                    QVector3D(verticesData[v1*3], verticesData[v1*3+1], verticesData[v1*3+2]),

                    QVector2D(textCoordsData[vt1*2], textCoordsData[vt1*2+1]),

                    QVector3D(normalsData[vn1*3], normalsData[vn1*3+1], normalsData[vn1*3+2])

                });


                vertices.push_back({

                    QVector3D(verticesData[v2*3], verticesData[v2*3+1], verticesData[v2*3+2]),

                    QVector2D(textCoordsData[vt2*2], textCoordsData[vt2*2+1]),

                    QVector3D(normalsData[vn2*3], normalsData[vn2*3+1], normalsData[vn2*3+2])

                });


                vertices.push_back({

                    QVector3D(verticesData[v3*3], verticesData[v3*3+1], verticesData[v3*3+2]),

                    QVector2D(textCoordsData[vt3*2], textCoordsData[vt3*2+1]),

                    QVector3D(normalsData[vn3*3], normalsData[vn3*3+1], normalsData[vn3*3+2])

                });

            }

        }


        VBO.allocate(vertices.data(), vertices.size() * sizeof(VertexData));


        delete[] verticesData;

        delete[] normalsData;

        delete[] textCoordsData;


        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << "file " << filename.toStdString() << " loaded in " << elapsed_seconds.count() << " second(s) ("<< vertices.size() << " vertices)." << std::endl;

        file.close();


        return true;

    }*/



}
