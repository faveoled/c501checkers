///File : MainPanel.hpp
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
#include <wx/vscroll.h>
#include <wx/event.h>
#include <wx/stdpaths.h>
#include <wx/statline.h>
#include <wx/listctrl.h>

#include <vector>

#include <Board.hpp>
//------------------------------------------------------------------------------
#include "config.hpp"
#include "findFile.hpp"
#include "AppConfig.hpp"
#include "SfmlBoard.hpp"

#include "DialogConfig.hpp"

//------------------------------------------------------------------------------
#if not defined MAIN_PANEL_HPP_C501_CHECKERS
#define MAIN_PANEL_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
class MainPanel : public wxPanel
{
public:
    MainPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL );

    void OnResize(wxSizeEvent &event);

    void OnNew(wxCommandEvent& event);
    void OnPlayForMe(wxCommandEvent& event);

    void ComputeDrawRect();

private:
    Board m_board;
    strGameSettings m_settings;

    wxControl *m_gameContainer;

    SfmlBoard *m_sfmlBoard;
};

#endif //MAIN_PANEL_HPP_C501_CHECKERS
