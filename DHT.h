#pragma once
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;
#include "List.h"

//Our main class holding the List and handling the ring
class DHT {
    List<string> machines; // Circular list of machines
    int no_of_machines; // Current number of machines
    int identifier_space; //Size of identifier_space
    Node<string>* currentmachine; //Pointing to currentmachine
public:
    DHT() {
        int choice = 0;
        do { //Loop to take inputs of identifier_space, number of machines and their ids
            cout << "What would you like the size of the identifier space to be?\n";
            cin >> identifier_space;
        } while (identifier_space < 2); // Input Validation of identifier_space
        do {
            cout << "How many machines would you like to add?\n";
            cin >> no_of_machines;
        } while (no_of_machines < 1); // Input Validation of no_of_machines
        do {
            cout << "Would you like to enter the Machine id yourself?\nPress 1 if you want to enter yourself\nPress 2 if you would like automatically generated ids\n";
            cin >> choice;
        } while (choice != 1 && choice != 2); // Input Validation of machine_ids
        if (choice == 1) {
            for (int counter = 0; counter < no_of_machines; counter++) {
                cout << "Enter id for machine " << counter + 1 << ": ";
                cin >> choice;
                while (choice < 1 && choice > identifier_space - 1) { // Input Validation of identifier_space
                    cout << "Invalid input\nPlease enter values inside the identifier_space\n";
                    cout << "Enter id for machine " << counter + 1 << ": ";
                    cin >> choice;
                }
                machines.insert(to_string(choice));
            }
        }
        else {
            Assignid(); // To assign Random ids to machine
        }
        createFTP(); // To create FTP tables 
        machines.display(); // To display machines created in start
        currentmachine = &(machines.getNode(0));
    }
    void display() {
        machines.display();
    }
    int getidentifierspace() const {
        return identifier_space;
    }
    void displayFTP(string nodeid) {
        (machines.getNodebyID(nodeid)).displayFTP(identifier_space);
    }
    //To check if a certain Nodeid is present inside the ring
    bool checkmachineid(string nodeid) {
        for (int counter = 0; counter < no_of_machines; counter++) {
            if (nodeid == machines.get(counter))
                return true;
        }
        return false;
    }
    //Function to insert new machine
    void insertnewmachine(string Nodeid) {
        if (Nodeid == "-1") { //If user gave choice for random id
            long int power = pow(2, identifier_space);
            srand(time(NULL));
            int random = 0;
            bool check = false;
            while (!check) {
                check = true;
                random = rand() % power;
                for (int count = 0; count < no_of_machines; count++) {
                    if (random == stoi(machines.getNode(count).Nodeid)) { //Making sure new random id isn't present in the ring already
                        check = false;
                        break;
                    }
                }
                if (check == true) { //If not present in ring, insert
                    machines.insert(to_string(random));
                    cout << "\nMachine with id " << random << " has been inserted" << endl;
                    break;
                }
            }
        }
        else {
            machines.insert(Nodeid);
            cout << "\nMachine with id " << Nodeid << " has been inserted" << endl;
        }
        no_of_machines++;
        createFTP(); //Create new FTP tables
        int index = 0;
        for (; index < no_of_machines; index++) {
            if (Nodeid == machines.get(index)) {
                break;
            }
        }
        //Conditions to pass to function for AVL tree swapping
        if (index == no_of_machines - 1) // If last index, give it first index AVL tree
            machines.Machineinsertion(stoi(Nodeid), stoi(machines.get(0)), pow(2, identifier_space));
        else if (index == 0) // If first index, swap with index 2
            machines.Machineinsertion(stoi(Nodeid), stoi(machines.get(index + 1)), pow(2, identifier_space));
        else // Swap with index + 1
            machines.Machineinsertion(stoi(Nodeid), stoi(machines.get(index + 1)), pow(2, identifier_space));

    }

