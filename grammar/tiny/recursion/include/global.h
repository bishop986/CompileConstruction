#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {


/*********State********/
enum STATE {
	START = 0, INCOMMENT, INNUMBER, INID, INASSIGN, DONE
};

/*********Token Type**********/
enum TYPE {
	NUM = 6, ID, ERROR, RESERVED, NONE, ASSIGN
};


/*********Node kind**********/
enum NodeKind {
	StmK = 12, ExpK
};

enum StmtKind {
	IfK = 14, RepeatK, AssignK, ReadK, WriteK
};

enum ExpKind {
	OpK = 19, ConstK, IdK
};

enum ExpType {
	Void = 22, Boolean, Integer
};

}
#endif
