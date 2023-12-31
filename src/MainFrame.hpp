///File : MainFrame.hpp
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
#if not defined MAIN_FRAME_HPP_C501_CHECKERS
#define MAIN_FRAME_HPP_C501_CHECKERS

//------------------------------------------------------------------------------
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//------------------------------------------------------------------------------
#include <wx/print.h>
#include <wx/html/helpctrl.h>
#include <wx/stdpaths.h>
//------------------------------------------------------------------------------
#include "findFile.hpp"
#include "MainPanel.hpp"
#include "DialogAbout.hpp"
#include "AppConfig.hpp"
//------------------------------------------------------------------------------

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxToolBarToolBase *ToolBarAddButton(const wxString &label, const wxString &dataFilename);
    wxMenuItem *MenuAddItem( wxMenu *parentMenu, const wxString &label, const wxString &dataFilename);

    void EnableButtons(const bool &enable);

protected:
    MainPanel *m_MainPanel;
    wxMenuBar *m_MenuBar;

    wxToolBarToolBase *m_toolNew, *m_toolPlayForMe;
    wxMenuItem *m_menuItemNew, *m_menuItemPlayForMe;
};

#endif //MAIN_FRAME_HPP_C501_CHECKERS
