#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node;

struct Entry {
    int key;
    Node* right = nullptr;
};

struct Record {
    std::unordered_map<std::string, std::string> record;
};

struct Node {
    bool is_leaf = false;
    virtual ~Node() = default;
};

struct InternalNode : public Node {
    Node* first_child = nullptr;
    std::vector<Entry> entries;

    InternalNode() {
        is_leaf = false;
    };
};

struct LeafNode : public Node {
    std::vector<int> keys;
    std::vector<Record> records;
    LeafNode* next = nullptr;

    LeafNode() {
        is_leaf = true;
    };
};

//  || a || b || c || d ||


