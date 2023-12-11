#include "../include/game.hpp"

#include <cstdlib>
#include <iostream>
#include <ranges>
#include <variant>

Game& Game::Get() {
    static Game m_Instance;
    return m_Instance;
}

void Game::Start() {
    Game& game = Game::Get();
    game.InitBoard();

    while (!game.m_GameOver) {
        game.NextTurn();
        if (game.m_Turn == Color::WHITE)
            game.m_Turn = Color::BLACK;
        else
            game.m_Turn = Color::WHITE;
    }

    std::cout << "\n\n" << (game.m_Winner == Color::WHITE ? "Weiß " : "Schwarz ") << "gewinnt!\n";
}

void Game::InitBoard() {
    std::cout << "Initializing Board\n";
    for (int i = 0; i < m_Board.size(); i++) {
        auto& rank = m_Board.at(i);
        switch (i) {
            case 0:
                rank.emplace_back(Piece{Role::ROOK, Color::BLACK});
                rank.emplace_back(Piece{Role::KNIGHT, Color::BLACK});
                rank.emplace_back(Piece{Role::BISHOP, Color::BLACK});
                rank.emplace_back(Piece{Role::QUEEN, Color::BLACK});
                rank.emplace_back(Piece{Role::KING, Color::BLACK});
                rank.emplace_back(Piece{Role::BISHOP, Color::BLACK});
                rank.emplace_back(Piece{Role::KNIGHT, Color::BLACK});
                rank.emplace_back(Piece{Role::ROOK, Color::BLACK});
                break;
            case 1:
                for (const auto field : std::views::iota(0, 8))
                    rank.emplace_back(Piece{Role::PAWN, Color::BLACK});
                break;
            case 6:
                for (const auto field : std::views::iota(0, 8))
                    rank.emplace_back(Piece{Role::PAWN, Color::WHITE});
                break;
            case 7:
                rank.emplace_back(Piece{Role::ROOK, Color::WHITE});
                rank.emplace_back(Piece{Role::KNIGHT, Color::WHITE});
                rank.emplace_back(Piece{Role::BISHOP, Color::WHITE});
                rank.emplace_back(Piece{Role::QUEEN, Color::WHITE});
                rank.emplace_back(Piece{Role::KING, Color::WHITE});
                rank.emplace_back(Piece{Role::BISHOP, Color::WHITE});
                rank.emplace_back(Piece{Role::KNIGHT, Color::WHITE});
                rank.emplace_back(Piece{Role::ROOK, Color::WHITE});
                break;
            default:
                for (const auto field : std::views::iota(0, 8))
                    rank.emplace_back(std::monostate());
        }
    }
}

void Game::NextTurn() {
    Game& game = Game::Get();
    std::cout << (game.m_Turn == Color::WHITE ? "Weiß " : "Schwarz ") << "ist dran.\n";
    char c1, c2;
    Move move;
    do {
        std::cout << "Von:  ";
        std::cin >> c1 >> move.y1;
        std::cout << "Nach: ";
        std::cin >> c2 >> move.y2;
        move.x1 = game.TranslateInput(c1);
        move.x2 = game.TranslateInput(c2);
        move.y1--;
        move.y2--;
    } while (!game.CheckInputs(move));
    game.CheckValidityOfMoves(move);

    game.m_GameOver = true;
}

int Game::TranslateInput(char& c) const {
    switch (c) {
        case 'a':
            return 7;
        case 'b':
            return 6;
        case 'c':
            return 5;
        case 'd':
            return 4;
        case 'e':
            return 3;
        case 'f':
            return 2;
        case 'g':
            return 1;
        case 'h':
            return 0;
        default:
            return -1;
    }
}

bool Game::CheckInputs(const Move& move) const {
    bool res = (move.x1 >= 0 && move.x1 <= 7 && move.x2 >= 0 && move.x2 <= 7 && move.y1 >= 0 &&
                move.y1 <= 7 && move.y2 >= 0 && move.y2 <= 7);
    if (!res)
        std::cout << "\nFehlerhafte Eingabe, erneut versuchen:\n";
    return res;
}

bool Game::CheckValidityOfMoves(const Move& move) const {
    Game& game = Game::Get();
    if (std::holds_alternative<std::monostate>(game.m_Board.at(move.y1).at(move.x1)))
        return false;
    auto& piece = std::get<Piece>(game.m_Board.at(move.y1).at(move.x1));
    if (piece.color != game.m_Turn)
        return false;
    return game.CheckMovement(piece, move);
}

bool Game::CheckMovement(const Piece& piece, const Move& move) const {
    Game& game = Game::Get();
    switch (piece.role) {
        case Role::PAWN:
            return game.CheckMovementPawn(piece.color, move);
        case Role::ROOK:
            return game.CheckMovementRook(piece.color, move);
        case Role::KNIGHT:
            return game.CheckMovementKnight(piece.color, move);
        case Role::BISHOP:
            return game.CheckMovementBishop(piece.color, move);
        case Role::QUEEN:
            return game.CheckMovementQueen(piece.color, move);
        case Role::KING:
            return game.CheckMovementKing(piece.color, move);
    }
}

