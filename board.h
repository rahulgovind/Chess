#ifndef _BOARD_H
#define _BOARD_H

#include <bits/stdc++.h>

using namespace std;

struct moves
{
    int x0;
    int y0;
    int x1;
    int y1;
    moves() {};
    moves(int X0, int Y0, int X1, int Y1) { x0 = X0; y0 = Y0; x1 = X1; y1 = Y1;}
};


class Board
{
protected:
    int board_matrix[8][8];
    vector<moves> previous_moves;

    bool IsValidMoveBase(int x0, int y0, int x1, int y1);
    bool IsValidPawn1(int x0, int y0, int x1, int y1);
    bool IsValidPawn2(int x0, int y0, int x1, int y1);
    bool IsValidBishop1(int x0, int y0, int x1, int y1);
    bool IsValidBishop2(int x0, int y0, int x1, int y1);
    bool IsValidKnight1(int,int,int,int);
    bool IsValidKnight2(int,int,int,int);
    bool IsValidRook1(int,int,int,int);
    bool IsValidRook2(int,int,int,int);
    bool IsValidQueen1(int,int,int,int);
    bool IsValidQueen2(int,int,int,int);
    bool IsValidKing1(int,int,int,int);
    bool IsValidKing2(int,int,int,int);

    bool IsIllegal1(int,int,int,int);
    bool IsIllegal2(int,int,int,int);
    bool IsCheckmate1();
    bool IsCheckmate2();
    bool IsStalemate1();
    bool IsStalemate2();
    bool IsCheck1();
    bool IsCheck2();
    bool PieceHasMoved(int,int);
    bool IsValidCastle1(int,int,int,int);
    bool IsValidCastle2(int,int,int,int);
    bool IsValidEnPassant1(int,int,int,int);
    bool IsValidEnPassant2(int,int,int,int);


    int MakeMove(int x0, int y0, int x1, int y1, int pawn_promo=1);
    void UndoMove(int,int,int,int,int,int);
public:
    Board();
    bool IsValidMove(int x0, int y0, int x1, int y1);
};

#endif //_BOARD_H
