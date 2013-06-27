/*******************************************************************************
 
 Espial IDS is a simple signature based IDS system with flexible rule
 implementation.
 
 Copyright (C) 2012 Joshua Reynolds
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 *******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include "ui.h"
#include "db.h"

using namespace std;

Ui::Ui()
{
    database = new DB;
}

int Ui::menu()
{
	cout << endl << "-----------------------" << endl
	<< "1) Start Espial IDS" << endl
	<< "2) Import Rule(s)" << endl
	<< "3) List Current Rule(s)" << endl
	<< "4) About Espial IDS" << endl
	<< "5) Quit" << endl
	<< "-----------------------" << endl
	<< "Choice: " << endl;
    
    return this->readInt();
}

int Ui::readInt()
{
	string choice;
    
    cin >> choice;
    
    int intChoice = atoi(choice.c_str());
    
	return intChoice;
}

void Ui::action()
{
	int choice = 0;
    
	while(choice != 5) {
        
        choice = this->menu();
        
        switch(choice)
        {
            case 1 :
                this->startIDS();
                break;
            case 2 :
                this->importRules();
                break;
            case 3 :
                this->listRules();
                break;
            case 4 :
                this->about();
                break;
            case 5 :
                this->exit();
                break;
            default:
                cout << "Invalid choice.";
        }
	}
}

void Ui::startIDS()
{
    if(database->constructDatabase()) {
        string tmp, newString;
        FILE *in;
        char buff[512]; //Establish a decent buffer size to read in data.
    
        if(!(in = popen("tcpdump -A -i en1", "r"))) //change this to your active network interface, leave the -A option.
            cout << "Sorry, tcpdump does not seem to be on the current system.";
        else{
            string data = "";
    
            while(!feof(in)) {
                while(fgets(buff, sizeof(buff), in) != NULL) { //While there is input from tcpdump.
                    data = buff;
                    this->database->search(data);
                }
            }
        }
    
        pclose(in);
    }
    else
        cout << "Sorry, doesn't look like you have any rules loaded yet...";
}

void Ui::importRules()
{
	string filename = this->getFilename();
	ifstream inFile;
	ofstream outFile;
    
	inFile.open(filename.c_str()); //The filename has to be a C char array.
	if(inFile.fail()) {
		cout << "Sorry, please enter a valid infile..." << endl;
    }
    
    outFile.open("rule.db");
    
    if(outFile.fail()) {
        
        cout << "Sorry, the rule.db outfile is not in the current dir. Making one." << endl;
        
    }
    
    string line;
    string word;
    
	while(getline(inFile, line))
    {
        if(this->database->addRule(line))
            cout << "Rule added!" << endl;
    }
    
    outFile.close();
    inFile.close();
}

string Ui::getFilename()
{
    string infile;
    
    cout << "Filename: ";
    cin >> infile;
    
    return infile;
}

void Ui::listRules()
{
    database->listRules();
}

void Ui::exit()
{
    cout << "Bye, thanks for using the Espial IDS :)\n";
}

void Ui::about()
{
    cout << "By Joshua Reynolds\n";
    cout << "Espial IDS is a regex signature intrusion detection system\nwhich provides alerts in real-time.\n";
    cout << "jersh.magersh@gmail.com\n";
    cout << "infosec4breakfast.com\n";
    cout << "@jershmagersh - twitter\n";
}