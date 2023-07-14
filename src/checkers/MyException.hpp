///File : MyException.hpp
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
#if not defined MY_EXCEPTION_HPP_C501_CHECKERS
#define MY_EXCEPTION_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
#include <exception>
#include <string>
//------------------------------------------------------------------------------
class MyException : public std::exception
{
public:
    MyException(const std::string &info) throw();
    virtual ~MyException() throw();
    virtual const char* what() const throw();

private:
    std::string m_info;
};
//------------------------------------------------------------------------------
#endif // MY_EXCEPTION_HPP_C501_CHECKERS
