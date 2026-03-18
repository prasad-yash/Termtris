// #include <iostream>
#include <bits/stdc++.h>
#include <ncurses.h>
#include <chrono>

enum class PieceType
{
    PIECE_I,
    PIECE_T,
    PIECE_S,
    PIECE_Z,
    PIECE_L,
    PIECE_J,
    PIECE_O
};

class Tetromino
{
private:
    PieceType piece_type;
    std::array<std::array<int, 4>, 4> piece_shape;
    int coord_x, coord_y;

public:
    Tetromino(PieceType p)
    {
        std::cout << "Tetromino()";
        piece_type = p;
        switch (p)
        {
        case PieceType::PIECE_I:
            piece_shape = {{{0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {1, 1, 1, 1},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_T:
            piece_shape = {{{0, 0, 0, 0},
                            {0, 1, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_S:
            piece_shape = {{{0, 0, 0, 0},
                            {0, 1, 1, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_Z:
            piece_shape = {{{0, 0, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_L:
            piece_shape = {{{0, 0, 0, 0},
                            {1, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_J:
            piece_shape = {{{0, 0, 0, 0},
                            {0, 0, 1, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0}}};
            break;
        case PieceType::PIECE_O:
            piece_shape = {{{0, 0, 0, 0},
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}}};
            break;
        default:
            break;
        }
    }
    int get_x()
    {
        return coord_x;
    }
    int get_y()
    {
        return coord_y;
    }
    const std::array<std::array<int, 4>, 4> &get_piece_shape()
    {
        return piece_shape;
    }
    void set_x(int x)
    {
        coord_x = x;
    }
    void set_y(int y)
    {
        coord_y = y;
    }
};

class Gameboard
{
private:
    Tetromino *active_piece;

public:
    int total_width;
    int total_height;
    int play_width;
    int play_height;
    std::vector<std::vector<int>> gameboard;

    Gameboard()
    {
        std::cout << "Gameboard()";
        play_width = 10;
        play_height = 40;
        total_width = play_width + 2;
        total_height = play_height + 5;
        active_piece = nullptr;
        gameboard.resize(total_height, std::vector<int>(total_width, 0));
        for (int i = 0; i < total_height; i++)
        {
            gameboard[i][0] = 1;
            gameboard[i][total_width - 1] = 1;
        }
        for (int i = 0; i < total_width; i++)
        {
            gameboard[total_height - 1][i] = 1;
        }
    }
    void spawn_tetromino(Tetromino *piece)
    {
        std::cout << "spawn_tetromino()";
        active_piece = piece;
        active_piece->set_x(4); // TODO Calculate x at runtime
        active_piece->set_y(0);
        this->redraw();
    }
    void fall()
    {
        // std::cout << "fall()";
        int x = active_piece->get_x();
        int y = active_piece->get_y();
        auto shape = active_piece->get_piece_shape();
        // Out of bounds check

        // Intersection check
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (shape[i][j] == 1 && gameboard[y + 1 + i][x + j] == 1)
                {
                    // std::cout << "Intersection Check Fail";
                    return;
                }
            }
        }
        clear_piece();
        active_piece->set_y(y + 1);
        redraw();
    }
    void clear_piece()
    {
        int x = active_piece->get_x();
        int y = active_piece->get_y();
        auto shape = active_piece->get_piece_shape();
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (shape[i][j] == 1)
                {
                    gameboard[y + i][x + j] = 0;
                }
            }
        }
    }
    void update()
    {
        std::cout << "update()";
        this->redraw();
    }
    void redraw()
    {
        std::cout << "redraw()";
        int x = active_piece->get_x();
        int y = active_piece->get_y();
        auto shape = active_piece->get_piece_shape();
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (shape[i][j] == 1)
                {
                    gameboard[y + i][x + j] = shape[i][j];
                }
            }
        }
    }
};

class GameboardRenderer
{
private:
    WINDOW *win;
    const Gameboard &board; // read-only view of game state

public:
    GameboardRenderer(const Gameboard &b);
    void draw()
    {

        // std::cout << "display()";
        for (int i = 0; i < this->board.play_height - 1; i++)
        {
            for (int j = 1; j < this->board.play_width + 1; j++)
            {
                if (this->board.gameboard[i][j] == 0)
                {
                    printw("..");
                }
                else if (this->board.gameboard[i][j] == 1)
                {
                    printw("##");
                }
            }
            printw("\n");
        }
    }
};

class Engine
{
private:
    Gameboard gameboard;

public:
};

int main(int argc, char **argv)
{
    std::cout << "Start of main()";
    const auto duration = std::chrono::duration<double, std::milli>{800};
    auto start{std::chrono::steady_clock::now()};
    auto current{std::chrono::steady_clock::now()};
    // const auto finish{std::chrono::steady_clock::now()};
    // const std::chrono::duration<double> elapsed_seconds{finish - start};

    Gameboard gameboard = Gameboard();

    initscr();
    std::cout << "After initscr()";

    Tetromino t = Tetromino(PieceType::PIECE_I);
    gameboard.spawn_tetromino(&t);

    while (true)
    {
        current = std::chrono::steady_clock::now();
        if (current - start > duration)
        {
            gameboard.fall();
            start = std::chrono::steady_clock::now();
        }
        gameboard.display();
        refresh();
        move(0, 0);
    }

    getch();
    endwin();

    return 0;
}