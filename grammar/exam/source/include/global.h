#ifndef _GLOBAL_H_
#define _GLOBAL_H_


namespace dh {

/*********State********/
enum STATE {
	START = 0, INID, INPLUS, INEQ,  INFIDECI, INP, INSEDECI, INMINUS, INSP, DONE 
};

/*********Token Type**********/
enum TYPE {
	IDENTIFIER, DECIMAL, NONE, SYMBOL, THREAD, FEATURES, FLOWS, PROPERTIES, END, NONES, OUT, DATA, PORT, EVENT, PARAMETER, FLOW, SOURCE, PATH, CONSTANT, ACCESS, SINK, IN
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
