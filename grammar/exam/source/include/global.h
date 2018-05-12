#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INID, INPLUS, INEQ,  INFIDECI, INP, INSEDECI, INMINUS, INSP, DONE 
};

/*********Token Type**********/
enum TYPE {
	IDENTIFIER, DECIMAL, NONE, SYMBOL, THREAD, FEATURES, FLOWS, PROPERTIES, END, NONES, OUT, DATA, PORT, EVENT, PARAMETER, FLOW, SOURCE, PATH, CONSTANT, ACCESS
};


/*********Node kind**********/
enum KIND {
	OpK = 20, NumK
};

}
#endif
