#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const string copyName = "cryptedBMP.bmp";
const int spliceMask = 0b00000011;

void makeCopy(string pathToFile)
{
    ifstream in(pathToFile, ios::binary);

    ofstream out(copyName, ios::binary);

    if(in.is_open())
        out << in.rdbuf();

    in.close();
    out.close();
}

uint16_t getWidthNum(ifstream& fileStream)
{
    int backup = fileStream.tellg();
    fileStream.seekg(0x12);
    uint16_t width;

    fileStream.read((char*)&width, sizeof (width));

    fileStream.seekg(backup);

    return width;
}

uint16_t getHeightNum(ifstream& fileStream)
{
    int backup = fileStream.tellg();
    fileStream.seekg(0x16);
    uint16_t height;

    fileStream.read((char*)&height, sizeof (height));

    fileStream.seekg(backup);

    return height;
}

uint32_t getOffBits(ifstream& fileStream)
{
    int backup = fileStream.tellg();
    fileStream.seekg(0x0a);

    uint32_t offBits;

    fileStream.read((char*)&offBits, sizeof(offBits));
    fileStream.seekg(backup);

    return offBits;
}

template <typename T>
vector<uint8_t> getSplices(T data)
{
    T mask = 3;

    vector<uint8_t> splices;

    for(unsigned i = 0, offset = 0;
        i < sizeof (T) * 4;
        i++, offset+=2)
    {
        splices.push_back( (data >> offset) & mask );

    }
    reverse(splices.begin(), splices.end());
    return splices;
}

uint8_t getOriginMask(uint8_t blueByte)
{
    return blueByte & 0b11111100;
}

uint8_t encryptBit(uint8_t bit, uint8_t splice)
{
    return getOriginMask(bit) | splice;
}

void crypt(string pathToFile, string text)
/*
 * Took path to file, create copy of picture, encrypt text size and text
 *
 * crypt(string pathToFile, string text)
 */
{
    makeCopy(pathToFile);

    ifstream inputPicture(copyName, ios::binary);
    ofstream toCrypt(copyName, ios::binary);

    const uint16_t textSize = text.length();

    const uint32_t width = getWidthNum(inputPicture);
    const uint32_t height = getHeightNum(inputPicture);
    const uint32_t offBits = getOffBits(inputPicture);



    if(inputPicture.is_open())
    {

    }

    inputPicture.close();
    toCrypt.close();
}

//for(int i = 0, offset = 0; i < 4; i++, offset+=2)
//    splices.push_back( size & (0b11000000 >> offset) );

int main()
{
//    crypt("penguin.bmp", "hello");
//    vector<uint8_t> temp = getSplices((unsigned short) 30);

//    for(unsigned i = 0; i < temp.size(); i++)
//        cout << hex << (int)temp[i] << " ";

    return 0;
}
