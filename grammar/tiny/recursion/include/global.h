#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INCOMMENT, INNUMBER, INID, INASSIGN, DONE
};

/*********Token Type**********/
enum TYPE {
	NUM, ID, ERROR, RESERVED, NONE, ASSIGN
};


/*********Node kind**********/
enum KIND {
	OpK = 20, NumK
};

}
#endif
