#pragma once
#include "UI.h"

enum WhatAMI
{
	WhatAmI_BACK,
	WhatAmI_ICON,
	WhatAMI_STAR,
	WhatAmI_END
};

class UIIcon :
	public UI
{
private:
	int m_Life;
public:

	

	
	UIIcon();
	~UIIcon();
};

