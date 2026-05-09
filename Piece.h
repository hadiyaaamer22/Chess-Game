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
class Rook:public Piece
{
public:
    Rook(char c):Piece(c,c=='W'?'R':'r'){}

    bool isValidMove(int fr,int fc,int tr,int tc,Piece* board[8][8])const override
    {
        if(fr!=tr&&fc!=tc)
            return false;

        int rs=(tr>fr)?1:(tr<fr)?-1:0;
        int cs=(tc>fc)?1:(tc<fc)?-1:0;
        int r=fr+rs;
        int c=fc+cs;

        while(r!=tr||c!=tc)
        {
            if(board[r][c]!=nullptr)
                return false;

            r+=rs;
            c+=cs;
        }

        return board[tr][tc]==nullptr||board[tr][tc]->getColor()!=color;
    }
};

class Knight:public Piece
{
public:
    Knight(char c):Piece(c,c=='W'?'N':'n'){}

    bool isValidMove(int fr,int fc,int tr,int tc,Piece* board[8][8])const override
    {
        int dr=abs(tr-fr);
        int dc=abs(tc-fc);

        if(!((dr==2&&dc==1)||(dr==1&&dc==2)))
            return false;

        return board[tr][tc]==nullptr||board[tr][tc]->getColor()!=color;
    }
};
