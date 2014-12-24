#ifndef MAIN_BLOCK_HPP
#define MAIN_BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "Block.hpp"


struct Declarator;
struct Function;

/* block principal contenant toute les variables globales et les declarations de fonctions*/

class Main_block : public Block{  
public:
  Main_block();

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override;

  // code source placé au debut du code assembleur
  std::string get_header();

  void add_function(Function* fFunction);


private:
  std::map<std::string, Function*> functions;
};


#endif
