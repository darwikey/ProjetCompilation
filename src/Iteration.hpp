#ifndef ITERATION_HPP
#define ITERATION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "Statement.hpp"


class Iteration : public Statement{
public:


  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    return "";
  }

  static std::string get_unique_label(){
    std::ostringstream str;
    str << "LOOP" << label_counter;

    label_counter++;
    return str.str();
  }


protected:
  // code à l'intérieur de la boucle
  Statement* body = nullptr;

private:
  static size_t label_counter;

};


#endif
