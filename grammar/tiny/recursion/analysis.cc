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
	this->tmp = ::std::make_shared<token>("", TYPE::NONE, 0);
	this->tmp_name_counter = 0;
	initFlag = false;
	genFlag = false;

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
		::std::cerr << "[ERROR] Syntax Error near \"" << tmp->getVal() 
			<< "\" in line " << tmp->getLineno()
			<< ::std::endl;
		::std::exit(1);
	}
}

int analysis::number()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in number " << ::std::endl;
#endif
	try {
		if ( tmp->getType() == TYPE::NUM)
		{
			::std::stringstream num_stream;
			int num_tmp = 0;
			
			num_stream << tmp->getVal();
			num_stream >> num_tmp;

			*tmp = _tokens.getToken();

			return num_tmp;
		} else
		{
			::std::cerr << "[ERROR] Syntax Error near \"" << tmp->getVal() 
				<< "\" in line " << tmp->getLineno()
				<< ::std::endl;
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

	if ( tmp->getType() == TYPE::ID)
	{
		::std::string ret = tmp->getVal();
		
		*tmp = _tokens.getToken();
		return ret;
	} else 
	{
		::std::cerr << "[ERROR] Syntax Error near \"" << tmp->getVal() 
			<< "\" in line " << tmp->getLineno()
			<< ::std::endl;
		::std::exit(1);
	}
}

NodePtr analysis::program()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in program\n";
#endif
	NodePtr ret;
	ret = stmt_sequence();	

	return ret;
}

NodePtr analysis::stmt_sequence()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in stmt_sequence\n";
#endif

	NodePtr ret;
	NodePtr cur_ptr;

	ret = statement();
	cur_ptr = ret;
	while(1)
	{
		if ( tmp->getVal() == "" 
				|| tmp->getVal() == "end" 
				|| tmp->getVal() == "else"
				|| tmp->getVal() == "until")
		{
			break;
		}

		if (tmp->getVal() != ";")
		{
			::std::cout << "[ERROR] Expected Symbol: \";\" while get \""
				<< tmp->getVal()
				<< "\""
				<< "in line " << tmp->getLineno()
			   	<< ::std::endl;
			::std::exit(1);
		}
		NodePtr tmp_ptr;

		match(";");

		tmp_ptr = statement();
		cur_ptr->setSibling(tmp_ptr);

		cur_ptr = tmp_ptr;
	}
	return ret;
}

NodePtr analysis::statement()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in statement\n";
#endif

	NodePtr ret;

	if ( tmp->getVal() == "if")
	{
		ret = if_stmt();

	} else if ( tmp->getVal() == "repeat")
	{
		ret = repeat_stmt();
		
	} else if ( tmp->getType() == TYPE::ID)
	{
		ret = assgin_stmt();

	} else if ( tmp->getVal() == "read")
	{
		ret = read_stmt();

	} else if ( tmp->getVal() == "write")
	{
		ret = write_stmt();
		
	} else 
	{
		::std::cerr << "[ERROR] Syntax error near \"" << tmp->getVal() 
			<< "\""
			<< " in line "
			<< tmp->getLineno()
			<< ::std::endl;
		::std::exit(1);
	}

	return ret;
}

NodePtr analysis::if_stmt()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in if_stmt\n";
#endif

	NodePtr ret = ::std::make_shared<TreeNode>("if");
	NodePtr tmp_ptr;

	ret->setNodeKind(NodeKind::StmK);
	ret->setKind(StmtKind::IfK);

	match("if");

	tmp_ptr = exp();
	ret->appendChild(tmp_ptr);

	match("then");

	tmp_ptr = stmt_sequence();
	ret->appendChild(tmp_ptr);

	if ( tmp->getVal() == "else")
	{
		match("else");

		tmp_ptr = stmt_sequence();
		ret->appendChild(tmp_ptr);
	}

	match("end");

	return ret;
}

