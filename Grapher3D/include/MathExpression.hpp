#pragma once

#include <ostream>
#include <variant>
#include <vector>
#include <string>

#include <unordered_map>

enum class OPERATOR
{
	ROOT_NODE,
	ADD,
	SUB,
	MUL,
	DIV,
	EXP,
	POW,
	MOD,
	ABS,
	SQRT,
	SIN,
	COS,
	TAN,
	ASIN,
	ACOS,
	ATAN,
	ATAN2,
	SEC,
	CSC,
	COT,


	OPERATOR_COUNT // should always be at end 
};

inline std::ostream& operator<<(std::ostream& out, const OPERATOR& in)
{
	switch (in)
	{
	case OPERATOR::ROOT_NODE:		out << "ROOT";		break;
	case OPERATOR::ADD:				out << "ADD";		break;
	case OPERATOR::SUB:				out << "SUB";		break;
	case OPERATOR::MUL:				out << "MUL";		break;
	case OPERATOR::DIV:				out << "DIV";		break;
	case OPERATOR::EXP:				out << "EXP";		break;
	case OPERATOR::POW:				out << "POW";		break;
	case OPERATOR::MOD:				out << "MOD";		break;
	case OPERATOR::ABS:				out << "ABS";		break;
	case OPERATOR::SQRT:			out << "SQRT";		break;
	case OPERATOR::SIN:				out << "SIN";		break;
	case OPERATOR::COS:				out << "COS";		break;
	case OPERATOR::TAN:				out << "TAN";		break;
	case OPERATOR::ASIN:			out << "ASIN";		break;
	case OPERATOR::ACOS:			out << "ACOS";		break;
	case OPERATOR::ATAN:			out << "ATAN";		break;
	case OPERATOR::ATAN2:			out << "ATAN2";		break;
	case OPERATOR::SEC:				out << "SEC";		break;
	case OPERATOR::CSC:				out << "CSC";		break;
	case OPERATOR::COT:				out << "COT";		break;
	case OPERATOR::OPERATOR_COUNT:	out << "ERROR";		break;
	}
	return out;
}

class MathExpression
{
private:
	struct ExpressionTree
	{
		struct Child;

		OPERATOR m_op;
		std::vector<Child> children;

		ExpressionTree();
		ExpressionTree(OPERATOR op);
		ExpressionTree(OPERATOR op, Child left, Child right);

		float eval(const std::unordered_map<std::string, float>& vars);

		std::string str();
	};
	struct ExpressionTree::Child
	{
		enum class Type : uint8_t
		{
			TREE,
			VAR,
			NUM
		};

		std::variant<ExpressionTree, std::string, float> value;
		Type type;

		inline Child(ExpressionTree value)
			: value(value), type(Type::TREE) {}
		inline Child(std::string value)
			: value(value), type(Type::VAR) {}
		inline Child(float value)
			: value(value), type(Type::NUM) {}

		inline float eval(const std::unordered_map<std::string, float>& vars)
		{
			switch (type)
			{
			case Type::TREE:
				return std::get<ExpressionTree>(value).eval(vars);
			case Child::Type::VAR:
				return vars.at(std::get<std::string>(value));
			case Type::NUM:
				return std::get<float>(value);
			}
		}
	};
	
	ExpressionTree m_exprTree;

public:

	MathExpression();
	MathExpression(const std::string& expr);

	operator std::string();

	float eval(const std::unordered_map<std::string, float>& vars);
};

