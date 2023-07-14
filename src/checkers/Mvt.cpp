///File : Mvt.cpp
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
#include <iostream>
#include <stdlib.h>
//------------------------------------------------------------------------------
#include "Mvt.hpp"
#include "MyException.hpp"
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
Mvt::Mvt() : m_direction(TopRight), m_distance(1), m_eat(false)
{

}
//------------------------------------------------------------------------------
Mvt::Mvt(const int &direction, const int &distance) : m_eat(false)
{
    SetDirection(direction);
    SetDistance(distance);
}
//------------------------------------------------------------------------------
Mvt::Mvt(const int &direction, const int &distance, const bool &eat) : m_eat(eat)
{
    SetDirection(direction);
    SetDistance(distance);
}
//------------------------------------------------------------------------------
Mvt::Mvt(const Position &begin, const Position &end)
{
    if(abs(end.x - begin.x) != abs(end.y - begin.y))
    {
        throw MyException("Mvt: Not diagonal !");
    }

    if(end.x - begin.x > 0)
    {
        SetRight();
    }
    else
    {
        SetLeft();
    }

    if(end.y - begin.y < 0)
    {
        SetBottom();
    }
    else
    {
        SetTop();
    }

    SetDistance(abs(end.x - begin.x));
}
//------------------------------------------------------------------------------
bool Mvt::operator==(const Mvt &mvt) const
{
    //return (m_direction == mvt.m_direction && m_distance == mvt.m_distance && m_eat == mvt.m_eat && m_eatPos == mvt.m_eatPos && m_possible == mvt.m_possible);
    return (m_direction == mvt.m_direction && m_distance == mvt.m_distance);
}
//------------------------------------------------------------------------------
bool Mvt::operator!=(const Mvt &mvt) const
{
    return !(mvt == *this);
}
//------------------------------------------------------------------------------
int Mvt::GetDirection() const
{
    return m_direction;
}
//------------------------------------------------------------------------------
void Mvt::SetDirection(const int &direction)
{
    if(direction == TopLeft || direction == TopRight || direction == BottomLeft || direction == BottomRight)
    {
        m_direction = direction;
    }
    else
    {
        throw MyException("Unknown direction !");
    }
}
//------------------------------------------------------------------------------
Position Mvt::GetEatPos() const
{
    if(IsEat())
    {
        return m_eatPos;
    }
    else
    {
        throw MyException("GetEatPos() : !IsEat() !");
    }
}
//------------------------------------------------------------------------------
void Mvt::SetEatPos(const Position &pos)
{
    m_eatPos = pos;
}
//------------------------------------------------------------------------------
bool Mvt::IsTop() const
{
    return (m_direction == TopLeft || m_direction == TopRight);
}
//------------------------------------------------------------------------------
bool Mvt::IsBottom() const
{
    return (!IsTop());
}
//------------------------------------------------------------------------------
bool Mvt::IsRight() const
{
    return (m_direction == BottomRight || m_direction == TopRight);
}
//------------------------------------------------------------------------------
bool Mvt::IsLeft() const
{
    return (!IsRight());
}
//------------------------------------------------------------------------------
void Mvt::SetEat(const bool &eat)
{
    m_eat = eat;
}
//------------------------------------------------------------------------------
bool Mvt::IsEat() const
{
    return m_eat;
}
//------------------------------------------------------------------------------
void Mvt::SetPossible(const bool &possible)
{
    m_possible = possible;
}
//------------------------------------------------------------------------------
bool Mvt::IsPossible() const
{
    return m_possible;
}
//------------------------------------------------------------------------------
int Mvt::GetDistance() const
{
    return m_distance;
}
//------------------------------------------------------------------------------
void Mvt::SetDistance(const int &distance)
{
    m_distance = distance;
}
//------------------------------------------------------------------------------
void Mvt::SetTop()
{
    if(IsLeft())
    {
        m_direction = TopLeft;
    }
    else
    {
        m_direction = TopRight;
    }
}
//------------------------------------------------------------------------------
void Mvt::SetBottom()
{
    if(IsLeft())
    {
        m_direction = BottomLeft;
    }
    else
    {
        m_direction = BottomRight;
    }
}
//------------------------------------------------------------------------------
void Mvt::SetRight()
{
    if(IsTop())
    {
        m_direction = TopRight;
    }
    else
    {
        m_direction = BottomRight;
    }
}
//------------------------------------------------------------------------------
void Mvt::SetLeft()
{
    if(IsTop())
    {
        m_direction = TopLeft;
    }
    else
    {
        m_direction = BottomLeft;
    }
}
//------------------------------------------------------------------------------
Mvt Mvt::GetReverse() const
{
    Mvt mvt= *this;
    switch(mvt.GetDirection())
    {
        case TopLeft:
            mvt.SetDirection(BottomRight);
            break;
        case TopRight:
            mvt.SetDirection(BottomLeft);
            break;
        case BottomLeft:
            mvt.SetDirection(TopRight);
            break;
        case BottomRight:
            mvt.SetDirection(TopLeft);
            break;
    }

    return mvt;
}
//------------------------------------------------------------------------------
Position Mvt::GetResultPos(Position pos) const
{
    if(IsRight())
    {
        pos.x += m_distance;
    }
    else
    {
        pos.x -= m_distance;
    }

    if(IsTop())
    {
        pos.y += m_distance;
    }
    else
    {
        pos.y -= m_distance;
    }

    return pos;
}
//------------------------------------------------------------------------------
