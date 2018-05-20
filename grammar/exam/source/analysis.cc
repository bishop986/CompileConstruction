#include "include/syntax_treenode.h"
#include "include/global.h"
#include "include/analysis.h"
#include <assert.h>
#include <sstream>
#include <fstream>

namespace dh 
{
analysis::analysis(scanner tokens)
{
	this->_tokens = tokens;
	this->tmp = ::std::make_shared<token>("", TYPE::NONES);
	initFlag = false;

	this->tabcounter = 0;

#ifdef _DEBUG_
	assert(tokens.isScanned());
#endif

	if ( !tokens.isScanned())
	{
		::std::cerr << "[ERROR] Tokens table is not ready" << ::std::endl;
		::std::exit(1);
	}
}

void analysis::match( ::std::string c)
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG in match]" << ::std::endl;
	::std::cout << "[get] " << tmp->getVal() << "[match] " << c << ::std::endl;
#endif

	if ( c == tmp->getVal())
	{
		*tmp =  _tokens.getToken();
		return;
	} else 
	{
		_tokens.ungetToken();
		*tmp = _tokens.getToken();
		::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
		::std::exit(1);
	}
}

double analysis::decimal()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in number " << ::std::endl;
#endif
	try {
		if ( tmp->getType() == TYPE::DECIMAL)
		{
			::std::stringstream num_stream;
			double num_tmp = 0.0;
			
			num_stream << tmp->getVal();
			num_stream >> num_tmp;

			*tmp = _tokens.getToken();

			return num_tmp;
		} else
		{
			::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
			::std::exit(1);
		}
	} catch ( ::std::exception& e)
	{
		::std::cerr << "[ERROR] " << e.what() << ::std::endl;
		::std::exit(1);
	}
}

::std::string analysis::identifier()
{
#ifdef _DEBUG_
	std::cout << "[DEBUG] " << "in identifier" << ::std::endl;
#endif

	if ( tmp->getType() == TYPE::IDENTIFIER)
	{
		::std::string ret = tmp->getVal();
		
		*tmp = _tokens.getToken();
		return ret;
	} else 
	{
		::std::cerr << "[ERROR] Syntax Error near" << tmp->getVal() << ::std::endl;
		::std::exit(1);
	}
}

NodePtr analysis::ThreadSpec()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in ThreadSpec " << ::std::endl;
#endif

	match("thread");
	::std::string tmp_str = identifier();
	
	NodePtr tmp_ptr = nullptr;
	NodePtr ret = ::std::make_shared<TreeNode>(tmp_str);

	if ( tmp->getType() == TYPE::FEATURES)
	{
		match("features");
		tmp_ptr = featureSpec();
		ret->appendChild(tmp_ptr);
	}

	if ( tmp->getType() == TYPE::FLOWS)
	{
		match("flows");
		tmp_ptr = flowSpec();
		ret->appendChild(tmp_ptr);
	}

	if ( tmp->getType() == TYPE::PROPERTIES)
	{
		match("properties");
		tmp_ptr = association();
		ret->appendChild(tmp_ptr);
		match(";");
	}

	match("end");

	tmp_str = identifier();
	tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
	tmp_ptr->setNodeKind(NodeKind::IDK);

	ret->appendChild(tmp_ptr);

	ret->setNodeKind( NodeKind::ThreadK);

	match(";");
	return ret;
}

NodePtr analysis::featureSpec()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in featrueSpec " << ::std::endl;
#endif

	if ( tmp->getType() == TYPE::NONE)
	{
		match("none");
		NodePtr ret = ::std::make_shared<TreeNode>("none");
		ret->setNodeKind( NodeKind::SpecK);
		ret->setKind( SpecKind::NoneK);

		match(";");
		return ret;
	}

	NodePtr tmp_ptr;
	::std::string tmp_str = identifier();
	NodePtr ret = ::std::make_shared<TreeNode>(tmp_str);
	ret->setNodeKind( NodeKind::SpecK);

	match(":");

	tmp_ptr = IOType();

	ret->appendChild(tmp_ptr);

	if ( tmp->getType() == TYPE::PARAMETER)
	{
		ret->setKind( SpecKind::ParamK);

		match("parameter");
		if ( tmp->getType() == TYPE::IDENTIFIER)
		{
			tmp_ptr = reference();
		}
		ret->appendChild(tmp_ptr);

		if ( tmp->getVal() == "{")
		{
			match("{");

			tmp_ptr = association();
			NodePtr tmp_cur_ptr = tmp_ptr;

			while ( tmp->getType() == TYPE::IDENTIFIER)
			{
				NodePtr tmp_in_ptr = association();
				tmp_cur_ptr->setSibling(tmp_in_ptr);
				tmp_cur_ptr = tmp_in_ptr;
			}

			match("}");
		}
		ret->appendChild(tmp_ptr);
	} else 
	{
		ret->setKind( SpecKind::PortK);

		tmp_ptr = portType();
		ret->appendChild( tmp_ptr);

		if ( tmp->getVal() == "{")
		{
			match("{");

			tmp_ptr = association();
			NodePtr tmp_cur_ptr = tmp_ptr;

			while ( tmp->getType() == TYPE::IDENTIFIER)
			{
				NodePtr tmp_in_ptr = association();
				tmp_cur_ptr->setSibling(tmp_in_ptr);
				tmp_cur_ptr = tmp_in_ptr;
			}

			match("}");
		}
		ret->appendChild(tmp_ptr);
	}

	match(";");
	return ret;
}

