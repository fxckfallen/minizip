#pragma once

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

struct comparator {
        bool operator()(Node* i, Node* j) {
        return i->freq > j->freq;
    }
};

std::vector<uint8_t> read_bytes(char* filepath);
std::vector<int> count_freq(std::vector<uint8_t> bytes);
std::priority_queue<Node*, std::vector<Node*>, comparator> freq_to_huffman_tree(std::vector<int> freq);
void generate_codes(Node* root, std::string code, std::unordered_map<uint8_t, std::string>& codes);

int compress(char* input, char* output);
int decompress(char* input, char* output);