#pragma once
#include<iostream> 
#include <fstream>
using namespace std;

// An AVL tree node
template<class T>
struct AVLNode
{
    T key; // Key in node
    int lineNumber;
    T nodeID;
    AVLNode<T>* left;  // Left child
    AVLNode<T>* right; // Right child
    AVLNode<T>* parent;
    int height;   //Height of node
    AVLNode() {
        key = nodeID = "-1";
        height = lineNumber = -1;
        left = right = parent = nullptr;
    }
};
//Main templatized class
template <class T>
class AVLTree {
    string filepath; //stores file name
public:
    AVLNode<T>* root;
    AVLTree() {
        root = nullptr;
    }
    AVLTree(string machineid) {
        filepath = "file" + machineid + ".txt";
        root = nullptr;
    }
    ~AVLTree() {

    }
    //Inserts key nodeID and value
    void insertvalue(T key, T value, T nodeiD) {
        insert(root, key, value, nodeiD);//calling recursive insert function
    }
    void setFile(string machineid) {//sets file name
        filepath = "file" + machineid + ".txt";
    }
    string getFile()const {//returns file name
        return filepath;
    }
    //searches key from avl and returns value
    T search(AVLNode<T>*& r, T key) {
        T s = "";
        if (r->key == key) {
            //cout<<"found"<<endl;
           // cout<<r->lineNumber<<endl;
            //cout<<filepath<<endl;

            s = ReadFromFile(filepath, r->lineNumber);
            //cout<<"func : "<<s<<endl;
            return s;
        }
        if (r->left != nullptr) {
            s = search(r->left, key);
        }
        if (r->right != nullptr) {
            s = search(r->right, key);
        }
        return s;
    }
    //searches id from tree and returns true if found
    bool searchtree(AVLNode<T>*& r, int id) {
        bool check = false;
        if (stoi(r->nodeID) == id) {
            return true;
        }
        if (r->left != nullptr) {
            check = searchtree(r->left, id);
        }
        if (r->right != nullptr) {
            check = searchtree(r->right, id);
        }
        return check;
    }
    //Creates and returns a new Node
    AVLNode<T>* CreateNewNode(T key, T value, T nodeid) {
        AVLNode<T>* temp = new AVLNode<T>;
        temp->key = key;
        temp->nodeID = nodeid;
        temp->left = nullptr;
        temp->right = nullptr;
        temp->parent = nullptr;
        WriteToFile(filepath, key, value);//function call to write value to file
        temp->lineNumber = getLastLineNumber(filepath);
        return(temp);
    }
    //Recursive insert function
    void insert(AVLNode<T>*& r, T key, T value, T nodeid) {
        if (isEmpty()) {
            r = CreateNewNode(key, value, nodeid);
            //cout << "Root: " << r->key << endl;
            return;
        }
        if (r != nullptr && r->key > key) {
            if (r->left == nullptr) {
                r->left = CreateNewNode(key, value, nodeid);
                r->left->parent = r;
                return;
            }
            else {
                insert(r->left, key, value, nodeid);
                //balance(r);
            }
        }
        if (r != nullptr && r->key < key) {
            if (r->right == nullptr) {
                r->right = CreateNewNode(key, value, nodeid);
                r->right->parent = r;
                return;
            }
            else {
                insert(r->right, key, value, nodeid);
                // balance(r);
            }
        }
        else
            return;
    }
    //Recursive function to search for the node to be deleted
    AVLNode<T> deletenode(T num, AVLNode<T>*& nodePtr) {
        if (nodePtr == nullptr) {
            cout << "Not found" << endl;
            AVLNode<T>* ret = new AVLNode<T>;
            return *ret;
        }
        else if (stoi(num) < stoi(nodePtr->key))
            deletenode(num, nodePtr->left);
        else if (stoi(num) > stoi(nodePtr->key))
            deletenode(num, nodePtr->right);
        else
            return deletion(nodePtr);
        //balance(nodePtr);
    }
    //Deletes the node from tree and returns it
    AVLNode<T> deletion(AVLNode<T>*& root) {
        AVLNode<T>* temp;
        //deleting value from file
        if (root->right == nullptr) {
            temp = root;
            cout << "\nDeleted value at : " << root->key << endl;
            root = root->left;
        }
        else if (root->left == nullptr) {
            temp = root;
            cout << "\nDeleted value at : " << root->key << endl;
            root = root->right;
        }
        else {
            temp = root->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            temp->left = root->left;
            temp = root;
            cout << "\nDeleted value at : " << root->key << endl;
            root = root->right;
        }
        deletefromfile(filepath, temp->lineNumber);
        return *temp;
    }
    //Recrusive function to balance tree if required
    void balance(AVLNode<T>* r) {
        //cout << "balance: " << r->key << endl;
        //display(root);
        if ((getHeight(r->right) - getHeight(r->left)) != 0 && (getHeight(r->right) - getHeight(r->left)) != 1 && (getHeight(r->right) - getHeight(r->left)) != 1) { //Checking balance factor here
            if (r->right->left != nullptr) { // Checking if double rotation required
                if (r->key < r->right->left->key) {
                    rotateright(r->right); //First rotate right
                    rotateleft(r); //Then rotate left
                }
            }
            else
                rotateleft(r); //If not double rotation, single rotation left
        }
        else if ((getHeight(r->left) - getHeight(r->right)) > 1 || (getHeight(r->left) - getHeight(r->right)) < -1) { // Checking balance factor
            if (r->left->right != nullptr) {
                if (r->key < r->left->right->key) { // Checking if double rotation required
                    rotateleft(r->left); //First rotate left
                    rotateright(r); // Then rotate right
                }
            }
            else
                rotateright(r); // If no double rotation required, single rotateright
        }
        if (r->left != nullptr) { // Keep searching left child
            balance(r->left);
        }
        if (r->right != nullptr) { // Keep searching right child
            balance(r->right);
        }
        else {
            return;
        }
    }
    // Function that rotates left if needed
    void rotateleft(AVLNode<T>* r) {
        AVLNode<T>* temp;
        //cout << "l->" << r->key << endl;
        temp = r->right->left;
        r->right->left = r;
        if (r == root) { //Joining root nood
            root = r->right; //Swap values here
            r->right = temp;
        }
        else {
            r->parent->right = r->right; // Joining nodes to keep track of tree
            r->right = temp;
        }
    }
    //Function that rotates right if needed
    void rotateright(AVLNode<T>* r) {
        AVLNode<T>* temp;
        //cout << "r->" << r->key << endl;
        temp = r->left->right;
        r->left->right = r;
        if (r == root) { //Joining root nood
            root = r->left; //Swap values here
            r->left = temp;
        }
        else {
            r->parent->left = r->left; // Joining nodes to keep track of tree
            r->left = temp;
        }
    }
    //checks if tree is empty or not
    bool isEmpty() {
        return root == nullptr;
    }
    //Function that displays the entire tree in infix order
    void display(AVLNode<T>* r) {
        if (isEmpty()) {
            return;
        }
        if (r == root) {
            cout << "\n===========================================\n";
            cout << "The AVL Tree is as follows\n";
            cout << "===========================================\n";
        }
        if (r->left != nullptr) {
            //cout << "left";
            display(r->left);
        }
        cout << r->key << "->";
        if (r->right != nullptr) {
            //cout << "right";
            display(r->right);
        }
        return;
    }
    //Infix display of keys in AVL along with line numbers
    void DisplaywithLine(AVLNode<T>* r) {
        if (isEmpty()) {
            return;
        }
        if (r == root) {
            cout << "\n===========================================\n";
            cout << "File name : " << filepath << endl;
            cout << "===========================================\n";
            cout << "The AVL Tree is as follows\n";
            cout << "===========================================\n";
        }
        if (r->left != nullptr) {
            //cout << "left";
            DisplaywithLine(r->left);
        }
        cout << "Line number : " << r->lineNumber << ", Key : " << r->key << endl;
        if (r->right != nullptr) {
            //cout << "right";
            DisplaywithLine(r->right);
        }
        return;
    }
    //Recursive function to get height of particular node
    int getHeight(AVLNode<T>* r) {
        if (isEmpty() != false) {
            return -1;
        }
        else if (r == nullptr) {
            return -1;
        }
        int left_height = 0, right_height = 0;
        left_height = getHeight(r->left);
        right_height = getHeight(r->right);
        left_height > right_height ? left_height += 1 : left_height = left_height;
        return right_height + 1;
    }
    //Function to break a string seperated with commas
    string tokenize(string str, int noOfcommas) {
        string word = "";
        int count = 0;
        for (int i = 0; str[i]; i++) {
            if (count == noOfcommas && str[i] != ',') {
                word += str[i];
            }
            if (str[i] == ',') {
                count++;
            }
        }
        return word;
    }
    // Function to get last linenumber of file while filewriting
    int getLastLineNumber(string file) {
        fstream fin;
        int count = 0;
        string line;
        fin.open(file);
        while (fin) {
            getline(fin, line);
            count++;
        }
        fin.close();
        return --count;
    }
    //Writes the value in given filepath
    void WriteToFile(string filepath, string key, string value) {
        //cout<<getLastLineNumber(filepath)<<" : ";
        fstream fout;
        fout.open(filepath, ios::app);
        fout << value << endl;
        fout.close();
        //cout<<getLastLineNumber(filepath)<<endl;
        return;
    }
    //Searches the line number in the file and returns the value stored on it
    string ReadFromFile(string file, int linenumber) {
        fstream fin;
        string str = "";
        fin.open(file);
        for (int i = 1; i <= linenumber; i++) {
            getline(fin, str);
        }
        fin.close();
        //cout<<str<<endl;
        return str;
    }
    //Deletes value from file when key nodes are deleted from tree 
    void deletefromfile(string file, int lineNumb) {
        string str;
        int count = 0;
        fstream f, f1;
        f.open(file, ios::in);
        f1.open("temp.txt", ios::out); // Writing in temporary file to keep track of deleted values
        while (count <= getLastLineNumber(file)) {
            str = "";
            getline(f, str);
            count++;
            if (count == lineNumb) {
                //cout<<"lets see"<<endl;
                f1 << "\n";
            }
            else f1 << str << endl;
        }
        f1.close();
        f.close();
        //Transferring data from temp file to original file
        f.open(file, ios::out);
        f1.open("temp.txt", ios::in);
        count = 0;
        while (count <= getLastLineNumber("temp.txt")) { // Keep running until last line to append
            str = "";
            getline(f1, str);
            count++;
            f << str << endl;
        }
        f1.close();
        f.close();

        return;
    }
};