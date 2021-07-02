#pragma once
#include <iostream>
#include "AVL.h"
using namespace std;
template <class T>
struct Node { //One Node is one Machine
    Node<T>* next;
    Node<T>** FTP;
    T data;
    AVLTree<T> tree;
    //fstream file;
    string filepath;
    int lineno;
    T Nodeid;
    Node() {
        //FTP = next = nullptr;
        next = nullptr;
        data = Nodeid = "";
        //tree = nullptr;
    }
    Node<T>(int identifier_space) {
        next = nullptr;
        data = Nodeid = "";
        identifier_space > 0 ? FTP = new Node<T> *[identifier_space] : FTP = nullptr;
    }
    void createFTParray(int size) { //Function to initialize arrays
        size > 0 ? FTP = new Node<T> *[size] : FTP = nullptr;
    }
    void displayFTP(int identifier_space) { //Display FTP table based on identifier_space
        cout << "==========================================================\n";
        cout << "FTP table for machine " << Nodeid << " is as follows: \n";
        cout << "==========================================================\n";
        for (int counter = 0; counter < identifier_space; counter++) {
            cout << counter + 1 << " | " << FTP[counter]->Nodeid << endl;
        }
        cout << "==========================================================\n";
    }
};

//Class list which creates a circular list
template <class T>
class List {
    Node<T>* head;
    int length;
public:
    List() {
        head = nullptr;
        length = 0;
    }
    void insert(T id) { // Insert new nodes into List
        Node<T>* newn = new Node<T>;
        newn->Nodeid = id; //Assign id
        newn->tree.setFile(id); // Set file path of new AVL Tree
        //cout << id << " : " << length << endl;
        if (head == nullptr) {
            head = newn;
            newn->next = head;
        }
        else {
            Node<T>* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newn;
            newn->next = head;
        }
        length++;
        sort();
    }
    //Function to sort our list in ascending order based on Nodeid
    void sort() {
        Node<T>* ptr = head, * ptr2 = head;
        while (ptr2->next != head) {
            ptr = head;
            while (ptr->next != head) {
                if (stoi(ptr->Nodeid) > stoi(ptr->next->Nodeid)) {
                    swap(ptr->Nodeid, ptr->next->Nodeid);
                    swap(ptr->FTP, ptr->next->FTP);
                    swap(ptr->tree, ptr->next->tree);
                }
                ptr = ptr->next;
            }
            ptr2 = ptr2->next;
        }
    }
    //Deletes Node depending on Nodeid
    void deleteNode(T val) {
        Node<T>* temp = head, * temp2 = temp;
        if (temp->Nodeid == val) {
            temp2 = temp2->next;
            while (temp2->next != head) {
                temp2 = temp2->next;
            }
            head = temp->next;
            temp2->next = head;
            delete temp;
            length--;
            return;
        }
        temp = temp->next;
        while (temp != head && temp->Nodeid != val) {
            temp2 = temp;
            temp = temp->next;
        }
        temp2->next = temp->next;
        length--;
        delete temp;
    }
    void clear() {
        for (int counter = length; counter > 0; counter--) {
            deleteNode(get(counter));
        }
        length = 0;
        head = nullptr;
    }
    bool isempty() {
        return head == nullptr;
    }
    // To display our entire list
    void display() {
        if (head == nullptr) {
            cout << "Empty list\n";
            return;
        }
        Node<T>* temp = head;
        cout << temp->Nodeid << "->";
        while (temp->next != head) {
            temp = temp->next;
            cout << temp->Nodeid << "->";
        }
        cout << endl;
    }
    //Get Nodeid of specific index in list
    T get(int Index) {
        if (head == nullptr) {
            return "List empty\n";
        }
        if (Index == 0) {
            return head->Nodeid;
        }
        Node<T>* temp = head;
        for (int counter = 0; temp->next != head && counter < Index; counter++) {
            temp = temp->next;
        }
        if (temp != nullptr) {
            return temp->Nodeid;
        }
        return "Not found\n";
    }
    //Get specific node by reference depending on index
    Node<T>& getNode(int Index) {
        if (head == nullptr) {
            Node<T>* returnval = new Node<T>;
            returnval->Nodeid = to_string(stoi(get(length - 1)) - 161);
            return *returnval;
        }
        if (Index >= length) {
            Node<T>* returnval = new Node<T>;
            returnval->Nodeid = to_string(stoi(get(length - 1)) - 161);
            return *returnval;
        }
        if (Index == 0) {
            return *head;
        }
        Node<T>* temp = head;
        for (int counter = 0; temp->next != head && counter < Index; counter++) {
            temp = temp->next;
        }
        if (temp != nullptr) {
            return *temp;
        }
    }
    //returns true if machine id is found else false
    bool checkMachine(int val) {
        Node<T>* temp = head;
        while (temp->next != head) {
            if (stoi(temp->Nodeid) == val)return true;
            temp = temp->next;
        }
        if (stoi(temp->Nodeid) == val)return true;
        return false;
    }
    void DisplayAVLwithLine(int id) {
        Node<T>* temp = head;
        while (temp->next != head) {
            if (stoi(temp->Nodeid) == id)temp->tree.DisplaywithLine(temp->tree.root);
            temp = temp->next;
        }
        if (stoi(temp->Nodeid) == id)temp->tree.DisplaywithLine(temp->tree.root);
        return;
    }
    void DisplayAVL(int id) {
        Node<T>* temp = head;
        while (temp->next != head) {
            if (stoi(temp->Nodeid) == id)temp->tree.display(temp->tree.root);
            temp = temp->next;
        }
        return;
    }
    int getlength()const {
        return length;
    }
    //Get specific node by reference depending on nodeid
    Node<T>& getNodebyID(T nodeid) {
        if (head == nullptr) {
            Node<T> returnval;
            return returnval;
        }
        if (nodeid == "0") {
            return *head;
        }
        Node<T>* temp = head;
        for (int counter = 0; temp->next != head; counter++) {
            if (temp->Nodeid == nodeid) {
                return *temp;
            }
            temp = temp->next;
        }
        if (temp->Nodeid == nodeid) {
            return *temp;
        }
    }
    // Function to swap AVL Trees on machine insertion
    void Machineinsertion(int nodeid1, int nodeid2, int identifier_space) { //Use mod with identifier_space to make it round, like if 33%32 it becomes 1 
     //nodeid1 is new machine , in which values are to be inserted
    //nodeid2 is machine from which values are to be taken out unless special condition
        Node<T>* temp = &(getNodebyID(to_string(nodeid2)));
        Node<T>* temp1 = &(getNodebyID(to_string(nodeid1)));
        T value = "Hello world";
        if (temp->tree.root == nullptr) {
            //cout << value << endl;
            return;
        }
        else {
            // cout << (temp1->tree.root->key);
        }
        AVLNode<T> temp3;
        int counter = 0; // Counter that runs for how many nodes are to be moved around
        if (nodeid1 == stoi(get(0))) { // If first index, swap with second index
            counter = (nodeid1 + identifier_space) - stoi(get(length - 1));
            for (; counter > 0; counter--) {
                //cout << value << endl;
                for (; temp->tree.searchtree(temp->tree.root, (stoi(get(length - 1)) + counter) % identifier_space);) { // Run until node is still in tree
                    value = temp->tree.search(temp->tree.root, to_string((stoi(get(length - 1)) + counter) % identifier_space)); // Take out value
                    temp3 = temp->tree.deletenode(to_string((stoi(get(length - 1)) + counter) % identifier_space), temp->tree.root); // Delete node from tree
                    temp1->tree.insertvalue(temp3.key, value, temp3.nodeID); // Insert values in new machine
                }
            }
        }
        else if (nodeid1 == stoi(get(length - 1))) {
            counter = nodeid1 - stoi(get(length - 2));
            for (; counter > 0; counter--) {
                //cout << value << endl;
                for (; temp->tree.searchtree(temp->tree.root, (stoi(get(length - 2)) + counter) % identifier_space);) { //Ye wala search returns bool // Run until node is still in tree
                    value = temp->tree.search(temp->tree.root, to_string((stoi(get(length - 2)) + counter) % identifier_space));  // Delete node from tree
                    temp3 = temp->tree.deletenode(to_string((stoi(get(length - 2)) + counter) % identifier_space), temp->tree.root);
                    temp1->tree.insertvalue(temp3.key, value, temp3.nodeID); // Insert values in new machine
                }
            }
        }
        else {
            counter = nodeid2 - nodeid1;
            for (; counter > 0; counter--) {
                //cout << value << endl;
                for (; temp->tree.searchtree(temp->tree.root, (nodeid1 + counter) % identifier_space);) { //Ye wala search returns bool // Run until node is still in tree
                    value = temp->tree.search(temp->tree.root, to_string((nodeid1 + counter) % identifier_space));
                    temp3 = temp->tree.deletenode(to_string((nodeid1 + counter) % identifier_space), temp->tree.root); // Delete node from tree
                    temp1->tree.insertvalue(temp3.key, value, temp3.nodeID); // Insert values in new machine
                }
            }
        }
    }
    void Machinedeletion(int nodeid0, int nodeid1, int nodeid2, int identifier_space) {
        //nodeid0 is the node before nodeid2
       // nodeid1 is new machine, in which values are to be inserted
        Node<T>* temp = &(getNodebyID(to_string(nodeid2)));
        Node<T>* temp1 = &(getNodebyID(to_string(nodeid1)));
        if (temp->tree.root == nullptr) {
            //cout << value << endl;
            return;
        }
        AVLNode<T> temp3;
        T value = "";
        int counter = 0;
        if (nodeid1 == stoi(get(0))) {
            counter = (nodeid1 + identifier_space) - stoi(get(length - 1));
            for (; counter > 0; counter--) { //Ye wala search returns bool
                //cout << value << endl;
                for (; !temp->tree.searchtree(temp->tree.root, (stoi(get(length - 1)) + counter) % identifier_space);) { //Ye wala search returns bool // Run until node is still in tree
                    value = temp->tree.search(temp->tree.root, to_string(stoi(get(length - 1)) + counter % identifier_space));
                    temp3 = temp->tree.deletenode(to_string((stoi(get(length - 1)) + counter) % identifier_space), temp->tree.root); // Delete node from tree
                    temp1->tree.insertvalue(temp3.key, value, temp3.nodeID); // Insert values in new machine
                }
            }
        }
        else {
            counter = nodeid2 - nodeid0;
            for (; counter > 0; counter--) {
                for (; !temp->tree.searchtree(temp->tree.root, (nodeid0 + counter) % identifier_space);) { //Ye wala search returns bool // Run until node is still in tree
                    value = temp->tree.search(temp->tree.root, to_string((nodeid0 + counter) % identifier_space));
                    temp3 = temp->tree.deletenode(to_string((nodeid0 + counter) % identifier_space), temp->tree.root); // Delete node from tree
                    temp1->tree.insertvalue(temp3.key, value, temp3.nodeID); // Insert values in new machine
                }
            }
        }
    }
};