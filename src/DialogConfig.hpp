///File : DialogConfig.hpp
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
#ifndef DIALOG_CONFIG_HPP_C501_CHECKERS
#define DIALOG_CONFIG_HPP_C501_CHECKERS

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/spinctrl.h>
//------------------------------------------------------------------------------
#include <Board.hpp>
//------------------------------------------------------------------------------
#include "config.hpp"
#include "findFile.hpp"
#include "AppConfig.hpp"
//------------------------------------------------------------------------------
struct strGameSettings
{
    bool isWhitesHuman;
    int whitesLevel;

    bool isBlacksHuman;
    int blacksLevel;

    //bool isWhitesOnTop;

    bool whitesStart;

    int vSize, hSize;
    int pieceNb;

    rulesStr rules;
};
//------------------------------------------------------------------------------
strGameSettings GetInternationalRules();
strGameSettings GetEnglishRules();
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
/// Class DialogConfig
///////////////////////////////////////////////////////////////////////////////
class DialogConfig : public wxDialog
{
public:
	DialogConfig(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("New Game..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,100 ), long style = wxDEFAULT_DIALOG_STYLE );
	void SetConfig(const strGameSettings &settings);
	strGameSettings GetConfig() const;

	void OnChoice(wxEvent &event);

protected:
    wxChoice *m_whitesChoice, *m_blacksChoice, *m_rulesChoice;
    wxSpinCtrl *m_hSpin, *m_vSpin, *m_pieceNbSpin;

    wxCheckBox *m_pieceCanEatBack, *m_slowQueen, *m_whitesStart;
    wxButton* m_BtOk;
};

#endif //DIALOG_CONFIG_HPP_C501_CHECKERS