NodePtr analysis::portType()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in portType " << ::std::endl;
#endif

	NodePtr tmp_ptr;
	NodePtr ret;

	if ( tmp->getType() == TYPE::DATA)
	{
		ret = ::std::make_shared<TreeNode>("data");
		ret->setNodeKind(NodeKind::TypeK);
		ret->setKind(TypeKind::PortType);

		match("data");
		match("port");

		if ( tmp->getType() == TYPE::IDENTIFIER)
		{
			tmp_ptr = reference();
		}
		ret->appendChild(tmp_ptr);
	} else 
	{
		match("event");

		if ( tmp->getType() == TYPE::DATA)
		{
			match("data");
			ret = ::std::make_shared<TreeNode>("event data");
			match("port");

			if ( tmp->getType() == TYPE::IDENTIFIER)
			{
				tmp_ptr = reference();
			}
			ret->appendChild(tmp_ptr);
		} else 
		{
			match("port");
			ret = ::std::make_shared<TreeNode>("event port");
		}

		ret->setNodeKind(NodeKind::TypeK);
		ret->setKind(TypeKind::PortType);
	}

	return ret;
}

NodePtr analysis::IOType()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in IOType " << ::std::endl;
#endif
	NodePtr ret;
	if ( tmp->getType() == TYPE::OUT)
	{
		match("out");
		ret = ::std::make_shared<TreeNode>("out");
	} else 
	{
		match("in");
		if ( tmp->getType() == TYPE::OUT)
		{
			match("out");
			ret = ::std::make_shared<TreeNode>("in out");
		} else 
		{
			ret = ::std::make_shared<TreeNode>("in");
		}
	}
	ret->setNodeKind(NodeKind::TypeK);
	ret->setKind(TypeKind::IOType);

	return ret;
}

NodePtr analysis::flowSpec()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in flowSpec " << ::std::endl;
#endif

	if ( tmp->getType() == TYPE::NONE)
	{
		match("none");
		NodePtr ret = ::std::make_shared<TreeNode>("none");
		ret->setNodeKind(NodeKind::SpecK);
		ret->setKind(SpecKind::NoneK);

		match(";");
		return ret;
	}

	::std::string tmp_str = identifier();
	NodePtr ret = ::std::make_shared<TreeNode>(tmp_str);
	NodePtr tmp_ptr = nullptr;

	ret->setNodeKind(NodeKind::SpecK);

	match(":");

	match("flow");

	if ( tmp->getType() == TYPE::SOURCE)
	{
		match("source");
		ret->setKind(SpecKind::flowSourceK);
		tmp_str = identifier();

		tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
		tmp_ptr->setNodeKind(NodeKind::IDK);

		ret->appendChild(tmp_ptr);

		if ( tmp->getVal() == "{")
		{
			match("{");
			tmp_ptr = association();
			NodePtr tmp_cur_ptr = tmp_ptr;

			while( tmp->getType() == TYPE::IDENTIFIER)
			{
				NodePtr tmp_in_ptr = association();
				tmp_cur_ptr->setSibling(tmp_in_ptr);
				tmp_cur_ptr = tmp_in_ptr;
			}

			match("}");
			ret->appendChild(tmp_ptr);
		}
	} else if ( tmp->getType() == TYPE::SINK)
	{
		match("sink");
		ret->setKind(SpecKind::flowSinkK);
		tmp_str = identifier();

		tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
		tmp_ptr->setNodeKind(NodeKind::IDK);

		ret->appendChild(tmp_ptr);

		if ( tmp->getVal() == "{")
		{
			match("{");
			tmp_ptr = association();
			NodePtr tmp_cur_ptr = tmp_ptr;

			while( tmp->getType() == TYPE::IDENTIFIER)
			{
				NodePtr tmp_in_ptr = association();
				tmp_cur_ptr->setSibling(tmp_in_ptr);
				tmp_cur_ptr = tmp_in_ptr;
			}

			match("}");
			ret->appendChild(tmp_ptr);
		}
	} else
	{
		match("path");
		ret->setKind(SpecKind::flowPathK);
		tmp_str = identifier();

		tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
		ret->appendChild(tmp_ptr);

		match("->");

		tmp_str = identifier();
		tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
		ret->appendChild(tmp_ptr);
	}

	match(";");
	return ret;
}

