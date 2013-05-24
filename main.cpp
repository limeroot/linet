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

#include <iostream>
#include "nicmon/nicmon.h"
#include "constants.h"
#include <vector>
#include "wan/wan_pppoe.h"
#include "database/database.h"
#include <signal.h>
#include <map>

using namespace std;

struct LimonEvent{
    
    //LCONST monitor;
    LCONST event;
    string device;
};

mutex event_queue_mutex;
mutex event_queue_enabled_mutex;

vector<LimonEvent> event_queue;
map<string, Wan*> _wans;
bool event_queue_enabled = false;

void eventCallback(LCONST event,string device);
void tnicmon_func();
void tevent_queue_func();
void getInterfaces();
void signal_callback_handler(int signum);

int main(){
    
    signal(SIGINT, signal_callback_handler);
    event_queue_enabled = true;
    getInterfaces();
    
    thread tnicmon(tnicmon_func);
    tnicmon.detach();
    thread tevent_queue(tevent_queue_func);
    tevent_queue.join();
    
    return 0;
}

void signal_callback_handler(int signum){

    event_queue_enabled_mutex.lock();
    event_queue_enabled = false;
    event_queue_enabled_mutex.unlock();
    
    
}

void getInterfaces(){
    
    Database db;
    Database::DatabaseValues vals;
    db.query("SELECT * FROM interface", &vals);
    
    for(auto &interface : vals){
        
        if(interface["connection"] == "pppoe"){
            WanPPPOE *wp = new WanPPPOE(vals);   
            _wans[interface["device"]] = wp;
        }
    }
}


void eventCallback(LCONST event, string device){
    
    event_queue_mutex.lock();
    LimonEvent le;
    le.event = event;
    le.device = device;
    event_queue.push_back(le);
    event_queue_mutex.unlock();
}

void tnicmon_func(){
  
    Nicmon nicmon(eventCallback);
    nicmon.start();
}

void tevent_queue_func(){

    event_queue_enabled_mutex.lock();
    bool run = event_queue_enabled;
    event_queue_enabled_mutex.unlock();

    while(run){ 
        
        while(event_queue.size()){
                
            if(event_queue_mutex.try_lock()){
                
                LimonEvent e = event_queue[0];               
                
                event_queue_mutex.unlock(); 
                        
                switch(e.event){
                    
                    case DEVICE_UP:{
                        
                        map<string, Wan*>::iterator it = _wans.find(e.device);
                        
                        if(it != _wans.end()){
                            it->second->start();
                        }

                        break;
                    }
                    
                    case DEVICE_DOWN:{
                        break;
                    }
                    
                    case INTERFACE_ADDRESSCHANGE:{
                        
                        break;
                    }
                    
                    default:
                        break;
                }
                
                event_queue.erase(event_queue.begin());
                 
                break;
                
            } else {
                break;
            }
        }
        
        this_thread::sleep_for(chrono::seconds(1));
        
        event_queue_enabled_mutex.lock();
        run = event_queue_enabled;
        event_queue_enabled_mutex.unlock();
    }
    
    cout << "exiting event_queue" << endl;
    
    
    for(auto &wan : _wans){
        wan.second->stop();
        delete wan.second;
        wan.second = NULL;
    }
}

