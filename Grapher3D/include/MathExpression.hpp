#pragma once

#include <variant>
#include <vector>
#include <string>

#include <unordered_map>

//struct Variable
//{
//	std::string name;
//
//};

enum class Operator
{
	ROOT_NODE,
	ADD,
	SUB,
	MUL,
	DIV,
	EXP,
	SIN,
	COS,
	MOD,
	SQRT
};

class MathExpression
{
private:
	enum class ChildType
	{
		TREE,
		VAR,
		NUM
	};
	struct ExpressionTree;
	typedef std::pair<std::variant<ExpressionTree, std::string, float>, ChildType> ExpressionTreeChild;
	struct ExpressionTree
	{
		Operator m_op;
		std::vector<ExpressionTreeChild> children;

		ExpressionTree();
		ExpressionTree(Operator op);
		ExpressionTree(Operator op, ExpressionTreeChild left, ExpressionTreeChild right);
		//~ExpressionTree();
		//ExpressionTree(ExpressionTree& other) = delete;
		//ExpressionTree& operator=(ExpressionTree& other) = delete;
		//ExpressionTree& operator=(ExpressionTree&& other) = delete;

		float eval(std::unordered_map<std::string, float> vars);

		std::string str();
	};
	
	ExpressionTree m_exprTree;

public:

	MathExpression();
	MathExpression(const std::string& expr);

	operator std::string();

	float eval(std::unordered_map<std::string, float> vars);

//	2*(x+5)
//
//		mul
//		/ \
//	   2   +
//		  / \
//		  x 5
};

