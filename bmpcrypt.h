#ifndef BMPCRYPT_H
#define BMPCRYPT_H

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class BMPCrypt
{
private:
    static void makeCopy(std::string pathToFile);
    static uint32_t getOffBits(std::fstream& fileStream);
    static uint8_t getOriginMask(uint8_t blueByte);
    static uint8_t encryptBit(uint8_t byte, uint8_t splice);
    static uint32_t getEncryptedTextSize(std::fstream& cryptedFile);

    template <typename T>
    static std::vector<uint8_t> getSplices(T data);

    template<typename T>
    static T read(std::fstream& stream);

    template<typename T>
    static void write(std::fstream& stream, T data);

    template<typename T>
    static void encryptWrite(std::fstream& stream, T data);

    template<typename T>
    static T decryptRead(std::fstream& stream);
public:
    BMPCrypt() = delete;
    ~BMPCrypt() = delete;

    static void crypt(const std::string pathToFile, const std::string text);
    static std::string decrypt(std::string pathToFile);
};

template <typename T>
std::vector<uint8_t> BMPCrypt::getSplices(T data)
{
    T mask = 3;

    std::vector<uint8_t> splices;

    for(unsigned i = 0, offset = 0;
        i < sizeof (T) * 4;
        i++, offset+=2)
    {
        splices.push_back( (data >> offset) & mask );

    }

    std::reverse(splices.begin(), splices.end());
    return splices;
}

template<typename T>
T BMPCrypt::read(std::fstream& stream)
{
    T byte;
    stream.read( (char*)&byte, sizeof(byte) );

    return byte;
}

template<typename T>
void BMPCrypt::write(std::fstream& stream, T data)
{
    stream.write( (char*)&data, sizeof (data) );
}

template<typename T>
void BMPCrypt::encryptWrite(std::fstream& stream, T data)
{
    std::vector<uint8_t> splices = getSplices(data);

    for(unsigned j = 0; j < splices.size(); j++)
    {
        uint8_t byte = read<uint8_t>(stream);

        stream.seekg(-1, std::ios::cur);

        uint8_t encryptedByte = encryptBit(byte, splices[j]);
        write(stream, encryptedByte);

        stream.seekg(2, std::ios::cur);
    }
}

template<typename T>
T BMPCrypt::decryptRead(std::fstream& stream)
{
    T data = 0;
    T spliceCount = sizeof (T) * 4;

    for(unsigned i = 0; i < spliceCount; i++)
    {
        uint8_t cryptedByte = read<uint8_t>(stream);

        uint8_t lastBits = cryptedByte & 0b00000011;

        data |= lastBits;

        if(i != spliceCount-1)  data <<= 2;

        stream.seekp(2, std::ios::cur);
    }

    return data;
}
#endif // BMPCRYPT_H
