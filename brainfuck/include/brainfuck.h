#ifndef _BRAINFUCK_H_
#define _BRAINFUCK_H_
#include <vector>

namespace dh {

struct Node {
	
	char cData;
	Node *pPre;
	Node *pAft;
	Node( char c)
	{
		cData = c;
		pPre = nullptr;
		pAft = nullptr;
	}
};

class Pointer{
	public:
		Pointer();
		~Pointer();
		Pointer& operator++();
		Pointer& operator--();
		char& operator*();
	private:
		Node* pHead;
		Node* pEnd;
		Node* pCur;
		void destroy();
};
}

#endif
