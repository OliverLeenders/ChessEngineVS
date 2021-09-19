#include "board.h"
#include <iostream>

/**
 * @brief Construct a new Board:: Board object -- standard constructor
 *
 */
Board::Board()
{
    for (int i = 0; i < 64; i++)
    {
        this->position[i] = new Piece(0);
    }
    this->white_to_move = true;
    this->last_move_origin = -1;
    this->last_move_target = -1;
    for (int i = 0; i < 4; i++)
    {
        this->castling_rights[i] = true;
    }
    for (int i = 0; i < 64; i++)
    {
        this->pins[i] = 0;
        this->checks[i] = false;
    }
}
/**
 * @brief Construct a new Board:: Board object from a given position
 *
 * @param set_pos position to copy
 * @param who_to_move which side to move
 */
Board::Board(Piece* set_pos[], bool who_to_move, bool* set_castling_rights)
{
    for (int i = 0; i < 64; i++)
    {
        this->position[i] = set_pos[i];
    }
    this->white_to_move = who_to_move;
    for (int i = 0; i < 4; i++)
    {
        this->castling_rights[i] = set_castling_rights[i];
    }
    this->last_move_origin = -1;
    this->last_move_target = -1;
    delete[] set_pos;
    delete[] set_castling_rights;
    for (int i = 0; i < 64; i++)
    {
        this->pins[i] = 0;
        this->checks[i] = false;
    }
}

/**
 * @brief Construct a new Board:: Board object according to specified fen
 *
 * @param fen fen-string
 */
