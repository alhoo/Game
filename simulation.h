#ifndef __SIMULATION_HH_
#define __SIMULATION_HH_

#include <unordered_map>
#include <chrono>
#include <random>

#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/compressed_matrix.hpp"

#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;
using namespace std;
typedef float ScalarType;

#define FOOD 1
#define WATER 2
#define GOLD 3
#define PEOPLE 10

#define SHOW_BORDERS 1
#define SHOW_CIRCLE 0
#define SHOW_CENTER 1
#define PEOPLESTATELEN 2

/**
 * Generate world people
 * @param people - resulting matrix containing people
 */
void generatePeople(viennacl::matrix<ScalarType> &people);
/**
 * Generate world minerals
 * @param state - current world state
 */
void generateMinerals(unordered_map<int, int> &state);

/**
 * Move people logically
 * @param state - current world state
 * @param people - current people states
 */
void updatePeople(unordered_map<int, int> &state, viennacl::matrix<ScalarType> &people);

/** A class representing the whole world **/
class World {
  public:
    void testVienna();
    World();
    void update();

    void draw(void (*fn)(unsigned char *, int, int),
              unsigned short width, unsigned short height);

  private:
     chrono::time_point<chrono::system_clock> start;
     unordered_map<int, int> state;
     viennacl::matrix<ScalarType> people;
};

#endif
