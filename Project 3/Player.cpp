// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
    void determineBestComputerMove(Scaffold& s, int N, int color, int& bestcol, int& rating, int depth);
    void determineBestHumanMove(Scaffold& s, int N, int color, int& bestcol, int& rating, int depth);
    int maxdepth = 7;
    int rate(Scaffold& s, int lastcol, int N);
};


int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int col;
    // Prompt user to choose a column to drop checker
    cout << "Choose a column: ";
    cin >> col;
    
    // If user chooses invalid column, prompt again for a column
    while(col < 1 || col > s.cols() || s.checkerAt(col, s.levels()) != VACANT)
    {
        cout << "Choose a column: ";
        cin >> col;
    }
    return col;
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int col = 1;
    
    // Check if a column is full
    while(s.checkerAt(col, s.levels()) != VACANT)
    {
        col++; // Update column value
    }
    return col;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    
    Scaffold temp = s; // Create a copy of the scaffold
    
    int rating = 0;
    int bestcol = 0;
    
    determineBestComputerMove(temp, N, color, bestcol, rating, 0);
    
    return bestcol;
}
  
void SmartPlayerImpl::determineBestComputerMove(Scaffold& s, int N, int color, int& bestcol, int& rating, int depth)
{
    if(depth == maxdepth)
    {
        rating = 0; // Return the move as a tie
        return;
    }
    
    // Stores the rating of each possible move
    vector<int> moves(s.cols(), -999);
    
    // Loop through each move
    for(int i = 0; i < moves.size(); i++)
    {
        if(s.makeMove(i+1, color))
        {
            moves[i] = rate(s, i+1, N) * (maxdepth - depth);
            if(moves[i] < 0)
            {
                int rating_h = -2;
                int col;
                determineBestHumanMove(s, N, (color-1) * -1, col, rating_h, depth+1);
                moves[i] = rating_h;
            }
            s.undoMove();
        }

    }
    
    int column = 0;
    int best_rating = -8;
    
    // Loop through the ratings of all possible moves
    for(int i = 0; i < moves.size(); i++)
    {
        // Check if another move has a better rating for the computer
        if(moves[i] > best_rating)
        {
            // Update the best rating and the column of the move for that rating
            best_rating = moves[i];
            column = i+1;
        }
    }
    
    bestcol = column;
    rating = best_rating;
    
}


void SmartPlayerImpl::determineBestHumanMove(Scaffold& s, int N, int color, int& bestcol, int& rating, int depth)
{
    if(depth == maxdepth)
    {
        rating = 0; // Return the move as a tie
        return;
    }
    
    // Stores the rating of each possible move
    vector<int> moves(s.cols(), 999);
    
    // Loop through each move
    for(int i = 0; i < moves.size(); i++)
    {
        if(s.makeMove(i+1, color))
        {
            moves[i] = rate(s, i+1, N) * (maxdepth - depth) * -1;
            if(moves[i] > 0)
            {
                int rating_c = 2;
                int col;
                determineBestComputerMove(s, N, (color-1) * -1, col, rating_c, depth+1);
                moves[i] = rating_c;
            }
            s.undoMove();
        }

    }
    
    int column = 0;
    int best_rating = 8;
    
    // Loop through the ratings of all possible moves
    for(int i = 0; i < moves.size(); i++)
    {
        // Check if another move has a better rating for the human
        if(moves[i] < best_rating)
        {
            // Update the best rating and the column of the move for that rating
            best_rating = moves[i];
            column = i+1;
        }
    }
    
    bestcol = column;
    rating = best_rating;
    
}


int SmartPlayerImpl::rate(Scaffold& s, int lastcol, int N)
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
    
    // Find the color of the most recent placed checker
    int color = s.checkerAt(lastcol, level);
    
    // Vertical win check
    for(int i = 1; i <= level; i++)
    {
        if(vertical > 0 && s.checkerAt(lastcol, i) != color)
        {
            vertical = 0;
        }
        if(s.checkerAt(lastcol, i) == color)
        {
            vertical++;
        }
        if(vertical == N)
        {
            return 1;
            
        }
    }
    
    // Horizontal win check
    for(int j = lastcol - (N-1); j < lastcol + N; j++)
    {
        if(horizontal > 0 && s.checkerAt(j, level) != color)
        {
            horizontal = 0;
        }
        if(s.checkerAt(j, level) == color)
        {
            horizontal++;
        }
        if(horizontal == N)
        {
            return 1;
        }
    }
    
    // Forward diagonal win check
    for(int i = level - (N-1), j = lastcol - (N-1); j < (lastcol + N); i++, j++)
    {
        if(forward > 0 && s.checkerAt(j, i) != color)
        {
            forward = 0;
        }
        if(s.checkerAt(j, i) == color)
        {
            forward++;
        }
        if(forward == N)
        {
            return 1;
        }
    }
    
    // Backward diagonal check
    for(int i = level - (N-1), j = lastcol + (N-1); j > (lastcol - N); i++, j--)
    {
        if(backward > 0 && s.checkerAt(j, i) != color)
        {
            backward = 0;
        }
        if(s.checkerAt(j, i) == color)
        {
            backward++;
        }
        if(backward == N)
        {
            return 1;
        }
    }
    
    // If there are no empty spaces, the game is a tie
    if(s.numberEmpty() == 0)
    {
        return 0;
    }
    
    return -1;
}







//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}



