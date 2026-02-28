// Winter'24
// Instructor: Diba Mirza
// Student name: Dylan Lee
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
  if (argc < 2){
    cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
    cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
    exit(1);
  }

  ifstream movieFile (argv[1]);
 
  if (movieFile.fail()){
    cerr << "Could not open file " << argv[1];
    exit(1);
  }

  set<Movies> movieSet; //obj created to store movies

  string line, movieName;
  double movieRating;
  // Read each file and store the name and rating
  while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
    Movies movie(movieName, movieRating); // create movie object using the name and rating
    //cout << movie.get_name() << " has rating " << movie.get_rating() << endl; TEST LINE, prints movie and rating
    movieSet.insert(movie); //inserts into set (movieSet)
  }

  movieFile.close();
  
  //part 1, will have 1 arg
  if (argc == 2){
    for (const Movies &movie : movieSet) {
      //since sorted already by the set, just printing gives order
      cout << movie.get_name() << ", " << movie.get_rating() << endl;
    }
    return 0;
  }

  //part 2, will have 2 args
  if (argc == 3){
    //clock function to start timer for time taken calculation
    clock_t startTime = clock();

    //reads prefix file
    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
      cerr << "Could not open file " << argv[2];
      exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
      if (!line.empty()) {
        prefixes.push_back(line);
      }
    }

    prefixFile.close(); //idk why closing file wasnt included in the starter code
                        //but you kinda gotta do that so ig its here now

    //vector holding best movies
    vector<pair<string, Movies>> bestMovies;

    //iterating through every prefix, finding movies with the prefix, sorting by rating, and then printing
    for (const string &prefix : prefixes) {
      vector<Movies> moviesWithPrefix;
      
      //gets all movies with the prefix and stores them in the vector moviesWithPrefix
      auto it = movieSet.lower_bound(Movies(prefix, 0.0));
      while (it != movieSet.end() && it -> get_name().substr(0, prefix.size()) == prefix) {
        moviesWithPrefix.push_back(*it);
        ++it;
      }

      //sorts in order of rating, then by name
      sort(moviesWithPrefix.begin(), moviesWithPrefix.end(), MovieRatingComparator());

      if (moviesWithPrefix.empty()) {
        //no movie with prefix found
        cout << "No movies found with prefix " << prefix << endl;
      } 
      else {
        //prints movies with the prefix in order of rating, then by name
        for(const Movies &movie : moviesWithPrefix) {
            cout << movie.get_name() << ", " << movie.get_rating() << endl;
        }
        cout << endl;
        bestMovies.push_back({prefix, moviesWithPrefix[0]}); //stores the best movie for each prefix in bestMovies
      }
    }
    //ending clock for run time calculation
    clock_t endTime = clock();

    //printing best movies for each prefix
    for(pair<string, Movies> &bestMovie : bestMovies) {
      cout << "Best movie with prefix " << bestMovie.first << ": " << bestMovie.second.get_name() << " with rating " << std::fixed << std::setprecision(1) << bestMovie.second.get_rating() << endl;
    }

    //time taken calculation
    cout.precision(10);
    cout << "Time taken: " << (double(endTime - startTime) / CLOCKS_PER_SEC) * 1000 << " milliseconds" << endl;
    return 0;
  }
}

/* 3a:
Given that there are n movies in the data set, 
m prefixes specified, 
at most k movies beginning with each prefix,
l being the maximum length of a movie name, 
and calculating for worst case time complexity:

for every prefix (m prefixes, and its in a loop so m *), 
  ->lower bound is called O(log(n)) operation
    ->set::lower_bound uses set properties, therefore is O(log(n))
  ->iterating through movies with the prefix (worst case) is O(k)
    ->given by default (k is defined as max movies matching prefix)
    ->adding to vector and incrementing iterator is O(1) for each movie
  ->sort with sort function is O(klog(k))
    ->given by default (k is defined as max movies matching prefix)
  ->printing movies with the prefix is O(k)
    ->if no movies with the prefix, is O(1)
    ->given by default (k is defined as max movies matching prefix)
  ->printing best movie for each prefix is O(m)
    ->given by default (m is defined as number of prefixes)

This leads to an overall time complexity of O(m * (log(n) + k + klog(k) + k) + m = O(m * (log(n) + klog(k)),
simplifying to O(mlog(n) + mklog(k)).

Randomized dataset results:
20: 11.44ms
100: 13.34ms
1000: 14.28ms
76920: 82.85ms

This makes sense because for small data sets from 20-1000, k matches n in the sense that there are a very small
amount of matches per prefix, but also a very small amount of movies as well. this leads to the run time not
increasing very much between the numbers 20-1000 (TLDR: since from 20-1000, n is small, k is also small, and 
therefore the time taken is not increasing much).
Jumping to 76920 movies there is a very large increase in time because the number of movies is significantly larger
, leading to functions like lower_bound increasing in time more (since its O(log(n)). The increase in number of
movies also leads to a larger k, which increases the time taken due to sorting and iterating.

Overall, the data makes sense and follows the expected time complexity.
*/

/* 3b:
Given that there are n movies in the data set, 
m prefixes specified, 
at most k movies beginning with each prefix,
l being the maximum length of a movie name,
and calculating for worst case space complexity:

l being the maximum length of a movie name (because applies to every movie, l *):
  vector of best movies is O(m)
    ->given because m is the number of prefixes
  vector of movies with the prefix is O(k)
    ->given because k is the max number of movies matching a prefix

This leads to an overall space complexity of O(l * (m + k)), simplifying to O(lm + lk).
*/

/* 3c:
Did you design your algorithm for a low time complexity, a low space complexity, or both? 
What were your target complexities?

My algorithm was designed for a reasonablely low time complexity.
My target complexity for time was anything below like an n or above time complexity, with space being as 
small as I could make it. An example would be,for instance scanning every movie by their entire length of 
the name etc leading to a O(mnl) complexity, and a space complexity of O(kl + ml).

1. If you designed your algorithm for a low time complexity, were you able to achieve a low space complexity as well?
Why or why not?
I think I achieved a reasonably low space capacity. Things like multiplying every value by l is unavoidable,
since nothing you do can really reduce that. Space taken by having to save vectors the number of times there 
were prefixes and movies matching a prefix is also mostly unavoidable, due to having to print this at the end.
However, there is a small space save, which can be done by having the set of all movies be defined with a different
functor, allowing for sorting by rating, and since the first movie of a prefix found will be the highest rated one
with that prefix, only one constantly rewritten variable can be used and therefore saving space complexity to 
O(lm + l) instead of O(lm + lk). However, I chose to go with the current implementation since it is more
straightforward, easier to understand, and the space save increases the complexity of the code and also
likely slightly reduces run time because the movies arent sorted by name.
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
  int commaIndex = line.find_last_of(",");
  movieName = line.substr(0, commaIndex);
  movieRating = stod(line.substr(commaIndex+1));
  if (movieName[0] == '\"') {
    movieName = movieName.substr(1, movieName.length() - 2);
  }
  return true;
}