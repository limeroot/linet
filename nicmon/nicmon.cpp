/*
 *  Created on  15/05/2013 10:02:30
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

#include "nicmon.h"
#include <thread>
#include <iostream>
//#include "netlink_send.h"
//#include "netlink_listener.h"
//#include "netlink_event.h"
//#include "../system/system.h"

#include <sstream>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/filesystem.hpp>

//using boost::property_tree::ptree; using boost::property_tree::read_json; using boost::property_tree::write_json;
using namespace boost::filesystem;

using namespace std;

Nicmon::Nicmon(function<void(LCONST,string )> callback){
    m_callback = &callback;
    getDevices();
}

NicState::NicState():state("down"),link("unknown"){}

Nicmon::~Nicmon(){ 
    
}


void Nicmon::start(){
    
    while(true){
        
        for(auto &device : m_nics_states){
            
            //cout << "checking " << device.first << endl;
            
            string filename("/sys/class/net/" + device.first + "/operstate");
            
            boost::iostreams::stream<boost::iostreams::file_source> file(filename.c_str());
            
            string state;
            
            /*while(*/getline(file, state);/*){
            }*/
            
            filename ="/sys/class/net/" + device.first + "/carrier";
            
            boost::iostreams::stream<boost::iostreams::file_source> file2(filename.c_str());
            
            string link;
            
            getline(file2, link);
            
            if(link == "0"){
                link = "unplugged";
            } else if(link == "1") {
                link = "plugged";
            } else {
                link = "unknown";
            }
            
            if( (device.second.state != state) || (device.second.link != link) ){
                
                device.second.state = state;
                
                device.second.link = link;
                
                
                //ptree pt;
                //
                //ostringstream buf;
                //
                //pt.put ("\"interface\"", '"' + device.first + '"');
                //
                //pt.put ("\"event\"", "\"nic_status\"");
                //
                //pt.put ("\"state\"", '"' + state + '"');
                //
                //pt.put ("\"link\"", '"' + link + '"');
                //
                //write_json (buf, pt, false);
                //
                LCONST status = (state=="up" && link=="plugged") ? DEVICE_UP : DEVICE_DOWN;
                
                (*m_callback)(status, device.first);
            }
        }
        
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Nicmon::getDevices(){
    
    path someDir("/sys/class/net");
    
    directory_iterator end_iter;
    
    if(exists(someDir) && is_directory(someDir)){
        
        for( directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter){
            
                const directory_entry& entry = *dir_iter; 
            
                string ldevice = entry.path().filename().string();;
            
                m_nics_states[ldevice];
        }
    }
}

