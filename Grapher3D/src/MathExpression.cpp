#include "MathExpression.hpp"

#include <sstream>

MathExpression::ExpressionTree::ExpressionTree()
	: m_op(Operator::ROOT_NODE), children()
{
}

MathExpression::ExpressionTree::ExpressionTree(Operator op)
	: m_op(op), children()
{
}

MathExpression::ExpressionTree::ExpressionTree(Operator op, ExpressionTreeChild left, ExpressionTreeChild right)
	: m_op(op), children()
{
	children.push_back(left);
	children.push_back(right);
}

float MathExpression::ExpressionTree::eval(std::unordered_map<std::string, float> vars)
{
	static ExpressionTree x;

	if (m_op == Operator::ADD)
	{
		float sum = 0.0f;
		for (const ExpressionTreeChild& child : children)
		{
			switch (child.second)
			{
			case ChildType::TREE:
				x = std::get<ExpressionTree>(child.first);
				sum += x.eval(vars);
				break;
			case ChildType::VAR:
				sum += vars[std::get<std::string>(child.first)];
				break;
			case ChildType::NUM:
				sum += std::get<float>(child.first);
				break;
			}
		}
		return sum;
	}
	else if (m_op == Operator::MUL)
	{
		float prod = 1.0f;
		for (const ExpressionTreeChild& child : children)
		{
			switch (child.second)
			{
			case ChildType::TREE:
				x = std::get<ExpressionTree>(child.first);
				prod *= x.eval(vars);
				break;
			case ChildType::VAR:
				prod *= vars[std::get<std::string>(child.first)];
				break;
			case ChildType::NUM:
				prod *= std::get<float>(child.first);
				break;
			}
		}
		return prod;
	}
	else
	{
		return -1;
	}
}

std::string MathExpression::ExpressionTree::str()
{
	std::stringstream rtn = std::stringstream();

	rtn << int(m_op);
	rtn << '(';
	for (const ExpressionTreeChild& child : children)
	{
		static ExpressionTree x;
		switch (child.second)
		{
		case ChildType::TREE:
			x = std::get<ExpressionTree>(child.first);
			rtn << x.str();
			break;
		case ChildType::VAR:
			rtn << std::get<std::string>(child.first);
			break;
		case ChildType::NUM:
			rtn << std::get<float>(child.first);
			break;
		}
		rtn << ", ";
	}
	rtn << ')';

	return rtn.str();
}

MathExpression::MathExpression()
	: m_exprTree()
{
	// 2*(x+5)
	m_exprTree.m_op = Operator::MUL;
	m_exprTree.children.push_back({ 2.0f, ChildType::NUM });
	m_exprTree.children.push_back({ ExpressionTree(Operator::ADD, { "x", ChildType::VAR }, { 5.0f, ChildType::NUM }), ChildType::TREE });
}

MathExpression::MathExpression(const std::string& expr)
	: m_exprTree()
{

}

MathExpression::operator std::string()
{
	return m_exprTree.str();
}

float MathExpression::eval(std::unordered_map<std::string, float> vars)
{
	return m_exprTree.eval(vars);
}
