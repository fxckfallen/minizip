#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

struct Node
{
    uint8_t byte;
    int freq;
    Node* left;
    Node* right;
};


std::vector<uint8_t> read_bytes(char* filepath) {
    std::ifstream ifs;
    ifs.open(filepath, std::ios::binary);

    if (!ifs.is_open()) std::cout << "Failed to open file: [" << filepath << "]" << std::endl;

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

struct comparator {
        bool operator()(Node* i, Node* j) {
        return i->freq > j->freq;
    }
};

std::priority_queue<Node*, std::vector<Node*>, comparator> freq_to_huffman_tree(std::vector<int> freq) {
    std::priority_queue<Node*, std::vector<Node*>, comparator> tree;

    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            Node* node = new Node();
            node->byte = static_cast<uint8_t>(i);
            node->freq = freq[i];
            tree.push(node);
        }
    }

    while (tree.size() > 1) {
        Node* left = tree.top();
        tree.pop();
        Node* right = tree.top();
        tree.pop();
        Node* root = new Node();
        root->freq = right->freq + left->freq;
        root->left = left;
        root->right = right;
        tree.push(root);
    }

    return tree;
}

void generate_codes(
    Node* root, 
    std::string code, 
    std::unordered_map<uint8_t, std::string>& codes
) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        codes[root->byte] = code;
        return;
    }

    generate_codes(root->left, code + "0", codes);
    generate_codes(root->right, code + "1", codes);
}

int compress(char* input, char* output) {
    std::vector<uint8_t> bytes = read_bytes(input);
    std::vector<int> freq = count_freq(bytes);

    std::priority_queue<Node*, std::vector<Node*>, comparator> tree = freq_to_huffman_tree(freq);

    if (tree.size() == 0) {
        std::cout << "Tree is empty" << std::endl;
        return 1;
    }
    Node* root = tree.top();

    std::unordered_map<uint8_t, std::string> codes;

    generate_codes(root, "", codes);

    uint8_t accumulator = 0;
    int bit_count = 0;

    std::ofstream ofs;

    ofs.open(output, std::ios::binary);

    ofs.write(reinterpret_cast<const char*>(freq.data()), freq.size() * sizeof(int));

    for (uint8_t byte : bytes) {
        std::string code = codes[byte];
        
        for (char symb : code) {
            accumulator <<= 1;
            if (symb == '1') {
                accumulator |= 1;
            }
            bit_count++;
            if (bit_count == 8) {
                ofs.put(accumulator);
                accumulator = 0;
                bit_count = 0;
            }
        }
    }
    if (bit_count > 0) {
        accumulator <<= (8 - bit_count);
        ofs.put(accumulator);
    }
   
    ofs.close();

    std::cout << "Compressed." << std::endl;

    return 0;
}

int decompress(char* input, char* output) {
    std::vector<int> freq(256, 0);
    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open(input, std::ios::binary);
    ofs.open(output, std::ios::binary);
    if (!ifs.is_open()) std::cout << "Failed to open file: [" << input << "]" << std::endl;
    if (!ofs.is_open()) std::cout << "Failed to open file: [" << output << "]" << std::endl;
    ifs.read(reinterpret_cast<char*>(freq.data()), 256 * sizeof(int));

    std::priority_queue<Node*, std::vector<Node*>, comparator> tree = freq_to_huffman_tree(freq);
    if (tree.size() == 0) return 1;
    Node* root = tree.top();
    Node* current = root;

    long long total_chars = 0;
    for (int symb : freq) {
        total_chars += symb;
    }

    long long decoded_chars = 0;

    char c = ifs.get();

    while (ifs.good()) {
        uint8_t byte = static_cast<uint8_t>(c);
        
        for (int i = 7; i >= 0; i--) {
            bool bit = (byte >> i) & 1;
            current = bit ? current->right : current->left;
            if (current->left == nullptr && current->right == nullptr) {
                ofs.put(current->byte);
                current = root;
                decoded_chars++;
                if (decoded_chars == total_chars) {
                    std::cout << "Decompressed." << std::endl;
                    ifs.close();
                    ofs.close();
                    return 0; 
                }
            }
        }

        c = ifs.get();
    }
    
    
    return 1;
}

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