    //Function to delete machines
    void deletemachine(string Nodeid) {
        int index = 0;
        for (; index < no_of_machines; index++) {
            if (Nodeid == machines.get(index)) {
                break;
            }
        }
        //Conditions to pass to function for AVL tree swapping
        if (index == no_of_machines - 1) // If last index, give it first index AVL tree
            machines.Machinedeletion(stoi(machines.get(index - 1)), stoi(machines.get(0)), stoi(Nodeid), pow(2, identifier_space));
        else  // Swap with index + 1
            machines.Machinedeletion(stoi(machines.get(index - 1)), stoi(machines.get(index + 1)), stoi(Nodeid), pow(2, identifier_space));
        machines.deleteNode(Nodeid);
        no_of_machines--;
        createFTP(); //Create new machine FTP tables
        cout << "\nMachine with id " << Nodeid << " has been deleted" << endl;
    }
    //To randomly assign ids to machines inserted at start
    void Assignid() {
        long int power = pow(2, identifier_space);
        srand(time(NULL)); // Seeding time for random everytime
        int* arr = new int[no_of_machines], random = 0;
        bool check = true;
        for (int counter = 0, index = 0; counter < no_of_machines; counter++) {
            check = true;
            if (counter == 0) {
                arr[0] = rand() % power;
                machines.insert(to_string(arr[0]));
                index++;
            }
            else {
                random = rand() % power;
                for (int count = 0; count < index; count++) {
                    if (random == arr[count]) { // If already present randomize again
                        check = false;
                        break;
                    }
                }
                if (check == true) { //If not present, insert in list
                    machines.insert(to_string(random));
                    arr[index] = random;
                    index++;
                }
                else {
                    counter--;
                }
            }
        }
    }
    //Insert key value pairs in machine
    void insert(string key, string value, int current) {
        int e = hashing(key); // Send to hashing function to get e
        cout << "Hashed value : " << e << endl;
        currentmachine = &(machines.getNodebyID(to_string(current))); // Move current machine to user choice
        //cout << currentmachine->Nodeid;
        cout << "Traveled distance: \n" << currentmachine->Nodeid;
        while (!searchmachine(e)) {} //Traverse until machine not reached
        currentmachine->tree.insertvalue(key, value, to_string(e)); //Insert value when machine reached
        //cout << currentmachine->tree.display();
        currentmachine->tree.display(currentmachine->tree.root); // Display tree of machine in which value was inserted
    }
    //Delete value of corresponding key and node
    void deletevalue(string key, int current) {
        int e = hashing(key); // Send to hashing function to get e
        currentmachine = &(machines.getNodebyID(to_string(current)));  // Move current machine to user choice
        cout << "Hashed value : " << e << endl;
        cout << "Traveled distance: \n" << currentmachine->Nodeid;
        while (!searchmachine(e)) {} //Traverse until machine not reached
        currentmachine->tree.deletenode(key, currentmachine->tree.root); //Delete value when machine reached
        //cout << currentmachine->tree.display();
        // Display tree of machine in which value was deleted
        if (!currentmachine->tree.isEmpty())currentmachine->tree.display(currentmachine->tree.root);
        else cout << "The tree is Empty" << endl;
    }