Board::Board(std::string fen)
{
    for (int i = 0; i < 64; i++)
    {
        this->position[i] = new Piece(0);
    }
    int pos_index = 56;
    long unsigned int i = 0;
    for (; i < fen.size(); i++)
    {
        char c = fen[i];
        switch (c)
        {
        case '/':
            pos_index -= pos_index % 8;
            pos_index -= 8;
            break;
        case 'K':
            this->position[pos_index]->set_piece_type(1, false);
            this->white_king_pos = pos_index;
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'k':
            this->position[pos_index]->set_piece_type(2, false);
            this->black_king_pos = pos_index;
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'Q':
            this->position[pos_index]->set_piece_type(3, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'q':
            this->position[pos_index]->set_piece_type(4, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'R':
            this->position[pos_index]->set_piece_type(5, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'r':
            this->position[pos_index]->set_piece_type(6, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'B':
            this->position[pos_index]->set_piece_type(7, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'b':
            this->position[pos_index]->set_piece_type(8, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'N':
            this->position[pos_index]->set_piece_type(9, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'n':
            this->position[pos_index]->set_piece_type(10, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'P':
            this->position[pos_index]->set_piece_type(11, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case 'p':
            this->position[pos_index]->set_piece_type(12, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case '1':
            this->position[pos_index]->set_piece_type(0, false);
            if (pos_index % 8 != 7)
            {
                pos_index++;
            }
            break;
        case '2':
            for (int j = 0; j < 2; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '3':
            for (int j = 0; j < 3; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '4':
            for (int j = 0; j < 4; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '5':
            for (int j = 0; j < 5; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '6':
            for (int j = 0; j < 6; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '7':
            for (int j = 0; j < 7; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case '8':
            for (int j = 0; j < 8; j++)
            {
                this->position[pos_index]->set_piece_type(0, false);
                if (pos_index % 8 != 7)
                {
                    pos_index++;
                }
            }
            break;
        case ' ':
            i++;
            goto rest;
        default:
            break;
        }
    }
rest:
    if (fen[i] == 'w')
    {
        this->white_to_move = true;
    }
    else
    {
        this->white_to_move = false;
    }
    i += 2;
    for (int j = 0; j < 4; j++)
    {
        this->castling_rights[j] = false;
    }
    // some side has castling rights
    if (fen[i] != '-')
    {
        while (fen[i] != ' ')
        {
            if (fen[i] == 'K')
            {
                this->castling_rights[0] = true;
            }
            else if (fen[i] == 'Q')
            {
                this->castling_rights[1] = true;
            }
            else if (fen[i] == 'k')
            {
                this->castling_rights[2] = true;
            }
            else if (fen[i] == 'q')
            {
                this->castling_rights[3] = true;
            }
            i++;
        }
    }
    i++;
    if (fen[i] != '-')
    {
        // get en passant target square
        int col = (int)fen[i] - 97;
        int row = (fen[i + 1] - '0') - 1;
        this->en_passant_target_index = row * 8 + col;
    }
    else
    {
        this->en_passant_target_index = -1;
    }
    this->last_move_origin = -1;
    this->last_move_target = -1;
    //std::cout << fen[i] << std::endl;
    /*
    for (int j = 0; j < 4; j++)
    {
        std::cout << this->castling_rights[j];
    }
    std::cout << std::endl;
    std::cout << "en passant target: " + std::to_string(this->en_passant_target_index) << std::endl;
    */
    for (int i = 0; i < 64; i++)
    {
        this->pins[i] = 0;
        this->checks[i] = false;
    }
    compute_attacked_squares();
    compute_pin_rays();
    compute_other_checks();
}

/**
 * @brief set a position at an index to a specific piece
 *
 * @param type
 * @param pos
 * @return int
 */
int Board::set_piece(unsigned type, int pos, bool set_moved)
{
    //std::cout << "type: " + std::to_string(type) + ", pos: " + std::to_string(pos) + ", moved: " + std::to_string(set_moved) << std::endl;
    this->position[pos]->set_piece_type(type, set_moved);
    return 0;
}
/**
 * @brief Destroy the Board:: Board object and release memory
 *
 */
Board::~Board()
{
    for (int i = 0; i < 64; i++)
    {
        delete this->position[i];
    }
    delete[] this->attacked_by_black;
    delete[] this->attacked_by_white;
    delete[] this->castling_rights;
    delete[] this->check_direction;
    delete[] this->pins;
    delete[] this->checks;
    delete[] this->position;
}

void Board::compute_attacked_squares()
{
    for (int i = 0; i < 64; i++)
    {
        this->attacked_by_white[i] = false;
        this->attacked_by_black[i] = false;
    }
    for (int i = 0; i < 64; i++)
    {
        Piece* p = this->position[i];
        if (!p->is_empty())
        {
            if (p->get_type() == 1) // white king
            {

                if (i % 8 < 7)
                {
                    this->attacked_by_white[i + 1] = true;
                }
                if (i % 8 > 0)
                {
                    this->attacked_by_white[i - 1] = true;
                }
                if (i >= 8)
                {
                    this->attacked_by_white[i - 8] = true;
                }
                if (i < 56)
                {
                    this->attacked_by_white[i + 8] = true;
                }
                if (i % 8 < 7 && i >= 8)
                {
                    this->attacked_by_white[i - 7] = true;
                }
                if (i % 8 > 0 && i >= 8)
                {
                    this->attacked_by_white[i - 9] = true;
                }
                if (i % 8 < 7 && i < 56)
                {
                    this->attacked_by_white[i + 9] = true;
                }
                if (i % 8 > 0 && i < 56)
                {
                    this->attacked_by_white[i + 7] = true;
                }
            }
            else if (p->get_type() == 2) // black king
            {
                if (i % 8 < 7)
                {
                    this->attacked_by_black[i + 1] = true;
                }
                if (i % 8 > 0)
                {
                    this->attacked_by_black[i - 1] = true;
                }
                if (i >= 8)
                {
                    this->attacked_by_black[i - 8] = true;
                }
                if (i < 56)
                {
                    this->attacked_by_black[i + 8] = true;
                }
                if (i % 8 < 7 && i >= 8)
                {
                    this->attacked_by_black[i - 7] = true;
                }
                if (i % 8 > 0 && i >= 8)
                {
                    this->attacked_by_black[i - 9] = true;
                }
                if (i % 8 < 7 && i < 56)
                {
                    this->attacked_by_black[i + 9] = true;
                }
                if (i % 8 > 0 && i < 56)
                {
                    this->attacked_by_black[i + 7] = true;
                }
            }
            else if (p->get_type() == 3) // white queen
            {
                add_diagonal_attack_rays(i, true);
                add_straight_attack_rays(i, true);
            }
            else if (p->get_type() == 4) // black queen
            {
                add_diagonal_attack_rays(i, false);
                add_straight_attack_rays(i, false);
            }
            else if (p->get_type() == 5) // white rook
            {
                add_straight_attack_rays(i, true);
            }
            else if (p->get_type() == 6) // black rook
            {
                add_straight_attack_rays(i, false);
            }
            else if (p->get_type() == 7) // white bishop
            {
                add_diagonal_attack_rays(i, true);
            }
            else if (p->get_type() == 8) // black bishop
            {
                add_diagonal_attack_rays(i, false);
            }
            else if (p->get_type() == 9) // white knight
            {
                if (i % 8 < 7 && i < 48)
                {
                    this->attacked_by_white[i + 17] = true;
                }
                if (i % 8 > 0 && i < 48)
                {
                    this->attacked_by_white[i + 15] = true;
                }
                if (i % 8 < 6 && i < 56)
                {
                    this->attacked_by_white[i + 10] = true;
                }
                if (i % 8 > 1 && i < 56)
                {
                    this->attacked_by_white[i + 6] = true;
                }
                if (i % 8 > 0 && i >= 16)
                {
                    this->attacked_by_white[i - 17] = true;
                }
                if (i % 8 < 7 && i >= 16)
                {
                    this->attacked_by_white[i - 15] = true;
                }
                if (i % 8 < 6 && i >= 8)
                {
                    this->attacked_by_white[i - 6] = true;
                }
                if (i % 8 > 1 && i >= 8)
                {
                    this->attacked_by_white[i - 10] = true;
                }
            }
            else if (p->get_type() == 10) // black knight
            {
                if (i % 8 < 7 && i < 48)
                {
                    this->attacked_by_black[i + 17] = true;
                }
                if (i % 8 > 0 && i < 48)
                {
                    this->attacked_by_black[i + 15] = true;
                }
                if (i % 8 < 6 && i < 56)
                {
                    this->attacked_by_black[i + 10] = true;
                }
                if (i % 8 > 1 && i < 56)
                {
                    this->attacked_by_black[i + 6] = true;
                }
                if (i % 8 > 0 && i >= 16)
                {
                    this->attacked_by_black[i - 17] = true;
                }
                if (i % 8 < 7 && i >= 16)
                {
                    this->attacked_by_black[i - 15] = true;
                }
                if (i % 8 < 6 && i >= 8)
                {
                    this->attacked_by_black[i - 6] = true;
                }
                if (i % 8 > 1 && i >= 8)
                {
                    this->attacked_by_black[i - 10] = true;
                }
            }
            else if (p->get_type() == 11) // white pawn
            {
                if (i % 8 > 0 && i < 56)
                {
                    this->attacked_by_white[i + 7] = true;
                }
                if (i % 8 < 7 && i < 56)
                {
                    this->attacked_by_white[i + 9] = true;
                }
            }
            else if (p->get_type() == 12) // black pawn
            {
                if (i % 8 > 0 && i >= 8)
                {
                    this->attacked_by_black[i - 9] = true;
                }
                if (i % 8 < 7 && i >= 8)
                {
                    this->attacked_by_black[i - 7] = true;
                }
            }
        }
    }
    return;
}

void Board::add_straight_attack_rays(int i, bool is_white)
{
    int j = i;
    while (j < 56)
    {
        j += 8;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j >= 8)
    {
        j -= 8;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j % 8 < 7)
    {
        j++;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j % 8 > 0)
    {
        j--;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
}

void Board::add_diagonal_attack_rays(int i, bool is_white)
{
    int j = i;
    while (j % 8 < 7 && j < 56) // diagonally right up
    {
        j += 9;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j % 8 > 0 && j < 56) // diagonally left up
    {
        j += 7;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j % 8 < 7 && j >= 8)
    {
        j -= 7;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
    j = i;
    while (j % 8 > 0 && j >= 8)
    {
        j -= 9;
        if (!this->position[j]->is_empty())
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
            break;
        }
        else
        {
            if (is_white)
                this->attacked_by_white[j] = true;
            else
                this->attacked_by_black[j] = true;
        }
    }
}

/**
 * @brief generates a list of possible moves
 *
 * @return std::list<Board *>*
 */
std::list<Board*>* Board::possible_moves()
{
    std::list<Board*>* moves = new std::list<Board*>();
    this->check_direction[0] = 0;
    this->check_direction[1] = 0;
    compute_attacked_squares();
    compute_pin_rays();
    compute_other_checks();
    if (this->num_checks > 1)
    {
        if (this->white_to_move)
        {
            add_king_moves(moves, this->white_king_pos);
        }
        else
        {
            add_king_moves(moves, this->black_king_pos);
        }
        return moves;
    }
    for (int i = 0; i < 64; i++)
    {
        if ((this->position[i]->is_white() == 1 && white_to_move) || (this->position[i]->is_black() && !white_to_move))
        {
            switch (this->position[i]->get_type())
            {
            case 1: // white king
                add_king_moves(moves, i);
                break;
            case 2: // black king
                add_king_moves(moves, i);
                break;
            case 3: // white queen
                add_queen_moves(moves, i);
                break;
            case 4: // black queen
                add_queen_moves(moves, i);
                break;
            case 5: // white
                add_rook_moves(moves, i);
                break;
            case 6:
                add_rook_moves(moves, i);
                break;
            case 7:
                add_bishop_moves(moves, i);
                break;
            case 8:
                add_bishop_moves(moves, i);
                break;
            case 9:
                add_knight_moves(moves, i);
                break;
            case 10:
                add_knight_moves(moves, i);
                break;
            case 11:
                add_pawn_moves(moves, i);
                break;
            case 12:
                add_pawn_moves(moves, i);
                break;
            default:
                break;
            }
        }
    }

    return moves;
}

void Board::compute_other_checks()
{
    if (this->white_to_move)
    {
        int king_pos = this->white_king_pos;
        // knight checks
        if (king_pos % 8 < 6 && king_pos < 56 && this->position[king_pos + 10]->get_type() == 10)
        {
            this->checks[king_pos + 10] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 1 && king_pos < 56 && this->position[king_pos + 6]->get_type() == 10)
        {
            this->checks[king_pos + 6] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 6 && king_pos >= 8 && this->position[king_pos - 6]->get_type() == 10)
        {
            this->checks[king_pos - 6] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 1 && king_pos >= 8 && this->position[king_pos - 10]->get_type() == 10)
        {
            this->checks[king_pos - 10] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 17]->get_type() == 10)
        {
            this->checks[king_pos + 17] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 15]->get_type() == 10)
        {
            this->checks[king_pos + 15] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 7 && king_pos >= 16 && this->position[king_pos - 15]->get_type() == 10)
        {
            this->checks[king_pos - 15] = true;
            this->num_checks++;
        }

        if (king_pos % 8 > 0 && king_pos >= 16 && this->position[king_pos - 17]->get_type() == 10)
        {
            this->checks[king_pos - 17] = true;
            this->num_checks++;
        }

        if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 7]->get_type() == 12)
        {
            this->checks[king_pos + 7] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 9]->get_type() == 12)
        {
            this->checks[king_pos + 9] = true;
            this->num_checks++;
        }
    }
    else
    {
        int king_pos = this->black_king_pos;
        if (king_pos % 8 < 6 && king_pos < 56 && this->position[king_pos + 10]->get_type() == 9)
        {
            this->checks[king_pos + 10] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 1 && king_pos < 56 && this->position[king_pos + 6]->get_type() == 9)
        {
            this->checks[king_pos + 6] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 6 && king_pos >= 8 && this->position[king_pos - 6]->get_type() == 9)
        {
            this->checks[king_pos - 6] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 1 && king_pos >= 8 && this->position[king_pos - 10]->get_type() == 9)
        {
            this->checks[king_pos - 10] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 17]->get_type() == 9)
        {
            this->checks[king_pos + 17] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 15]->get_type() == 9)
        {
            this->checks[king_pos + 15] = true;
            this->num_checks++;
        }
        if (king_pos % 8 < 7 && king_pos >= 16 && this->position[king_pos - 15]->get_type() == 9)
        {
            this->checks[king_pos - 15] = true;
            this->num_checks++;
        }

        if (king_pos % 8 > 0 && king_pos >= 16 && this->position[king_pos - 17]->get_type() == 9)
        {
            this->checks[king_pos - 17] = true;
            this->num_checks++;
        }

        if (king_pos % 8 < 7 && king_pos >= 8 && this->position[king_pos - 7]->get_type() == 11)
        {
            this->checks[king_pos - 7] = true;
            this->num_checks++;
        }
        if (king_pos % 8 > 0 && king_pos > 8 && this->position[king_pos - 9]->get_type() == 11)
        {
            this->checks[king_pos - 9] = true;
            this->num_checks++;
        }
    }
    // add pawn checks
}

std::list<Board*>* Board::get_legal_moves()
{
    std::list<Board*>* moves = this->possible_moves();
    //filter_illegal_moves(moves);
    //moves->sort(compare);
    
    return moves;
}
/*
std::list<Board*>* Board::get_sorted_legal_moves(Evaluator *e) {
    std::list<Board*>* moves = this->possible_moves();
    moves->sort(e->compare);
    return moves;
}
*/
void Board::compute_pin_rays()
{
    num_checks = 0;
    if (this->white_to_move)
    {
        int king_pos = this->white_king_pos;

        // horizontal left -- 1
        bool comrad_on_ray = false;
        bool exists_pin = false;
        int pin_index = -1;
        bool exists_check = false;
        for (int i = king_pos - 1; i >= 0 && i % 8 != 7; i--)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 1;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 1; i++)
            {
                this->pins[i] = 1;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 1; i++)
            {
                this->checks[i] = true;
            }
        }
        // horizontal right -- 2
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 1; i % 8 != 0; i++)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 2;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i > 0 && this->position[i]->get_type() != 1; i--)
            {
                this->pins[i] = 2;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i > 0 && this->position[i]->get_type() != 1; i--)
            {
                this->checks[i] = true;
            }
        }
        // vertical down -- 3
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 8; i >= 0; i -= 8)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 3;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i < 64 && this->position[i]->get_type() != 1; i += 8)
            {
                this->pins[i] = 3;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i < 64 && this->position[i]->get_type() != 1; i += 8)
            {
                this->checks[i] = true;
            }
        }
        // vertical up -- 4
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 8; i < 64; i += 8)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 4;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 1; i -= 8)
            {
                this->pins[i] = 4;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 1; i -= 8)
            {
                this->checks[i] = true;
            }
        }
        // diagonal SW -- 5
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 9; i % 8 != 7 && i > 0; i -= 9)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 5;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 1; i += 9)
            {
                this->pins[i] = 5;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 1; i += 9)
            {
                this->checks[i] = true;
            }
        }
        // diagonal NW -- 6
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 7; i % 8 != 7 && i < 64; i += 7)
        {

            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 6;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 1; i -= 7)
            {
                this->pins[i] = 6;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 1; i -= 7)
            {
                this->checks[i] = true;
            }
        }
        // diagonal NE -- 7
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 9; i % 8 != 0 && i < 64; i += 9)
        {
            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 7;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 1; i -= 9)
            {
                this->pins[i] = 7;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 1; i -= 9)
            {
                this->checks[i] = true;
            }
        }
        // diagonal SE -- 8
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 7; i % 8 != 0 && i > 0; i -= 7)
        {
            if (comrad_on_ray && this->position[i]->is_white())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_black())
            {
                if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
                {
                    exists_check = true;
                    num_checks++;
                    pin_index = i;
                    unsigned int set_dir = 8;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    break;
                }
                break;
            }
            else if (this->position[i]->is_white())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 1; i += 7)
            {
                this->pins[i] = 8;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 1; i += 7)
            {
                this->checks[i] = true;
            }
        }
    }
    else
    {
        int king_pos = this->black_king_pos;

        bool comrad_on_ray = false;
        bool exists_pin = false;
        int pin_index = -1;
        bool exists_check = false;

        for (int i = king_pos - 1; i > 0 && i % 8 != 7; i--)
        {

            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_check = true;
                    unsigned int set_dir = 1;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 2; i++)
            {
                this->pins[i] = 1;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 2; i++)
            {
                this->checks[i] = true;
            }
        }
        // horizontal right -- 2
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 1; i % 8 != 0; i++)
        {

            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_check = true;
                    unsigned int set_dir = 2;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i > 0 && this->position[i]->get_type() != 2; i--)
            {
                this->pins[i] = 2;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i > 0 && this->position[i]->get_type() != 2; i--)
            {
                this->checks[i] = true;
            }
        }
        // vertical down -- 3
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 8; i >= 0; i -= 8)
        {

            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_check = true;
                    unsigned int set_dir = 3;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i < 64 && this->position[i]->get_type() != 2; i += 8)
            {
                this->pins[i] = 3;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i < 64 && this->position[i]->get_type() != 2; i += 8)
            {
                this->checks[i] = true;
            }
        }
        // vertical up -- 4
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 8; i < 64; i += 8)
        {

            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
                {
                    exists_check = true;
                    unsigned int set_dir = 4;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 2; i -= 8)
            {
                this->pins[i] = 4;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 2; i -= 8)
            {
                this->checks[i] = true;
            }
        }
        // diagonal SW -- 5
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 9; i % 8 != 7 && i > 0; i -= 9)
        {

            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_check = true;
                    unsigned int set_dir = 5;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 2; i += 9)
            {
                this->pins[i] = 5;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 2; i += 9)
            {
                this->checks[i] = true;
            }
        }
        // diagonal NW -- 6
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 7; i % 8 != 7 && i < 64; i += 7)
        {
            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_check = true;
                    unsigned int set_dir = 6;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 2; i -= 7)
            {
                this->pins[i] = 6;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 2; i -= 7)
            {
                this->checks[i] = true;
            }
        }
        // diagonal NE -- 7
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos + 9; i % 8 != 0 && i < 64; i += 9)
        {
            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_check = true;
                    unsigned int set_dir = 7;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 2; i -= 9)
            {
                this->pins[i] = 7;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 2; i -= 9)
            {
                this->checks[i] = true;
            }
        }
        // diagonal SE -- 8
        comrad_on_ray = false;
        exists_pin = false;
        exists_check = false;
        pin_index = -1;
        for (int i = king_pos - 7; i % 8 != 0 && i > 0; i -= 7)
        {
            if (comrad_on_ray && this->position[i]->is_black())
            {
                break;
            }
            else if (comrad_on_ray && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_pin = true;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if ((!comrad_on_ray) && this->position[i]->is_white())
            {
                if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
                {
                    exists_check = true;
                    unsigned int set_dir = 8;
                    if (check_direction[0] != 0)
                    {
                        check_direction[1] = set_dir;
                    }
                    else
                    {
                        check_direction[0] = set_dir;
                    }
                    num_checks++;
                    pin_index = i;
                    break;
                }
                break;
            }
            else if (this->position[i]->is_black())
            {
                comrad_on_ray = true;
            }
        }
        if (exists_pin)
        {
            for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 2; i += 7)
            {
                this->pins[i] = 8;
            }
        }
        else if (exists_check)
        {
            for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 2; i += 7)
            {
                this->checks[i] = true;
            }
        }
    }
}

