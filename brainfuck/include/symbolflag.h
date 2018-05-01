#ifndef _SYMBOLFLAG_H_
#define _SYMBOLFLAG_H_

#include <iostream>

namespace dh{
enum exceptFlag {
	UNEXPECTTOKEN, EXPECTTOEKN, ALRIGHT
};

struct exceptExplain{

	exceptFlag flag;
	char cData;
	size_t errorPos;

	exceptExplain()
	{
		this->flag = ALRIGHT;
		cData = ' ';
	}

	exceptExplain( exceptFlag flag, char c, size_t errorPos)
	{
		this->flag = flag;
		cData = c;
		this->errorPos = errorPos;
	}

	bool what()
	{
		if ( ALRIGHT == flag)
		{
			return true;
		}

		if ( UNEXPECTTOKEN == flag)
		{
			::std::cout << "[ERROR] Unexpected Token: " << cData << ::std::endl;
			::std::cout << "[ERROR] Position: " << errorPos << ::std::endl;
		} else 
		{
			::std::cout << "[ERROR] Expected Token: " << cData << ::std::endl;
			::std::cout << "[ERROR] Position: " << errorPos << ::std::endl;
		}
		return false;
	}
};
}

#endif
