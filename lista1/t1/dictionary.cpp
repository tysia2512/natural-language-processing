// Trie implementation: https://github.com/Tessil/hat-trie
// ruda@tysia2512:~/Documents/pjn/lista1/t1$ tr -d ' \t\n\r\f' < ../tokens.txt > tokens_merged.txt

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <sstream>
#include <unordered_set>

using namespace std;

const int SAMPLE_SIZE = 100000;
const int MAX_TOK_LEN = 30;
const string token_file = "../tokens.txt";

int count_matches(const vector<int>& sep1, const vector<int>& sep2) {
  int p1 = sep1[0], p2 = sep2[0];
  int i1 = 1, i2 = 1;
  int matches = 0;
  while (i1 < sep1.size() && i2 < sep2.size()) {
    if (p1 == p2 && sep1[i1] == sep2[i2])
      matches++;
    if (i1 == sep1.size() - 1 || sep2[i2] < sep1[i1]) {
      p2 = sep2[i2];
      i2++;
    } else {
      p1 = sep1[i1];
      i1++;
    }
  }
  return matches;
}

int main() {
  ifstream input;
  input.open(token_file);
  string token;

  // Create dictionary
  unordered_set<string> dictionary;

  // Prepare sample
  stringstream merged;

  // [p, q)
  int len = 0;
  vector<int> token_separators;
  token_separators.push_back(len);
  string line;
  while(getline(input, line)) {
    stringstream line_input(line);
    while (getline(line_input, token, ' ')) {
      dictionary.insert(token);
      len += token.size();
      if (token_separators.size() <= SAMPLE_SIZE) {
        token_separators.push_back(len);
        merged << token;
      }
    }
    // if (dictionary.size() > 1000)
    //   break;
  }
  cout << dictionary.size() << " words loaded to dictionary\n";

  string sample = merged.str();

  vector<int> predicted_separators;
  predicted_separators.push_back(0);
  int last_correct = 0;
  int q = 0;
  string word = "";
  while (q < sample.size()) {
    word.push_back(sample[q]);
    q++;
    if (dictionary.find(word) != dictionary.end())
      last_correct = q;
    if (word.size() > MAX_TOK_LEN) {
      last_correct = max(last_correct, predicted_separators.back() + 1);
      predicted_separators.push_back(last_correct);
      q = last_correct;
      word = "";
    }
  }
  if (predicted_separators.back() < sample.size())
    predicted_separators.push_back(sample.size());
  int matches = count_matches(token_separators, predicted_separators);
  double accuracy = 100.0 * (double)matches / (double)(token_separators.size() - 1);
  stringstream fn;
  fn << "RESULT" << time(0) << ".txt";
  string filename = fn.str();

  ofstream output;
  output.open(filename);

  output << "Reached accuracy of " << accuracy << "% with " << matches << " matches of " << (token_separators.size() - 1) << " tokens\n";

  int sepi = 1;
  string tok = "";

  for (int i = 1; i < predicted_separators.size(); i++) {
    string tok(sample.begin() + predicted_separators[i-1], sample.begin() + predicted_separators[i]);
    output << tok << " ";
  }
}
