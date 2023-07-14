///File : strWxStdConv.hpp
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
#include "math.h"
#include "config.hpp"
#include <nbStrConvert.hpp>
#include <SFML/Graphics.hpp>
//------------------------------------------------------------------------------

#if not defined PACKAGE
    #error "This program should be built with its package !"// #define PACKAGE devismaker
#endif

#if not defined STR_WX_STD_CONV_HPP_C501_CHECKERS
#define STR_WX_STD_CONV_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
// String
std::string StrWxToStd(const wxString &str);
wxString StrStdToWx(const std::string &str);
wxString wxStrAutoCut(wxString str, const int &maxLen = 60);

sf::String StrWxToSfUtf8(const wxString &str);

// Filename (Pour Windows)
std::string filenameWxToStd(const wxString &str);
wxString filenameStdToWx(const std::string &str);
//------------------------------------------------------------------------------
template < typename T >
bool wxStrToNb(wxString str, T *nb) // Avec verification
{
    // 1 : Remplacement des "." par des ","
    int lang = wxLocale::GetSystemLanguage();

    if(lang == wxLANGUAGE_FRENCH || lang == wxLANGUAGE_FRENCH_BELGIAN || lang == wxLANGUAGE_FRENCH_CANADIAN || lang == wxLANGUAGE_FRENCH_LUXEMBOURG || lang == wxLANGUAGE_FRENCH_MONACO || lang == wxLANGUAGE_FRENCH_SWISS)
    {
        // Si on est en france ==> On remplace les "." du pavé numérique par des ",".
        str.Replace(_T("."), _T(","));
    }

    // On transforme en double ==> Le plus de précision.
    double tempNb;
    bool isOk = str.ToDouble(&tempNb);

    // On cast en "T"

    *nb = T(tempNb);

	return isOk;
}
//------------------------------------------------------------------------------
// Sans verif...
template < typename T >
T wxStrToNb(const wxString &str)
{
    T tempNb;
    wxStrToNb(str, &tempNb);
    return tempNb;
}
//------------------------------------------------------------------------------
template < typename T >
wxString wxNbToStr(const T &nb)
{
    // On doit passer par les std::string pour eviter les nb scientifiques.
	return StrStdToWx(nbToString(nb));
}
//------------------------------------------------------------------------------

#endif // STR_WX_STD_CONV_HPP_C501_CHECKERS
