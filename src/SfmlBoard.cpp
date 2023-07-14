///File : SfmlBoard.cpp
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
#include <MyException.hpp>

#include "findFile.hpp"
#include "strWxStdConv.hpp"
#include "SfmlBoard.hpp"

#include <SFML/Graphics.hpp>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
SfmlBoard::SfmlBoard(wxWindow* Parent, wxWindowID Id, Board &board, strGameSettings &settings, const wxPoint& Position, const wxSize& Size, long Style) : wxSFMLCanvas(Parent, Id, Position, Size, Style), m_board(board), m_settings(settings), m_mvtStatus(1), m_mvtFrameCount(0), m_dragAndDropOrigin(-1, -1), m_hightlightPos(-1, -1), m_winner(WINNER_NO)
{
    // Preparation des images...
    m_bmp_W.LoadFromFile(StrWxToStd(findFile(_T("data/wq.png"))));
    m_bmp_w.LoadFromFile(StrWxToStd(findFile(_T("data/w.png"))));
    m_bmp_B.LoadFromFile(StrWxToStd(findFile(_T("data/bq.png"))));
    m_bmp_b.LoadFromFile(StrWxToStd(findFile(_T("data/b.png"))));
    m_bmp_blackCase.LoadFromFile(StrWxToStd(findFile(_T("data/black.png"))));
    m_bmp_back.LoadFromFile(StrWxToStd(findFile(_T("data/back.jpg"))));


    // Defaut
    m_settings = GetInternationalRules();
    m_board.SetRules(m_settings.rules);
    m_board.InitGame(m_settings.whitesStart, m_settings.hSize, m_settings.vSize, m_settings.pieceNb);

    // wxWidgets evt...
    Connect(wxEVT_SIZE, wxSizeEventHandler(SfmlBoard::OnResize));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(SfmlBoard::OnMouse));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(SfmlBoard::OnMouse));
    Connect(wxEVT_MOTION, wxMouseEventHandler(SfmlBoard::OnMouse));
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(SfmlBoard::OnMouse));
}
//------------------------------------------------------------------------------
void SfmlBoard::AbortAll()
{
    m_mvtFrameCount = 0;
    m_currentDndPos = wxPoint(0, 0);
    m_dragAndDropOrigin = Position(-1, -1);
    m_hightlightPos = Position(-1, -1);
    m_mvtStatus = 1;
    m_relativePosMvt.first = Position(-1, -1);
    m_relativePosMvt.second = Mvt(TopLeft, -1);
    m_winner = WINNER_NO;
}
//------------------------------------------------------------------------------
bool SfmlBoard::IsWhitesOnTop() const
{
    if(m_settings.isWhitesHuman)
    {
        if(m_settings.isBlacksHuman)
        {
            return !m_board.GetTurn();
        }
        else
        {
            return BLACKS;
        }
    }
    else
    {
        if(m_settings.isBlacksHuman)
        {
            return WHITES;
        }
        else
        {
            return BLACKS;
        }
    }
}
//------------------------------------------------------------------------------
void SfmlBoard::OnMouse(wxMouseEvent &event)
{
    if(event.GetEventType() == wxEVT_LEFT_DOWN && !IsMoving())
    {
        Position pos = GetCasePos(event.GetPosition());
        if(m_board.IsCaseInGameRelative(IsWhitesOnTop(), pos))
        {
            const Board::dameCase &dame = static_cast<const Board&>(m_board).GetDameRelative(IsWhitesOnTop(), pos);
            if(dame.isPiece && dame.isW == m_board.GetTurn())
            {
                m_dragAndDropOrigin = pos;
            }
        }
    }
    else if(event.GetEventType() == wxEVT_LEFT_UP && m_board.IsCaseInGameRelative(IsWhitesOnTop(), m_dragAndDropOrigin))
    {
        // Récupération de la case cible
        Position destPos = GetCasePos(event.GetPosition());

        if(m_board.GetTurn() != IsWhitesOnTop())
        {
            destPos = m_board.ExchangeRelativePos(destPos);
            m_dragAndDropOrigin = m_board.ExchangeRelativePos(m_dragAndDropOrigin);
        }

        // Tentative de créer le mouvement
        Mvt mvt;
        try
        {
            mvt = Mvt(m_dragAndDropOrigin, destPos);
        }
        catch(MyException &e) // Mouvement non diagonal
        {
            m_dragAndDropOrigin = Position(-1,-1);
            return;
        }

        // Mouvement possible ?
        // => Il faut prendre tous les mouvements, au cas où on puisse prendre...
        bool mvtOk = false;
        vector<pair<Position, vector<Mvt> > > tbl = m_board.GetAllPlayerPoosibilities(m_board.GetTurn());

        for(size_t posId = 0; posId < tbl.size(); posId++)
        {
            if(tbl[posId].first == m_dragAndDropOrigin)
            {
                for(size_t mvtId = 0; mvtId < tbl[posId].second.size(); mvtId++)
                {
                    Mvt mvt2 = tbl[posId].second[mvtId];
                    if(mvt2 == mvt)
                    {
                        mvtOk = true;
                        break;
                    }
                }

                break; // Quoiqu'il arrive, on peut arréter
            }
        }

        if(!mvtOk)
        {
            m_dragAndDropOrigin = Position(-1,-1);
            return;
        }

        MovePieceAnim(m_dragAndDropOrigin, mvt);

        m_dragAndDropOrigin = Position(-1,-1);
    }
    else if(event.GetEventType() == wxEVT_LEAVE_WINDOW)
    {
        m_dragAndDropOrigin = Position(-1,-1);
    }


    m_currentDndPos = event.GetPosition();
    m_hightlightPos = GetCasePos(m_currentDndPos);

    event.Skip();
}
//------------------------------------------------------------------------------
void SfmlBoard::DrawWinnerInfo()
{
    if(m_winner != WINNER_NO)
    {
        sf::String msg;
        if(m_winner == WINNER_WHITE)
        {
            msg.SetText(StrWxToSfUtf8(_("Whites won !")));
        }
        else
        {
            msg.SetText(StrWxToSfUtf8(_("Blacks won !")));
        }

        msg.SetColor(sf::Color(0, 0, 255));

        sf::FloatRect strRect = msg.GetRect();
        msg.SetPosition(GetSize().GetWidth() / 2 - strRect.GetWidth() / 2, GetSize().GetHeight() / 2 - strRect.GetHeight() / 2);


        Draw(sf::Shape::Rectangle(msg.GetRect().Left - 15, msg.GetRect().Top - 15, msg.GetRect().Right + 15, msg.GetRect().Bottom + 25, sf::Color(0, 0, 0), 4, sf::Color(0, 0, 255)));

        Draw(msg);
    }
}

