#include <iostream>
#include <string>

#include "Archiver.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " method input output" << std::endl;
        return 1;
    }

    int result;
    if (std::string(argv[1]) == "compress") {
        result = compress(argv[2], argv[3]);
    }
    else {
        result = decompress(argv[2], argv[3]);
    }

    return result;
}