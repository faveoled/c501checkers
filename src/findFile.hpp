///File : findFile.hpp
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
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//------------------------------------------------------------------------------
#include <wx/stdpaths.h>
//------------------------------------------------------------------------------
#include "config.hpp"
//------------------------------------------------------------------------------

#if not defined PACKAGE
    #error PACKAGE should be defined.
#endif

#if not defined FIND_FILE_HPP_C501_CHECKERS
#define FIND_FILE_HPP_C501_CHECKERS

wxString findFile(const wxString &str);

#endif //FIND_FILE_HPP_C501_CHECKERS
