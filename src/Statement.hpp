#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>

class Block;
struct Function;

class Statement{
public:

  // fParent_block : liste des blocs parent
  // fFunction : function appelante
  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) {
    throw std::logic_error("internal error : Statement:get_code() can't be called");
    return "";
  };

  void add_statement(Statement* fStatement){
    for (Statement* it : fStatement->statements){
      statements.push_back(it);
    }
  }

  void add_statement(std::vector<Statement*> fStatement){
    for (Statement* it : fStatement){
      statements.push_back(it);
    }
  }


protected:
  std::vector<Statement*> statements;

};




#endif
