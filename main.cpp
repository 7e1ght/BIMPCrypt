#include <iostream>

#include "bmpcrypt.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc == 2)
        cout << BMPCrypt::decrypt(argv[1]) << endl;
    else if(argc == 3)
    {
        BMPCrypt::crypt(argv[1], argv[2]);
        cout << "File encrypted." << endl;
    }
    else
        cout << "Error when parse keys." << endl;

    return 0;
}
