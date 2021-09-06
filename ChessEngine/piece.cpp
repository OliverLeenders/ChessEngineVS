#include "Piece.h"

Piece::Piece(unsigned set_type)
{
    this->type = set_type;
}

int Piece::set_piece_type(unsigned t, bool set_moved)
{
    this->type = t;
    this->moved = set_moved;
    return t;
}

unsigned Piece::get_type()
{
    return this->type;
}

bool Piece::is_white()
{
    return (this->type % 2 == 1);
}

bool Piece::is_black()
{
    return (!this->is_white()) && (this->type > 0);
}

bool Piece::is_empty()
{
    return this->get_type() == 0;
}

Piece* Piece::clone()
{
    Piece* p = new Piece(this->type);
    p->moved = this->moved;
    return p;
}

bool Piece::has_already_moved()
{
    return this->moved;
}

char Piece::piece_to_char()
{
    switch (this->type)
    {
    case 0:
        return ' ';
    case 1:
        return 'K';
    case 2:
        return 'k';
    case 3:
        return 'Q';
    case 4:
        return 'q';
    case 5:
        return 'R';
    case 6:
        return 'r';
    case 7:
        return 'B';
    case 8:
        return 'b';
    case 9:
        return 'N';
    case 10:
        return 'n';
    case 11:
        return 'P';
    case 12:
        return 'p';
    default:
        break;
    }
    return 'E';
}

bool Piece::is_opponent(Piece* other)
{
    return ((other->is_black() && this->is_white()) || (other->is_white() && this->is_black()));
}

bool Piece::is_team_member(Piece* other)
{
    return ((other->is_black() && this->is_black()) || (other->is_white() && this->is_white()));
}

double Piece::value() {
    switch (this->type)
    {
    case 0:
        return 0;
    case 1:
        return 1000;
    case 2:
        return 1000;
    case 3:
        return 9.5;
    case 4:
        return 9.5;
    case 5:
        return 5.0;
    case 6:
        return 5.0;
    case 7:
        return 3.5;
    case 8:
        return 3.5;
    case 9:
        return 3.2;
    case 10:
        return 3.2;
    case 11:
        return 1.0;
    case 12:
        return 1.0;
    default:
        break;
    }
    return 0;
}

Piece::~Piece()
{
}
