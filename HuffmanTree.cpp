#include "HuffmanTree.hpp"
#include "HuffmanBase.hpp"
#include <iostream>

// Destructor
HuffmanTree::~HuffmanTree() {
    clearTree(root);
}

void HuffmanTree::clearTree(HuffmanNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// Compress
std::string HuffmanTree::compress(const std::string &inputStr) {
    std::map<char, int> frequencyMap;
    for (char ch : inputStr) {
        frequencyMap[ch]++;
    }
    buildTree(frequencyMap);
    generateCodes(root, "");
    
    std::string compressedCode;
    for (char ch : inputStr) {
        compressedCode += huffmanCodes[ch];
    }
    return compressedCode;
}

// Serialize Tree
std::string HuffmanTree::serializeTree() const {
    std::string result;
    postOrderSerialize(root, result);
    return result;
}

void HuffmanTree::postOrderSerialize(HuffmanNode* node, std::string &result) const {
    if (!node) return;
    postOrderSerialize(node->left, result);
    postOrderSerialize(node->right, result);

    if (node->isLeaf()) {
        result += "L" + std::string(1, node->getCharacter());
    } else {
        result += "B";
    }
}
HuffmanNode* HuffmanTree::deserializeTree(const std::string& serializedTree, int& index) {
    if (index < 0) return nullptr;  // Base case: no more characters

    // Determine if current node is a leaf or branch
    // Check the current character and decrement index
    char current = serializedTree[index--];

    //Edge case where two L follow each other (one of the Ls is not a leaf node but rather a charecter)
    if (index >= 1 && serializedTree[index] == 'L' && serializedTree[index - 1] == 'L');
    // Step 1: Handle the leaf node case
    else if (index >= 0 && serializedTree[index] == 'L') {
        // Leaf node: decrement index again to consume 'L' and use current character as leaf data
        index--;  // Move past 'L'
        return new HuffmanNode(current, 0);  // Assuming leaf frequency is set to 0 as per example
    }
    // If it's a branch node ('B'), recursively build right and left children
    HuffmanNode* right = deserializeTree(serializedTree, index);
    HuffmanNode* left = deserializeTree(serializedTree, index);

    // Create branch node with left and right children, sum frequency placeholder
    return new HuffmanNode('\0', left->getFrequency() + right->getFrequency(), nullptr, left, right);

}

//decompress function
std::string HuffmanTree::decompress(const std::string &inputCode, const std::string &serializedTree) {
    // Step 1: Reconstruct the Huffman Tree from the serialized tree string
    int index = serializedTree.size() - 1;  // Start from the end of the serialized string
    root = deserializeTree(serializedTree, index);

    // Step 2: Decode the encoded string using the reconstructed Huffman Tree
    std::string decodedStr;
    HuffmanNode* currentNode = root;
    for (char bit : inputCode) {
        // Traverse left for '0' and right for '1'
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;
        // If we reach a leaf node, we found a character
        if (currentNode->isLeaf()) {
            decodedStr += currentNode->getCharacter();
            currentNode = root;  // Return to the root for the next character
        }
    }
    return decodedStr;
}

// Build Tree
void HuffmanTree::buildTree(const std::map<char, int> &frequencyMap) {
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> priorityQueue;

    for (const auto &pair : frequencyMap) {
        char ch = pair.first;
        int freq = pair.second;
        priorityQueue.insert(new HuffmanNode(ch, freq));  // Use insert() to add elements
    }

    while (priorityQueue.size() > 1) {
        HuffmanNode* left = priorityQueue.min(); 
        priorityQueue.removeMin(); // Use removeMin() to retrieve elements
        HuffmanNode* right = priorityQueue.min();
        priorityQueue.removeMin();

        HuffmanNode* parent = new HuffmanNode('\0', left->getFrequency() + right->getFrequency(), nullptr, left, right);
        priorityQueue.insert(parent);  // Use insert() to add parent node
    }
    root = priorityQueue.min(); 
    priorityQueue.removeMin(); // Final root node
}

// Generate Codes
void HuffmanTree::generateCodes(HuffmanNode* node, const std::string &code) {
    if (!node) return;
    if (node->isLeaf()) {
        huffmanCodes[node->getCharacter()] = code;
    } else {
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
}
