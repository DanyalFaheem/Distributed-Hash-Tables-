/*
Danyal Faheem
19I-2014
CS-F
Mehmood Amjad
19I-0472
CS-F
Data Structures Project
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>
#include "List.h"
#include "DHT.h"
using namespace std;

/*Look in left side, Code is in List.h*/
int main() {
	DHT Ring; // Main ring holding our machines
	int choice; //For menu choice 
	int check; //For menu choice
	int numOfMachines = 0, ids_size; // For inputs 
	int machine_id; // For inputs
	string key_input; // For inputs
	string value_input; // For inputs
	do { // Printing main menu till program exit
		cout << "\n______________________________________________________________________________________\n";
		cout << "--------------------------------------------------------------------------------------\n\n";
		cout << "1 - Enter data in form of Key Value pair" << endl;
		cout << "2 - Remove data by inputting key" << endl;
		cout << "3 - Print routing Table of any machine" << endl;
		cout << "4 - Display AVL tree of any machine along with file path and line numbers of values" << endl;
		cout << "5 - ADD machine" << endl;
		cout << "6 - Delete machine" << endl;
		cout << "0 - Exit Menu" << endl;
		cout << "\n______________________________________________________________________________________\n";
		cout << "--------------------------------------------------------------------------------------\n\n";
		cin >> check; //User choice
		switch (check) {
		case 1: // Insert key,value pair
			cout << "--------------------------------------------------------------------------------------\n\n";
			do {
				Ring.display();
				cout << "Enter the machine id you would like to start searching from the above choices: ";
				cin >> machine_id;
			} while (!Ring.checkmachineid(to_string(machine_id)));
			cout << "Enter the key : ";
			cin >> key_input;
			cout << "Enter value : ";
			cin.ignore();
			getline(cin, value_input);
			cout << "--------------------------------------------------------------------------------------\n\n";
			Ring.insert(key_input, value_input, machine_id);
			break;
		case 2: // Delete key and its value
			cout << "--------------------------------------------------------------------------------------\n\n";
			do {
				Ring.display();
				cout << "Enter the machine id you would like to start searching from the above choices: ";
				cin >> machine_id;
			} while (!Ring.checkmachineid(to_string(machine_id)));
			cout << "Enter the key : ";
			cin >> key_input;
			cout << "--------------------------------------------------------------------------------------\n\n";
			Ring.deletevalue(key_input, machine_id);
			cout << endl;
			break;
		case 3: // Display FTP table of any single machine
			cout << "--------------------------------------------------------------------------------------\n\n";
			do {
				Ring.display();
				cout << "Enter the machine id you would like to display FTP of from the above choices: ";
				cin >> machine_id;
			} while (!Ring.checkmachineid(to_string(machine_id)));
			Ring.displayFTP(to_string(machine_id));
			break;
		case 4: // Display AVL tree of any one machine
			cout << "--------------------------------------------------------------------------------------\n\n";
			Ring.display();
			//cout<<"--------------------------------------------------------------------------------------\n\n";
			cout << "Enter the machine id from above choices : " << endl;
			cin >> machine_id;
			if (Ring.checkMachine(machine_id)) {
				Ring.DisplayAVLwithLine(machine_id);
			}
			else cout << "Machine is not present\n";
			cout << "--------------------------------------------------------------------------------------\n\n";
			break;
		case 5: // Add a new machine on the fly
			cout << "--------------------------------------------------------------------------------------\n\n";
			do {
				cout << "If you would like to enter the new machine id yourself, press 1. If you would like random machine id, press 2.\n";
				cin >> machine_id;
			} while (machine_id != 1 && machine_id != 2);
			if (machine_id == 1) {
				do {
					Ring.display();
					cout << "Enter the machine id that is not present in the above the above choices and that lies between 0 and " << pow(2, Ring.getidentifierspace()) - 1 << " :";
					cin >> machine_id;
				} while (Ring.checkmachineid(to_string(machine_id)) && machine_id < 0 && machine_id >= pow(2, Ring.getidentifierspace()));
				Ring.insertnewmachine(to_string(machine_id));
			}
			else
				Ring.insertnewmachine("-1");
			cout << "--------------------------------------------------------------------------------------\n\n";
			break;
		case 6: // Delete a machine inside the ring
			cout << "--------------------------------------------------------------------------------------\n\n";
			Ring.display();
			cout << "Enter the machine id you would like to delete from the above choices: ";
			cin >> machine_id;
			if (Ring.checkMachine(machine_id)) {
				Ring.deletemachine(to_string(machine_id));
			}
			else cout << "Machine is not present\n";
			cout << "--------------------------------------------------------------------------------------\n\n";
			break;
		case 0:
			cout << "Exiting menu....\n";
			break;
		default:
			cout << "Invalid Input.\nPlease only choose the options provided.\n" << endl;
			break;
		}
	} while (check != 0); // If user wants to exit menu

	return 0;
}