#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INID, INNUM, INCOMMENT, DONE
};

/*********Token Type**********/
enum TYPE {
	ID, NUM, ELSE, IF, INT, RETURN, VOID, WHILE, OP, NONES
};


/*********Node kind**********/
enum NodeKind {
	ThreadK = 20, SpecK, TypeK, AssociationK, ReferenceK, IDK, ConstK, PackageK
};

enum SpecKind {
	PortK, ParamK, flowSourceK, flowSinkK, flowPathK, NoneK
};

enum TypeKind {
	IOType, PortType
};

}
#endif
