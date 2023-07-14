///File : MainFrame.cpp
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
#include "MainFrame.hpp"

#include <wx/toolbar.h>
//------------------------------------------------------------------------------

void MainFrame::EnableButtons(const bool &enable)
{
    // ToolBar
    wxToolBar *toolBar = GetToolBar();

    toolBar->EnableTool(m_toolPlayForMe->GetId(), enable);

    // Menu
    m_menuItemPlayForMe->Enable(enable);
}
//------------------------------------------------------------------------------
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(NULL, -1, title, pos, size, style)
{
    // Presentation général
    wxString ImgFileName;
    #if defined(__WXMSW__)
        SetIcon(wxIcon(_T("win32icon")));
    #elif defined(__UNIX__)
        ImgFileName = findFile(_T("data/icon.png"));
        if(wxFileExists(ImgFileName))
        {
            SetIcon(wxIcon(ImgFileName, wxBITMAP_TYPE_PNG));
        }
    #endif //#ifdef WIN32
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

        // File menu
    wxMenu *FileMenu = new wxMenu;
    FileMenu->Append(wxID_EXIT);

        // Edit menu
    wxMenu *EditMenu = new wxMenu;
    EditMenu->Append(m_menuItemNew = MenuAddItem(EditMenu, wxString(_("New")) + _T("\tCtrl+N"), _T("new.png")));
    EditMenu->Append(m_menuItemPlayForMe = MenuAddItem(EditMenu, wxString(_("Play for me")) + _T("\tCtrl+P"), _T("playForMe.png")));

        // Help Menu
    wxMenu *HelpMenu = new wxMenu;
    HelpMenu->Append(wxID_ABOUT);

        // Menubar
    m_MenuBar = new wxMenuBar();
    m_MenuBar->Append(FileMenu,_("&File"));
    m_MenuBar->Append(EditMenu,_("&Edit"));
    m_MenuBar->Append(HelpMenu,_("&Help"));

    SetMenuBar(m_MenuBar);

    // Tool bar
    wxToolBar *toolBar = CreateToolBar( wxNO_BORDER | wxTB_HORIZONTAL | wxTB_TEXT | wxTB_HORZ_LAYOUT );
    //GetToolBar()->SetMargins( 2, 2 );

    m_toolNew = ToolBarAddButton(_("New"), _T("new.png"));
    m_toolPlayForMe = ToolBarAddButton(_("Play for me"), _T("playForMe.png"));

    GetToolBar()->Realize();

    // StatusBar
    wxStatusBar *statusBar = CreateStatusBar(2);

    // Body
	wxBoxSizer* MainSizer = new wxBoxSizer( wxVERTICAL );

	m_MainPanel = new MainPanel(this ,wxID_ANY);
    MainSizer->Add(m_MainPanel, 1, wxEXPAND);

	SetSizer( MainSizer );

	// Events
        // Toolbar
    Connect( m_toolNew->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainPanel::OnNew), NULL, m_MainPanel);
    Connect( m_toolPlayForMe->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainPanel::OnPlayForMe), NULL, m_MainPanel);

        // Menu
    Connect( m_menuItemNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainPanel::OnNew), NULL, m_MainPanel);
    Connect( m_menuItemPlayForMe->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainPanel::OnPlayForMe), NULL, m_MainPanel);

    Connect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));
    Connect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAbout));
}
//------------------------------------------------------------------------------
wxToolBarToolBase *MainFrame::ToolBarAddButton(const wxString &label, const wxString &dataFilename)
{
    wxToolBar *toolBar = GetToolBar();
    wxToolBarToolBase* tool;

    wxString ImgFileName = findFile(_T("data/toolbar_") + dataFilename);
    if(wxFileExists(ImgFileName))
    {
        tool = toolBar->AddTool(wxID_ANY, label, wxBitmap(ImgFileName, wxBITMAP_TYPE_PNG), label);
    }
    else
    {
        tool = toolBar->AddTool(wxID_ANY, label);
    }

    return tool;
}
//------------------------------------------------------------------------------
wxMenuItem *MainFrame::MenuAddItem( wxMenu *parentMenu, const wxString &label, const wxString &dataFilename)
{
    wxMenuItem *item = new wxMenuItem(parentMenu, wxID_ANY, label, label);
    wxString ImgFileName = findFile(_T("data/menu_") + dataFilename);
    if(wxFileExists(ImgFileName))
    {
        wxImage img(ImgFileName, wxBITMAP_TYPE_PNG);
        item->SetBitmap(wxBitmap(img));
    }

    return item;
}
//------------------------------------------------------------------------------
void MainFrame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}
//------------------------------------------------------------------------------
void MainFrame::OnAbout(wxCommandEvent& event)
{
    DialogAbout MyDialogAbout(this, wxID_ANY);
    MyDialogAbout.ShowModal();
}
//------------------------------------------------------------------------------
