#include "bmpcrypt.h"

#include <cmath>

using namespace std;


const string copyName = "cf.bmp";
const uint8_t spliceCharCount = 4;
const uint8_t spliceNumCount = 8;

void BMPCrypt::makeCopy(string pathToFile)
{
    ifstream in(pathToFile, ios::binary);

    ofstream out(copyName, ios::binary);

    if(in.is_open())
        out << in.rdbuf();

    in.close();
    out.close();
}
uint32_t BMPCrypt::getOffBits(fstream& fileStream)
{
    int backup = fileStream.tellg();
    fileStream.seekg(0x0a);

    uint32_t offBits;

    fileStream.read((char*)&offBits, sizeof(offBits));

    fileStream.seekg(backup);
    return offBits;
}
uint8_t BMPCrypt::getOriginMask(uint8_t blueByte)
{
    return blueByte & 0b11111100;
}
uint8_t BMPCrypt::encryptBit(uint8_t byte, uint8_t splice)
{
    return getOriginMask(byte) | splice;
}
void BMPCrypt::crypt(const string pathToFile, const string text)
/*
 * Took path to file, create copy of picture, encrypt text size and text
 *
 * crypt(string pathToFile, string text)
 */
{
    makeCopy(pathToFile);

    fstream inputPicture(copyName);

    if(inputPicture.is_open())
    {
        const uint16_t textSize = text.length();
        const uint32_t offBits = getOffBits(inputPicture);
        inputPicture.seekg(offBits);

        encryptWrite(inputPicture, textSize);

        for(int i = 0; i < textSize; i++)
        {
            encryptWrite(inputPicture, text[i]);
        }
    }

    inputPicture.close();
}
uint32_t BMPCrypt::getEncryptedTextSize(fstream& cryptedFile)
{
    uint16_t textSize = 0;
    uint32_t offBits = getOffBits(cryptedFile);

    cryptedFile.seekp(offBits);

    for(unsigned i = 0; i < spliceNumCount; i++)
    {
        uint8_t cryptedByte = read<uint8_t>(cryptedFile);

        uint8_t lastBits = cryptedByte & 0b00000011;

        textSize |= lastBits;

        if(i != 7)  textSize <<= 2;

        cryptedFile.seekp(2, ios::cur);
    }

    return textSize;
}
string BMPCrypt::decrypt(string pathToFile)
{
    fstream inFile(pathToFile);
    inFile.seekp(getOffBits(inFile));

    uint16_t textSize = decryptRead<uint16_t>(inFile);
    string str = "";


    for(unsigned i = 0; i < textSize; i++)
    {
        uint8_t ch = decryptRead<uint8_t>(inFile);

        str += ch;
    }

    return str;
}