    string search(string key) {
        int e = hashing(key);
        cout << "Hashed value : " << e << endl;
        cout << "Traveled distance: \n" << currentmachine->Nodeid;
        while (!searchmachine(e)) {}
        cout << "\nValue is: " << currentmachine->tree.search(currentmachine->tree.root, key) << endl;
    }
    //Function to traverse through FTP and reach machine 
    bool searchmachine(int e) {
        bool check = false;
        //cout << e << " : " << (currentmachine->FTP[0])->Nodeid << " : " << (currentmachine->FTP[1])->Nodeid << " : " << (currentmachine->FTP[2])->Nodeid << endl;
        if (stoi(currentmachine->Nodeid) == e) { //First condition if current machine is same as e
            cout << "->" << currentmachine->Nodeid;
            return true;
        }
        else if ((stoi(currentmachine->Nodeid) < e && e <= stoi((currentmachine->FTP[0])->Nodeid)) || (stoi(currentmachine->Nodeid) < e && e < pow(2, identifier_space) && (currentmachine->Nodeid == machines.get(no_of_machines - 1)))) { //Second condition if machine is on first index of FTP
            currentmachine = currentmachine->FTP[0];
            cout << "->" << currentmachine->Nodeid;
            return true;
        }
        else {
            for (int counter = 1; counter < identifier_space - 1; counter++) { //keep traversing till e < FTp[j+1] not satisfied
                if (stoi((currentmachine->FTP[counter])->Nodeid) < e && e <= stoi((currentmachine->FTP[counter + 1])->Nodeid)) {
                    //cout << "else->" << currentmachine->Nodeid;
                    currentmachine = currentmachine->FTP[counter];
                    cout << "->" << currentmachine->Nodeid;
                    check = true;
                    break;
                }
            }
        }
        check == false ? currentmachine = currentmachine->FTP[identifier_space - 1] : currentmachine = currentmachine; // Pass last index to currentmachine if all conditions not satisfied
         //cout << "->" << currentmachine->Nodeid;
        return false;
    }
    //Function to create FTP tables
    void createFTP() {
        bool check = true;
        string s = "";
        Node<string>* temp; // temporary pointer to handle nodes
        long int power = pow(2, identifier_space);
        int st = 0;
        for (int counter = 0; counter < machines.getlength(); counter++) {
            temp = &(machines.getNode(counter)); // 
            //cout << temp->Nodeid << endl;
            temp->createFTParray(identifier_space);
            for (int count = 0; count < identifier_space; count++) {
                check = false;
                st = stoi(temp->Nodeid) + pow(2, count); // Applying formula for FTp
                st %= power; // Taking mod to round off value
                s = to_string(st);
                if (st <= stoi((machines.getNode(counter + 1)).Nodeid)) { //If value falls in index + 1 machine, then give index of index + 1
                    temp->FTP[count] = &(machines.getNode(counter + 1));
                    //cout << count << " : " << st << " : " << temp->Nodeid << " : " << temp->FTP[count]->Nodeid << endl;
                    check = true;
                }
                else if (st > stoi((machines.getNode(no_of_machines - 1)).Nodeid) || st <= stoi((machines.getNode(0)).Nodeid)) { // If greater than last machine, or less than first, pass index of first machine
                    temp->FTP[count] = &(machines.getNode(0));
                    //cout << "else if " << count << " : "<< st << " : " << temp->Nodeid << " : " << temp->FTP[count]->Nodeid << endl;
                    check = true;
                }
                else {
                    for (int count1 = counter + 2; count1 < machines.getlength(); count1++) {
                        if (st <= stoi((machines.getNode(count1)).Nodeid)) { // Run loop until FTp[j + 1]
                            temp->FTP[count] = &(machines.getNode(count1));
                            //cout << "else " << count << " : " << st << " : " << temp->Nodeid << " : " << temp->FTP[count]->Nodeid << endl;
                            check = true;
                            break;
                        }
                    }
                }
                if (check == false) { //If no values found, run loop from start
                    for (int count1 = 0; count1 < machines.getlength(); count1++) {
                        if (st <= stoi((machines.getNode(count1)).Nodeid)) {
                            temp->FTP[count] = &(machines.getNode(count1));
                            //cout << "Check " << count << " : " << st << " : " << temp->Nodeid << " : " << temp->FTP[count]->Nodeid << endl;
                            check = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    // Our main hashing function which returns value on hashing of string
    int hashing(string key) {
        long int size = pow(2, identifier_space); //Using long int for upto 63 bits
        int hashed = 0; //Stores our hashed value
        for (int counter = 0; counter < key.length(); counter++) {
            hashed += key[counter]; // Add ascii value of each index of string key
        }
        while (hashed < size) // If hashed value is less than identifier_space, multiply it by 2 until larger than identifier_space
            hashed *= 2;
        return hashed % size; // Mod by size so that value falls inside identifier_space
    }
    bool checkMachine(int val) {
        return machines.checkMachine(val);
    }
    void DisplayAVLwithLine(int id) {
        machines.DisplayAVLwithLine(id);
        return;
    }
    void DisplayAVL(int id) {
        machines.DisplayAVL(id);
        return;
    }
};