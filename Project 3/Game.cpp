// Game.cpp

#include "provided.h"
#include <iostream>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
private:
    Scaffold s;
    int number;
    Player* red_player;
    Player* black_player;
    int current = RED;
    int lastcol = 0;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
 : s(nColumns, nLevels), number(N), red_player(red), black_player(black)
{
    if(N < 1)
    {
        cerr << "Invalid number to win input" << endl;
        exit(1);
    }
}

bool GameImpl::completed(int& winner) const
{
    if(current == RED)
    {
        int vertical = 0;
        int horizontal = 0;
        int forward = 0;
        int backward = 0;
        int level = 0;
        
        // Loop through to find the top most level of the last column that is not vacant
        for(int i = 1; i <= s.levels(); i++)
        {
            if(s.checkerAt(lastcol, i) != VACANT)
            {
                level++;
            }
        }
        
        // Vertical win check
        for(int i = 1; i <= level; i++)
        {
            if(vertical > 0 && s.checkerAt(lastcol, i) != BLACK)
            {
                vertical = 0;
            }
            if(s.checkerAt(lastcol, i) == BLACK)
            {
                vertical++;
            }
            if(vertical == number)
            {
                winner = BLACK;
                cout << "Black Wins!" << endl;
                return true;
            }
        }
        
        // Horizontal win check
        for(int j = lastcol - (number-1); j < lastcol + number; j++)
        {
            if(horizontal > 0 && s.checkerAt(j, level) != BLACK)
            {
                horizontal = 0;
            }
            if(s.checkerAt(j, level) == BLACK)
            {
                horizontal++;
            }
            if(horizontal == number)
            {
                winner = BLACK;
                cout << "Black Wins!" << endl;
                return true;
            }
        }
        
        // Forward diagonal win check
        for(int i = level - (number-1), j = lastcol - (number-1); j < (lastcol + number); i++, j++)
        {
            if(forward > 0 && s.checkerAt(j, i) != BLACK)
            {
                forward = 0;
            }
            if(s.checkerAt(j, i) == BLACK)
            {
                forward++;
            }
            if(forward == number)
            {
                winner = BLACK;
                cout << "Black Wins!" << endl;
                return true;
            }
        }
        
        // Backward diagonal win check
        for(int i = level - (number-1), j = lastcol + (number-1); j > (lastcol - number); i++, j--)
        {
            if(backward > 0 && s.checkerAt(j, i) != BLACK)
            {
                backward = 0;
            }
            if(s.checkerAt(j, i) == BLACK)
            {
                backward++;
            }
            if(backward == number)
            {
                winner = BLACK;
                cout << "Black Wins!" << endl;
                return true;
            }
        }
    }
    
    else if(current == BLACK)
    {
        int vertical = 0;
        int horizontal = 0;
        int forward = 0;
        int backward = 0;
        int level = 0;
        
        // Loop through to find the top most level of the last column that is not vacant
        for(int i = 1; i <= s.levels(); i++)
        {
            if(s.checkerAt(lastcol, i) != VACANT)
            {
                level++;
            }
        }
        
        // Vertical win check
        for(int i = 1; i <= level; i++)
        {
            if(vertical > 0 && s.checkerAt(lastcol, i) != RED)
            {
                vertical = 0;
            }
            if(s.checkerAt(lastcol, i) == RED)
            {
                vertical++;
            }
            if(vertical == number)
            {
                winner = RED;
                cout << "Red Wins!" << endl;
                return true;
            }
        }
        
        // Horizontal win check
        for(int j = lastcol - (number-1); j < lastcol + number; j++)
        {
            if(horizontal > 0 && s.checkerAt(j, level) != RED)
            {
                horizontal = 0;
            }
            if(s.checkerAt(j, level) == RED)
            {
                horizontal++;
            }
            if(horizontal == number)
            {
                winner = RED;
                cout << "Red Wins!" << endl;
                return true;
            }
        }
        
        // Forward diagonal win check
        for(int i = level - (number-1), j = lastcol - (number-1); j < (lastcol + number); i++, j++)
        {
            if(forward > 0 && s.checkerAt(j, i) != RED)
            {
                forward = 0;
            }
            if(s.checkerAt(j, i) == RED)
            {
                forward++;
            }
            if(forward == number)
            {
                winner = RED;
                cout << "Red Wins!" << endl;
                return true;
            }
        }
        
        // Backward diagonal check
        for(int i = level - (number-1), j = lastcol + (number-1); j > (lastcol - number); i++, j--)
        {
            if(backward > 0 && s.checkerAt(j, i) != RED)
            {
                backward = 0;
            }
            if(s.checkerAt(j, i) == RED)
            {
                backward++;
            }
            if(backward == number)
            {
                winner = RED;
                cout << "Red Wins!" << endl;
                return true;
            }
        }
    }
    
    // If there are no empty spaces, the game is a tie
    if(s.numberEmpty() == 0)
    {
        winner = TIE_GAME;
        cout << "Tie Game!" << endl;
        return true;
    }
    
    return false;
}

bool GameImpl::takeTurn()
{
    int winner = 0;
    if(completed(winner)) // Check if the game is over
    {
        return false;
    }
    
    // Have the current player choose a move and then make it
    else if(current == RED)
    {
        lastcol = red_player->chooseMove(s, number, RED);
        s.makeMove(lastcol, RED);
        current = BLACK; // Update the current player
        return true;
    }
    else
    {
        lastcol = black_player->chooseMove(s, number, BLACK);
        s.makeMove(lastcol, BLACK);
        current = RED; // Update the current player
        return true;
    }
}

void GameImpl::play()
{
    int winner = -2;
    while(!completed(winner)) // Check if the game is not completed
    {
        // Take a turn, and then print display of the scaffold
        takeTurn();
        s.display();
    }
}

int GameImpl::checkerAt(int c, int r) const
{
    return s.checkerAt(c, r);
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}

