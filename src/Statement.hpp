#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>


class Statement{
public:
  virtual std::string get_code();

  void add_statement(Statement* fStatement){
    for (Statement* it : fStatement->statements){
      statements.push_back(it);
    }
  }


private:
  std::vector<Statement*> statements;

};




#endif