bool Board::is_capture(Board* b)
{
    if (b->prev_pos->position[b->last_move_target]->is_empty()) {
        delete b;
        return true;
    }
    return false;
}

std::list<Board*>* Board::get_legal_captures()
{
    std::list<Board*>* moves = this->get_legal_moves();
    moves->remove_if(is_capture);
    return moves;
}
/*
std::list<Board*>* Board::get_sorted_legal_captures(Evaluator *e)
{
    std::list<Board*>* moves = this->get_sorted_legal_moves(e);
    moves->remove_if(is_capture);
    return moves;
}
*/
void Board::filter_illegal_moves(std::list<Board*>* moves)
{
    // std::cout << "here  " + std::to_string(moves->size()) << std::endl;
    std::list<Board*>::iterator itr = moves->begin();
    while (itr != moves->end())
    {
        Board* b = *itr;
        //std::cout << b->get_last_move() << std::endl;
        std::list<Board*>* p_moves = b->possible_moves();
        bool contains_king_capture = false;
        for (Board* const& c : *p_moves)
        {
            if (!c->contains_both_kings(c))
            {
                contains_king_capture = true;
                // std::cout << c->pos_as_str() << std::endl;
                //std::cout << std::to_string(c->white_king_pos) + " " + std::to_string(c->black_king_pos) << std::endl;
            }
        }
        for (Board* const& c : *p_moves)
        {
            delete c;
        }
        delete p_moves;
        if (contains_king_capture)
        {
            //std::cout << "here" << std::endl;
            itr = moves->erase(itr);
            delete b;
        }
        else
        {
            itr++;
        }
    }
}

