#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

std::vector<uint8_t> read_bytes(char* filepath) {
    std::ifstream ifs;
    ifs.open(filepath, std::ios::binary);

    std::vector<uint8_t> chars;

    char c = ifs.get();

    while (ifs.good()) {
        uint8_t byte = static_cast<uint8_t>(c);
        chars.push_back(byte);
        
        c = ifs.get();
    }

    ifs.close();

    return chars;
}

std::vector<int> count_freq(std::vector<uint8_t> bytes) {
    std::vector<int> freq(256, 0);

    for (int i = 0; i < bytes.size(); ++i) {
        freq[bytes[i]]++;
    }

    return freq;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return 1;
    }
    std::vector<uint8_t> bytes = read_bytes(argv[1]);
    std::vector<int> freq = count_freq(bytes);

    for (int i = 0; i < 256; ++i) std::cout << static_cast<char>(i) << ": " << freq[i] << std::endl;

    return 0;
}