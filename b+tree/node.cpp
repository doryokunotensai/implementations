#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node;

struct Record {
    // overly naive: just stores value of each row corresponding 
    // to its column in the appropriate index, so record[i] is 
    // value corresponding to ith column in the table
    std::vector<std::string> record;
};

struct Node {
    bool is_leaf = false;
    virtual ~Node() = default;
};

struct InternalNode : public Node {
    Node* first_child = nullptr;
    std::vector<int> keys;
    
    // right_children[i] points to Node where all values >= keys[i]
    std::vector<Node*> right_children; 

    InternalNode() {
        is_leaf = false;
    };
};

struct LeafNode : public Node {
    std::vector<int> keys;
    std::vector<Record> records;
    LeafNode* prev = nullptr;
    LeafNode* next = nullptr;

    LeafNode() {
        is_leaf = true;
    };
};