bool Board::contains_both_kings(Board* b)
{
    if (b->position[b->white_king_pos]->get_type() == 1 && b->position[b->black_king_pos]->get_type() == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief generates pawn moves for a piece at a specific position
 *
 * @param moves list of moves to append to
 * @param i index of piece to move in position array
 */
void Board::add_pawn_moves(std::list<Board*>* moves, int i)
{
    if (this->position[i]->is_white()) // white pawn moves
    {
        if (i < 48 && (!this->position[i + 8]->is_black()) && (!this->position[i + 8]->is_white()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 8]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i + 8);
        }
        if (i < 16 && (!this->position[i]->has_already_moved()) && (this->position[i + 16]->is_empty()) && (this->position[i + 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 16]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i + 16);
            moves->back()->en_passant_target_index = i + 8;
        }
        // captures
        //std::cout << this->pins[i] << std::endl;
        if (i % 8 > 0 && i < 48 && this->position[i + 7]->is_black() && (this->pins[i] == 0 || this->pins[i] == 8 || this->pins[i] == 6) && ((this->num_checks == 1 && this->checks[i + 7]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i + 7);
        }

        if (i % 8 < 7 && i < 47 && this->position[i + 9]->is_black() && ((this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)) && ((this->num_checks == 1 && this->checks[i + 9]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i + 9);
        }

        // promotions
        // basic promotion
        if (i >= 48 && i < 56 && (this->position[i + 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 8]) || this->num_checks == 0))
        {
            promote_with_offset(moves, i, i + 8, 3);
            promote_with_offset(moves, i, i + 8, 5);
            promote_with_offset(moves, i, i + 8, 7);
            promote_with_offset(moves, i, i + 8, 9);
        }
        // capture promotions
        if (i > 48 && i < 56 && i % 8 != 0 && (this->position[i + 7]->is_black()) && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && this->checks[i + 7]) || this->num_checks == 0))
        {
            promote_with_offset(moves, i, i + 7, 3);
            promote_with_offset(moves, i, i + 7, 5);
            promote_with_offset(moves, i, i + 7, 7);
            promote_with_offset(moves, i, i + 7, 9);
        }
        if (i >= 48 && i < 55 && i % 8 != 7 && (this->position[i + 9]->is_black()) && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i + 9]) || this->num_checks == 0))
        {
            promote_with_offset(moves, i, i + 9, 3);
            promote_with_offset(moves, i, i + 9, 5);
            promote_with_offset(moves, i, i + 9, 7);
            promote_with_offset(moves, i, i + 9, 9);
        }

        // en-passant to the left
        if (i > 32 && i < 40 && en_passant_target_index == i + 7 && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && (this->checks[i + 7] || this->checks[this->en_passant_target_index - 8])) || this->num_checks == 0))
        {
            move_en_passant(moves, i, i + 7);
        }
        // en-passant to the right
        if (i >= 32 && i < 39 && en_passant_target_index == i + 9 && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && (this->checks[i + 9] || this->checks[this->en_passant_target_index - 8])) || this->num_checks == 0))
        {
            move_en_passant(moves, i, i + 9);
        }
    }
    else // black pawn moves
    {
        if (i > 15 && (this->position[i - 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 8]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i - 8);
        }
        if (i >= 48 && (!this->position[i]->has_already_moved()) && (this->position[i - 16]->is_empty()) && (this->position[i - 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 16]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i - 16);
            moves->back()->en_passant_target_index = i - 8;
        }

        // captures
        if (i % 8 > 0 && i > 16 && this->position[i - 9]->is_white() && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i - 9]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i - 9);
        }

        if (i % 8 < 7 && i >= 16 && this->position[i - 7]->is_white() && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && this->checks[i - 7]) || this->num_checks == 0))
        {
            move_with_offset(moves, i, i - 7);
        }
        // promotions
        // basic promotion
        if (i < 16 && i >= 8 && this->position[i - 8]->is_empty() && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 8]) || this->num_checks == 0))
        {
            //std::cout << i - 8 << std::endl;
            promote_with_offset(moves, i, i - 8, 4);
            promote_with_offset(moves, i, i - 8, 6);
            promote_with_offset(moves, i, i - 8, 8);
            promote_with_offset(moves, i, i - 8, 10);
        }
        // capture promotions
        if (i < 15 && i >= 8 && this->position[i - 7]->is_white() && (this->pins[i] == 0 || this->pins[i] == 8 || this->pins[i] == 6) && ((this->num_checks == 1 && this->checks[i - 7]) || this->num_checks == 0))
        {
            promote_with_offset(moves, i, i - 7, 4);
            promote_with_offset(moves, i, i - 7, 6);
            promote_with_offset(moves, i, i - 7, 8);
            promote_with_offset(moves, i, i - 7, 10);
        }
        if (i < 16 && i > 8 && this->position[i - 9]->is_white() && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i - 9]) || this->num_checks == 0))
        {
            promote_with_offset(moves, i, i - 9, 4);
            promote_with_offset(moves, i, i - 9, 6);
            promote_with_offset(moves, i, i - 9, 8);
            promote_with_offset(moves, i, i - 9, 10);
        }

        // en-passant to the left
        if (i > 24 && i < 32 && en_passant_target_index == i - 9 && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && (this->checks[i - 9] || this->checks[this->en_passant_target_index + 8])) || this->num_checks == 0))
        {
            // std::cout << "here" << std::endl;
            move_en_passant(moves, i, i - 9);
        }
        // en-passant to the right
        if (i >= 24 && i < 31 && en_passant_target_index == i - 7 && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && (this->checks[i - 7] || this->checks[this->en_passant_target_index + 8])) || this->num_checks == 0))
        {
            move_en_passant(moves, i, i - 7);
        }
    }
}

