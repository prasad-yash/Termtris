// #include <iostream>
#include <bits/stdc++.h>
#include <ncurses.h>
#include <chrono>

std::ofstream logfile("debug.log");

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
        // logfile << "Tetromino()";
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
        logfile << "Gameboard()";
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
        logfile << "spawn_tetromino()";
        active_piece = piece;
        active_piece->set_x(4); // TODO Calculate x at runtime
        active_piece->set_y(0);
        this->redraw();
    }
    void fall()
    {
        // logfile << "fall()";
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
                    // logfile << "Intersection Check Fail";
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
        logfile << "update()";
        this->redraw();
    }
    void redraw()
    {
        logfile << "redraw()";
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
    int startx, starty, width = 24, height = 45;
    const Gameboard &board; // read-only view of game state

public:
    GameboardRenderer(const Gameboard &b) : board(b)
    {

        int rows, columns;
        getmaxyx(stdscr, rows, columns);
        starty = (rows - height) / 2;
        startx = (columns - width) / 2;

        win = newwin(height, width, starty, startx);
        box(win, 0, 0);
        wrefresh(win);
    }
    void draw()
    {
        werase(win); // replace with moving back to 0,0
        // logfile << "draw()";
        for (int i = 4; i < this->board.play_height + 4; i++)
        {
            for (int j = 1; j < this->board.play_width + 1; j++)
            {
                if (this->board.gameboard[i][j] == 0)
                {
                    wprintw(win, "..");
                }
                else if (this->board.gameboard[i][j] == 1)
                {
                    wprintw(win, "##");
                }
            }
            wprintw(win, "\n");
        }
        wrefresh(win);
    }
};

class Engine
{
private:
    Gameboard gameboard;
    GameboardRenderer gameboardrenderer;
    const std::chrono::duration<double, std::milli> duration = std::chrono::duration<double, std::milli>{800};
    std::chrono::_V2::steady_clock::time_point start{std::chrono::steady_clock::now()};
    std::chrono::_V2::steady_clock::time_point current{std::chrono::steady_clock::now()};

public:
    Engine() : gameboard(), gameboardrenderer(gameboard)
    {
        Tetromino *t = new Tetromino(PieceType::PIECE_I);
        gameboard.spawn_tetromino(t);
    }
    void run()
    {
        while (true)
        {
            current = std::chrono::steady_clock::now();
            if (current - start > duration)
            {
                gameboard.fall();
                start = std::chrono::steady_clock::now();
            }
            gameboardrenderer.draw();
            refresh();
            move(0, 0);
        }
    }
};

int main(int argc, char **argv)
{
    logfile << std::unitbuf;
    logfile << "Start of main()";
    initscr();
    logfile << "After initscr()";
    // printw("After initscr()");

    Engine engine;
    // const auto finish{std::chrono::steady_clock::now()};
    // const std::chrono::duration<double> elapsed_seconds{finish - start};

    engine.run();

    // getch();
    endwin();

    logfile.flush();
    return 0;
}

// rotations, movement
// piece queue (7-8 pieces), also keep appending every time a piece is dropped