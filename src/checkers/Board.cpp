///File : Board.cpp
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
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <ctime>
//------------------------------------------------------------------------------
#include "Board.hpp"
#include "MyException.hpp"
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
int hasard(const int &min, const int &max)
{
    static bool initOk = false;
    if(!initOk)
    {
        srand(time(NULL)); // NE JAMAIS L'OUBLIER ==> Pour hasard()
        initOk = true;
    }

    return (int) (min + ((float) rand() / RAND_MAX * (max - min + 1)));
}
//------------------------------------------------------------------------------
Board::Board() : m_posContinue(Position(-1,-1))
{
    m_turnColor = WHITES;
    m_rules.pieceCanEatBack = true;
    m_rules.slowQueen = false;
    InitGame();
}
//------------------------------------------------------------------------------
#if defined TEST
std::pair<Position, Mvt> Board::GetIaMvt2(const int &level)
{
    bool color = GetTurn();

    vector<pair<Position, Mvt> > selectedList;
    float scoreMax = float(-1./0.); // Plus petite valeur possible pour un float

    bool isSelected = false;

    vector<pair<Position, vector<Mvt> > > possiblilities = GetAllPlayerPoosibilities(color);

    for(size_t pieceId = 0; pieceId < possiblilities.size(); pieceId++)
    {
        const Position &currentPos = possiblilities[pieceId].first;
        for(size_t mvtId = 0; mvtId < possiblilities[pieceId].second.size(); mvtId++)
        {
            float currentScore = IaProceed2(level - 1, color, currentPos, possiblilities[pieceId].second[mvtId]);
            if(scoreMax <= currentScore)
            {
                if(scoreMax < currentScore) // Nouveau score
                {
                    selectedList.clear();
                    scoreMax = currentScore;
                }

                selectedList.push_back(pair<Position, Mvt>(currentPos, possiblilities[pieceId].second[mvtId]));
                isSelected = true;
            }
        }
    }

    if(!isSelected)
    {
        throw MyException("Error : GetIaMvt: No possibilities !!!");
    }

    return selectedList[hasard(0, selectedList.size() - 1)];
}
#endif
//------------------------------------------------------------------------------
std::pair<Position, Mvt> Board::GetIaMvt(const int &level)
{
    vector<pair<Position, Mvt> > selectedList;
    float scoreMax = float(-1./0.); // Plus petite valeur possible pour un float

    bool isSelected = false;

    vector<pair<Position, vector<Mvt> > > possiblilities = GetAllPlayerPoosibilities(GetTurn());

    for(size_t pieceId = 0; pieceId < possiblilities.size(); pieceId++)
    {
        const Position &currentPos = possiblilities[pieceId].first;
        for(size_t mvtId = 0; mvtId < possiblilities[pieceId].second.size(); mvtId++)
        {
            float currentScore = IaProceed(level -1, GetTurn(), currentPos, possiblilities[pieceId].second[mvtId]);
            if(scoreMax <= currentScore)
            {
                if(scoreMax < currentScore) // Nouveau score
                {
                    selectedList.clear();
                    scoreMax = currentScore;
                }

                selectedList.push_back(pair<Position, Mvt>(currentPos, possiblilities[pieceId].second[mvtId]));
                isSelected = true;
            }
        }
    }

    if(!isSelected)
    {
        throw MyException("Error : GetIaMvt: No possibilities !!!");
    }

    return selectedList[hasard(0, selectedList.size() - 1)];
}
//------------------------------------------------------------------------------
Position Board::ExchangeRelativePos(Position pos) const
{
    // La gauche et la droite s'inversent...
    pos.x = GetBoardHSize() - 1 - pos.x;

    // Le bas et le haut s'inversent...
    pos.y = GetBoardVSize() - 1 - pos.y;

    return pos;
}
//------------------------------------------------------------------------------
rulesStr Board::GetRules() const
{
    return m_rules;
}
//------------------------------------------------------------------------------
void Board::SetRules(const rulesStr &rules)
{
    m_rules = rules;
}
//------------------------------------------------------------------------------
#if defined TEST
float Board::IaProceed2(const int &level, const bool &color, const Position &pos, const Mvt &mvt) const
{
    float score = 0;

    // 1 - Calcul du score
    if(!GetDameRelative(GetTurn(), pos).isQueen)
    {
        // Verif : Est ce qu'on est un pion (pr faire avancer les pions vers la damme) => On favorise les pions.
        score += 0.2;

        // Plus le pion est proche de la dame, plus il a de points (de 0 à 0.5)
        score += (float(mvt.GetResultPos(pos).y) / float(GetBoardVSize() - 1)) * 0.5;
    }

    // Si on boufe...
    if(mvt.IsEat())
    {
        score+=2;

        // Et si on mange une reine, c'est encore mieux...
        const dameCase &dame = GetDameRelative(GetTurn(), mvt.GetEatPos());
        if(dame.isQueen)
        {
            score+=1;
        }
    }

    // 2 - Si on ne joue pas, c'est pas bon pour nous => negatif
    bool wePlay = (GetTurn() == color);
    if(!wePlay)
    {
        score *= -1.5; // Limite la prise de risques.
    }

    // 3 - On continue l'IA
    if(level > 0)
    {
        // 3.1 - On copie
        Board tmpBoard = *this;

        // 3.2 - On fait le mouvement dans le tmpBoard...
        tmpBoard.MovePiece(tmpBoard.GetTurn(), pos, mvt);

        float tmpScore = 0; // Pour le diviser par le totale...
        int total = 0;

        vector<pair<Position, vector<Mvt> > > possiblilities = tmpBoard.GetAllPlayerPoosibilities(tmpBoard.GetTurn());
        for(size_t pieceId = 0; pieceId < possiblilities.size(); pieceId++)
        {
            const Position &currentPos = possiblilities[pieceId].first;
            for(size_t mvtId = 0; mvtId < possiblilities[pieceId].second.size(); mvtId++)
            {
                tmpScore += tmpBoard.IaProceed2(level -1, color, currentPos, possiblilities[pieceId].second[mvtId]);
                total++;
            }
        }

        if(total != 0)
        {
            tmpScore /= total; // Plus on est sûr du mouvement, mieux c'est !
            score += tmpScore;
        }
        else // Bloqué
        {
            if(tmpBoard.GetTurn() == color) // C'est nous qui somme bloqué...
            {
                score -= 4 * level;
            }
            else // C'est l'ennemie qui est bloqué
            {
                score += 3 * level;
            }
        }
    }

    return score;
}
#endif
//------------------------------------------------------------------------------
float Board::IaProceed(const int &level, const bool &color, const Position &pos, const Mvt &mvt) const
{
    float score = 0;

    // 1 - Calcul du score
    if(!GetDameRelative(GetTurn(), pos).isQueen)
    {
        // Verif : Est ce qu'on est un pion (pr faire avancer les pions vers la damme) => On favorise les pions.
        score += 0.2;

        // Plus le pion est proche de la dame, plus il a de points (de 0 à 0.5)
        score += (float(mvt.GetResultPos(pos).y) / float(GetBoardVSize() - 1)) * 0.5;
    }

    // Si on boufe...
    if(mvt.IsEat())
    {
        score+=2;

        // Et si on mange une reine, c'est encore mieux...
        const dameCase &dame = GetDameRelative(GetTurn(), mvt.GetEatPos());
        if(dame.isQueen)
        {
            score+=1;
        }
    }

    // 2 - Si on ne joue pas, c'est pas bon pour nous => negatif
    bool wePlay = (GetTurn() == color);
    if(!wePlay)
    {
        score *= -1.5; // Limite la prise de risques.
    }

    // 3 - On continue l'IA
    if(level > 0)
    {
        // 3.1 - On copie
        Board tmpBoard = *this;

        // 3.2 - On fait le mouvement dans le tmpBoard...
        tmpBoard.MovePiece(tmpBoard.GetTurn(), pos, mvt);

        float tmpScore = 0; // Pour le diviser par le totale...
        int total = 0;

        vector<pair<Position, vector<Mvt> > > possiblilities = tmpBoard.GetAllPlayerPoosibilities(tmpBoard.GetTurn());
        for(size_t pieceId = 0; pieceId < possiblilities.size(); pieceId++)
        {
            const Position &currentPos = possiblilities[pieceId].first;
            for(size_t mvtId = 0; mvtId < possiblilities[pieceId].second.size(); mvtId++)
            {
                tmpScore += tmpBoard.IaProceed(level -1, color, currentPos, possiblilities[pieceId].second[mvtId]);
                total++;
            }
        }

        if(total != 0)
        {
            tmpScore /= total; // Plus on est sûr du mouvement, mieux c'est !
            score += tmpScore;
        }
        else // Bloqué
        {
            if(tmpBoard.GetTurn() == color) // C'est nous qui somme bloqué...
            {
                score -= 4 * level;
            }
            else // C'est l'ennemie qui est bloqué
            {
                score += 3 * level;
            }
        }
    }

    return score;
}
//------------------------------------------------------------------------------
/*Board::Board(const Board &src)
{
    *this = src;
}*/
//------------------------------------------------------------------------------
bool Board::GetTurn() const
{
    return m_turnColor;
}
//------------------------------------------------------------------------------
/*const Board &Board::operator=(const Board &src)
{
    // Tbl
    for(size_t x = 0; x < GetBoardHSize(); x++)
    {
        for(size_t y = 0; y < GetBoardVSize(); y++)
        {
            m_board[x][y] = src.m_board[x][y];
        }
    }

    //m_isWhiteOnBottom = src.m_isWhiteOnBottom;
    m_turnColor = src.m_turnColor;
    m_posContinue = src.m_posContinue;

    return *this;
}*/
//------------------------------------------------------------------------------
void Board::InitGame(const bool &turnColor, const int &hSize, const int &vSize, int pieceNb)
{
    // Tests
    if(hSize <= 0 || hSize % 2 != 0 || vSize <= 0 || vSize % 2 != 0 || pieceNb <= 0)
    {
        throw MyException("InitGame() : Bad size !");
    }

    // Nb de cases
    const int max = (hSize / 2) * (vSize / 2) - 1;
    if(pieceNb > max)
    {
        pieceNb = max;
    }

    m_turnColor = turnColor;

    // On vide et on recrée le jeu
    m_board.clear();

    dameCase emptyDame;
    emptyDame.isPiece = false;
    emptyDame.isW = false;
    emptyDame.isQueen = false;

    m_board.resize(hSize);
    for(size_t i = 0; i < m_board.size(); i++)
    {
        m_board[i].resize(vSize, emptyDame);
    }

    // On remplie
    bool isWhite = true;

    do
    {
        Position relativePos;
        relativePos.x = 1; // Tjr case tout en haut à gauche vide.
        relativePos.y = 0;
        for(size_t i = pieceNb; i > 0; i--)
        {
            dameCase &dame = GetDameRelative(isWhite, relativePos);
            dame.isPiece = true;
            dame.isW = isWhite;

            relativePos.x += 2;
            if(relativePos.x >= GetBoardHSize())
            {
                relativePos.x-= GetBoardHSize();
                relativePos.y++;

                // Décallage
                if(relativePos.y % 2) // Lignes paires
                {
                    relativePos.x--;
                }
                else
                {
                    relativePos.x++;
                }

            }
        }

        isWhite = !isWhite;
    }
    while(!isWhite);
}
//------------------------------------------------------------------------------
/*void Board::Cat() const
{
    cout << " ";
    for(size_t i = 0; i < GetBoardVSize(); i++)
    {
        cout << GetBoardVSize() - 1 - i << " ";
    }
    cout << "\n";


    for(size_t x = 0; x < GetBoardHSize(); x++)
    {
        cout << GetBoardHSize() - 1 - x;
        for(size_t y = 0; y < GetBoardVSize(); y++)
        {
            if(m_board[y][x].isPiece)
            {
                if(m_board[y][x].isW)
                {
                    if(m_board[y][x].isQueen)
                    {
                        cout << "W";
                    }
                    else
                    {
                        cout << "w";
                    }
                }
                else
                {
                    if(m_board[y][x].isQueen)
                    {
                        cout << "B";
                    }
                    else
                    {
                        cout << "b";
                    }
                }
            }
            else
            {
                cout << "_";
            }
            cout << " ";
        }
        cout << x;
        cout << "\n";
    }

    cout << " ";
    for(size_t i = 0; i < GetBoardVSize(); i++)
    {
        cout << i << " ";
    }
    cout << "\n";

    cout << flush;
}*/
//------------------------------------------------------------------------------
Board::dameCase &Board::GetDameRelative(const bool &isWhite, Position pos)
{
    pos = GetRealPos(isWhite, pos);
    return m_board[pos.x][pos.y];
}
//------------------------------------------------------------------------------
Board::dameCase &Board::GetDameRelative(const bool &isWhite, Position pos, const Mvt &mvt)
{
    pos = GetRealPos(isWhite, pos, mvt);
    return m_board[pos.x][pos.y];
}
//------------------------------------------------------------------------------
const Board::dameCase &Board::GetDameRelative(const bool &isWhite, Position pos) const
{
    pos = GetRealPos(isWhite, pos);
    return m_board[pos.x][pos.y];
}
//------------------------------------------------------------------------------
const Board::dameCase &Board::GetDameRelative(const bool &isWhite, Position pos, const Mvt &mvt) const
{
    pos = GetRealPos(isWhite, pos, mvt);
    return m_board[pos.x][pos.y];
}
//------------------------------------------------------------------------------
Position Board::GetRealPos(const bool &isWhite, Position pos, const Mvt &mvt) const
{
    return(GetRealPos(isWhite, mvt.GetResultPos(pos)));
}
//------------------------------------------------------------------------------
Position Board::GetRealPos(const bool &isWhite, Position pos) const // On prend la Position du pion par rapport au joueur et on donne la Position rééle.
{
    const bool isWhiteOnBottom = false;
    bool isSeenFromTop = (isWhiteOnBottom && !isWhite) || (!isWhiteOnBottom && isWhite);

    if(isSeenFromTop)
    {
        // La gauche et la droite s'inversent...
        pos.x = GetBoardHSize() - 1 - pos.x;

        // On voit du haut... donc y ok
    }
    else
    {
        // La gauche et la droite sont ok.

        // Le bas et le haut s'inversent...
        pos.y = GetBoardVSize() - 1 - pos.y;
    }

    if(!IsCaseInGame(pos))
    {
        stringstream str;
        str << "Error : not in game !\nPosition : (" << pos.x << ";" << pos.y << ")";
        throw MyException(str.str());
    }

    return pos;
}
//------------------------------------------------------------------------------
bool Board::IsCaseInGameRelative(const bool &isWhite, const Position &pos) const
{
    try
    {
        GetRealPos(isWhite, pos);
    }
    catch(MyException &e)
    {
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool Board::IsCaseInGame(const Position &pos) const
{
    if(pos.x >= GetBoardHSize() || pos.x < 0 || pos.y >= GetBoardVSize() || pos.y < 0)
    {
        return false;
    }

    // Pos y paire
    if(pos.y % 2 == 0)
    {
        if(pos.x % 2 == 0) // Pos x paire
        {
            return true;
        }
        else // Pos x paire
        {
            return false;
        }
    }
    else // Pos y impare
    {
        if(pos.x % 2 == 0) // Pos x paire
        {
            return false;
        }
        else // Pos x paire
        {
            return true;
        }
    }

    // Erreur
    return false;
}
//------------------------------------------------------------------------------
void Board::SwapCases(dameCase &c1, dameCase &c2)
{
    dameCase tmp;
    tmp = c1;
    c1 = c2;
    c2 = tmp;
}
//------------------------------------------------------------------------------
vector<pair<Position, vector<Mvt> > > Board::GetAllPlayerPoosibilities(const bool &isWhite)
{
    vector<pair<Position, vector<Mvt> > > tbl;
    std::vector<Position> dameList = GetAllPlayerDames(isWhite);

    bool canEat = false;

    tbl.resize(dameList.size());
    for(int i = dameList.size() - 1; i >= 0; i--)
    {
        tbl[i].first = dameList[i];
        tbl[i].second = GetMvtPossibilitiesRelative(isWhite, tbl[i].first);

        if(tbl[i].second.empty())
        {
            tbl.erase(tbl.begin() + i);
            continue;
        }

        if(MUST_EAT)
        {
            if(tbl[i].second[0].IsEat() && !canEat) // Il faut manger et on ne le savait pas encore...
            {
                // On supprime tous ceux d'avant (qui sont après dans le tableau)...
                while(tbl.size() > i + 1)
                {
                    tbl.erase(tbl.begin() + i + 1);
                }

                // On indique qu'il faut manger...
                canEat = true;
            }
            else if(canEat && !tbl[i].second[0].IsEat()) // On peut manger mais ce pion là non ==> On supprime
            {
                tbl.erase(tbl.begin() + i);
            }
        }
    }

    return tbl;
}
//------------------------------------------------------------------------------
std::vector<Mvt> Board::GetMvtPossibilitiesRelative(const bool &isWhite, Position pos)
{
    std::vector<Mvt> tbl;

    const int directionList[] = {TopLeft, TopRight, BottomRight, BottomLeft};
    const int directionListCount = 4;

    bool canEat = false;

    dameCase dame = GetDameRelative(isWhite, pos);
    if(dame.isPiece)
    {
        if(dame.isW != isWhite)
        {
            throw MyException("Error : GetMvtPossibilitiesRelative() bad color !");
        }

        Mvt tmpMvt;
        int distLimite;
        if(dame.isQueen)
        {
            distLimite = GetBoardHSize() * GetBoardVSize(); // On va jusqu'au bord: la limite doit être plus grande que la diagonale.
        }
        else // Pion simple
        {
            distLimite = 2;
        }

        for(int i = 0; i < directionListCount; i++)
        {
            tmpMvt.SetDirection(directionList[i]);
            for(int dist = 0; dist <= distLimite; dist++)
            {
                tmpMvt.SetDistance(dist);

                // Dans le jeu ?
                try
                {
                    GetRealPos(isWhite, pos, tmpMvt);
                }
                catch(MyException &e)
                {
                    break;
                }

                tmpMvt = MovePiece(isWhite, pos, tmpMvt, false);
                if(tmpMvt.IsPossible())
                {
                    if(tmpMvt.IsEat())
                    {
                        canEat = true;
                    }

                    tbl.push_back(tmpMvt);
                }
            }
        }
    }

    // Si on peut manger, on enléve le reste...
    if(MUST_EAT && canEat)
    {
        for(int i = tbl.size() - 1; i >= 0; i--)
        {
            if(! tbl[i].IsEat())
            {
                tbl.erase(tbl.begin() + i);
            }
        }
    }

    return tbl;
}
//------------------------------------------------------------------------------
std::vector<Position> Board::GetAllPlayerDames(const bool &isWhite) const
{
    vector<Position> tbl;

    Position relativePos;
    relativePos.x = 1; // Tjr case tout en haut à gauche vide.
    relativePos.y = 0;
    while(relativePos.y < GetBoardVSize())
    {
        const dameCase &dame = GetDameRelative(isWhite, relativePos);
        if(dame.isPiece && dame.isW == isWhite)
        {
            tbl.push_back(relativePos);
        }

        relativePos.x += 2;
        if(relativePos.x >= GetBoardHSize())
        {
            relativePos.x-= GetBoardHSize();
            relativePos.y++;

            // Décallage
            if(relativePos.y % 2) // Lignes paires
            {
                relativePos.x--;
            }
            else
            {
                relativePos.x++;
            }

        }
    }

    return tbl;
}
//------------------------------------------------------------------------------
int Board::GetAllPlayerDamesCount(const bool &isWhite) const
{
    int nb = 0;

    Position relativePos;
    relativePos.x = 1; // Tjr case tout en haut à gauche vide.
    relativePos.y = 0;
    while(relativePos.y < GetBoardVSize())
    {
        const dameCase &dame = GetDameRelative(isWhite, relativePos);
        if(dame.isPiece && dame.isW == isWhite)
        {
            nb++;
        }

        relativePos.x += 2;
        if(relativePos.x >= GetBoardHSize())
        {
            relativePos.x-= GetBoardHSize();
            relativePos.y++;

            // Décallage
            if(relativePos.y % 2) // Lignes paires
            {
                relativePos.x--;
            }
            else
            {
                relativePos.x++;
            }

        }
    }

    return nb;
}
//------------------------------------------------------------------------------
int Board::GetBoardVSize() const
{
    if(m_board.empty())
    {
        return 0;
    }
    else
    {
        return m_board[0].size();
    }
}
//------------------------------------------------------------------------------
int Board::GetBoardHSize() const
{
    return m_board.size();
}
//------------------------------------------------------------------------------
Mvt Board::MovePiece(const bool &isWhite, const Position &pos, Mvt mvt, const bool &act)
{
    mvt.SetPossible(false);
    mvt.SetEat(false);

    // Vérif du tour...
    if(act && isWhite != m_turnColor)
    {
        throw MyException("Error : MovePiece: Bad turn !");
        return mvt;
    }

    // Verif
    dameCase &dame = GetDameRelative(isWhite, pos);
    dameCase &dameResult = GetDameRelative(isWhite, pos, mvt);
    if(!dame.isPiece)
    {
        throw MyException("Error : MovePiece: Bad color !");
        return mvt;
    }

    // Est ce qu'une piéce en particulier doit jouer ?
    if(IsCaseInGame(m_posContinue))
    {
        if(GetRealPos(isWhite, pos) != m_posContinue) // Pas la bonne piece
        {
            return mvt;
        }
        else if(m_lastMvt.GetReverse().GetDirection() == mvt.GetDirection() /* Pas le droit de revenir en arriére... */ ) // Bonne piece mais mauvaise direction
        {
            return mvt;
        }
    }

    if(dame.isQueen) // Cas d'un reine...
    {
        if(dameResult.isPiece) // Pas de dames là ou on veut aller...
        {
            return mvt;
        }

        // Régles non françaises ?
        if(m_rules.slowQueen && mvt.GetDistance() > 2)
        {
            return mvt;
        }

        for(int i = 1; i < mvt.GetDistance(); i++)
        {
            dameCase &dameMiddle = GetDameRelative(isWhite, pos, Mvt(mvt.GetDirection(), i));
            if(dameMiddle.isPiece)
            {
                if(dameMiddle.isW == isWhite) // Dame / piece de mon camp => Impossible
                {
                    return mvt;
                }
                else if(!mvt.IsEat()) // Pion énemie et on n'a toujours pas mangé
                {
                    mvt.SetEat(true);
                    mvt.SetEatPos(Mvt(mvt.GetDirection(), i).GetResultPos(pos));
                }
                else // Pion énemie et on a déjà mangé => On mange qu'une seule fois par tour.
                {
                    return mvt;
                }
            }
        }

        // Régles non françaises ?
        if(m_rules.slowQueen && !mvt.IsEat() && mvt.GetDistance() > 1)
        {
            return mvt;
        }

        if(act)
        {
            // On netoye
            for(int i = 1; i < mvt.GetDistance(); i++)
            {
                dameCase &dameMiddle = GetDameRelative(isWhite, pos, Mvt(mvt.GetDirection(), i));
                dameMiddle.isPiece = false;
            }

            SwapCases(dame, dameResult); // On met la dame sur sa case but
        }
    }
    else // Pion simple
    {
        if(mvt.GetDistance() == 1) // On bouge juste d'une case...
        {
            if(dameResult.isPiece == false && mvt.IsTop())
            {
                if(act)
                {
                    SwapCases(dame, dameResult);
                }
            }
            else
            {
                return mvt;
            }
        }
        else if(mvt.GetDistance() == 2) // On prend un pion
        {
            dameCase &dameMiddle = GetDameRelative(isWhite, pos, Mvt(mvt.GetDirection(), mvt.GetDistance() - 1));

            if(!m_rules.pieceCanEatBack && mvt.IsBottom())
            {
                return mvt;
            }

            if(dameMiddle.isPiece && dameMiddle.isW != isWhite && !dameResult.isPiece)
            {
                if(act)
                {
                    dameMiddle.isPiece = false;
                    SwapCases(dame, dameResult);
                }

                mvt.SetEat(true);
                mvt.SetEatPos(Mvt(mvt.GetDirection(), mvt.GetDistance() - 1).GetResultPos(pos));
            }
            else
            {
                return mvt;
            }
        }
        else
        {
            return mvt;
        }
    }

    // Si on est arrivé jusque là, me mouvement est possible...
    mvt.SetPossible(true);

    if(act) // Changement de tour + dame...
    {
        // Peut-on continuer à prendre ?
        bool isMultiTurn = false;
        if(MUST_EAT && mvt.IsEat())
        {
            m_posContinue = GetRealPos(isWhite, pos, mvt);
            m_lastMvt = mvt;

            vector<Mvt> possiblilities = GetMvtPossibilitiesRelative(isWhite, mvt.GetResultPos(pos));
            if(!possiblilities.empty() && possiblilities[0].IsEat()) // Si le pion déplacé peut coninuer à prendre...
            {
                isMultiTurn = true;
            }
        }

        if(!dame.isQueen && !isMultiTurn && mvt.GetResultPos(pos).y == GetBoardVSize() - 1) // Dame (pas le droit de faire une dame si l'on "passe" seulement sur la case
        {
            dameResult.isQueen = true;
        }

        if(isMultiTurn == false)
        {
            m_posContinue = Position(-1,-1);
            m_turnColor = !m_turnColor;
        }
    }

    return mvt;
}

//------------------------------------------------------------------------------
