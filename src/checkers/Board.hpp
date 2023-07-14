///File : Board.hpp
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
#if not defined BOARD_HPP_C501_CHECKERS
#define BOARD_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
#define MUST_EAT true

#define WHITES true
#define BLACKS false
//------------------------------------------------------------------------------
#include "Mvt.hpp"
#include <vector>
#include <utility>
//------------------------------------------------------------------------------
//#define TEST
//------------------------------------------------------------------------------
int hasard(const int &min, const int &max);
//------------------------------------------------------------------------------
struct rulesStr
{
    bool pieceCanEatBack;
    bool slowQueen;
};
//------------------------------------------------------------------------------
class Board
{
public:
    Board();
    //Board(const Board &src);

    //const Board &operator=(const Board &src);

    bool GetTurn() const;

    void InitGame(const bool &turnColor = WHITES, const int &hSize = 10, const int &vSize = 10, int pieceNb = 20);
    //void Cat() const;
    bool IsCaseInGameRelative(const bool &isWhite, const Position &pos) const;
    Mvt MovePiece(const bool &isWhite, const Position &pos, Mvt mvt, const bool &act = true);

    std::vector<Mvt> GetMvtPossibilitiesRelative(const bool &isWhite, Position pos);
    std::vector<std::pair<Position, std::vector<Mvt> > > GetAllPlayerPossibilities(const bool &isWhite);

    Position ExchangeRelativePos(Position pos) const;

    std::vector<Position> GetAllPlayerDames(const bool &isWhite) const;
    int GetAllPlayerDamesCount(const bool &isWhite) const;

    std::pair<Position, Mvt> GetIaMvt(const int &level = 3);
    #if defined TEST
    std::pair<Position, Mvt> GetIaMvt2(const int &level = 3);
    #endif

    rulesStr GetRules() const;
    void SetRules(const rulesStr &rules);

    struct dameCase
    {
        bool isPiece;
        bool isW;
        bool isQueen;
    };

    const dameCase &GetDameRelative(const bool &isWhite, Position pos, const Mvt &mvt) const;
    const dameCase &GetDameRelative(const bool &isWhite, Position pos) const;

    int GetBoardVSize() const;
    int GetBoardHSize() const;

private:
    bool IsCaseInGame(const Position &pos) const;

    #if defined TEST
    float IaProceed2(const int &level, const bool &color, const Position &pos, const Mvt &mvt) const;
    #endif
    float IaProceed(const int &level, const bool &color, const Position &pos, const Mvt &mvt) const;

    Position GetRealPos(const bool &isWhite, Position pos) const;
    Position GetRealPos(const bool &isWhite, Position pos, const Mvt &mvt) const;

    dameCase &GetDameRelative(const bool &isWhite, Position pos);
    dameCase &GetDameRelative(const bool &isWhite, Position pos, const Mvt &mvt);

    void SwapCases(dameCase &c1, dameCase &c2);

    // Board
    bool m_turnColor;
    std::vector<std::vector<dameCase> > m_board;
    rulesStr m_rules;

    // Prise 2 fois
    Position m_posContinue;
    Mvt m_lastMvt; // On ne peut pas jouer dans une direction opposée
};
//------------------------------------------------------------------------------
#endif //BOARD_HPP_C501_CHECKERS
