#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INID, INDECIMAL,  DONE
};

/*********Token Type**********/
enum TYPE {
	IDENTIFIER, DECIMAL, NONE, ASSIGN
};


/*********Node kind**********/
enum KIND {
	OpK = 20, NumK
};

}
#endif
