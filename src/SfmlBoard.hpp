///File : SfmlBoard.hpp
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
#include "wxSFMLCanvas.hpp"
#include "DialogConfig.hpp"
#include <Board.hpp>
//------------------------------------------------------------------------------
#if not defined SFML_BOARD_HPP_C501_CHECKERS
#define SFML_BOARD_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
enum
{
    WINNER_WHITE,
    WINNER_BLACK,
    WINNER_NO
};
//------------------------------------------------------------------------------
class SfmlBoard : public wxSFMLCanvas
{
public:
    SfmlBoard(wxWindow* Parent, wxWindowID Id, Board &board, strGameSettings &settings, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);

    void MovePieceAnim(const Position &pos, const Mvt &mvt);
    void OnMouse(wxMouseEvent &event);

    bool IsMoving() const;

    void AbortAll();

    bool IsPaused() const;
    void SetPaused(const bool &paused);

    int GetWinner() const;
    void SetWinner(const bool &color);

private:
    bool IsWhitesOnTop() const;

    void DrawBoard();
    void DrowDames();
    void DrawWinnerInfo();
    sf::Rect<float> GetDameImgRect(const Position &pos, const Mvt &mvt = Mvt(TopRight,0), const float &mvtStatus = 0) const;

    Position GetCasePos(const wxPoint &mousePos) const;

    virtual void OnUpdate();
    void OnResize(wxSizeEvent &event);

    sf::Image m_bmp_W, m_bmp_w, m_bmp_B, m_bmp_b, m_bmp_blackCase, m_bmp_back;

    // Mouvement en cours...
    float m_mvtStatus;
    std::pair<Position, Mvt> m_posMvt;
    std::pair<Position, Mvt> m_relativePosMvt;
    int m_mvtFrameCount;

    Position m_dragAndDropOrigin;
    Position m_hightlightPos;

    wxPoint m_currentDndPos;

    // Config + board
    Board &m_board;
    strGameSettings &m_settings;

    // Status...
    bool m_isPaused;
    int m_winner;
};
//------------------------------------------------------------------------------
#endif // SFML_BOARD_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
