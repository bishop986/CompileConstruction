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
enum NodeKind {
	StmK, ExpK
};

enum StmtKind {
	IfK, RepeatK, AssignK, ReadK, WriteK
};

enum ExpKind {
	OpK, ConstK, IdK
};

enum ExpType {
	Void, Integer, Boolean
};

}
#endif
