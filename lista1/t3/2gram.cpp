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

unordered_map<string, vector<pair<string, int> > > gram;
vector<string> starters_dictionary;

int main() {
  srand(time(0));
  ifstream input;
  ofstream output;
  input.open("../2grams.txt");
  int apps;
  string w1, w2;
  string line;
  int lines = 0;
  while(getline(input, line, '\n')) {
    std::istringstream iss(line);
    iss >> apps >> w1 >> w2;
    lines++;
    if(lines % 100000 == 0)
      cout << lines << endl;
    if (gram[w1].size() == 0)
      starters_dictionary.push_back(w1);
    gram[w1].push_back(make_pair(w2, apps));
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
    int base = 0;
    for (int i = 0; i < gram[prev].size(); i++) {
      base += gram[prev][i].second;
    }
    if (base == 0) {
      string word = starters_dictionary[rand() % starters_dictionary.size()];
      output << "\n" << word << " ";
      prev = word;
    }
    else {
      int r = rand() % base;
      int sum = 0;
      string word;
      for (int i = 0; i < gram[prev].size(); i++) {
        sum += gram[prev][i].second;
        if(sum > r) {
          word = gram[prev][i].first;
        }
      }
      output << word << " ";
      prev = word;
    }
  }
  cout << "Text generated!\n";
}
