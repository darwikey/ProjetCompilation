#ifndef JUMP_HPP
#define JUMP_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Statement.hpp"


class Jump : public Statement{
public:


  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    return "";
  }


private:


};



#endif
