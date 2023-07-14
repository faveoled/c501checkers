///File : nbStrConvert.hpp
//------------------------------------------------------------------------------
/*
    This file is part of C501Checkers.

    C501Checkers is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    C501Checkers is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with C501Checkers.  If not, see <http://www.gnu.org/licenses/>.
*/
//------------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <sstream>
//------------------------------------------------------------------------------
#if not defined NB_STR_CONVERT_HPP
#define NB_STR_CONVERT_HPP
//------------------------------------------------------------------------------
template < typename T >
std::string nbToString(const T &nb)
{
    std::stringstream ss;
    std::string str;
    if(sizeof(T) != 1) // Pas char
    {
        ss << std::fixed << nb; // fixed : pas de nb scientifique. Par contre, plein de zero...
    }
    else
    {
        // Type char ==> on le tranforme en int
        ss << std::fixed << int(nb); // fixed : pas de nb scientifique. Par contre, plein de zero...
    }
    ss >> str;

    // On enléve les 0 et . innutils
    std::string::iterator it = str.end() - 1;
    if(str.find('.') != std::string::npos) // On ne racourci pas le nb si pas de virgule
    {
        while(str.size() > 1 && *it == '0')
        {
            str.erase(it);
            it = str.end() - 1;
        }

        if(*it == '.') // Si on arrive à la virgule ==> suppression.
        {
            str.erase(it);
        }
    }

    return str;
}

//------------------------------------------------------------------------------
template < typename T >
T stringToNb(std::string nbStr)
{
    std::stringstream ss;
    T nb;
    ss << nbStr;
    if(sizeof(T) != 1) // Pas char
    {
        ss >> nb;
    }
    else
    {
        // Type char ==> on le tranforme en int
        int tmp;
        ss >> tmp;
        nb = char(tmp);
    }

    return nb;
}
//------------------------------------------------------------------------------
#endif // NB_STR_CONVERT_HPP
