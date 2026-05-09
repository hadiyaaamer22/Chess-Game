#include "Board.h"
#include<iostream>
#include<cstdlib>
using namespace std;

Board::Board()
{
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            grid[r][c]=nullptr;
        }
    }

    setupPieces();
}

Board::~Board()
{
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            delete grid[r][c];
        }
    }
}

void Board::setupPieces()
{
    grid[0][0]=new Rook('B');
    grid[0][7]=new Rook('B');
    grid[0][1]=new Knight('B');
    grid[0][6]=new Knight('B');
    grid[0][2]=new Bishop('B');
    grid[0][5]=new Bishop('B');
    grid[0][3]=new Queen('B');
    grid[0][4]=new King('B');

    for(int c=0;c<8;c++)
        grid[1][c]=new Pawn('B');

    grid[7][0]=new Rook('W');
    grid[7][7]=new Rook('W');
    grid[7][1]=new Knight('W');
    grid[7][6]=new Knight('W');
    grid[7][2]=new Bishop('W');
    grid[7][5]=new Bishop('W');
    grid[7][3]=new Queen('W');
    grid[7][4]=new King('W');

    for(int c=0;c<8;c++)
        grid[6][c]=new Pawn('W');
}
void Board::display()const
{
    cout<<endl;
    cout<<"    a  b  c  d  e  f  g  h"<<endl;
    cout<<"  +------------------------+"<<endl;

    for(int r=0;r<8;r++)
    {
        cout<<(8-r)<<" | ";

        for(int c=0;c<8;c++)
        {
            if(grid[r][c]==nullptr)
                cout<<".  ";
            else
                cout<<grid[r][c]->getSymbol()<<"  ";
        }

        cout<<"| "<<(8-r)<<endl;
    }

    cout<<"  +------------------------+"<<endl;
    cout<<"    a  b  c  d  e  f  g  h"<<endl;
    cout<<endl;
}
