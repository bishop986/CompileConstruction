#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INNUMBER, DONE
};

/*********Token Type**********/
enum TYPE {
	ADDOP = 10, MULOP, NUMBER, NONE, BRACKET
};

}
#endif