NodePtr analysis::association()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in association " << ::std::endl;
#endif

	if ( tmp->getType() == TYPE::NONE)
	{
		match("none");
		NodePtr ret = ::std::make_shared<TreeNode>("none");
		ret->setNodeKind( NodeKind::AssociationK);

		return ret;
	}

	NodePtr tmp_ptr;
	::std::string tmp_str;
	tmp_str = identifier();

	NodePtr ret = ::std::make_shared<TreeNode>(tmp_str);
	ret->setNodeKind(NodeKind::AssociationK);

	if ( tmp->getVal() == "::")
	{
		match("::");
		tmp_str = identifier();
		tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
		tmp_ptr->setNodeKind(NodeKind::IDK);

		ret->appendChild(tmp_ptr);
	}

	if ( tmp->getVal() == "=>")
	{
		match("=>");
	} else 
	{
		match("+=>");
	}

	if ( tmp->getType() == TYPE::CONSTANT)
	{
		match("constant");
	}

	match("access");

	double tmp_num = decimal();
	tmp_ptr = ::std::make_shared<TreeNode>(tmp_num);
	tmp_ptr->setNodeKind(NodeKind::ConstK);

	ret->appendChild(tmp_ptr);

	return ret;
}

NodePtr analysis::reference()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in reference " << ::std::endl;
#endif

	::std::vector< ::std::string > sv;
	::std::string tmp_str = identifier();
	NodePtr tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);
	NodePtr ret;

	tmp_ptr->setNodeKind(NodeKind::IDK);
	if ( tmp->getVal() == "::")
	{
		match("::");
		tmp_ptr->setNodeKind( NodeKind::PackageK);
	}

	NodePtr tmp_cur_ptr = tmp_ptr;
	while( tmp->getType() == TYPE::IDENTIFIER)
	{
		tmp_str =  identifier();
		NodePtr tmp_in_ptr = ::std::make_shared<TreeNode>(tmp_str);

		if ( tmp->getVal() == "::")
		{
			match("::");
			tmp_in_ptr->setNodeKind(NodeKind::PackageK);

			tmp_cur_ptr->setSibling(tmp_in_ptr);
			tmp_cur_ptr = tmp_in_ptr;
		} else 
		{
			ret = tmp_in_ptr;
			ret->appendChild(tmp_ptr);
			ret->setNodeKind(NodeKind::ReferenceK);
			break;
		}
	}
	return ret;
}
void analysis::init()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in init" << ::std::endl;
#endif
	*tmp = _tokens.getToken();
	this->_root = ThreadSpec();

	NodePtr tmp_cur_ptr = this->_root;
	while ( tmp->getType() == TYPE::THREAD)
	{
		NodePtr tmp_in_ptr = ThreadSpec();
		tmp_cur_ptr->setSibling(tmp_in_ptr);
		tmp_cur_ptr = tmp_in_ptr;
	}
	initFlag = true;
}

void analysis::printTree()
{
	this->init();
	assert(initFlag == true);

	printTree(this->_root);
}

void analysis::printTree( const NodePtr& ptr)
{
	::std::ofstream outf("../SyntaxOut.txt", std::ios::app);

	for ( int i = 0; i < tabcounter; ++i)
	{
		outf << "  ";
	}

	::boost::variant< ::std::string, int> tmp = ptr->getData();

	outf << ::boost::apply_visitor( get_visitor(), tmp) << ::std::endl;

	outf.close();

	for (auto i : ptr->getChildren())
	{
		tabcounter++;
		printTree(i);
		--tabcounter;
	}

	if ( ptr->getSibling() != nullptr)
	{
		printTree(ptr->getSibling());
	}
}

NodePtr analysis::getRoot()
{
	return this->_root;
}

}
