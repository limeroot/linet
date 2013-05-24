/*
 *  Created on  23/05/2013 17:08:24
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

#ifndef INTERFACE_H
#define INTERFACE_H

#include<string>

class Interface{
    
    public:
        Interface(std::string device);
        ~Interface();
        std::string ipv4();
    private:
        void parse();
        std::string m_device;
        std::string m_ipv4;
        std::string m_ipv6;
        std::string m_mac;
        std::string m_state;
};

#endif //INTERFACE_H
