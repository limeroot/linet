/*
 *  Created on  22/05/2013 16:59:45
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

#ifndef WAN_H
#define WAN_H

#include <thread>
#include <mutex>
#include "../database/database.h"
#include "../interface/interface.h"
 
class Wan{
    
    public:
        Wan();
        virtual ~Wan();
        virtual void start() = 0;
        virtual void stop() = 0;
        
    private:
        virtual void t_start() = 0;
        virtual void t_check_start_success() = 0;
    
    protected:
        std::thread *m_t_start;
        std::thread *m_t_check_start_success;
        Interface *m_interface;
};

#endif //WAN_H
