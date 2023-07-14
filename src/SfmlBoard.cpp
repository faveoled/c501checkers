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
    // Preparation of images...
    m_bmp_W.loadFromFile(StrWxToStd(findFile(_T("data/wq.png"))));
    m_bmp_w.loadFromFile(StrWxToStd(findFile(_T("data/w.png"))));
    m_bmp_B.loadFromFile(StrWxToStd(findFile(_T("data/bq.png"))));
    m_bmp_b.loadFromFile(StrWxToStd(findFile(_T("data/b.png"))));
    m_bmp_blackCase.loadFromFile(StrWxToStd(findFile(_T("data/black.png"))));
    m_bmp_back.loadFromFile(StrWxToStd(findFile(_T("data/back.jpg"))));


    // Default
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
        // Retrieve the target cell
        Position destPos = GetCasePos(event.GetPosition());

        if(m_board.GetTurn() != IsWhitesOnTop())
        {
            destPos = m_board.ExchangeRelativePos(destPos);
            m_dragAndDropOrigin = m_board.ExchangeRelativePos(m_dragAndDropOrigin);
        }

        // Attempt to create the movement
        Mvt mvt;
        try
        {
            mvt = Mvt(m_dragAndDropOrigin, destPos);
        }
        catch(MyException &e) // Movement non diagonal
        {
            m_dragAndDropOrigin = Position(-1,-1);
            return;
        }

        // Movement possible ?
        // => We must take all the movements, in case we can take...
        bool mvtOk = false;
        vector<pair<Position, vector<Mvt> > > tbl = m_board.GetAllPlayerPossibilities(m_board.GetTurn());

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

                break; // Whatever happens, we can stop
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
        sf::Text msg;
        if(m_winner == WINNER_WHITE)
        {
            msg.setString(StrWxToSfUtf8(_("Whites won !")));
        }
        else
        {
            msg.setString(StrWxToSfUtf8(_("Blacks won !")));
        }


        msg.setColor(sf::Color(0, 0, 255));

        sf::FloatRect strRect = msg.getLocalBounds();
        msg.setPosition(GetSize().GetWidth() / 2 - strRect.width / 2, GetSize().GetHeight() / 2 - strRect.height / 2);


        sf::FloatRect textRect = msg.getGlobalBounds(); // get the text bounds
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(textRect.width + 30, textRect.height + 40)); // set the size of the rectangle with some padding
        rect.setPosition(sf::Vector2f(textRect.left - 15, textRect.top - 15)); // set the position of the rectangle to match the text
        rect.setFillColor(sf::Color(0, 0, 0)); // set the fill color of the rectangle
        rect.setOutlineColor(sf::Color(0, 0, 255)); // set the outline color of the rectangle
        rect.setOutlineThickness(4); // set the outline thickness of the rectangle

        draw(rect); // draw the rectangle shape
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
    sf::Clock clock;

    bool mvtEnd = false;

    wxTopLevelWindow *frame = dynamic_cast<wxTopLevelWindow*>(GetParent()->GetParent()->GetParent());

    if(IsMoving() && !IsPaused() && m_winner == WINNER_NO && frame->IsActive()) // There is a movement
    {
        if(m_mvtFrameCount <= 2) // We skip a few frames because the frame time is very large.
        {
            m_mvtFrameCount++;
        }
        else
        {
            m_mvtStatus += clock.restart().asMilliseconds() * 8 / float(m_relativePosMvt.second.GetDistance());
        }

        if(!IsMoving()) // Movement finished...
        {
            // ==> We apply it
            m_board.MovePiece(m_board.GetTurn(), m_posMvt.first, m_posMvt.second);
            mvtEnd = true;
        }
    }

    // Stats
    // Status Bar
    wxStatusBar *statusBar = static_cast<wxFrame*>(frame)->GetStatusBar();

    statusBar->SetStatusText(_("Whites: ") + wxNbToStr(m_board.GetAllPlayerDamesCount(WHITES)), 0);
    statusBar->SetStatusText(_("Blacks: ") + wxNbToStr(m_board.GetAllPlayerDamesCount(BLACKS)), 1);

    // Display
    DrawBoard();
    DrowDames();
    DrawWinnerInfo();

    // If the other player is a bot, we play for him.
    if(m_winner == WINNER_NO)
    {
        if(!IsMoving() && ((m_board.GetTurn() == WHITES && !m_settings.isWhitesHuman) || (m_board.GetTurn() == BLACKS && !m_settings.isBlacksHuman)))
        {
            // We display the buffer.
            display();

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
        else if (mvtEnd) // If not, if the mvt is finished and we are human, we check that the human has not lost
        {
            vector<pair<Position, vector<Mvt> > > possiblilities = m_board.GetAllPlayerPossibilities(m_board.GetTurn());
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
    if(IsMoving()) // An unfinished move...
    {
        // ==> We apply it
        m_board.MovePiece(m_board.GetTurn(), m_posMvt.first, m_posMvt.second);
    }

    // We start another movement.
    m_mvtStatus = 0;
    m_mvtFrameCount = 0;
    m_posMvt.first = pos;
    m_posMvt.second = mvt;

    m_relativePosMvt = m_posMvt;

    if(m_board.GetTurn() != IsWhitesOnTop()) // Whether to swap
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

                    sf::Texture bmpTexture;
                    bmpTexture.loadFromImage(*bmp);

                    // Sprite
                    sf::Sprite sprite(bmpTexture);

                    // The position
                    sf::Rect<float> drawPos;
                    drawPos = GetDameImgRect(Position(x, y));

                    if(IsMoving()) // There is a movement
                    {
                        if(m_relativePosMvt.first == Position(x,y)) // The current pawn must move...
                        {
                            drawPos = GetDameImgRect(Position(x, y), m_relativePosMvt.second, m_mvtStatus);
                        }
                        else if(m_relativePosMvt.second.IsEat() && m_relativePosMvt.second.GetEatPos() == Position(x,y))
                        {
                            sprite.setColor(sf::Color(255, 255, 255, (1.f - m_mvtStatus) * 255.f));
                        }
                    }

                    if(m_dragAndDropOrigin == Position(x,y))
                    {
                        float height = drawPos.height;
                        float width = drawPos.width;

                        drawPos.left = m_currentDndPos.x - width / 2;
                        drawPos.top = m_currentDndPos.y - height / 2;
                        // TODO Fix
//                        drawPos.right = drawPos.left + width;
//                        drawPos.bottom = drawPos.top + height;
                    }


                    // We draw the image
                    sprite.setPosition(drawPos.left, drawPos.top);
                    sprite.setScale(
                            drawPos.width / sprite.getLocalBounds().width,
                            drawPos.height / sprite.getLocalBounds().height
                    );
                    draw(sprite);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void SfmlBoard::DrawBoard()
{
    //Clear(sf::Color(255, 255, 255));

    sf::Texture backTexture;
    backTexture.loadFromImage(m_bmp_back);

    // Fond
    sf::Sprite back(backTexture);
    back.setPosition(0, 0);

    back.setScale(
            GetSize().GetWidth() / back.getLocalBounds().width,
            GetSize().GetHeight() / back.getLocalBounds().height
    );
    draw(back);

    sf::Texture blackCaseTexture;
    blackCaseTexture.loadFromImage(m_bmp_blackCase);

    // Cases
    sf::Sprite blackCase(blackCaseTexture);

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
                blackCase.setColor(caseColor);
                blackCase.setScale(
                        caseRect.width / blackCase.getLocalBounds().width,
                        caseRect.height / blackCase.getLocalBounds().height
                );
                blackCase.setPosition(caseRect.left, caseRect.top);
                draw(blackCase);

            }
        }
    }
}
//------------------------------------------------------------------------------
sf::Rect<float> SfmlBoard::GetDameImgRect(const Position &pos, const Mvt &mvt, const float &mvtStatus) const
{
    const float caseWidth = float(GetSize().GetWidth()) / float(m_board.GetBoardHSize());
    const float caseHeight = float(GetSize().GetHeight()) / float(m_board.GetBoardVSize());

    // The position
    sf::Rect<float> drawPos(pos.x * caseWidth, pos.y * caseHeight, caseWidth, caseHeight);

    // Case of a movement
    if(mvtStatus != 1 && mvtStatus != 0)
    {
        if(mvt.IsRight()) // To the right
        {
            drawPos.left = caseWidth * float(mvt.GetDistance()) * mvtStatus;
            drawPos.top = 0;
        }
        else
        {
            drawPos.left = caseWidth * float(mvt.GetDistance()) * mvtStatus * (-1.f);
            drawPos.top = 0;
        }

        if(mvt.IsTop()) // To the left
        {
            drawPos.left = 0;
            drawPos.top = caseHeight * float(mvt.GetDistance()) * mvtStatus;
        }
        else
        {
            drawPos.left = 0;
            drawPos.top = caseHeight * float(mvt.GetDistance()) * mvtStatus * (-1.f);
        }
    }

    return drawPos;
}
//------------------------------------------------------------------------------
