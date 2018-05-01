#include "include/brainfuck.h"

namespace dh{

Pointer::Pointer()
{
	pHead = new Node(0);
	pCur  = pHead;
	pEnd  = pHead;
}

Pointer::~Pointer()
{
	destroy();
}

Pointer& Pointer::operator++()
{
	if ( nullptr == pCur->pAft)
	{
		pCur->pAft = new Node(0);
		pCur->pAft->pPre = pCur;
		pCur = pCur->pAft;
		pEnd = pCur;
	} else 
	{
		pCur = pCur->pAft;
	}
	return *this;
}

Pointer& Pointer::operator--()
{
	if ( nullptr == pCur->pPre)
	{
		pCur->pPre = new Node(0);
		pCur->pPre->pAft = pCur;
		pCur = pCur->pPre;
		pHead = pCur;
	} else 
	{
		pCur = pCur->pPre;
	}
	return *this;
}

char& Pointer::operator*()
{
	return pCur->cData;
}

void Pointer::destroy()
{
	pCur = pHead;
	while( nullptr != pCur)
	{
		Node* tmp = pCur;
		pCur = pCur->pAft;
		delete tmp;
	}
}

}
