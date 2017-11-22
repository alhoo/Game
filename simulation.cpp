#include "simulation.h"

std::default_random_engine generator;

unsigned char * pixels = new unsigned char[256*256 * 3];


/** A class representing the whole world **/
void generatePeople(viennacl::matrix<ScalarType> &people){
  std::uniform_int_distribution<int> position(10, 256-10);
  auto tribex = position(generator);
  auto tribey = position(generator);
      
      ublas::matrix<ScalarType> tmp(PEOPLESTATELEN, 10);

  std::uniform_int_distribution<int> distribution(-10, 10);
  for( auto i = 0; i < 10; i++){
    auto x = tribex + distribution(generator);
    auto y = tribey + distribution(generator);
//    state[x*256 + y] = PEOPLE;
    tmp(0, i) = x;
    tmp(1, i) = y;
  }
      
      viennacl::copy(tmp, people);
}
/**
 * Generate world minerals
 * @param state - current world state
 */
void generateMinerals(unordered_map<int, int> &state){
  std::uniform_int_distribution<int> distribution(10, 256-10);
  for( auto i = 0; i < 50; i++){
    auto x = distribution(generator);
    auto y = distribution(generator);
    state[x*256 + y] = FOOD;
  }
  for( auto i = 0; i < 20; i++){
    auto x = distribution(generator);
    auto y = distribution(generator);
    state[x*256 + y] = WATER;
  }
  for( auto i = 0; i < 5; i++){
    auto x = distribution(generator);
    auto y = distribution(generator);
    state[x*256 + y] = GOLD;
  }
}

/**
 * Move people logically
 * @param state - current world state
 * @param people - current people states
 */
