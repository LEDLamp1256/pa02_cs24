#ifndef MOVIES_H
#define MOVIES_H

#include <string>

class Movies{
  private:
    std::string name;
    double rating;
  public:
    Movies(const std::string& n, double r);
    
    std::string get_name() const; //returns name
    double get_rating() const; //returns rating

    bool operator<(const Movies &movie2) const; //for letter comparison of movie names
};

struct MovieRatingComparator {
    bool operator()(const Movies& movie1, const Movies& movie2) const {
        if(movie1.get_rating() != movie2.get_rating()) {
            return movie1.get_rating() > movie2.get_rating(); //if ratings arent the same, compare by rating 
        }
        return movie1.get_name() < movie2.get_name(); //if ratings are the same, compare by name
    }
};
#endif