// -*- C++ -*-
#ifndef ROPE_H
#define ROPE_H

#include <utility> 
#include <string>

class rope {
  struct chunk;

  chunk* first;
  chunk* last;

public:
  rope(const std::string &);
  void insert(int position, const std::string &input);

private:
  std::pair<chunk*, chunk*> split(std::string buffer);
};

#endif
