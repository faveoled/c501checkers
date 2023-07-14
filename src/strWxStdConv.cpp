///File : strWxStdConv.cpp
//------------------------------------------------------------------------------
/*
    Copyright (C) 2009 Colin GILLE / congelli501

    This file is part of Devis Maker.
    Devis Maker is sold under Devis Maker license (read COPYING file).
*/
//------------------------------------------------------------------------------
#include "strWxStdConv.hpp"
#include <SFML/Graphics.hpp>
//------------------------------------------------------------------------------
wxString wxStrAutoCut(wxString str, const int &maxLen)
{
    if(str.Len() > maxLen)
    {
        str = str.Mid(0, maxLen - 3);
        str << _T("...");
    }

    return str;
}
//------------------------------------------------------------------------------
std::string StrWxToStd(const wxString &str)
{
	return std::string(str.mb_str(wxConvUTF8));
}
//------------------------------------------------------------------------------
sf::Unicode::UTF8String StrWxToSfUtf8(const wxString &str)
{
	return sf::Unicode::UTF8String(static_cast<const unsigned char*>(static_cast<const void*>(StrWxToStd(str).c_str())));
}
//------------------------------------------------------------------------------
wxString StrStdToWx(const std::string &str)
{
	return wxString(str.c_str(), wxConvUTF8);
}
//------------------------------------------------------------------------------
// Filename
std::string filenameWxToStd(const wxString &str)
{
	return std::string(str.mb_str(wxConvFile));
}
//------------------------------------------------------------------------------
wxString filenameStdToWx(const std::string &str)
{
	return wxString(str.c_str(), wxConvFile);
}
//------------------------------------------------------------------------------