NodePtr analysis::repeat_stmt()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in repeat_stmt\n";
#endif

	NodePtr ret;
	NodePtr tmp_ptr;
	ret = ::std::make_shared<TreeNode>("repeat");
	ret->setNodeKind(NodeKind::StmK);
	ret->setKind(StmtKind::RepeatK);

	match("repeat");

	tmp_ptr = stmt_sequence();
	ret->appendChild(tmp_ptr);

	match("until");

	tmp_ptr = exp();
	ret->appendChild(tmp_ptr);

	return ret;
}

NodePtr analysis::assgin_stmt()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in assgin_stmt\n";
#endif

	NodePtr ret;
	NodePtr tmp_ptr;
	::std::string tmp_str = identifier();

	ret = ::std::make_shared<TreeNode>(tmp_str);
	ret->setNodeKind(NodeKind::StmK);
	ret->setKind(StmtKind::AssignK);

	match(":=");

	tmp_ptr = exp();
	ret->appendChild(tmp_ptr);

	return ret;
}

NodePtr analysis::read_stmt()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in read_stmt\n";
#endif

	NodePtr ret = ::std::make_shared<TreeNode>("read");
	NodePtr tmp_ptr;

	ret->setNodeKind(NodeKind::StmK);
	ret->setKind(StmtKind::ReadK);

	match("read");
	
	::std::string tmp_str = identifier();

	tmp_ptr = ::std::make_shared<TreeNode>(tmp_str);

	tmp_ptr->setNodeKind(NodeKind::ExpK);
	tmp_ptr->setKind(ExpKind::IdK);
	tmp_ptr->setType(ExpType::Void);

	ret->appendChild(tmp_ptr);

	return ret;
}

NodePtr analysis::write_stmt()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in write_stmt\n";
#endif

	NodePtr ret = ::std::make_shared<TreeNode>("write");
	NodePtr tmp_ptr;

	ret->setNodeKind(NodeKind::StmK);
	ret->setKind(StmtKind::WriteK);

	match("write");

	tmp_ptr = exp();

	ret->appendChild(tmp_ptr);
	return ret;
}

NodePtr analysis::exp()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in exp\n";
#endif
	NodePtr tmp_ptr = simple_exp();
	NodePtr ret;

	if ( tmp->getVal() == "<")
	{
		match("<");

		ret = ::std::make_shared<TreeNode>("<");
		ret->setNodeKind( NodeKind::ExpK);
		ret->setKind(ExpKind::OpK);
		ret->setType(ExpType::Boolean);

		ret->appendChild(tmp_ptr);

		tmp_ptr = simple_exp();
		ret->appendChild(tmp_ptr);

	} else if ( tmp->getVal() == "=")
	{
		match("=");

		ret = ::std::make_shared<TreeNode>("=");
		ret->setNodeKind( NodeKind::ExpK);
		ret->setKind(ExpKind::OpK);
		ret->setType(ExpType::Boolean);

		ret->appendChild(tmp_ptr);

		tmp_ptr = simple_exp();
		ret->appendChild(tmp_ptr);
	} else
	{
		ret = tmp_ptr;
	}

	return ret;
}

NodePtr analysis::simple_exp()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in simple exp\n";
#endif
	NodePtr ret;
	NodePtr tmp_ptr = term();

	if ( tmp->getVal() != "+" && tmp->getVal() != "-")
	{
		return tmp_ptr;
	}

	while ( tmp->getVal() == "+" || tmp->getVal() == "-")
	{
		if ( tmp->getVal() == "+")
		{
			match("+");

			ret = ::std::make_shared<TreeNode>("+");

			ret->setNodeKind(NodeKind::ExpK);
			ret->setKind(ExpKind::OpK);
			ret->setType(ExpType::Integer);

			ret->appendChild(tmp_ptr);

			tmp_ptr = simple_exp();
			ret->appendChild(tmp_ptr);
		} else if ( tmp->getVal() == "-")
		{
			match("-");

			ret = ::std::make_shared<TreeNode>("-");

			ret->setNodeKind(NodeKind::ExpK);
			ret->setKind(ExpKind::OpK);
			ret->setType(ExpType::Integer);

			ret->appendChild(tmp_ptr);

			tmp_ptr = simple_exp();
			ret->appendChild(tmp_ptr);
		}
	}

	return ret;
}

