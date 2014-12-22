#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Statement.hpp"


class Selection : public Statement{
public:


  virtual std::string get_code() override {
    return "";
  }


private:


};



#endif
