#include "include/scan.h"
#include "include/global.h"
#include "include/analysis.h"
#include "include/syntax_tree.h"
#include <exception>
#include <sstream>

namespace dh{

/*
 * G[exp]
 * exp->term{ addop term}
 * term->factor{ mulop term}
 * factor->(exp)|number
 * addop=+|-
 * mulop=*
 * number=[0-9]+
 */

analysis::analysis(scanner tokens)
{
	this->tokens = tokens;
	this->result = 0;
	tmp = ::boost::make_shared<token>("",TYPE::NONE);

	if ( !tokens.isScanned())
	{
		throw "[ERROR] scanner is not ready";
	}
}

int analysis::getResult( const ::std::unique_ptr<TreeNode>& root)
{
	int ret = 0;
	if ( root->getKind() == KIND::NumK)
	{
		ret = ::boost::get<int>(root->_data);
		::std::cout << ret << " ";
	} else if ( root->getKind() == KIND::OpK)
	{
		char c = ::boost::get< ::std::string >(root->_data).c_str()[0];
		::std::cout << c << " ";
		switch(c)
		{
			case '+':
				ret = getResult(root->_left_child) + getResult(root->_right_child);
				break;
			case '-':
				ret = getResult(root->_left_child) - getResult(root->_right_child);
				break;
			case '*':
				ret = getResult(root->_left_child) * getResult(root->_right_child);
				break;
			default:
				::std::cerr << "[ERROR] Unexpected Token: " << c << ::std::endl;
		}
	}
	return ret;
}

int analysis::getResult()
{
#ifdef _DEBUG_
	tokens.debug();
#endif

	*tmp = tokens.getToken();
	_root = lexp();
	
	::std::cout << "[MSG] Use Mid-order to print this tree" << ::std::endl;
	int ret = getResult( _root);
	::std::cout << ::std::endl;

	return ret;
}

void analysis::match( ::std::string c)
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in match"  << ::std::endl;
	::std::cout << "[get] "<< tmp->getVal() <<" [match] " << c << ::std::endl;
#endif
	if ( tmp->getVal() == c)
	{
		*tmp = tokens.getToken();
		return;
	}

	tokens.ungetToken();
	*tmp = tokens.getToken();
	::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
	::std::exit(1);
}

int analysis::number()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in number"  << ::std::endl;
#endif
	if ( tmp->getType() == TYPE::NUMBER)
	{
		::std::stringstream sstream;
		int ret = 0;

		sstream << tmp->getVal();
		sstream >> ret;
		*tmp = tokens.getToken();

		return ret;
	}

	tokens.ungetToken();
	*tmp = tokens.getToken();
	::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
	::std::exit(1);
}

::std::unique_ptr<TreeNode> analysis::lexp()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in lexp"  << ::std::endl;
#endif

	::std::unique_ptr<TreeNode> ret;
	::std::unique_ptr<TreeNode> tmp_node;

	match("(");
	if ( tmp->getType() != TYPE::MULOP && tmp->getType() != TYPE::ADDOP)
	{
		::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
		::std::exit(1);
	}

	if ( tmp->getVal() == "*")
	{
		match("*");
		ret = ::std::unique_ptr<TreeNode>(new TreeNode("*"));
			
	} else if ( tmp->getVal() == "+")
	{
		match("+");
		ret = ::std::unique_ptr<TreeNode>(new TreeNode("+"));
	} else if ( tmp->getVal() == "-")
	{
		match("-");
		ret = ::std::unique_ptr<TreeNode>(new TreeNode("-"));
	} 

	match(" ");
	tmp_node = lexpseq();
	ret->setLeft( tmp_node);
	match(" ");
	tmp_node = lexpseq();
	ret->setRight( tmp_node);


	match(")");

	return ret;
}

::std::unique_ptr<TreeNode> analysis::lexpseq()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in lexpseq"  << ::std::endl;
#endif

	::std::unique_ptr<TreeNode> ret;
	if ( tmp->getVal() == "(")
	{
		ret = lexp();
		return ret;
	}

	ret = ::std::unique_ptr<TreeNode>(new TreeNode(number()));
	return ret;
}
	

}