bool Board::en_passant_illegal()
{
    if (this->white_to_move)
    {
        if (this->white_king_pos / 8 != 4)
        {
            return false;
        }
        else if (this->white_king_pos % 8 < this->en_passant_target_index % 8)
        {
            // king to the left ef ep target square
            int num_pieces_passed = 0;
            int i = this->white_king_pos + 1;
            for (; num_pieces_passed < 3 && i % 8 != 0; i++)
            {
                if (!this->position[i]->is_empty())
                {
                    num_pieces_passed++;
                }
                if (num_pieces_passed == 3)
                {
                    break;
                }
            }
            if (i % 8 > this->en_passant_target_index % 8 && i % 8 != 0 && (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            int num_pieces_passed = 0;
            int i = this->white_king_pos - 1;
            for (; num_pieces_passed < 3 && i % 8 != 7; i--)
            {
                if (!this->position[i]->is_empty())
                {
                    num_pieces_passed++;
                }

                if (num_pieces_passed == 3)
                {
                    break;
                }
            }
            if (i % 8 < this->en_passant_target_index % 8 && i % 8 != 7 && (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        if (this->black_king_pos / 8 != 3)
        {
            return false;
        }
        else if (this->black_king_pos % 8 < this->en_passant_target_index % 8)
        {
            // king to the left ef ep target square
            int num_pieces_passed = 0;
            int i = this->black_king_pos + 1;
            for (; num_pieces_passed < 3 && i % 8 != 0; i++)
            {
                if (!this->position[i]->is_empty())
                {
                    num_pieces_passed++;
                }

                if (num_pieces_passed == 3)
                {
                    break;
                }
            }
            if (i % 8 > this->en_passant_target_index % 8 && i % 8 != 0 && (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            int num_pieces_passed = 0;
            int i = this->black_king_pos - 1;
            for (; num_pieces_passed < 3 && i % 8 != 7; i--)
            {
                if (!this->position[i]->is_empty())
                {
                    num_pieces_passed++;
                }

                if (num_pieces_passed == 3)
                {
                    break;
                }
            }
            if (i % 8 < this->en_passant_target_index % 8 && i % 8 != 7 && (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5))
            {
                return true;
            }
            else
            {
                //std::cout << i << std::endl;
                //std::cout << num_pieces_passed << std::endl;
                return false;
            }
        }
    }
    return false;
}

void Board::move_en_passant(std::list<Board*>* moves, int i, int j)
{
    if (this->en_passant_illegal())
    {
        // std::cout << "en-passant is illegal here" << std::endl;
        return;
    }

    Board* move = this->clone();
    move->switch_move();
    move->set_piece(0, i, false);
    if (this->position[i]->is_white())
    {
        move->set_piece(0, en_passant_target_index - 8, false);
    }
    else
    {
        move->set_piece(0, en_passant_target_index + 8, false);
    }
    move->set_piece(this->position[i]->get_type(), j, true);
    std::string move_str = this->create_move_str(i, j);
    move->set_last_move(move_str);
    move->en_passant_target_index = -1;
    move->last_move_origin = i;
    move->last_move_target = j;
    move->prev_pos = this;
    moves->push_back(move);
}

/**
 * @brief adds possible knight moves to a moves list
 *
 * @param moves moves list
 * @param i position of knight
 */
void Board::add_knight_moves(std::list<Board*>* moves, int i)
{
    if (this->pins[i] != 0)
    {
        return;
    }
    if (i % 8 < 6 && i + 10 < 64 && !this->position[i + 10]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 10]))))
    {
        move_with_offset(moves, i, i + 10);
    }
    if (i % 8 > 1 && i + 6 < 64 && !this->position[i + 6]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 6]))))
    {
        move_with_offset(moves, i, i + 6);
    }
    if (i % 8 != 7 && i + 17 < 64 && !this->position[i + 17]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 17]))))
    {
        move_with_offset(moves, i, i + 17);
    }
    if (i % 8 != 0 && i + 15 < 64 && !this->position[i + 15]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 15]))))
    {
        move_with_offset(moves, i, i + 15);
    }
    // move down
    if (i % 8 > 1 && i - 10 >= 0 && !this->position[i - 10]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 10]))))
    {
        move_with_offset(moves, i, i - 10);
    }
    if (i % 8 < 6 && i - 6 >= 0 && !(this->position[i - 6]->is_team_member(this->position[i])) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 6]))))
    {
        move_with_offset(moves, i, i - 6);
    }
    if (i % 8 != 7 && i - 15 >= 0 && !this->position[i - 15]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 15]))))
    {
        move_with_offset(moves, i, i - 15);
    }
    if (i % 8 != 0 && i - 17 >= 0 && !this->position[i - 17]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 17]))))
    {
        move_with_offset(moves, i, i - 17);
    }
}
/**
 * @brief adds possible bishop moves to a moves list
 *
 * @param moves moves list
 * @param i position of bishop
 */
