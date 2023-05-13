#pragma once

#include <ostream>
#include <variant>
#include <vector>
#include <string>

#include <unordered_map>

enum class OPERATOR {
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

inline std::ostream& operator<<(std::ostream& out, const OPERATOR& in) {
  switch (in) {
  case OPERATOR::ROOT_NODE:       out << "ROOT";   break;     
  case OPERATOR::ADD:             out << "ADD";    break;
  case OPERATOR::SUB:             out << "SUB";    break;
  case OPERATOR::MUL:             out << "MUL";    break;
  case OPERATOR::DIV:             out << "DIV";    break;
  case OPERATOR::EXP:             out << "EXP";    break;
  case OPERATOR::POW:             out << "POW";    break;
  case OPERATOR::MOD:             out << "MOD";    break;
  case OPERATOR::ABS:             out << "ABS";    break;
  case OPERATOR::SQRT:            out << "SQRT";   break;
  case OPERATOR::SIN:             out << "SIN";    break;
  case OPERATOR::COS:             out << "COS";    break;
  case OPERATOR::TAN:             out << "TAN";    break;
  case OPERATOR::ASIN:            out << "ASIN";   break;
  case OPERATOR::ACOS:            out << "ACOS";   break;
  case OPERATOR::ATAN:            out << "ATAN";   break;
  case OPERATOR::ATAN2:           out << "ATAN2";  break;
  case OPERATOR::SEC:             out << "SEC";    break;
  case OPERATOR::CSC:             out << "CSC";    break;
  case OPERATOR::COT:             out << "COT";    break;
  case OPERATOR::OPERATOR_COUNT:  out << "ERROR";  break;
  }
  return out;
}

class MathExpression {
  
private:

  struct ExpressionTreeChild;
  
  struct ExpressionTree {
    OPERATOR m_op;
    std::vector<ExpressionTreeChild> children;

    ExpressionTree();
    ExpressionTree(OPERATOR op);
    ExpressionTree(OPERATOR op, ExpressionTreeChild left, ExpressionTreeChild right);

    float eval(const std::unordered_map<std::string, float>& vars);

    std::string str();
  };
  
  struct ExpressionTreeChild {
    enum class Type : uint8_t {
      TREE,
      VAR,
      NUM
    };

    std::variant<ExpressionTree, std::string, float> value;
    Type type;

    inline ExpressionTreeChild(ExpressionTree value)
      : value(value), type(Type::TREE) {}
    inline ExpressionTreeChild(std::string value)
      : value(value), type(Type::VAR) {}
    inline ExpressionTreeChild(float value)
      : value(value), type(Type::NUM) {}

    inline float eval(const std::unordered_map<std::string, float>& vars) {
      switch (type) {
      case Type::TREE:
	return std::get<ExpressionTree>(value).eval(vars);
      case ExpressionTreeChild::Type::VAR:
	return vars.at(std::get<std::string>(value));
      case Type::NUM:
	return std::get<float>(value);
      default:
	throw new std::runtime_error("invalid type for ExpressionTreeChild");
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

