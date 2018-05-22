#pragma once
#include "Piece.h"
#include "GameManager.h"
class Block :
	public Piece
{
private:
	vector<Piece*>		m_vecBlock;

public:

	virtual int HowMuchMineIs(int x,int y,vector<Piece*>m_vecPiece);

	Block();
	virtual ~Block();
};

