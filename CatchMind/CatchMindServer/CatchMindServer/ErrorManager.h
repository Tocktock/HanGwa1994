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

	// ���� �Լ� ���� ��� �� ����
	void err_quit(char *msg);

	// ���� �Լ� ���� ���
	void err_display(char *msg);
	
	virtual ~ErrorManager();
};

