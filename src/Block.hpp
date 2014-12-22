#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "Statement.hpp"


struct Declarator;
struct Function;

class Block : public Statement{  
public:
  Block() {
  }

  virtual std::string get_code() override;


  void add_declaration(std::vector<Declarator*> fList);
  void add_function(Function* fFunction);


private:
  std::map<std::string, Declarator*> variables;
  std::map<std::string, Function*> functions;
};


#endif
