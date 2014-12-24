#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Statement.hpp"


class Expression : public Statement{
public:


  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    return "";
  }

  // retourne le bloc contenant une variable s'appelant fIdentifier
  // jete une exception si elle n'existe pas
  static Block* get_block(std::vector<Block*> fParent_blocks, std::string fIdentifier){
    Block* declaration_block = nullptr;
    for (Block* block : fParent_blocks){
      if (block->is_variable(fIdentifier)){
	declaration_block = block;
      }
    }
    
    if (declaration_block == nullptr){
      throw std::logic_error(fIdentifier + " not declared");
    }
    return declaration_block;
  }

private:


};



#endif
