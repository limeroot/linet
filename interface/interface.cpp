/*
 *  Created on  23/05/2013 17:09:05
 *
 *  This file is part of __________
 *
 *  Copyright (c) LimeRoot, http://www.limeroot.org, devel@limeroot.org
 *
 *  Author: Daniel R. Torres Ortiz, daniel@limeroot.org
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "interface.h"
#include "../system/system.h"
#include <boost/algorithm/string.hpp>
//#include <iostream>
using namespace std;

Interface::Interface(string device):
    m_device(device),
    m_ipv4(string()),
    m_ipv6(string()),
    m_mac(string()),
    m_state(string()){  
        
        parse();
}

Interface::~Interface(){
    
}

string Interface::ipv4(){
    return m_ipv4;
}

void Interface::parse(){
    
    vector<string> lines;
    
    // GET IPV4 && IPV6
    System::execute("ip -o addr show dev " + m_device, &lines);

    for(auto line : lines){
        //cout << line << endl;
        vector<string> words;
        
        boost::split(words, line, boost::is_any_of(" "));
            
        int count = 0;
        
        string next_info = "";
        
        //Printable* nic = NULL;
        
        for(auto word : words){
            
            if(next_info == "ipv4"){
                next_info = "";
                m_ipv4 = word;
                continue;
            }
            else if(next_info == "ipv6"){
                next_info = "";
                m_ipv6 = word; 
                continue;
            }
            
            // IF IS THE INTERFACE NAME 
            if(count == 1){
                if(word.size()){
                    if(word[word.size()-1] == ':')
                        word = word.substr(0, word.size()-1);
                }
            }
            else if(word == "inet"){
                
                next_info = "ipv4";
                continue;   
            }
            else if(word == "inet6"){
                
                next_info = "ipv6";
                continue;   
            }
            else{
                next_info = "";      
            }
            count++;
        }
    }
        
            // GET MAC && STATE 
    System::execute("ip -o link show dev " + m_device, &lines);
    
    for(auto line : lines){
        
        vector<string> words;
        
        boost::split(words, line, boost::is_any_of(" "));
            
        int count = 0;
        
        string next_info = "";
        
        //Printable* nic = NULL;
        
        for(auto word : words){
            
            if(next_info == "mac"){
                next_info = "";
                m_mac = word;
                continue;
            }
            else if(next_info == "state"){
                next_info = "";
                m_state = word;
                continue;
            }
            
            // IF IS THE INTERFACE NAME 
            if(count == 1){
                if(word.size()){
                    if(word[word.size()-1] == ':')
                        word = word.substr(0, word.size()-1);
                }
                
                //nic = &m_nic_list[word];                
            }
            else if(word == "link/ether"){
                
                next_info = "mac";   
                continue;   
            }
            else if(word == "state"){
                
                next_info = "state";
                continue;   
            } else {
                
                next_info = "";      
            }
                            
            count++;
        }
    }
}