void updatePeople(unordered_map<int, int> &state, viennacl::matrix<ScalarType> &people){
  std::uniform_int_distribution<int> move(-1, 1);
  for( auto it = state.begin(); it != state.end(); ++it){
    if(it->second == PEOPLE){
      state[it->first] = 0;
//      state[min(255*255, max(0, it->first + dx + dy*256))] = PEOPLE;
    }
  }
  ublas::matrix<ScalarType> tmp(PEOPLESTATELEN, 10);
  for( auto i = 0; i < 10; ++i){
    auto dx = move(generator);
    auto dy = move(generator);
    tmp(0, i) = dx;
    tmp(1, i) = dy;
  }
  viennacl::matrix<ScalarType> people_move(PEOPLESTATELEN, 10);
  viennacl::copy(tmp, people_move);
  people = people + people_move;
  viennacl::copy(people, tmp);
  for( auto i = 0; i < 10; ++i){
    float x = tmp(0, i);
    float y = tmp(1, i);
    state[min(255*255, max(0, int(x + y*256)))] = PEOPLE;
  }
}

    void World::testVienna() {
      vector<float> vec1;
      int M = 256;
      for(int i = 0; i < M; i++){
        vec1.push_back(i);
      }
      viennacl::vector<ScalarType> vcl_vec1(vec1.size());
      viennacl::vector<ScalarType> vcl_vec2(vec1.size());
      viennacl::compressed_matrix<ScalarType, 1> vcl_compressed_matrix_1(vec1.size(), vec1.size());
      ublas::matrix<ScalarType> tmp(vec1.size(), vec1.size());
      for(int i = 0; i < M; i++){
        vcl_vec2(i) = 0;
        for(int j = 0; j < M; j++){
          tmp(i, j) = 1.0005*(i == j);
        }
      }
      vcl_vec2(5) = 1;
      viennacl::copy(tmp, vcl_compressed_matrix_1);

      for(int k = 0; k < 100; k++){
        vcl_vec1 = viennacl::linalg::prod(vcl_compressed_matrix_1, vcl_vec2);
        for(int i = 4; i < 6; i++){
          cout << vcl_vec1(i) << endl;
        }
        vcl_vec2 = viennacl::linalg::prod(vcl_compressed_matrix_1, vcl_vec1);
        for(int i = 4; i < 6; i++){
          cout << vcl_vec2(i) << endl;
        }
      }
    }

    World::World(){
      generateMinerals(this->state);
      generatePeople(this->people);
      this->state[0] = 0;
      start = chrono::system_clock::now();

      testVienna();
    }

    void World::update(){
      auto end = chrono::system_clock::now();
      float dt = chrono::duration_cast<chrono::microseconds>(end - start).count()*0.000001;
      updatePeople(this->state, this->people);
      start = end;
      this->state[0] += 100*dt;
    }

    void World::draw(void (*fn)(unsigned char *, int, int),
              unsigned short width, unsigned short height){
//      auto k = (((int)(this->state[0]))%3000 + 1000);
      int W = 1;
      for (auto i = 0; i < 3 * width * height; i++) {
        pixels[i] = 0x00;
      }
      for(auto it = this->state.begin(); it != this->state.end(); ++it){
        switch (it->second){
          case 1:
            pixels[3*it->first] = 0x0F;
            pixels[3*it->first + 1] = 0xFF;
            pixels[3*it->first + 2] = 0x0a;
            break;
          case 2:
            pixels[3*it->first] = 0x0F;
            pixels[3*it->first + 1] = 0x0F;
            pixels[3*it->first + 2] = 0xFa;
            break;
          case 3:
            pixels[3*it->first] = 0xf0;
            pixels[3*it->first + 1] = 0x0F;
            pixels[3*it->first + 2] = 0xFa;
            break;
          case 10:
            pixels[3*it->first] = 0xF0;
            pixels[3*it->first + 1] = 0xFF;
            pixels[3*it->first + 2] = 0xF0;
            break;
        }
      }
      //Draw circle
      auto cp = 3*((int)(3*width * height/2 + 3*width/2)/3);
      if(SHOW_CIRCLE){
        auto px = 300*sin(this->state[0]*0.1);
        auto py = 300*cos(this->state[0]*0.1);
        for (auto i = 0; i < 100; i++) {
          int x = 3*((int)((20*sin(i*0.063) + px)/3));
          int y = 3*((int)((20*cos(i*0.063) + py)/3));
          pixels[cp + x*width + y] = 0xFF;
          pixels[cp + x*width + y + 1] = 0xFF;
          pixels[cp + x*width + y + 2] = 0xFF;
        }
      }
      //Borders
      if(SHOW_BORDERS){
        for (auto k = 0; k < 2*W; k++){
          for (auto i = 0; i < width; i++) {
            pixels[3*i + k*(3 * width)] = 0xFF;
            pixels[3*i + k*(3 * width) + 1] = 0xFF;
            pixels[3*i + (3 * width)*(height - 1 - k)] = 0xFF;
            pixels[3*i + (3 * width)*(height - 1 - k) + 1] = 0xFF;
          }
          for (auto i = 0; i < height; i++) {
            pixels[i*(3*width) + 3*k] = 0xFF;
            pixels[i*(3*width) + 3*k + 2] = 0xFF;
            pixels[i*(3*width) + 3*(width - 1 - k)] = 0xFF;
            pixels[i*(3*width) + 3*(width - 1 - k)+2] = 0xFF;
          }
        }
      }
      //Center cross
      if(SHOW_CENTER){
        for (auto k = -W; k <= W; k++){
          for (auto i = -50; i < +50; i++) {
            pixels[cp + 3*i + 1 + k*3*width] = 0xFF;
            pixels[cp + 3*i + 2 + k*3*width] = 0xFF;
          }
          for (auto i = height/2 - 50; i < height/2 + 50; i++) {
            pixels[i*(3*width) + 3*k + 3*(3*width/6) + 1] = 0xFF;
            pixels[i*(3*width) + 3*k + 3*(3*width/6) + 2] = 0xFF;
          }
        }
      }
      fn(pixels,width,height);
    }

