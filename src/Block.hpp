#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "Statement.hpp"


struct Declarator;

/* block de declaration et d'instruction
   un nouveau block est creer dans chaque {...}*/

class Block : public Statement{  
public:
  Block() {
  }

  virtual std::string get_code() override;


  void add_declaration(std::vector<Declarator*> fList);


protected:
  std::map<std::string, Declarator*> variables;
};


#endif
