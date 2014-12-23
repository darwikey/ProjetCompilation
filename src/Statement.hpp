#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>


class Statement{
public:

  virtual std::string get_code() {
    throw std::logic_error("internal error : Statement:get_code() can't be called");
    return "";
  };

  void add_statement(Statement* fStatement){
    for (Statement* it : fStatement->statements){
      statements.push_back(it);
    }
  }


private:
  std::vector<Statement*> statements;

};




#endif
