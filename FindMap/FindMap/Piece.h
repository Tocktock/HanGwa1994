#pragma once
#include <Windows.h>
#include <vector>

using namespace std;


#define OPEN	1
#define CLOSE	2
#define FLAG	3

#define EMPTYBLOCK	2
#define EIGHTBLOCK	10
#define MINEBLOCK	12
#define FLAGBLOCK	11

class Bitmap;
class Piece
{
protected:
	Bitmap*		m_pBitmap;
	Bitmap*		m_pBackBitmap;
	Bitmap*		m_pFlagBitmap;
	vector<Piece*>		m_vecPiece;

	int			m_iX;
	int			m_iY;
	int			m_whatAmI;
	int		    m_iState;
	bool		m_isMine;
	bool		m_leftFlag;
	bool		m_rightFlag;
	bool		m_topFlag;
	bool		m_bottomFlag;
	bool		m_firstCheckInLR;

public:
	void		Init(Bitmap* pBitmap,Bitmap* pBackBitmap,Bitmap* pFlagBitmap, int x, int y,int whatAmI);
	void		Draw(HDC hdc);
	void		SetOpen(int x, int y, vector<Piece*> m_vecPiece,HWND hWnd, int DirectionFlag);
	void		SetOpenOperate(int x, int y, vector<Piece*> m_vecPiece, HWND hWnd, int DirectionFlag);
	void		SetLROpen(int x, int y, vector<Piece*> m_vecPiece, HWND hWnd);

	inline bool isMine()
	{
		return m_isMine;
	}
	
	inline int	GetX()
	{
		return m_iX;
	}
	inline int	GetY()
	{
		return m_iY;
	}

	inline void FirstSet()
	{
		m_firstCheckInLR = true;
	}
	
	
	inline void SetClose()
	{
		m_iState = CLOSE;
	}

	inline void SetFlag()
	{
		m_iState = FLAG;
	}

	inline void ResetFlag()
	{
		m_iState = CLOSE;
	}

	inline int isOpen()
	{
		return m_iState;
	}

	Piece();
	virtual ~Piece();
};

