///File : Mvt.hpp
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
#if not defined MVT_HPP_C501_CHECKERS
#define MVT_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
enum
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};
//------------------------------------------------------------------------------
class Position
{
public:
    Position(const int &posX, const int &posY) : x(posX), y(posY) {};
    Position() : x(0), y(0) {};
    bool operator==(const Position &pos) const {return (pos.x == x && pos.y == y);};
    bool operator!=(const Position &pos) const {return !(*this == pos);};

    int x;
    int y;
};
//------------------------------------------------------------------------------
class Mvt
{
public:
    Mvt();
    Mvt(const int &direction, const int &distance);
    Mvt(const int &direction, const int &distance, const bool &eat);
    Mvt(const Position &begin, const Position &end);

    bool operator==(const Mvt &mvt) const;
    bool operator!=(const Mvt &mvt) const;

    int GetDirection() const;
    void SetDirection(const int &direction);
    bool IsTop() const;
    bool IsBottom() const;
    bool IsRight() const;
    bool IsLeft() const;

    void SetTop();
    void SetBottom();
    void SetRight();
    void SetLeft();

    void SetEat(const bool &eat);
    bool IsEat() const;

    void SetPossible(const bool &possible);
    bool IsPossible() const;

    int GetDistance() const;
    void SetDistance(const int &distance);

    Position GetEatPos() const;
    void SetEatPos(const Position &pos);

    Position GetResultPos(Position pos) const;

    Mvt GetReverse() const;

private:
    int m_direction;
    int m_distance;
    bool m_eat;
    bool m_possible;

    Position m_eatPos;
};
//------------------------------------------------------------------------------
#endif // MVT_HPP_C501_CHECKERS
//------------------------------------------------------------------------------
