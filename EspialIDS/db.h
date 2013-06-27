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

#ifndef SecurityProjectIDS_db_h
#define SecurityProjectIDS_db_h

#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <vector>
#include <fstream>
using namespace std;

class DB
{
    struct RuleInfo
    {
        string packetMultiplier;
        string alert;
        string match;
    };
    
    map<string, RuleInfo> *rules;
    map<string, RuleInfo>::const_iterator rulesIter;
    vector<RuleInfo> *rulesVec;
    regex *globalMatchBlock;
    int ruleCount;
    ofstream outfile;
    
public:
    DB();
    ~DB();
    bool addRule(string rule);
    void search(string &currLine);
    void alert(string alert);
    void listRules();
    bool matchRule(string rule);
    bool constructDatabase();
};


#endif
