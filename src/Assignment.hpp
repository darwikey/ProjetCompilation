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
								 expression(fExpression),
								 index(nullptr){
  }
  
  // Pour les tableaux
  // fIdentifier[fIndex] = fExpression
  Assignment(std::string fIdentifier, Expression* fIndex, Expression* fExpression) : identifier(fIdentifier),
								 expression(fExpression),
								 index(fIndex){
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;

    // cas d'un tableau
    if (index != nullptr){
      code += index->get_code(fParent_blocks, fFunction);
      // on empile le resultat
      code += "pushl %eax\n";//TODO FLOAT

      //Verif type
      if (index->get_expression_type(fParent_blocks) != Type::INT){
	throw std::logic_error("variable " + identifier + " can't be an index");
      }
    }

    Block* declaration_block = Expression::get_block(fParent_blocks, identifier);

    code += expression->get_code(fParent_blocks, fFunction);
    
    //Verif type
    Type expr_type = expression->get_expression_type(fParent_blocks);
    if (expr_type != Type::INT 
	&& expr_type != Type::FLOAT 
	&& expr_type != Type::POINTER){
      throw std::logic_error("variable " + identifier + " can't be used in an assignment");
    }

    if (index == nullptr){
      if (expr_type == Type::FLOAT){
	code += declaration_block->get_code_store_variable(identifier, "xmm0", expr_type);
      }
      else{
	code += declaration_block->get_code_store_variable(identifier, "eax", expr_type);
      }
    }
    else{
      if (expr_type == Type::FLOAT){
	code += "movl %xmm0, %xmm1 \n";
	//on recupère l'index
	code += "popl %xmm0\n";//TODO
	code += declaration_block->get_code_store_array(identifier, "xmm0", "xmm1", expr_type);
      }
      else{
	code += "movl %eax, %ecx \n";
	//on recupère l'index
	code += "popl %eax\n";
	code += declaration_block->get_code_store_array(identifier, "eax", "ecx", expr_type);
	}
    }

    return code;
  }

 
private:
  std::string identifier;
  Expression* expression;
  Expression* index;

};



#endif