void Board::add_bishop_moves(std::list<Board*>* moves, int i)
{
    if (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)
    {
        // diagonally right up <=> NE
        for (int j = i + 9; j < 64 && i % 8 != 7 && j % 8 != 0 && i < 56; j += 9)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
    }
    if (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8)
    {
        // diagonally left up
        for (int j = i + 7; j < 64 && i % 8 != 0 && j % 8 != 7 && i < 56; j += 7)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
        // diagonally right down
        for (int j = i - 7; j >= 0 && i % 8 != 7 && j % 8 != 0 && i >= 8; j -= 7)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
    }
    // diagonally left down
    if (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)
    {
        for (int j = i - 9; j >= 0 && i % 8 != 0 && j % 8 != 7 && i > 8; j -= 9)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
    }
}

/**
 * @brief adds possible rook moves to a moves list
 *
 * @param moves moves list
 * @param i position of rook
 */
void Board::add_rook_moves(std::list<Board*>* moves, int i)
{
    if (this->pins[i] < 3)
    {
        for (int j = i + 1; j % 8 != 0 && i % 8 != 7 && j <= 63; j++)
        {

            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
        for (int j = i - 1; j % 8 != 7 && i % 8 != 0 && j >= 0; j--)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
    }
    if (this->pins[i] == 0 || (this->pins[i] > 2 && this->pins[i] < 5))
    {
        for (int j = i + 8; j < 64; j += 8)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
        for (int j = i - 8; j >= 0; j -= 8)
        {
            if (this->position[j]->is_empty())
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
            }
            else if (!this->position[j]->is_team_member(this->position[i]))
            {
                if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
                {
                    move_with_offset(moves, i, j);
                }
                break;
            }
            else if (this->position[j]->is_team_member(this->position[i]))
            {
                break;
            }
        }
    }
}
/**
 * @brief adds possible queen moves to a moves list
 *
 * @param moves moves list
 * @param i position of queen
 */
void Board::add_queen_moves(std::list<Board*>* moves, int i)
{
    add_rook_moves(moves, i);
    add_bishop_moves(moves, i);
}
/**
 * @brief adds possible king moves to a moves list
 *
 * @param moves moves list
 * @param i position of king
 */