//------------------------------------------------------------------------------
bool SfmlBoard::IsPaused() const
{
    return false;
}
//------------------------------------------------------------------------------
void SfmlBoard::SetPaused(const bool &paused)
{
    m_isPaused = paused;
}
//------------------------------------------------------------------------------
bool SfmlBoard::IsMoving() const
{
    return (m_mvtStatus < 1);
}
//------------------------------------------------------------------------------
void SfmlBoard::OnUpdate()
{
    bool mvtEnd = false;

    wxTopLevelWindow *frame = static_cast<wxTopLevelWindow*>(GetParent()->GetParent()->GetParent());

    if(IsMoving() && !IsPaused() && m_winner == WINNER_NO && frame->IsActive()) // Il y a un mouvement
    {
        if(m_mvtFrameCount <= 2) // On passe quelques frames parce que le frame time est très grand.
        {
            m_mvtFrameCount++;
        }
        else
        {
            m_mvtStatus += GetFrameTime() * 8 / float(m_relativePosMvt.second.GetDistance());
        }

        if(!IsMoving()) // Mouvement terminé...
        {
            // ==> On l'applique
            m_board.MovePiece(m_board.GetTurn(), m_posMvt.first, m_posMvt.second);
            mvtEnd = true;
        }
    }

    // Stats
        // Status Bar
    wxStatusBar *statusBar = static_cast<wxFrame*>(frame)->GetStatusBar();

    statusBar->SetStatusText(_("Whites: ") + wxNbToStr(m_board.GetAllPlayerDamesCount(WHITES)), 0);
    statusBar->SetStatusText(_("Blacks: ") + wxNbToStr(m_board.GetAllPlayerDamesCount(BLACKS)), 1);

    // On affiche
    DrawBoard();
    DrowDames();
    DrawWinnerInfo();

    // Si l'autre joueur est un bot, on joue pour lui.
    if(m_winner == WINNER_NO)
    {
        if(!IsMoving() && ((m_board.GetTurn() == WHITES && !m_settings.isWhitesHuman) || (m_board.GetTurn() == BLACKS && !m_settings.isBlacksHuman)))
        {
            // On affiche le buffer.
            Display();

            // On calcule
            int level = m_board.GetTurn() == WHITES ? m_settings.whitesLevel : m_settings.blacksLevel;
            try
            {
                //#define TEST
                #if defined TEST
                std::pair<Position, Mvt> posMvt;
                if(m_board.GetTurn() == WHITES)
                {
                    posMvt = m_board.GetIaMvt2(level);
                }
                else
                {
                    posMvt = m_board.GetIaMvt(level);
                }
                #else
                std::pair<Position, Mvt> posMvt = m_board.GetIaMvt(level);
                #endif
                MovePieceAnim(posMvt.first, posMvt.second);
            }
            catch(MyException &e)
            {
                SetWinner(!m_board.GetTurn());
            }
        }
        else if (mvtEnd) // Si non, si le mvt est fini et qu'on est humain, on verifie que l'humain n'a pas perdu
        {
            vector<pair<Position, vector<Mvt> > > possiblilities = m_board.GetAllPlayerPoosibilities(m_board.GetTurn());
            if(possiblilities.empty())
            {
                SetWinner(!m_board.GetTurn());
            }
        }
    }
}
//------------------------------------------------------------------------------
int SfmlBoard::GetWinner() const
{
    return m_winner;
}
//------------------------------------------------------------------------------
void SfmlBoard::SetWinner(const bool &color)
{
    AbortAll();

    if(color == WHITES)
    {
        m_winner = WINNER_WHITE;
    }
    else
    {
        m_winner = WINNER_BLACK;
    }
}
//------------------------------------------------------------------------------
Position SfmlBoard::GetCasePos(const wxPoint &mousePos) const
{
    Position pos;
    pos.x = float(mousePos.x)  / (float(GetSize().GetWidth()) / float(m_board.GetBoardHSize()));
    pos.y = float(mousePos.y)  / (float(GetSize().GetHeight()) / float(m_board.GetBoardVSize()));

    return pos;
}
//------------------------------------------------------------------------------
void SfmlBoard::OnResize(wxSizeEvent &event)
{
    event.Skip();
}
//------------------------------------------------------------------------------
void SfmlBoard::MovePieceAnim(const Position &pos, const Mvt &mvt)
{
    if(IsMoving()) // Un mouvement non terminé...
    {
        // ==> On l'applique
        m_board.MovePiece(m_board.GetTurn(), m_posMvt.first, m_posMvt.second);
    }

    // On commence un autre mouvement.
    m_mvtStatus = 0;
    m_mvtFrameCount = 0;
    m_posMvt.first = pos;
    m_posMvt.second = mvt;

    m_relativePosMvt = m_posMvt;

    if(m_board.GetTurn() != IsWhitesOnTop()) // S'il faut échanger
    {
        m_relativePosMvt.first = m_board.ExchangeRelativePos(m_relativePosMvt.first);
        m_relativePosMvt.second = m_relativePosMvt.second.GetReverse();
        if(m_relativePosMvt.second.IsEat())
        {
            m_relativePosMvt.second.SetEatPos(m_board.ExchangeRelativePos(m_relativePosMvt.second.GetEatPos()));
        }
    }
}
//------------------------------------------------------------------------------
void SfmlBoard::DrowDames()
{
    for(size_t x = 0; x < m_board.GetBoardHSize(); x++)
    {
        for(size_t y = 0; y < m_board.GetBoardVSize(); y++)
        {
            if(m_board.IsCaseInGameRelative(IsWhitesOnTop(), Position(x,y)))
            {
                const Board::dameCase &dame = (static_cast<const Board&>(m_board)).GetDameRelative(IsWhitesOnTop(), Position(x,y));
                if(dame.isPiece)
                {
                    const sf::Image *bmp;
                    if(dame.isW)
                    {
                        if(dame.isQueen)
                        {
                            bmp = &m_bmp_W;
                        }
                        else
                        {
                            bmp = &m_bmp_w;
                        }
                    }
                    else
                    {
                        if(dame.isQueen)
                        {
                            bmp = &m_bmp_B;
                        }
                        else
                        {
                            bmp = &m_bmp_b;
                        }
                    }

                    // Sprite
                    sf::Sprite sprite(*bmp);

                    // La position
                    sf::Rect<float> drawPos;
                    drawPos = GetDameImgRect(Position(x, y));

                    if(IsMoving()) // Il y a un mouvement
                    {
                        if(m_relativePosMvt.first == Position(x,y)) // Le pion courrant doit bouger...
                        {
                            drawPos = GetDameImgRect(Position(x, y), m_relativePosMvt.second, m_mvtStatus);
                        }
                        else if(m_relativePosMvt.second.IsEat() && m_relativePosMvt.second.GetEatPos() == Position(x,y))
                        {
                            sprite.SetColor(sf::Color(255, 255, 255, (1.f - m_mvtStatus) * 255.f));
                        }
                    }

                    if(m_dragAndDropOrigin == Position(x,y))
                    {
                        float height = drawPos.GetHeight();
                        float width = drawPos.GetWidth();

                        drawPos.Left = m_currentDndPos.x - width / 2;
                        drawPos.Top = m_currentDndPos.y - height / 2;
                        drawPos.Right = drawPos.Left + width;
                        drawPos.Bottom = drawPos.Top + height;
                    }


                    // On dessine l'image
                    sprite.SetPosition(drawPos.Left, drawPos.Top);
                    sprite.Resize(drawPos.GetWidth(), drawPos.GetHeight());
                    Draw(sprite);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void SfmlBoard::DrawBoard()
{
    //Clear(sf::Color(255, 255, 255));

    // Fond
    sf::Sprite back(m_bmp_back);
    back.SetPosition(0, 0);
    back.Resize(GetWidth(), GetHeight());
    Draw(back);

    // Cases
    sf::Sprite blackCase(m_bmp_blackCase);

    for(size_t x = 0; x < m_board.GetBoardHSize(); x++)
    {
        for(size_t y = 0; y < m_board.GetBoardVSize(); y++)
        {
            if(m_board.IsCaseInGameRelative(IsWhitesOnTop(), Position(x,y)))
            {
                sf::Rect<float> caseRect = GetDameImgRect(Position(x,y));
                sf::Color caseColor;

                if(m_hightlightPos == Position(x,y))
                {
                    caseColor = sf::Color(180, 180, 180);
                }
                else
                {
                    caseColor = sf::Color(255, 255, 255);
                }
                blackCase.SetColor(caseColor);
                blackCase.Resize(caseRect.GetWidth(), caseRect.GetHeight());
                blackCase.SetPosition(caseRect.Left, caseRect.Top);
                Draw(blackCase);

            }
        }
    }
}
//------------------------------------------------------------------------------
sf::Rect<float> SfmlBoard::GetDameImgRect(const Position &pos, const Mvt &mvt, const float &mvtStatus) const
{
    const float caseWidth = float(GetWidth()) / float(m_board.GetBoardHSize());
    const float caseHeight = float(GetHeight()) / float(m_board.GetBoardVSize());

    // La position
    sf::Rect<float> drawPos(pos.x * caseWidth, pos.y * caseHeight, (pos.x + 1) * caseWidth, (pos.y + 1)* caseHeight);

    // Cas d'un mouvement
    if(mvtStatus != 1 && mvtStatus != 0)
    {
        if(mvt.IsRight()) // Vers la droite
        {
            drawPos.Offset(caseWidth * float(mvt.GetDistance()) * mvtStatus, 0);
        }
        else
        {
            drawPos.Offset(caseWidth * float(mvt.GetDistance()) * mvtStatus * (-1.f), 0);
        }

        if(mvt.IsTop()) // Vers le haut
        {
            drawPos.Offset(0, caseHeight * float(mvt.GetDistance()) * mvtStatus);
        }
        else
        {
            drawPos.Offset(0, caseHeight * float(mvt.GetDistance()) * mvtStatus * (-1.f));
        }
    }

    return drawPos;
}
//------------------------------------------------------------------------------
