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


// Encodes the string text using Huffman code
void encodeString(Node* root, std::string str, std::map<char, std::string> &char_code)
{
	if (root == NULL) {
		return;
    }
	if (!root->left && !root->right) {
		char_code[root->ch] = str;
    }
	encodeString(root->left, str + "0", char_code);
	encodeString(root->right, str + "1", char_code);
}


// Decode a file back to its original state
void decodeString(Node* root, int &idx, std::string str)
{
    if (root == NULL) 
        return;
    if (!root->left && !root->right) 
        return;

    idx++;
    if (str[idx] == '1') 
        decodeString(root->right, idx, str);
    else 
        decodeString(root->left, idx, str);
}


// Compresses down the file provided and writes it to memory
void buildTree(std::ifstream &infile, std::ofstream &outfile)
{
    if (!infile.is_open() || !outfile.is_open()) {
        return;
    }

    std::priority_queue<Node*, std::vector<Node*>, comparator> pqueue;
    std::map<char, Node*> nodes;
    std::map<char, Node*>::iterator it;

    // Get the frequency of each character in the text file
    char ch;
    std::string file_text = "";
    while (infile >> ch) 
    {
        if ((it = nodes.find(ch)) == nodes.end()) {
            nodes[ch] = createNode(ch, 1, NULL, NULL);
        }
        else {
            nodes[ch]->freq++;
        }
        file_text += ch;
    }
    
    // Add the nodes to the priority queue, ordered by lowest frequency
    for (it = nodes.begin(); it != nodes.end(); it++) {
        pqueue.push(it->second);
    }

    // Create the Huffman tree
    while (pqueue.size() != 1) 
    {
        // Create a new node with left and right node as children; freq as sum
        Node* l_node = pqueue.top(); pqueue.pop();
        Node* r_node = pqueue.top(); pqueue.pop();
        int sum = l_node->freq + r_node->freq;
        pqueue.push(createNode('\0', sum, l_node, r_node));
    }

    // Get encoding for each character
    Node* root = pqueue.top();
    std::map<char, std::string> char_code;
    encodeString(root, "", char_code);

    // Build the encoding of the entire file
    std::string encoding = "";
    for (char ch : file_text) {
        encoding += char_code[ch];
    }

    // Dump the encoded file
    outfile.write(encoding.c_str(), encoding.size());    
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