NodePtr analysis::term()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in term\n";
#endif
	NodePtr ret;
	NodePtr tmp_ptr = factor();

	if ( tmp->getVal() != "/" && tmp->getVal() != "*")
	{
		return tmp_ptr;
	}

	while ( tmp->getVal() == "/" || tmp->getVal() == "*")
	{
		if ( tmp->getVal() == "/")
		{
			match("/");

			ret = ::std::make_shared<TreeNode>("/");

			ret->setNodeKind(NodeKind::ExpK);
			ret->setKind(ExpKind::OpK);
			ret->setType(ExpType::Integer);

			ret->appendChild(tmp_ptr);

			tmp_ptr = factor();
			ret->appendChild(tmp_ptr);
		} else if ( tmp->getVal() == "*")
		{
			match("*");

			ret = ::std::make_shared<TreeNode>("*");

			ret->setNodeKind(NodeKind::ExpK);
			ret->setKind(ExpKind::OpK);
			ret->setType(ExpType::Integer);

			ret->appendChild(tmp_ptr);

			tmp_ptr = factor();
			ret->appendChild(tmp_ptr);
		}
	}
	return ret;
}

NodePtr analysis::factor()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in factor\n";
#endif

	NodePtr ret;
	if ( tmp->getVal() == "+")
	{
		match("+");
		int tmp_num = number();
		ret = ::std::make_shared<TreeNode>(tmp_num);

		ret->setNodeKind(NodeKind::ExpK);
		ret->setKind(ExpKind::ConstK);
		ret->setType(ExpType::Integer);
		return ret;
	} else if ( tmp->getVal() == "-")
	{
		match("-");
		int tmp_num = number();
		tmp_num *= -1;
		ret = ::std::make_shared<TreeNode>(tmp_num);

		ret->setNodeKind(NodeKind::ExpK);
		ret->setKind(ExpKind::ConstK);
		ret->setType(ExpType::Integer);
		return ret;
	}

	if ( tmp->getVal() == "(")
	{
		match("(");

		ret = exp();
	} else if ( tmp->getType() == TYPE::NUM)
	{
		int tmp_num = number();
		ret = ::std::make_shared<TreeNode>(tmp_num);

		ret->setNodeKind(NodeKind::ExpK);
		ret->setKind(ExpKind::ConstK);
		ret->setType(ExpType::Integer);

	} else if ( tmp->getType() == TYPE::ID)
	{
		::std::string tmp_str = identifier();
		ret = ::std::make_shared<TreeNode>(tmp_str);

		ret->setNodeKind(NodeKind::ExpK);
		ret->setKind(ExpKind::IdK);
		ret->setType(ExpType::Void);

	}
	return ret;
}

auto analysis::getRoot()
{
	return _root;
}

void analysis::initSynTree()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in init" << ::std::endl;
#endif
	*tmp = _tokens.getToken();
	this->_root = program();
	initFlag = true;
}

void analysis::buildSymTab()
{
	this->initSynTree();
	assert(initFlag == true);

	buildSymTab(_root);

#ifdef _DEBUG_
	for ( auto it = symTab.begin(); it != symTab.end(); ++it)
	{
		::std::cout << "[Key] " << it->first
		<< " [value] " << it->second
		<< ::std::endl;
	}
#endif
}

void analysis::printTree( const NodePtr& ptr) const
{
	::boost::variant< ::std::string, int> tmp = ptr->getData();
	::std::cout << ::boost::apply_visitor( get_visitor(), tmp) << ::std::endl;

	for (auto i : ptr->getChildren())
	{
		printTree(i);
	}

	if ( ptr->getSibling() != nullptr)
	{
		printTree(ptr->getSibling());
	}
}

