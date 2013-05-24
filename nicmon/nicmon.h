/*
 *  Created on  15/05/2013 10:01:15
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

#ifndef NICMON_H
#define NICMON_H

#include <functional>
#include <map>
#include <string>
#include "../constants.h"
#include <vector>

struct NicState{
    NicState();
    std::string state;
    std::string link;
};

class Nicmon{
    
    public:
        Nicmon(std::function<void(LCONST,std::string )> callback);
        ~Nicmon();
        void start();
        
    private:
        void getDevices();
        //std::vector<std::string> m_devices;        
        std::function<void(LCONST,std::string )> *m_callback;
        std::map<std::string, NicState> m_nics_states;
};

#endif //NICMON_H
