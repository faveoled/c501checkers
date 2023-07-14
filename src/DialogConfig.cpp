///File : DialogConfig.cpp
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
#include "DialogConfig.hpp"
//------------------------------------------------------------------------------
strGameSettings GetInternationalRules()
{
    strGameSettings settings;

    settings.blacksLevel = 4;
    settings.isBlacksHuman = false;
    #if defined TEST
    settings.whitesLevel = 4;
    settings.isWhitesHuman = false;
    #else
    settings.whitesLevel = 0;
    settings.isWhitesHuman = true;
    #endif
    settings.pieceNb = 20;
    settings.hSize = 10;
    settings.vSize = 10;
    settings.rules.pieceCanEatBack = true;
    settings.rules.slowQueen = false;
    settings.whitesStart = true;

    return settings;
}
//------------------------------------------------------------------------------
strGameSettings GetEnglishRules()
{
    strGameSettings settings;

    settings.blacksLevel = 4;
    settings.isBlacksHuman = false;
    settings.whitesLevel = 0;
    settings.isWhitesHuman = true;
    settings.pieceNb = 12;
    settings.hSize = 8;
    settings.vSize = 8;
    settings.rules.pieceCanEatBack = false;
    settings.rules.slowQueen = true;
    settings.whitesStart = true;

    return settings;
}
//------------------------------------------------------------------------------
DialogConfig::DialogConfig( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* MainSizer = new wxBoxSizer( wxVERTICAL );

	// Big Rules
    wxArrayString choices;
    choices.Add(_("International rules"));
    choices.Add(_("English rules"));

    wxBoxSizer *lineSizer = new wxBoxSizer( wxHORIZONTAL );

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _("Rules:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_rulesChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    lineSizer->Add(m_rulesChoice, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    MainSizer->Add(lineSizer, 0, wxEXPAND);

    MainSizer->AddSpacer(20);


    // Human / bots
    MainSizer->Add(new wxStaticText(this, wxID_ANY, _("Players")), 0, wxALL, 5);

    choices.Clear();
    choices.Add(_("Human"));
    choices.Add(_("c501 Checkers, level 1"));
    choices.Add(_("c501 Checkers, level 2"));
    choices.Add(_("c501 Checkers, level 3"));
    choices.Add(_("c501 Checkers, level 4"));
    choices.Add(_("c501 Checkers, level 5"));

    // Blancs
    lineSizer = new wxBoxSizer( wxHORIZONTAL );

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _("Whites:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_whitesChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    lineSizer->Add(m_whitesChoice, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	MainSizer->Add(lineSizer, 0, wxEXPAND | wxLEFT, 10);

    // Noirs
    lineSizer = new wxBoxSizer( wxHORIZONTAL );

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _("Blacks:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_blacksChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    lineSizer->Add(m_blacksChoice, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	MainSizer->Add(lineSizer, 0, wxEXPAND | wxLEFT, 10);

	// Rules
	MainSizer->AddSpacer(20);
	MainSizer->Add(new wxStaticText(this, wxID_ANY, _("Rules")), 0, wxALL, 5);

        // Board
    lineSizer = new wxBoxSizer( wxHORIZONTAL );

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _("Board size:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_hSpin = new wxSpinCtrl(this, wxID_ANY, _T("10"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 50);
    lineSizer->Add(m_hSpin, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _T("*2 x ")), 0, wxALIGN_CENTER_VERTICAL);

    m_vSpin = new wxSpinCtrl(this, wxID_ANY, _T("10"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 50);
    lineSizer->Add(m_vSpin, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _T("*2")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	MainSizer->Add(lineSizer, 0, wxEXPAND | wxLEFT, 10);

        // Piece nb
    lineSizer = new wxBoxSizer( wxHORIZONTAL );

    lineSizer->Add(new wxStaticText(this, wxID_ANY, _("Piece number:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_pieceNbSpin = new wxSpinCtrl(this, wxID_ANY, _T("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2498);
    lineSizer->Add(m_pieceNbSpin, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	MainSizer->Add(lineSizer, 0, wxEXPAND | wxLEFT, 10);

        // Rules

	m_pieceCanEatBack = new wxCheckBox(this, wxID_ANY, _("Pieces can eat back"));
	m_pieceCanEatBack->SetValue(true);
	MainSizer->Add(m_pieceCanEatBack, 0, wxLEFT, 10);

	MainSizer->AddSpacer(5);

	m_slowQueen = new wxCheckBox(this, wxID_ANY, _("Slow queens"));
	MainSizer->Add(m_slowQueen, 0, wxLEFT, 10);

	MainSizer->AddSpacer(5);

	m_whitesStart = new wxCheckBox(this, wxID_ANY, _("Whites start"));
	MainSizer->Add(m_whitesStart, 0, wxLEFT, 10);

    // Bt Ok
    wxSizer *BtsSizer = CreateSeparatedButtonSizer( wxOK | wxCANCEL);
    MainSizer->Add(BtsSizer, 0, wxALL | wxEXPAND, 5);

	SetSizerAndFit(MainSizer);

	// Events
	m_rulesChoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxEventHandler(DialogConfig::OnChoice), NULL, this);
}
//------------------------------------------------------------------------------
void DialogConfig::OnChoice(wxEvent &event)
{
    if(m_rulesChoice->GetSelection() == 0) // International
    {
        SetConfig(GetInternationalRules());
    }
    else if(m_rulesChoice->GetSelection() == 1) // English
    {
        SetConfig(GetEnglishRules());
    }
}
//------------------------------------------------------------------------------
void DialogConfig::SetConfig(const strGameSettings &settings)
{
    if(settings.isBlacksHuman)
    {
        m_blacksChoice->SetSelection(0);
    }
    else
    {
        m_blacksChoice->SetSelection(settings.blacksLevel);
    }

    if(settings.isWhitesHuman)
    {
        m_whitesChoice->SetSelection(0);
    }
    else
    {
        m_whitesChoice->SetSelection(settings.whitesLevel);
    }

    m_hSpin->SetValue(settings.hSize / 2);
    m_vSpin->SetValue(settings.vSize / 2);
    m_pieceNbSpin->SetValue(settings.pieceNb);

    m_pieceCanEatBack->SetValue(settings.rules.pieceCanEatBack);
    m_slowQueen->SetValue(settings.rules.slowQueen);
    m_whitesStart->SetValue(settings.whitesStart);
}
//------------------------------------------------------------------------------
strGameSettings DialogConfig::GetConfig() const
{
    strGameSettings settings;
    settings.isBlacksHuman = (m_blacksChoice->GetSelection() == 0);
    settings.blacksLevel = m_blacksChoice->GetSelection();

    settings.isWhitesHuman = (m_whitesChoice->GetSelection() == 0);
    settings.whitesLevel = m_whitesChoice->GetSelection();

    settings.hSize = m_hSpin->GetValue() * 2;
    settings.vSize = m_vSpin->GetValue() * 2;
    settings.pieceNb = m_pieceNbSpin->GetValue();

    settings.rules.pieceCanEatBack = m_pieceCanEatBack->GetValue();
    settings.rules.slowQueen = m_slowQueen->GetValue();
    settings.whitesStart = m_whitesStart->GetValue();

    return settings;
}
//------------------------------------------------------------------------------