void analysis::EvalType( const NodePtr& ptr)
{
	if ( ptr->getNodeKind() == NodeKind::StmK)
	{
		if ( ptr->getKind() == StmtKind::ReadK)
		{
			tmp_dType = ExpType::Integer;
			EvalType(ptr->getChildren().at(0));
		} else if ( ptr->getKind() == StmtKind::AssignK)
		{
			EvalType(ptr->getChildren().at(0));
			auto tmp_data = ptr->getData();
			auto tmp_str = ::boost::apply_visitor( get_visitor(), tmp_data);
			auto it = symTab.find(tmp_str);
			if ( it == symTab.end())
			{
				symTab[tmp_str] = tmp_dType;
			} else 
			{
				if ( tmp_dType > symTab[tmp_str])
				{
					symTab[tmp_str] = tmp_dType;
				} else 
				{
					tmp_dType = symTab[tmp_str];
				}
			}
		} else if ( ptr->getKind() == StmtKind::RepeatK)
		{
			EvalType(ptr->getChildren().at(0));
			EvalType(ptr->getChildren().at(1));
		} else if ( ptr->getKind() == StmtKind::IfK)
		{
			EvalType(ptr->getChildren().at(0));
			EvalType(ptr->getChildren().at(1));
			if ( ptr->getChildSize() == 3)
			{
				EvalType(ptr->getChildren().at(2));
			}
		}
	} else if ( ptr->getNodeKind() == NodeKind::ExpK)
	{
		if ( ptr->getKind() == ExpKind::ConstK)
		{
			tmp_dType = ExpType::Integer;
		} else if ( ptr->getKind() == ExpKind::IdK)
		{
			auto tmp_data = ptr->getData();
			auto tmp_str = ::boost::apply_visitor( get_visitor(), tmp_data);
			auto it = symTab.find(tmp_str);
			if ( it == symTab.end())
			{
				symTab[tmp_str] = tmp_dType;
			} else 
			{
				if ( tmp_dType > symTab[tmp_str])
				{
					symTab[tmp_str] = tmp_dType;
				} else {
					tmp_dType = symTab[tmp_str];
				}
			}
		} else if ( ptr->getKind() == ExpKind::OpK)
		{
			auto tmp_data = ptr->getData();
			auto tmp_str = ::boost::apply_visitor( get_visitor(), tmp_data);

			if ( tmp_str == "<" || tmp_str == "=")
			{
				tmp_dType = ExpType::Boolean;
			} else 
			{
				EvalType(ptr->getChildren().at(0));
				EvalType(ptr->getChildren().at(1));
			}
		}
	}
}

void analysis::buildSymTab( const NodePtr& ptr)
{
	auto tmp_ptr = ptr;
	while( tmp_ptr != nullptr)
	{
		EvalType(tmp_ptr);
		tmp_ptr = tmp_ptr->getSibling();
	}
}

::std::string analysis::newtmpVal()
{
	::std::stringstream ss;

	ss << tmp_name_counter++;

	symTab["@"+ss.str()] = ExpType::Integer;

	return "@" + ss.str();
}

::std::string analysis::newtmpLab()
{
	::std::stringstream ss;

	ss << tmp_name_counter++;

	return ".L" + ss.str();

}

