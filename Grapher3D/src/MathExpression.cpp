#include "MathExpression.hpp"
#include <cmath>
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
		int count =0;
		for (float& val : vals)
		{
			if(count==0)
			{
				sum = 2*val;
			}
			sum -= val;
			count+=1;
		}
		return sum;
	}
	else
	{
		throw std::runtime_error(std::string("SUB had incorrect number of inputs (expected >1 but got ") + std::to_string(vals.size()) + ")");
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
	if(vals.size()==2)
	{
		int count =0;
		float sum = 0.0f;
		for (float& val : vals)
		{
			if(count==0)
			{
				sum = 2*val;
			}
			if(count==1)
			{
				sum /=val;
				return sum;
			}
			sum -= val;
			count+=1;
		}	
	}
	else
	{
		throw std::runtime_error(std::string("DIV had incorrect number of inputs (expected 2 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(EXP)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("SQRT had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return exp(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(MOD)
{
	if(vals.size()==2)
	{
		int count =0;
		float sum = 0.0f;
		for (float& val : vals)
		{
			if(count==0)
			{
				sum = 2*val;
			}
			if(count==1)
			{
				sum % val;
				return sum;
			}
			sum -= val;
			count+=1;
		}	
	}
	else
	{
		throw std::runtime_error(std::string("MOD had incorrect number of inputs (expected 2 but got ") + std::to_string(vals.size()) + ")");
	}
}
OPERATOR_FUNCTION_DEFINITION(ABS)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("ABS had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return abs(val);
		}
	}
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
		throw std::runtime_error(std::string("SIN had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return sin(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(COS)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("COS had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return cos(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(TAN)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("TAN had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return tan(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(ASIN)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("ASIN had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return asin(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(ACOS)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("ACOS had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return acos(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(ATAN)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("ATAN had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return atan(val);
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(SEC)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("SEC had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return (1/cos(val));
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(CSC)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("CSC had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return (1/sin(val));
		}
	}
}
OPERATOR_FUNCTION_DEFINITION(COT)
{
	if (vals.size() > 1)
	{
		throw std::runtime_error(std::string("COT had incorrect number of inputs (expected 1 but got ") + std::to_string(vals.size()) + ")");
	}
	else
	{
		for (float& val : vals)
		{
			return (1/tan(val));
		}
	}
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