bool Game::CheckMovementPawn(Color color, const Move& move) const {
    Game& game = Game::Get();
    if (std::abs(int(move.y2) - int(move.y2)) != 1)
        return false;

    if (color == Color::WHITE) {
        if (move.y1 > move.y2)
            return false;
    } else {
        if (move.y1 < move.y2)
            return false;
    }

    if (move.x1 == move.x2) {
        if (std::holds_alternative<Piece>(game.m_Board.at(move.y2).at(move.x2)))
            return false;
    } else {
        if (std::abs(int(move.x1) - int(move.x2)) != 1)
            return false;
        if (std::holds_alternative<std::monostate>(game.m_Board.at(move.y2).at(move.x2)))
            return false;
        if (std::get<Piece>(game.m_Board.at(move.y2).at(move.x2)).color == color)
            return false;
    }

    return true;
}

bool Game::CheckMovementRook(Color color, const Move& move) const {
    Game& game = Game::Get();
    if (move.x1 != move.x2 && move.y1 != move.y2)
        return false;

    if (move.x1 == move.x2)
        return game.CheckCol(color, move);

    return game.CheckRow(color, move);
}

bool Game::CheckMovementKnight(Color color, const Move& move) const {
    return false;
}

bool Game::CheckMovementBishop(Color color, const Move& move) const {
    Game& game = Game::Get();
    if (move.x1 == move.x2 || move.y1 == move.y2)
        return false;
    return game.CheckDiagonal(color, move);
}

bool Game::CheckMovementQueen(Color color, const Move& move) const {
    Game& game = Game::Get();
    return game.CheckDiagonal(color, move) || game.CheckRow(color, move) ||
           game.CheckCol(color, move);
}

bool Game::CheckMovementKing(Color color, const Move& move) const {
    Game& game = Game::Get();
    if (std::abs(int(move.x2) - int(move.x1)) != 1)
        return false;
    if (std::abs(int(move.y2) - int(move.y1)) != 1)
        return false;
    if (std::holds_alternative<Piece>(game.m_Board.at(move.y2).at(move.x2)))
        if (std::get<Piece>(game.m_Board.at(move.y2).at(move.x2)).color == color)
            return false;
    return game.CheckAroundKing(move.x2, move.y2);
}

bool Game::CheckRow(Color color, const Move& move) const {
    Game& game = Game::Get();
    auto& row = game.m_Board.at(move.y1);
    int it = move.x1;
    int inc = move.x1 < move.x2 ? 1 : -1;
    it += inc;
    while (it != move.x2) {
        if (std::holds_alternative<Piece>(row.at(it)))
            return false;
        it += inc;
    }
    if (std::holds_alternative<Piece>(row.at(move.x2)))
        if (std::get<Piece>(row.at(move.x2)).color == color)
            return false;
    return true;
}

bool Game::CheckCol(Color color, const Move& move) const {
    Game& game = Game::Get();
    int it = move.y1;
    int inc = move.y1 < move.y2 ? 1 : -1;
    it += inc;
    while (it != move.y2) {
        if (std::holds_alternative<Piece>(game.m_Board.at(move.x1).at(it)))
            return false;
        it += inc;
    }
    if (std::holds_alternative<Piece>(game.m_Board.at(move.y2).at(move.x2)))
        if (std::get<Piece>(game.m_Board.at(move.y2).at(move.x2)).color == color)
            return false;
    return true;
}

bool Game::CheckDiagonal(Color color, const Move& move) const {
    Game& game = Game::Get();

    int itX = move.x1, itY = move.y1;
    int incX, incY;
    if (move.x1 > move.x2) {
        incX = -1;
        incY = move.y1 > move.y2 ? -1 : 1;
    } else {
        incX = 1;
        incY = move.y1 > move.y2 ? -1 : 1;
    }
    itX += incX;
    itY += incY;
    bool hit = false;
    while (0 <= incX && incX <= 7 && 0 <= incY && incY <= 7) {
        if (itX == move.x2 && itY == move.y2) {
            hit = true;
            break;
        }
        if (std::holds_alternative<Piece>(game.m_Board.at(itY).at(itY)))
            return false;
        itX += incX;
        itY += incY;
    }
    if (!hit)
        return false;
    if (std::holds_alternative<Piece>(game.m_Board.at(move.y2).at(move.x2)))
        if (std::get<Piece>(game.m_Board.at(move.y2).at(move.x2)).color == color)
            return false;
    return true;
}

bool Game::CheckAroundKing(int x, int y) const {
    Game& game = Game::Get();
    if (x == 0 && y == 0 || x == 7 && y == 0 || x == 7 && y == 7 || x == 0 && y == 7)
        return true;

    int itX = x - 1, itY = y - 1;
    int counter = 0;
    while (counter < 8) {
        if (itX == x && itY == y) {
            itX++;
            continue;
        }

        if (itX < 0)
            itX = 0;
        else if (itX > 7)
            itX = 7;
        if (itY < 0)
            itY = 0;
        else if (itY > 7)
            itY = 7;

        if (std::holds_alternative<Piece>(game.m_Board.at(itY).at(itX)))
            if (std::get<Piece>(game.m_Board.at(itY).at(itX)).role == Role::KING)
                return false;

        if (counter % 3 == 0) {
            itX = x - 1;
            itY++;
        }
        counter++;
    }
    return true;
}
