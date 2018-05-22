#include "Block.h"
#include "Macro.h"



Block::Block()
{
	m_isMine = false;
}


Block::~Block()
{

	m_vecBlock.clear();
}

int Block::HowMuchMineIs(int x,int y, vector<Piece*> m_vecMine)
{
	int MineCount = 2;
	m_vecBlock = m_vecMine;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		
		if ((*iter)->GetX() == x - 1 && (*iter)->GetY() == y - 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x && (*iter)->GetY() == y - 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y - 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}


		else if ((*iter)->GetX() == x - 1 && (*iter)->GetY() == y)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x - 1 && (*iter)->GetY() == y + 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x && (*iter)->GetY() == y + 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y + 1)
		{
			if ((*iter)->isMine())
				MineCount++;
		}

	}
	return MineCount;
}