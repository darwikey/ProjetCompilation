#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"


class Assignment : public Expression{
public:

  // pour les variables
  // fIdentifier = fExpression
  Assignment(std::string fIdentifier, Expression* fExpression) : identifier(fIdentifier),
								 expression(fExpresion),
								 index(nullptr){
  }
  
  // Pour les tableaux
  // fIdentifier[fIndex] = fExpression
  Assignment(std::string fIdentifier, Expression* fIndex, Expression* fExpression) : identifier(fIdentifier),
								 expression(fExpresion),
								 index(fIndex){
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    return "";
  }


private:
  std::string identifier;
  Expression* index, expression;

};



#endif
