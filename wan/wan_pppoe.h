/*
 *  Created on  22/05/2013 17:17:40
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

#ifndef WANPPPOE_H
#define WANPPPOE_H

#include "wan.h"
#include <string>

class WanPPPOE : public Wan{
    
    public:
        WanPPPOE(Database::DatabaseValues &vals);
        ~WanPPPOE();
        void start();
        void stop();
    private:
        std::string m_user;
        std::string m_password;
        std::string m_name;
        std::string m_device;
        
        void t_start();
        void t_check_start_success();
        bool m_retry_connection;
        bool m_retry_check_start;
        bool m_checking_start;
        bool m_trying_start;
        std::mutex m_retry_connection_mutex;
        std::mutex m_retry_check_start_mutex;
        std::mutex m_pid_ppx_mutex;
        std::string m_pid;
        std::string m_pppx;
};

#endif //WANPPPOE_H
