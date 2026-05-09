#pragma once
#include<cstdlib>

class Piece
{
protected:
    char color;
    char symbol;

public:
    Piece(char c,char s):color(c),symbol(s){}
    virtual ~Piece(){}

    char getColor()const
    {
        return color;
    }

    char getSymbol()const
    {
        return symbol;
    }

    virtual bool isValidMove(int fr,int fc,int tr,int tc,Piece* board[8][8])const=0;
};

class Pawn:public Piece
{
public:
    Pawn(char c):Piece(c,c=='W'?'P':'p'){}

    bool isValidMove(int fr,int fc,int tr,int tc,Piece* board[8][8])const override
    {
        int d=(color=='W')?-1:1;
        int sr=(color=='W')?6:1;

        if(tc==fc&&tr==fr+d&&board[tr][tc]==nullptr)
            return true;

        if(tc==fc&&fr==sr&&tr==fr+2*d&&board[fr+d][fc]==nullptr&&board[tr][tc]==nullptr)
            return true;

        if(abs(tc-fc)==1&&tr==fr+d&&board[tr][tc]!=nullptr&&board[tr][tc]->getColor()!=color)
            return true;

        return false;
    }
};
