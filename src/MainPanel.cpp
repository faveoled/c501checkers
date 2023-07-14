///File : MainPanel.cpp
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
#include "MainPanel.hpp"

#include "MainFrame.hpp"
#include "strWxStdConv.hpp"
#include <MyException.hpp>

#include <wx/listctrl.h>
#include <wx/wupdlock.h>

using namespace std;
//------------------------------------------------------------------------------
MainPanel::MainPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    // Controls
	wxBoxSizer *MainSizer = new wxBoxSizer( wxHORIZONTAL );

    // 1 : Le jeu

    m_gameContainer = new wxControl(this, wxID_ANY);
    m_sfmlBoard = new SfmlBoard(m_gameContainer, wxID_ANY, m_board, m_settings);

    wxBoxSizer *gameSizer = new wxBoxSizer( wxVERTICAL );
    gameSizer->Add(m_gameContainer, 1, wxEXPAND);

    MainSizer->Add(gameSizer, 1, wxEXPAND, 0);

    // 2 : Bare
    /*MainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND | wxBOTTOM | wxLEFT | wxTOP, 5);

    // 3 : Controles
    wxBoxSizer *CtrlSizer = new wxBoxSizer( wxVERTICAL );
    CtrlSizer->Add(new wxButton(this, wxID_ANY, _T("test avec un boutton !!!")), 0, wxALL, 3);
    CtrlSizer->Add(new wxButton(this, wxID_ANY, _T("test avec un boutton !!!")), 0, wxALL, 3);
    CtrlSizer->Add(new wxButton(this, wxID_ANY, _T("test avec un boutton !!!")), 0, wxALL, 3);
    CtrlSizer->Add(new wxButton(this, wxID_ANY, _T("test avec un boutton !!!")), 0, wxALL, 3);


    MainSizer->Add(CtrlSizer, 0, wxEXPAND | wxALL, 5);*/

    // Finition
	SetSizer(MainSizer);
	ComputeDrawRect();

	// Events
	m_gameContainer->Connect(wxEVT_SIZE, wxSizeEventHandler(MainPanel::OnResize), NULL, this);
}
//------------------------------------------------------------------------------
void MainPanel::ComputeDrawRect()
{
    //SetSizer(m_gameSizer);

    float boardRatio = float(m_board.GetBoardHSize()) / float(m_board.GetBoardVSize());

    wxRect drawRect = wxRect(m_gameContainer->GetPosition(), m_gameContainer->GetSize());
    if(drawRect.GetHeight() * boardRatio > drawRect.GetWidth()) // Hauteur trop grande, on réduit.
    {
        drawRect.y += (drawRect.GetHeight() - drawRect.GetWidth() / boardRatio)  / 2;
        drawRect.height = drawRect.width / boardRatio;
    }
    else
    {
        drawRect.x += (drawRect.GetWidth() - drawRect.GetHeight() * boardRatio) / 2;
        drawRect.width = drawRect.height * boardRatio;
    }

    m_sfmlBoard->wxWindow::SetPosition(drawRect.GetPosition());
    m_sfmlBoard->wxWindow::SetSize(drawRect.GetSize());
}
//------------------------------------------------------------------------------
void MainPanel::OnResize(wxSizeEvent &event)
{
    ComputeDrawRect();

    event.Skip(); // Imporant
}
//------------------------------------------------------------------------------
void MainPanel::OnPlayForMe(wxCommandEvent& event)
{
    if(m_sfmlBoard->GetWinner() == WINNER_NO && !m_sfmlBoard->IsMoving() && ((m_board.GetTurn() == WHITES && m_settings.isWhitesHuman) || (m_board.GetTurn() == BLACKS && m_settings.isBlacksHuman)))
    {
        try
        {
            std::pair<Position, Mvt> posMvt = m_board.GetIaMvt(4);
            m_sfmlBoard->MovePieceAnim(posMvt.first, posMvt.second);
        }
        catch(MyException &e)
        {
            m_sfmlBoard->SetWinner(!m_board.GetTurn());
        }
    }
}
//------------------------------------------------------------------------------
void MainPanel::OnNew(wxCommandEvent& event)
{
    try
    {
        DialogConfig MyDialogConfig(this, wxID_ANY);
        MyDialogConfig.SetConfig(m_settings);
        if(MyDialogConfig.ShowModal() == wxID_OK)
        {
            m_sfmlBoard->AbortAll();
            m_settings = MyDialogConfig.GetConfig();
            m_board.SetRules(m_settings.rules);
            m_board.InitGame(m_settings.whitesStart, m_settings.hSize, m_settings.vSize, m_settings.pieceNb);

            ComputeDrawRect();

            m_sfmlBoard->Refresh();
            m_sfmlBoard->Update();
        }
    }
    catch(const std::exception& e) //Rattrape toutes les exceptions
    {
        m_board.InitGame();
        wxMessageBox(_("Error : ") + wxString(_T("\n")) + StrStdToWx(e.what()), _("Error..."),  wxOK | wxICON_ERROR, this);
    }
}

//------------------------------------------------------------------------------