void Board::add_king_moves(std::list<Board*>* moves, int i)
{

    if ((i % 8 != 0) && (!(this->position[i - 1]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 1 && this->check_direction[0] != 2 && this->check_direction[1] != 1 && this->check_direction[1] != 2) || (this->check_direction[0] == 1 && (!this->position[i - 1]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i - 1);
        }
    }
    // move right
    if ((i % 8 != 7) && (!(this->position[i + 1]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 1 && this->check_direction[0] != 2 && this->check_direction[1] != 1 && this->check_direction[1] != 2) || (this->check_direction[0] == 2 && (!this->position[i + 1]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i + 1);
        }
    }
    // move down
    if ((i > 7) && (!(this->position[i - 8]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 3 && this->check_direction[0] != 4 && this->check_direction[1] != 3 && this->check_direction[1] != 4) || (this->check_direction[0] == 3 && (!this->position[i - 8]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i - 8);
        }
    }
    // move up
    if ((i < 56) && (!(this->position[i + 8]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 3 && this->check_direction[0] != 4 && this->check_direction[1] != 3 && this->check_direction[1] != 4) || (this->check_direction[0] == 4 && (!this->position[i + 8]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i + 8);
        }
    }
    if ((i % 8 != 0) && (i > 7) && (!(this->position[i - 9]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 5 && this->check_direction[0] != 7 && this->check_direction[1] != 5 && this->check_direction[1] != 7) || (this->check_direction[0] == 5 && (!this->position[i - 9]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i - 9);
        }
    }
    if ((i % 8 != 0) && (i < 56) && (!(this->position[i + 7]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 6 && this->check_direction[0] != 8 && this->check_direction[1] != 6 && this->check_direction[1] != 8) || (this->check_direction[0] == 6 && (!this->position[i + 7]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i + 7);
        }
    }
    if ((i % 8 != 7) && (i > 7) && (!(this->position[i - 7]->is_team_member(this->position[i]))))
    {
        // std::cout << "here " + std::to_string(this->check_direction[0]) << std::endl;
        if (this->num_checks == 0 || (this->check_direction[0] != 6 && this->check_direction[0] != 8 && this->check_direction[1] != 6 && this->check_direction[1] != 8) || (this->check_direction[0] == 8 && (!this->position[i - 7]->is_team_member(this->position[i]))))
        {
            //std::cout << "here " + std::to_string(this->check_direction[0]) << std::endl;
            move_with_offset(moves, i, i - 7);
        }
    }
    //std::cout << std::to_string(this->check_direction[0]) << std::endl;
    if ((i % 8 != 7) && (i < 56) && (!(this->position[i + 9]->is_team_member(this->position[i]))))
    {
        if (this->num_checks == 0 || (this->check_direction[0] != 5 && this->check_direction[0] != 7 && this->check_direction[1] != 5 && this->check_direction[1] != 7) || (this->check_direction[0] == 7 && (!this->position[i + 9]->is_team_member(this->position[i]))))
        {
            move_with_offset(moves, i, i + 9);
        }
    }
    if (white_to_move && i == 4)
    {
        if (this->castling_rights[0])
        {
            if ((!this->attacked_by_black[4]) && (!this->attacked_by_black[5]) && (!this->attacked_by_black[6]) && this->position[5]->is_empty() && this->position[6]->is_empty())
            {
                Board* move = this->clone();
                move->switch_move();
                move->set_piece(0, 4, false);
                move->set_piece(0, 7, false);
                move->set_piece(5, 5, true);
                move->set_piece(1, 6, true);
                move->white_king_pos = 6;
                move->set_last_move("O-O");
                move->castling_rights[0] = false;
                move->castling_rights[1] = false;
                move->last_move_origin = 4;
                move->last_move_target = 6;
                move->prev_pos = this;
                moves->push_back(move);
            }
        }
        if (this->castling_rights[1])
        {
            if ((!this->attacked_by_black[4]) && (!this->attacked_by_black[3]) && (!this->attacked_by_black[2]) && this->position[3]->is_empty() && this->position[2]->is_empty() && this->position[1]->is_empty())
            {
                Board* move = this->clone();
                move->switch_move();
                move->set_piece(0, 4, false);
                move->set_piece(0, 0, false);
                move->set_piece(5, 3, true);
                move->set_piece(1, 2, true);
                move->white_king_pos = 2;
                move->set_last_move("O-O-O");
                move->castling_rights[1] = false;
                move->castling_rights[0] = false;
                move->last_move_origin = 4;
                move->last_move_target = 2;
                move->prev_pos = this;
                moves->push_back(move);
            }
        }
    }
    else if ((!white_to_move) && i == 60)
    {
        if (this->castling_rights[2])
        {
            if ((!this->attacked_by_white[60]) && (!this->attacked_by_white[61]) && (!this->attacked_by_white[62]) && this->position[61]->is_empty() && this->position[62]->is_empty())
            {
                Board* move = this->clone();
                move->switch_move();
                move->set_piece(0, 60, false);
                move->set_piece(0, 63, false);
                move->set_piece(6, 61, true);
                move->set_piece(2, 62, true);
                move->black_king_pos = 62;
                move->set_last_move("O-O");
                move->castling_rights[2] = false;
                move->castling_rights[3] = false;
                move->last_move_origin = 60;
                move->last_move_target = 62;
                move->prev_pos = this;
                moves->push_back(move);
            }
        }
        if (this->castling_rights[3])
        {
            if ((!this->attacked_by_white[60]) && (!this->attacked_by_white[59]) && (!this->attacked_by_white[58]) && this->position[59]->is_empty() && this->position[58]->is_empty() && this->position[57]->is_empty())
            {
                Board* move = this->clone();
                move->switch_move();
                move->set_piece(0, 60, false);
                move->set_piece(0, 56, false);
                move->set_piece(6, 59, true);
                move->set_piece(2, 58, true);
                move->black_king_pos = 58;
                move->set_last_move("O-O-O");
                move->castling_rights[2] = false;
                move->castling_rights[3] = false;
                move->last_move_origin = 60;
                move->last_move_target = 58;
                move->prev_pos = this;
                moves->push_back(move);
            }
        }
    }
}

/**
 * @brief moves a piece from a position to another using an integer offset
 *
 * @param moves list of moves to append to
 * @param i current piece location (moves from)
 * @param j intended piece location (moves to)
 */
void Board::move_with_offset(std::list<Board*>* moves, int i, int j)
{
    Board* move = this->clone();

    move->switch_move();
    move->set_piece(0, i, false);
    move->set_piece(this->position[i]->get_type(), j, true);
    if (this->position[i]->get_type() == 1)
    {
        if (this->attacked_by_black[j])
        {
            delete move;
            return;
        }
        move->castling_rights[0] = false;
        move->castling_rights[1] = false;
        move->white_king_pos = j;
    }
    else if (this->position[i]->get_type() == 2)
    {
        if (this->attacked_by_white[j])
        {
            delete move;
            return;
        }
        move->castling_rights[2] = false;
        move->castling_rights[3] = false;
        move->black_king_pos = j;
    }
    else if (i == 7 || j == 7)
    {

        if (this->position[i]->get_type() == 5 || this->position[j]->get_type() == 5)
        {
            move->castling_rights[0] = false;
        }
    }
    else if (i == 0 || j == 0)
    {
        if (this->position[i]->get_type() == 5 || this->position[j]->get_type() == 5)
        {
            move->castling_rights[1] = false;
        }
    }
    else if (i == 56 || j == 56)
    {
        if (this->position[i]->get_type() == 6 || this->position[j]->get_type() == 6)
        {
            move->castling_rights[3] = false;
        }
    }
    else if (i == 63 || j == 63)
    {
        if (this->position[i]->get_type() == 6 || this->position[j]->get_type() == 6)
        {
            move->castling_rights[2] = false;
        }
    }
    std::string move_str = this->create_move_str(i, j);
    move->set_last_move(move_str);
    move->en_passant_target_index = -1;
    move->last_move_origin = i;
    move->last_move_target = j;
    move->prev_pos = this;
    if ((!this->position[j]->is_empty()))
    {
        moves->push_front(move);
        // std::cout << "here" << std::endl;
    }
    else
    {
        moves->push_back(move);
    }
}

void Board::promote_with_offset(std::list<Board*>* moves, int i, int j, unsigned promote_to)
{
    //std::cout << "reached" << std::endl;
    Board* move = this->clone();
    move->switch_move();
    if (i == 7 || j == 7)
    {

        if (this->position[i]->get_type() == 5 || this->position[j]->get_type() == 5)
        {
            move->castling_rights[0] = false;
        }
    }
    else if (i == 0 || j == 0)
    {
        if (this->position[i]->get_type() == 5 || this->position[j]->get_type() == 5)
        {
            move->castling_rights[1] = false;
        }
    }
    else if (i == 56 || j == 56)
    {
        if (this->position[i]->get_type() == 6 || this->position[j]->get_type() == 6)
        {
            move->castling_rights[3] = false;
        }
    }
    else if (i == 63 || j == 63)
    {
        if (this->position[i]->get_type() == 6 || this->position[j]->get_type() == 6)
        {
            move->castling_rights[2] = false;
        }
    }
    move->set_piece(0, i, false);
    move->set_piece(promote_to, j, true);

    std::string move_str = this->create_move_str(i, j);
    move_str += '=';
    move_str += move->position[j]->piece_to_char();
    move->set_last_move(move_str);
    move->en_passant_target_index = -1;
    move->last_move_origin = i;
    move->last_move_target = j;
    move->prev_pos = this;
    moves->push_front(move);
}

/**
 * @brief changes the side to move
 *
 */
void Board::switch_move()
{
    this->white_to_move = !(this->white_to_move);
}

/**
 * @brief stores the move with which the position was reached as a string
 *
 * @param set_move
 */
void Board::set_last_move(std::string set_move)
{
    this->last_move = set_move;
}
/**
 * @brief generates a deep copy of the board
 *
 * @return Board*
 */
Board* Board::clone()
{
    Piece** clone_position = new Piece * [64];
    for (int i = 0; i < 64; i++)
    {
        clone_position[i] = this->position[i]->clone();
    }

    bool* copy_castling_rights = new bool[4];
    for (int i = 0; i < 4; i++)
    {
        copy_castling_rights[i] = this->castling_rights[i];
    }

    Board* b = new Board(clone_position, this->white_to_move, copy_castling_rights);

    std::string buffer = this->get_last_move();
    b->set_last_move(buffer);
    b->white_king_pos = this->white_king_pos;
    b->black_king_pos = this->black_king_pos;
    b->en_passant_target_index = -1;
    return b;
}

std::string Board::pos_as_str()
{
    std::string str;
    if (white_to_move)
    {
        str += "white to move:\n";
    }
    else
    {
        str += "black to move\n";
    }
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            str += "+---";
        }
        else
        {
            str += "+---";
        }
        for (int j = 1; j < 8; j++)
        {
            str += "+---";
        }
        if (i == 0)
        {
            str += "+\n| ";
        }
        else
        {
            str += "+\n| ";
        }

        for (int j = 0; j < 8; j++)
        {
            int pos_in_arr = (7 - i) * 8 + j;

            str += this->position[pos_in_arr]->piece_to_char();
            str += " | ";
        }
        str += '\n';
    }
    for (int j = 0; j < 8; j++)
    {
        str += "+---";
    }
    str += "+\n";
    return str;
}

/**
 * @brief generates algebraic coordinate for a position index
 *
 * @param i positon index
 * @return std::string
 */
std::string Board::get_coord_str_from_index(int i)
{
    std::string str = "";
    char c = 97 + (i % 8);
    str += c;
    str += std::to_string((i / 8 + 1));
    return str;
}

std::string Board::get_last_move()
{
    return this->last_move;
}

std::string Board::create_move_str(int from, int to)
{
    std::string move = "";
    move += this->get_coord_str_from_index(from);
    move += this->get_coord_str_from_index(to);
    return move;
}

std::string Board::get_attacked_squares()
{
    std::string str = "attacked by white:\n";
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            str += "+---";
        }
        else
        {
            str += "+---";
        }
        for (int j = 1; j < 8; j++)
        {
            str += "+---";
        }
        if (i == 0)
        {
            str += "+\n| ";
        }
        else
        {
            str += "+\n| ";
        }

        for (int j = 0; j < 8; j++)
        {
            int pos_in_arr = (7 - i) * 8 + j;
            if (this->attacked_by_white[pos_in_arr])
            {
                str += "X";
            }
            else
            {
                str += " ";
            }
            str += " | ";
        }
        str += '\n';
    }
    for (int j = 0; j < 8; j++)
    {
        str += "+---";
    }
    str += "+\n";
    str += "\nattacked by black:\n";
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            str += "+---";
        }
        else
        {
            str += "+---";
        }
        for (int j = 1; j < 8; j++)
        {
            str += "+---";
        }
        if (i == 0)
        {
            str += "+\n| ";
        }
        else
        {
            str += "+\n| ";
        }

        for (int j = 0; j < 8; j++)
        {
            int pos_in_arr = (7 - i) * 8 + j;
            if (this->attacked_by_black[pos_in_arr])
            {
                str += "X";
            }
            else
            {
                str += " ";
            }
            str += " | ";
        }
        str += '\n';
    }
    for (int j = 0; j < 8; j++)
    {
        str += "+---";
    }
    str += "+\n";
    str += "\npins:\n";
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            str += "+---";
        }
        else
        {
            str += "+---";
        }
        for (int j = 1; j < 8; j++)
        {
            str += "+---";
        }
        if (i == 0)
        {
            str += "+\n| ";
        }
        else
        {
            str += "+\n| ";
        }

        for (int j = 0; j < 8; j++)
        {
            int pos_in_arr = (7 - i) * 8 + j;
            if (pins[pos_in_arr] > 0)
            {
                str += std::to_string(pins[pos_in_arr]);
            }
            else
            {
                str += ' ';
            }
            str += " | ";
        }
        str += '\n';
    }
    for (int j = 0; j < 8; j++)
    {
        str += "+---";
    }
    str += "+\n";
    str += "\nchecks:\n";
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            str += "+---";
        }
        else
        {
            str += "+---";
        }
        for (int j = 1; j < 8; j++)
        {
            str += "+---";
        }
        if (i == 0)
        {
            str += "+\n| ";
        }
        else
        {
            str += "+\n| ";
        }

        for (int j = 0; j < 8; j++)
        {
            int pos_in_arr = (7 - i) * 8 + j;
            if (this->checks[pos_in_arr])
            {
                str += 'X';
            }
            else
            {
                str += ' ';
            }
            str += " | ";
        }
        str += '\n';
    }
    for (int j = 0; j < 8; j++)
    {
        str += "+---";
    }
    str += "+\n";
    return str;
}

