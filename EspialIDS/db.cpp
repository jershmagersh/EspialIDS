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
#include <string>
#include <regex>
#include <map>
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

DB::DB()
{
    rules = new map<string, RuleInfo>;
    globalMatchBlock = new regex;
    rulesVec = new vector<RuleInfo>(10);
    ruleCount = 0;
    outfile.open("test.txt");
}

DB::~DB()
{
    delete rules;
    outfile.close();
}

bool DB::addRule(string rule)
{
    if(matchRule(rule)){
        regex rgx("^!!START_MATCH!!(.*)!!END_MATCH!!::([0-9])::([a-zA-Z!.? ]+)");
        smatch result;
        regex_search(rule, result, rgx);
        
        RuleInfo newRule;
        
        newRule.match = result[1];
        newRule.packetMultiplier = result[2];
        newRule.alert = result[3];
        
        rules->insert(std::pair<string,RuleInfo>(newRule.match,newRule));
        
        //Keeping rules in a vector as well, in order to access by index when matches occur, for O(1) lookups.
        if(ruleCount == rulesVec->size()) {
            rulesVec->resize(ruleCount + 10); //Add 10 more rules to fill in the vector.
            (*rulesVec)[ruleCount] = newRule;
        }
        else
            (*rulesVec)[ruleCount] = newRule;
        
        ruleCount++;
        return true;
    }
    else
        return false;
}

void DB::search(string &currLine)
{
    smatch result;
    ofstream outfile;
    regex_search(currLine, result, *globalMatchBlock);
    
    for(int i = 0; i < result.size(); i++){
        if(result[i].length() > 2 && i != 0) {
            outfile << "Match: " << (*rulesVec)[i-1].alert << endl; //Access alert in vector at that index, since that's the one that matched...
            outfile << "From: " << result[0] << endl << endl;
            
            outfile.flush();
        }
    }
}

void DB::alert(string alert)
{
    
}

void DB::listRules()
{
    for(rulesIter = this->rules->begin(); rulesIter != this->rules->end(); rulesIter++)
        cout << "Match: " << rulesIter->first << "Alert: " << rulesIter->second.alert << endl;
}

bool DB::matchRule(string rule)
{
    regex rgx("^!!START_MATCH!!.*!!END_MATCH!!::[0-9]::[a-zA-Z!.? ]+$"); //Check to see if this is a valid rule.
    
    bool test = regex_search(rule, rgx);
    
    if(test)
        return true;
    else
        return false;
}

bool DB::constructDatabase()
{
    if(this->rules->size() > 0) {
        string searchBlock = "";
        for(rulesIter = this->rules->begin(); rulesIter != this->rules->end(); rulesIter++) {
            if(rulesIter == this->rules->begin()) //Construct a big fat regex from all of the rules.
                searchBlock += ".*(" + rulesIter->first + ").*";
            else
                searchBlock += "|.*(" + rulesIter->first + ").*";
        }
        globalMatchBlock = new regex(searchBlock);
        
       return true;
    }
    else
        return false;
}