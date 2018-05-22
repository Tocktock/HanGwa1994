#pragma once
#include <Windows.h>
class ErrorManager
{
	static ErrorManager * s_this;

	ErrorManager();
public:

	static ErrorManager* GetInstance()
	{
		if (s_this == NULL)
			s_this = new ErrorManager();

		return s_this;
	}

	// 소켓 함수 오류 출력 후 종료
	void err_quit(char *msg);

	// 소켓 함수 오류 출력
	void err_display(char *msg);
	
	virtual ~ErrorManager();
};

