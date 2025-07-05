#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for min-heap (priority queue)
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Recursively generate Huffman Codes and fill the map
void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) // Leaf node
        huffmanCode[root->ch] = code;
    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

// Recursively print the Huffman Tree
void printTree(Node* root, string indent = "", bool last = true) {
    if (!root) return;
    cout << indent;
    if (last) {
        cout << "└─";
        indent += " ";
    } else {
        cout << "├─";
        indent += "| ";
    }
    if (root->ch)
        cout << "'" << root->ch << "' (" << root->freq << ")" << endl;
    else
        cout << "* (" << root->freq << ")" << endl;
    printTree(root->left, indent, false);
    printTree(root->right, indent, true);
}

// Build Huffman Tree and generate codes
Node* buildHuffmanTree(const string& text, unordered_map<char, string>& huffmanCode, unordered_map<char, int>& freq) {
    // Count frequency
    for (char ch : text) freq[ch]++;
    // Min-heap for nodes
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& p : freq)
        pq.push(new Node(p.first, p.second));
    // Build tree
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    Node* root = pq.top();
    generateCodes(root, "", huffmanCode);
    return root;
}

// Calculate original bit size (8 bits per char)
int getOriginalBitSize(const string& text) {
    return text.length() * 8;
}

// Calculate compressed bit size
int getCompressedBitSize(const string& text, unordered_map<char, string>& huffmanCode) {
    int bits = 0;
    for (char ch : text)
        bits += huffmanCode[ch].length();
    return bits;
}

// Print original bit table
void printOriginalBitTable(const unordered_map<char, int>& freq) {
    cout << left << setw(10) << "Char" << setw(10) << "ASCII" << setw(10) << "Bits" << setw(10) << "Count" << endl;
    cout << "-------------------------------------" << endl;
    for (auto& p : freq)
        cout << left << setw(10) << (p.first == ' ' ? "' '" : string(1, p.first)) << setw(10) << int(p.first) << setw(10) << 8 << setw(10) << p.second << endl;
}

// Print Huffman code table
void printHuffmanTable(const unordered_map<char, string>& huffmanCode, const unordered_map<char, int>& freq) {
    cout << left << setw(10) << "Char" << setw(15) << "Huffman Code" << setw(10) << "Count" << endl;
    cout << "-------------------------------------" << endl;
    for (auto& p : huffmanCode)
        cout << left << setw(10) << (p.first == ' ' ? "' '" : string(1, p.first)) << setw(15) << p.second << setw(10) << freq.at(p.first) << endl;
}

int main() {
    string text;
    cout << "Enter your text: ";
    getline(cin, text);

    unordered_map<char, int> freq;
    unordered_map<char, string> huffmanCode;
    Node* root = buildHuffmanTree(text, huffmanCode, freq);

    int choice;
    do {
        cout << "\nMenu:\n"; 
        cout << "1. Reduce (Compress)\n"; 
        cout << "2. See the Huffman Tree\n";
        cout << "3. See the Original Bit Size\n";
        cout << "4. See the Original Bit Table\n";
        cout << "5. See the New Bit Size\n";
        cout << "6. See the New Bit Table\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "Compressed bit string:\n";
                for (char ch : text) cout << huffmanCode[ch];
                cout << endl;
                break;
            }
            case 2: {
                cout << "Huffman Tree:\n";
                printTree(root);
                break;
            }
            case 3: {
                cout << "Original bit size: " << getOriginalBitSize(text) << " bits" << endl;
                break;
            }
            case 4: {
                cout << "Original Bit Table:\n";
                printOriginalBitTable(freq);
                break;
            }
            case 5: {
                cout << "Compressed bit size: " << getCompressedBitSize(text, huffmanCode) << " bits" << endl;
                break;
            }
            case 6: {
                cout << "Huffman Code Table:\n";
                printHuffmanTable(huffmanCode, freq);
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
