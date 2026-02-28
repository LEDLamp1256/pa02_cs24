#include "movies.h"

#include <iostream>

Movies::Movies(const std::string &n, double r){
  name = n;
  rating = r;
}

std::string Movies::get_name() const{
  //name getter
  return name;
}

double Movies::get_rating() const{
  //rating getter
  return rating; 
}

bool Movies::operator<(const Movies &movie2) const{
  //compares movie names by letter 
  return name < movie2.name;
}

