// g++ -std=c++11 -o task3 task3.cpp

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
#include <algorithm>

using namespace std;

const int len = 1000;

unordered_map<string, vector<pair<string, int> > > gram;
vector<pair<string, string> > starters_dictionary;

int main() {
  srand(time(0));
  ifstream input;
  ofstream output;
  input.open("../3grams.txt", std::ifstream::binary);
  int apps;
  string line;
  string w1, w2, w3;
  int lines = 0;
  while(getline(input, line, '\n')) {
    std::istringstream iss(line);
    iss >> apps >> w1 >> w2 >> w3;
    if (apps < 2)
      continue;

    std::stringstream ss;
    ss << w1 << " " << w2;
    std::string key = ss.str();

    lines++;
    if (lines % 1000 == 0)
      cout << lines << "\n";
    if (gram[key].size() == 0)
      starters_dictionary.push_back(make_pair(w1, w2));
    gram[key].push_back(make_pair(w3, apps));
  }
  string ap1;
  input >> ap1;
  cout << ap1 << ",\n";
  cout << "Data in!\n";

  auto t = to_string(time(0));

  std::stringstream ss;
  ss << "result3gram" << std::time(0) << ".txt";
  std::string filename = ss.str();
  output.open(filename);

  auto starting = starters_dictionary[rand() % starters_dictionary.size()];
  string prev1 = starting.first, prev2 = starting.second;
  output  << prev1 << " " << prev2 << " ";
  cout << lines << "\n";
  for(int i = 0; i < len; i++) {
    std::stringstream ss;
    ss << prev1 << " " << prev2;
    std::string key = ss.str();
    int options = gram[key].size();

    if (options == 0) {
      auto words = starters_dictionary[rand() % starters_dictionary.size()];
      output << "\n" << words.first << " " << words.second << " ";
      prev1 = words.first;
      prev2 = words.second;
    }
    else {
      int base = 0;
      for (int i = 0; i < gram[key].size(); i++) {
        base += gram[key][i].second;
      }
      int r = rand() % base;
      int sum = 0;
      string word;
      for (int i = 0; i < gram[key].size(); i++) {
        sum += gram[key][i].second;
        if(sum > r) {
          word = gram[key][i].first;
        }
      }

      output << word << " ";
      prev1 = prev2;
      prev2 = word;
    }
  }
  cout << "Text generated!\n";
}
