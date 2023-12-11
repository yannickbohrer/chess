#ifndef GAME_HPP
#define GAME_HPP

#include <variant>
#include <vector>
#include "models.hpp"

class Game {
public:
    static void Start();

private:
    static Game m_Instance;
    static Game& Get();

    Color m_Turn;
    std::vector<std::vector<std::variant<std::monostate, Piece>>> m_Board;
    bool m_GameOver;
    Color m_Winner;

    Game()
        : m_Board(std::vector<std::vector<std::variant<std::monostate, Piece>>>(8)),
          m_Turn(Color::WHITE),
          m_GameOver(false),
          m_Winner(Color::WHITE) {}
    Game(const Game&) = delete;

    void InitBoard();
    void NextTurn();
    int TranslateInput(char&) const;
    bool CheckInputs(const Move&) const;
    bool CheckValidityOfMoves(const Move&) const;
    bool CheckMovement(const Piece&, const Move&) const;
    bool CheckMovementPawn(Color, const Move&) const;
    bool CheckMovementRook(Color, const Move&) const;
    bool CheckMovementKnight(Color, const Move&) const;
    bool CheckMovementBishop(Color, const Move&) const;
    bool CheckMovementQueen(Color, const Move&) const;
    bool CheckMovementKing(Color, const Move&) const;
    bool CheckRow(Color, const Move&) const;
    bool CheckCol(Color, const Move&) const;
    bool CheckDiagonal(Color, const Move&) const;
    bool CheckAroundKing(int, int) const;
};

#endif