void analysis::genMidCode( const NodePtr& ptr)
{
	if ( ptr == nullptr)
	{
		return;
	}

	if ( ptr->getNodeKind() == NodeKind::ExpK)
	{
		if ( ptr->getKind() != ExpKind::OpK)
		{
			::std::stringstream ss;
			auto tmp_data = ptr->getData();

			ptr->setStrVal( ::boost::apply_visitor( get_visitor(), tmp_data));
		} else 
		{
			genMidCode( ptr->getChildren().at(0));
			genMidCode( ptr->getChildren().at(1));

			auto tmp_data = ptr->getData();
			auto tmp_str = ::boost::apply_visitor(get_visitor(), tmp_data);
			auto tmp_name = newtmpVal();

			ptr->setStrVal(tmp_name);

			midcodes.push_back(trival(tmp_name, 
						tmp_str, 
						ptr->getChildren().at(0)->getStrVal(), 
						ptr->getChildren().at(1)->getStrVal()));
		}
	} else 
	{
		if ( ptr->getKind() == StmtKind::AssignK)
		{
			auto tmp_data = ptr->getData();
			auto tmp_str = ::boost::apply_visitor(get_visitor(), tmp_data);

			genMidCode(ptr->getChildren().at(0));
			midcodes.push_back(trival(tmp_str, 
						":=", 
						ptr->getChildren().at(0)->getStrVal()));
		} else if ( ptr->getKind() == StmtKind::WriteK)
		{
			genMidCode(ptr->getChildren().at(0));
			midcodes.push_back(trival("write", 
						ptr->getChildren().at(0)->getStrVal()));

		} else if ( ptr->getKind() == StmtKind::ReadK)
		{
			genMidCode(ptr->getChildren().at(0));
			midcodes.push_back(trival("read", 
						ptr->getChildren().at(0)->getStrVal()));
		} else if ( ptr->getKind() == StmtKind::IfK)
		{
			auto tmp_lab = newtmpLab();
			auto tmp_lab2 = newtmpLab();
			genMidCode(ptr->getChildren().at(0));
			midcodes.push_back(trival("cmp", midcodes.at(midcodes.size()-1).getRes()));
			midcodes.push_back(trival("jne", tmp_lab));
			genMidCode(ptr->getChildren().at(1));
			midcodes.push_back(trival("jmp",tmp_lab2));
			midcodes.push_back(trival("label", tmp_lab));

			if ( ptr->getChildSize() == 3)
			{
				genMidCode(ptr->getChildren().at(2));
			}
			midcodes.push_back(trival("label", tmp_lab2));
		} else if ( ptr->getKind() == StmtKind::RepeatK)
		{
			auto tmp_lab = newtmpLab();
			midcodes.push_back(trival("label", tmp_lab));
			genMidCode(ptr->getChildren().at(0));
			genMidCode(ptr->getChildren().at(1));
			midcodes.push_back(trival("jne", tmp_lab));
		}

		genMidCode(ptr->getSibling());
	}
}

void analysis::genMidCode()
{
	genMidCode(_root);
#ifdef _DEBUG_
	for( auto x : midcodes)
	{
		int size = x.getSize();
		switch(size)
		{
			case 1:
				::std::cout << "[Operand] " << x.getOperand();
				::std::cout << " [Val1] " << x.getVal1();
				::std::cout << ::std::endl;
				break;
			case 2:
				::std::cout << "[Res] " << x.getRes();
				::std::cout << " [Operand] " << x.getOperand();
				::std::cout << " [Val1] " << x.getVal1();
				::std::cout << ::std::endl;
				break;
			case 3:
				::std::cout << "[Res] " << x.getRes();
				::std::cout << " [Operand] " << x.getOperand();
				::std::cout << " [Val1] " << x.getVal1();
				::std::cout << " [Val2] " << x.getVal2();
				::std::cout << ::std::endl;
				break;
		}
	}
#endif
	genFlag = true;
}

void analysis::exportMidCode( const ::std::string path)
{
	if ( !genFlag)
	{
		::std::cerr << "[ERROR] Not genMidCode" << ::std::endl;
		return;
	}
	::std::ofstream outf(path);

	for( auto x : midcodes)
	{
		int size = x.getSize();
		switch(size)
		{
			case 1:
				outf << "[Operand] ";
				outf << x.getOperand();
				outf << " [Val1] " ;
				outf << x.getVal1();
				outf << ::std::endl;
				break;
			case 2:
				outf << "[Res] " << x.getRes();
				outf << " [Operand] " << x.getOperand();
				outf << " [Val1] " << x.getVal1();
				outf << ::std::endl;
				break;
			case 3:
				outf << "[Res] " << x.getRes();
				outf << " [Operand] " << x.getOperand();
				outf << " [Val1] " << x.getVal1();
				outf << " [Val2] " << x.getVal2();
				outf << ::std::endl;
				break;
		}
	}

}
}
