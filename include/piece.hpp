#ifndef PIECE_HPP
#define PIECE_HPP

#include "enums.hpp"

class Piece {
public:
    Piece() : m_Role(Role::PAWN), m_Color(Color::WHITE) {}
    Piece(const Role role, const Color color) : m_Role(role), m_Color(color) {}

    Role GetRole() const;
    Color GetColor() const;

private:
    Role m_Role;
    Color m_Color;
};

#endif
