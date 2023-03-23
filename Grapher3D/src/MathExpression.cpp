#include "MathExpression.hpp"

#include <sstream>

typedef float (*OperatorFunc)(std::vector<float, std::allocator<float>>);
#define OPERATOR_FUNCTION_DEFINITION(oper) float op_##oper(std::vector<float> vals)

OPERATOR_FUNCTION_DEFINITION(ROOT_NODE)
{
	if (vals.size() == 1)
	{
		return vals[0];
	}
	else
	{
		throw std::runtime_error(std::string("ROOT_NODE had incorrect number of children (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(ADD)
{
	if (vals.size() > 1)
	{
		float sum = 0.0f;
		for (float& val : vals)
		{
			sum += val;
		}
		return sum;
	}
	else
	{
		throw std::runtime_error(std::string("ADD had incorrect number of inputs (expected >1 but got ") + std::to_string(vals.size()) + ")");
	}
	
}
OPERATOR_FUNCTION_DEFINITION(SUB)
{
	if (vals.size() > 1)
	{
		float sum = 0.0f;
		for (float& val : vals)
		{
			sum -= val;
		}
		return sum;
	}
	else
	{
		throw std::runtime_error(std::string("ADD had incorrect number of inputs (expected >1 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(MUL)
{
	if (vals.size() > 1)
	{
		float prod = 1.0f;
		for (float& val : vals)
		{
			prod *= val;
		}
		return prod;
	}
	else
	{
		throw std::runtime_error(std::string("MUL had incorrect number of inputs (expected >1 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(DIV)
{
	throw std::logic_error("Operator (DIV) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(EXP)
{
	throw std::logic_error("Operator (EXP) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(MOD)
{
	throw std::logic_error("Operator (MOD) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(ABS)
{
	throw std::logic_error("Operator (ABS) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(SQRT)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("SQRT had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return sqrt(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(SIN)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string(" had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		throw std::runtime_error(std::string("MUL had incorrect number of inputs (expected >1 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(COS)
{
	throw std::logic_error("Operator (COS) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(TAN)
{
	throw std::logic_error("Operator (TAN) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(ASIN)
{
	throw std::logic_error("Operator (ASIN) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(ACOS)
{
	throw std::logic_error("Operator (ACOS) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(ATAN)
{
	throw std::logic_error("Operator (ATAN) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(SEC)
{
	throw std::logic_error("Operator (SEC) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(CSC)
{
	throw std::logic_error("Operator (CSC) not implemented");
}
OPERATOR_FUNCTION_DEFINITION(COT)
{
	throw std::logic_error("Operator (COT) not implemented");
}

OperatorFunc OperatorImplementations[int(OPERATOR::OPERATOR_COUNT)] = {
	&op_ROOT_NODE,
	&op_ADD,
	&op_SUB,
	&op_MUL,
	&op_DIV,
	&op_EXP,
	&op_MOD,
	&op_ABS,
	&op_SQRT,
	&op_SIN,
	&op_COS,
	&op_TAN,
	&op_ASIN,
	&op_ACOS,
	&op_ATAN,
	&op_SEC,
	&op_CSC,
	&op_COT
};

MathExpression::ExpressionTree::ExpressionTree()
	: m_op(OPERATOR::ROOT_NODE), children()
{
}

MathExpression::ExpressionTree::ExpressionTree(OPERATOR op)
	: m_op(op), children()
{
}

MathExpression::ExpressionTree::ExpressionTree(OPERATOR op, Child left, Child right)
	: m_op(op), children()
{
	children.push_back(left);
	children.push_back(right);
}

float MathExpression::ExpressionTree::eval(const std::unordered_map<std::string, float>& vars)
{
	std::vector<float> childrenValues = std::vector<float>();
	childrenValues.reserve(children.size());
	for (Child& child : children)
	{
		childrenValues.push_back(child.eval(vars));
	}
	return OperatorImplementations[int(m_op)](childrenValues);
	//if (m_op == OPERATOR::ADD)
	//{
	//	float sum = 0.0f;
	//	for (Child& child : children)
	//	{
	//		sum += child.eval(vars);
	//	}
	//	return sum;
	//}
	//else if (m_op == OPERATOR::MUL)
	//{
	//	float prod = 1.0f;
	//	for (Child& child : children)
	//	{
	//		prod *= child.eval(vars);
	//	}
	//	return prod;
	//}
	//else
	//{
	//	return -1;
	//}
}

std::string MathExpression::ExpressionTree::str()
{
	std::stringstream rtn = std::stringstream();

	rtn << int(m_op);
	rtn << '(';
	for (const Child& child : children)
	{
		static ExpressionTree x;
		switch (child.type)
		{
		case Child::Type::TREE:
			x = std::get<ExpressionTree>(child.value);
			rtn << x.str();
			break;
		case Child::Type::VAR:
			rtn << std::get<std::string>(child.value);
			break;
		case Child::Type::NUM:
			rtn << std::get<float>(child.value);
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
	// TEMPORARY CONSTRUCTOR THAT MAKES THIS EXPRESSION: 2*(x+5)
	m_exprTree.m_op = OPERATOR::MUL;
	m_exprTree.children.push_back({ 2.0f });
	m_exprTree.children.push_back({ ExpressionTree(OPERATOR::ADD, { "x" }, { 5.0f }) });

//		 *
//		/ \
//	   2   +
//		  / \
//		  x 5
}

MathExpression::MathExpression(const std::string& expr)
	: m_exprTree()
{

}

MathExpression::operator std::string()
{
	return m_exprTree.str();
}

float MathExpression::eval(const std::unordered_map<std::string, float>& vars)
{
	return m_exprTree.eval(vars);
}


