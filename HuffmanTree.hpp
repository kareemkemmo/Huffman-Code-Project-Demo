#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <string>
#include <map>
#include <stack>
#include <vector>
#include "HeapQueue.hpp" // Priority Queue for handling nodes
#include "HuffmanBase.hpp" // Base classes including HuffmanNode

class HuffmanTree {
public:
    HuffmanTree() : root(nullptr) {}  // Constructor initializes root to nullptr

    // Compress the input string into a Huffman-encoded string
    std::string compress(const std::string &inputStr);

    // Serialize the Huffman tree to a string for storage or transmission
    std::string serializeTree() const;

    // Decompress the Huffman-encoded string using the serialized tree
    std::string decompress(const std::string &inputCode, const std::string &serializedTree);

    // Destructor to properly clean up the tree
    ~HuffmanTree();

private:
    // Root of the Huffman Tree
    HuffmanNode* root;

    // Helper function to build the Huffman Tree from frequency map
    void buildTree(const std::map<char, int> &frequencyMap);

    // Helper function to generate Huffman codes from the tree
    void generateCodes(HuffmanNode* node, const std::string &code);

    // Helper function to serialize the tree using post-order traversal
    void postOrderSerialize(HuffmanNode* node, std::string &result) const;

    // Helper function to deserialize the tree from a serialized string
    HuffmanNode* deserializeTree(const std::string &serializedTree, int &index);

    // Storage for generated Huffman codes
    std::map<char, std::string> huffmanCodes;

    // Function to clear the tree (for destructor)
    void clearTree(HuffmanNode* node);
};

#endif // HUFFMANTREE_HPP
