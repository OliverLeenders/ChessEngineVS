#ifndef Piece_H
#define Piece_H

class Piece
{
private:
    /* data */
    unsigned type : 4;
    bool moved;
    /**
     * no Piece:        0
     * king white:      1
     * king black:      2
     * queen white:     3
     * queen black:     4
     * rook white:      5
     * rook black:      6
     * bishop white:    7
     * bishop black:    8
     * knight white:    9
     * knight black:    10
     * pawn white:      11
     * pawn black:      12
     */
public:
    Piece(unsigned set_type);
    char piece_to_char();
    int set_piece_type(unsigned t, bool set_moved);
    unsigned get_type();
    bool is_white();
    bool is_empty();
    bool has_already_moved();
    void set_already_moved();
    bool is_black();
    Piece* clone();
    double value();
    bool is_opponent(Piece* other);
    bool is_team_member(Piece* other);
    ~Piece();
};

#endif