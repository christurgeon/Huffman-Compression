#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>

// Node structure to hold information about a character
struct Node 
{
    char ch;
    int freq;
    Node *left, *right;
};


// Custom comparator for the priority queue
struct comparator 
{
    bool operator()(const Node* lhs, const Node* rhs)
    {
        return lhs->freq > rhs->freq;
    }
};


// Returns a pointer to a new Node
Node* createNode(char ch, int freq, Node* left, Node* right)
{
    Node *node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}


// Compresses down the file provided and writes it to memory
void buildTree(std::ifstream &infile, std::ofstream &outfile)
{
    if (!infile.is_open() || !outfile.is_open()) 
        return;

    std::priority_queue<Node*, std::vector<Node*>, comparator> pqueue;
    std::map<char, Node*> nodes;
    std::map<char, Node*>::iterator it;

    // Get the frequency of each character in the text file
    char ch;
    while (infile >> ch) 
    {
        if ((it = nodes.find(ch)) == nodes.end())  
        {
            nodes[ch] = createNode(ch, 1, NULL, NULL);
        }
        else 
        {
            nodes[ch]->freq++;
        }
    }
    
    // Add the nodes to the priority queue, ordered by lowest frequency
    for (it = nodes.begin(); it != nodes.end(); it++)
    {
        std::cout << "char: " << it->first << "  ===>  " << it->second->freq << std::endl; 
        pqueue.push(it->second);
    }

    // Create the Huffman tree
    while (!pqueue.empty()) 
    {
        Node* node_one = pqueue.pop();
        Node* node_two = pqueue.pop();
    }
}


// Read in files from command line and build Huffman Tree
int main(int argc, char** argv) 
{
    int files_compressed = 0;
    for (int i = 1; i < argc; i++) 
    {
        std::ifstream infile;
        std::string filename(argv[i]);
        infile.open(filename, std::ios::in);
        std::cout << "Compressing " << filename << "..." << std::endl;
        filename += "_compressed.bin";
        std::ofstream outfile;   
        outfile.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        buildTree(infile, outfile);
        files_compressed++;
        infile.close();
        outfile.close();
    }
    std::cout << "Compressed " << files_compressed << " files!" << std::endl; 
    return 0;
}