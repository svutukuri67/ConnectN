// Scaffold.cpp

#include "provided.h"
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
private:
    int m_cols;
    int m_levels;
    stack<int> levelStack;
    stack<int> columnStack;
    vector<vector<int>> m_Scaffold;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
 : m_cols(nColumns), m_levels(nLevels), m_Scaffold(nLevels, vector<int>(nColumns, VACANT))
{
    if(nColumns < 1 || nLevels < 1)
    {
        cerr << "Invalid column or level input" << endl;
        exit(1);
    }

}

int ScaffoldImpl::cols() const
{
    return m_cols;
}

int ScaffoldImpl::levels() const
{
    return m_levels;
}

int ScaffoldImpl::numberEmpty() const
{
    int empty = 0;
    // Loop through each possible cell and check if its vacant
    for(int i = 0; i < m_levels; i++)
    {
        for(int j = 0; j < m_cols; j++)
        {
            if(m_Scaffold[i][j] == VACANT)
            {
                empty++;
            }
        }
    }
    return empty;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    // Check if the cell is an invalid cell
    if(column < 1 || column > m_cols || level < 1 || level > m_levels)
    {
        return -2;
    }
    
    return(m_Scaffold[level-1][column-1]); // Return what checker is at corresponding space
}

void ScaffoldImpl::display() const
{
    // Loop through 2D array
    for(int i = m_levels - 1; i >= 0; i--)
    {
        for(int j = 0; j < m_cols; j++)
        {
            cout << '|';
            // If the cell is vancant, print an empty space
            if(m_Scaffold[i][j] == VACANT)
            {
                cout << ' ';
            }
            // If the cell has a value of 0, print R
            if(m_Scaffold[i][j] == RED)
            {
                cout << 'R';
            }
            // If the cell has a value of 1, print B
            if(m_Scaffold[i][j] == BLACK)
            {
                cout << 'B';
            }
        }
        cout << '|' << endl;
    }
    for(int i = 0; i < m_cols; i++)
    {
        cout << "+-";
    }
    cout << '+' << endl;

}

bool ScaffoldImpl::makeMove(int column, int color)
{
    for(int i = 0; i < m_levels; i++)
    {
        int space = m_Scaffold[i][column-1];
        if(space == -1) // Check if space is vacant
        {
            m_Scaffold[i][column-1] = color; // Update space with proper color checker
            
            // Push the move on to the level and column stack to keep track of moves
            levelStack.push(i);
            columnStack.push(column-1);
            return true;
        }
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    // If stacks are empty, there is no move to undo
    if(levelStack.empty() && columnStack.empty())
    {
        return 0;
    }
    else
    {
        // Take the top values of both level and top stacks
        // Update the corresponding space on the scaffold to be vacant
        int level = levelStack.top();
        levelStack.pop();
        int column = columnStack.top();
        columnStack.pop();
        m_Scaffold[level][column] = -1;
        return column+1; // Return column where checker was removed
    }
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}