bool Board::compare(Board* b1, Board* b2)
{
    int i1 = b1->last_move_origin;
    int j1 = b1->last_move_target;
    int i2 = b2->last_move_origin;
    int j2 = b2->last_move_target;
    Board* prev_1 = b1->prev_pos;
    Board* prev_2 = b2->prev_pos;
    if ((!prev_1->position[j1]->is_empty()) && (!prev_2->position[j2]->is_empty())) {
        double diff_1 = prev_1->position[j1]->value() - prev_1->position[i1]->value();
        double diff_2 = prev_2->position[j2]->value() - prev_2->position[i2]->value();
        return diff_1 > diff_2;
    }
    else if (!prev_2->position[j2]->is_empty()) {
        return 0 > 1;
    }
    else if (!prev_1->position[j1]->is_empty()) {
        return 0 < 1;
    }
    return 0 > 1;
}

bool Board::equals(Board* b)
{
    if (this->white_to_move != b->white_to_move) {
        return false;
    }

    if (this->en_passant_target_index > 0 && b->en_passant_target_index > 0 && this->en_passant_target_index != b->en_passant_target_index) {
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        if (this->castling_rights[i] != b->castling_rights[i]) {
            return false;
        }
    }

    for (int i = 0; i < 64; i++) {
        if (this->position[i]->get_type() != b->position[i]->get_type()) {
            return false;
        }
    }

    return true;
}