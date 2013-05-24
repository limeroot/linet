/*
 *  Created on  22/05/2013 17:18:18
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

#include "wan_pppoe.h"
#include <iostream>
#include "../system/system.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <cstdio>
using namespace std;

WanPPPOE::WanPPPOE(Database::DatabaseValues &vals):
    m_retry_connection(true),
    m_retry_check_start(true),
    m_checking_start(false),
    m_trying_start(false),
    m_pid(string()),
    m_pppx(string()){
    
    if(vals.size()){
        m_device    = vals[0]["device"];
        m_user      = vals[0]["user"];
        m_password  = vals[0]["password"];
        m_name      = vals[0]["name"];
    }
}

WanPPPOE::~WanPPPOE(){
    cout << "waiting threads of " << m_name  << " to finish." << endl;
    
    while(true){
        
        if(!m_trying_start){
            if(m_t_start){
                delete m_t_start;
                m_t_start = NULL;
            }
        }else {
        }
        
        if(!m_checking_start){
            if(m_t_check_start_success){
                delete m_t_check_start_success;
                m_t_check_start_success = NULL;
            }
        } else {
        }
        
        if(! m_t_check_start_success && ! m_trying_start) break;
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    if(m_interface){
        delete m_interface;
        m_interface = NULL;
    }
    cout << "destroyed " << m_name << endl;
    
}

void WanPPPOE::start(){
    
    m_t_start = new thread(&WanPPPOE::t_start, this);
    
    m_t_start->detach();    
}

void WanPPPOE::stop(){
    
    cout << "stopping " << m_name << endl;
    
    m_retry_connection_mutex.lock();
    m_retry_connection = false; 
    m_retry_connection_mutex.unlock();
    
    m_retry_check_start_mutex.lock();
    m_retry_check_start = false;
    m_retry_check_start_mutex.unlock();
    
    m_pid_ppx_mutex.lock();
    if(m_pid != string()){
        System::execute("kill -1 "+m_pid);
        remove( string("/var/run/ppp-" + m_name + ".pid").c_str());
        remove( string("/var/run/" + m_pppx + ".pid").c_str());
    }
    
    m_pid_ppx_mutex.unlock();
}

void WanPPPOE::t_start(){
    
    cout << "starting " << m_name << endl;

    m_retry_connection_mutex.lock();
    m_trying_start = true;
    bool retry = m_retry_connection; 
    m_retry_connection_mutex.unlock();
    
    while(retry){
        
        m_retry_check_start_mutex.lock();
        m_retry_check_start = true;
        m_retry_check_start_mutex.unlock();
        
        m_t_check_start_success = new thread(&WanPPPOE::t_check_start_success, this);
    
        m_t_check_start_success->detach() ;

        string command =    "/usr/sbin/pppd plugin "
                            "rp-pppoe.so " + m_device + " "
                            "noipdefault noauth default-asyncmap "
                            "nodefaultroute hide-password nodetach "
                            "mtu 1492 mru 1492 noaccomp "
                            "nodeflate nopcomp novj novjccomp "
                            "linkname " + m_name + " "
                            "user " + m_user + " password " + m_password + "";
        
        System::execute(command);
        
        m_retry_check_start_mutex.lock();
        m_retry_check_start = false;
        m_retry_check_start_mutex.unlock();
        
        while(true){
        
            if(!m_checking_start){
                if(m_t_check_start_success){
                    delete m_t_check_start_success;
                    m_t_check_start_success = NULL;
                }
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));
        }
        
        m_retry_connection_mutex.lock();
        retry = m_retry_connection; 
        m_retry_connection_mutex.unlock();
        cout << m_name << " disconnected." << endl;
    }
    //cout << "outing" << endl;
    m_retry_connection_mutex.lock();
    m_trying_start = false;
    m_retry_connection_mutex.unlock();
}

void WanPPPOE::t_check_start_success(){
    
    // PID file = ppp-m_name.pid

    m_retry_check_start_mutex.lock();
    m_checking_start = true;
    bool retry = m_retry_check_start;
    m_retry_check_start_mutex.unlock();
    bool connected = false;
    while(retry){
        if(connected) break;
        m_pid_ppx_mutex.lock();
        m_pid = string();
        m_pppx = string();
        m_pid_ppx_mutex.unlock();
        //If there is a pidfile we are connecting
        if(boost::filesystem::exists( "/var/run/ppp-" + m_name + ".pid" )){
            cout <<endl << m_name << " connecting, wait..." << endl;
            
            while(true){
                
                ifstream ifs("/var/run/ppp-" + m_name + ".pid");
                
                if( ifs.is_open() ){
                    
                    string line;
            
                    //If the pid file contains two lines: a number and a pppX then we are connected
                    unsigned count = 0;
                    while(getline(ifs, line)){
                        count++;
                        if(count == 1){
                            m_pid_ppx_mutex.lock();
                            m_pid = line;
                            m_pid_ppx_mutex.unlock();
                        }else if(count == 2) {
                            m_pid_ppx_mutex.lock();
                            m_pppx = line;
                            m_pid_ppx_mutex.unlock();
                        }
                    }
                    
                    ifs.close();
                    
                    if(count >=2){
                        if(m_interface){
                            delete m_interface;
                            m_interface = NULL;
                        }
                        
                        m_interface = new Interface(m_pppx);
                        if(m_interface->ipv4() != string()){
                            cout << m_name << " connected!" << endl;
                            connected = true;
                            
                            break;
                        } else {
                            //cout << m_interface->ipv4() << "<<" << endl;
                        }
                    }
                    //else cout << ".";
                    this_thread::sleep_for(chrono::seconds(1));
                } else {
                    cout << m_name << " could not connect." << endl;
                    break;
                }
            }
            //If there is no pid file the we are disconnected
                
        } else {
            this_thread::sleep_for(chrono::seconds(1));
        }
        
        m_retry_check_start_mutex.lock();
        retry = m_retry_check_start;
        m_retry_check_start_mutex.unlock();
    }
    //cout << "out off t_check_start_success()" << endl;

    m_retry_check_start_mutex.lock();
    m_checking_start = false;
    m_retry_check_start_mutex.unlock();
}
