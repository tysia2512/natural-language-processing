// g++ -std=c++11 -o task2 task2.cpp

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <random>
#include <ctime>
#include <sstream>
#include <string>
#include <random>
#include <unordered_map>

using namespace std;

const int len = 1000;

unordered_map<string, vector<string> > gram;
vector<string> starters_dictionary;

int main() {
  srand(time(0));
  ifstream input;
  ofstream output;
  input.open("../2grams.txt");
  int apps;
  string w1, w2;
  while(input >> apps) {
    input >> w1 >> w2;
    if (apps < 5)
      continue;
    if (gram[w1].size() == 0)
      starters_dictionary.push_back(w1);
    gram[w1].push_back(w2);
  }
  cout << "Data in!\n";

  auto t = to_string(time(0));

  std::stringstream ss;
  ss << "result" << std::time(0) << ".txt";
  std::string filename = ss.str();
  output.open(filename);

  string prev = starters_dictionary[rand() % starters_dictionary.size()];
  output << prev << " ";
  for(int i = 0; i < len; i++) {
    int options = gram[prev].size();
    if (options == 0) {
      string word = starters_dictionary[rand() % starters_dictionary.size()];
      output << "\n" << word << " ";
      prev = word;
    }
    else {
      string word = gram[prev][rand() % options];
      output << word << " ";
      prev = word;
    }
  }
  cout << "Text generated!\n";
}
