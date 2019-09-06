#include <iostream>
#include <fstream>
#include <string>

// Node structure to hold information about a character
struct Node 
{
    char c;
    int freq;
    Node *left, *right;
};

// Returns a pointer to a new Node
Node* createNode(char c, int freq, Node *left, Node *right)
{
    Node *node = new Node();
    node->c = c;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// Compresses down the file provided and writes it to memory
void buildTree(std::ifstream &infile, std::ofstream &outfile)
{

}

int main(int argc, char **argv) 
{
    int files_compressed = 0;
    for (int i = 1; i < argc; i++) 
    {
        std::ifstream infile;
        std::string filename(argv[i]);
        infile.open(filename, std::ios::in);
        std::cout << "Compressing " << filename << "..." << std::endl;
        filename += "_compressed.dat";
        std::ofstream outfile;   
        outfile.open(filename, std::ios::out | std::ios::trunc);
        buildTree(infile, outfile);
        files_compressed++;
        infile.close();
        outfile.close();
    }
    std::cout << "Compressed " << files_compressed << " files!" << std::endl; 
    return 0;
}