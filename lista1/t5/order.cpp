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
#include <unordered_set>
#include <iterator>

using namespace std;
const string data2 = "../2grams.txt", data3 = "../3grams.txt";

const string z1 = "wszystkie dzieci bardzo kochają swoich rodziców";
const string z2 = "każdy chce się czasem zabawić z przyjaciółmi";
const string z3 = "czasem chcę się po prostu poddać";
vector<string> zdania{z1, z2, z3};
string evaluation_used;

bool file_exists(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

string key3(string& key1, string& key2) {
  std::stringstream ss;
  ss << key1 << " " << key2;
  return ss.str();
}
// Wczytaj dane i, jeśli preprocessowany plik jeszcze nie istnieje, stwórz go
void process_input(
  const string& fname,
  unordered_map<string, vector<pair<string, int> > >& gram,
  int n,
  const vector<string>& words,
  bool cached = false) {

  cout << "Read input: " << fname << ", cached: " << cached << "\n";
  unordered_set <string> wds;
  for (string word : words)
    wds.insert(word);

  string filename;
  if (n == 2)
    filename = data2;
  else
    filename = data3;

  if (cached)
    filename = fname;

  ifstream input;
  input.open(filename);
  cout << "Reading from: " << filename << "\n";

  ofstream output;
  if (!cached) {
    output.open(fname);
    cout << "Writing to: " << fname << "\n";
  }

  int apps;
  string line;
  string key, key1, value;
  int lines = 0;
  while(getline(input, line, '\n')) {
    std::istringstream iss(line);
    if (n == 2)
      iss >> apps >> key >> value;
    else
      iss >> apps >> key >> key1 >> value;
    lines++;
    if (lines % 100 == 0)
      cout << "Read " << lines << " lines\n";

    if (!cached && (wds.find(value) == wds.end() || wds.find(key) == wds.end()))
      continue;
    if (!cached && (n == 3 && wds.find(key1) == wds.end()))
      continue;

    if (!cached)
      output << line << endl;

    if (n == 3) {
      key = key3(key, key1);
    }
    gram[key].push_back(make_pair(value, apps));
  }
}

// Załaduj dane do pamieci
void get_data(
  const vector<string>& words,
  int n,
  unordered_map<string, vector<pair<string, int> > >& gram) {
  std::stringstream ss;
  for (const string& word : words) {
    ss << word << "_";
  }
  ss << n << "gram.txt";
  std::string filename = ss.str();

  process_input(filename, gram, n, words, file_exists(filename));
}

int sum_apps(
  const vector<string>& words,
  unordered_map<string, vector<pair<string, int> > >& gram2,
  unordered_map<string, vector<pair<string, int> > >& gram3,
  int w2 = 2,
  int w3 = 3) {

  int all_matches = 0;
  for (int i = 1; i < words.size(); i++) {
    string key = words[i-1];
    string value = words[i];
    for (int j = 0; j < gram2[key].size(); j++) {
      if (gram2[key][j].first == value)
        all_matches += gram2[key][j].second * w2;
    }
  }

  for (int i = 2; i < words.size(); i++) {
    string key1 = words[i-2], key2 = words[i-1];
    string key = key3(key1, key2);
    string value = words[i];
    for (int j = 0; j < gram3[key].size(); j++) {
      if (gram3[key][j].first == value)
        all_matches += gram3[key][j].second * w3;
    }
  }
  return all_matches;
}

int eval_perm(
  const vector<string>& words,
  unordered_map<string, vector<pair<string, int> > >& gram2,
  unordered_map<string, vector<pair<string, int> > >& gram3) {
  evaluation_used = "sum_apps";
  return sum_apps(words, gram2, gram3);
}

string make_sentence(const vector<string>& words) {
  std::stringstream ss;
  for (const string& word : words) {
    ss << word << " ";
  }
  return ss.str();
}

int main() {
  for (int i = 0; i < zdania.size(); i++) {
    // Podziel zdanie na słowa
    istringstream to_split(zdania[i]);
    vector<string> words((istream_iterator<string>(to_split)),
                          istream_iterator<string>());

    // Wczytaj gramy do pamięci
    unordered_map<string, vector<pair<string, int> > > gram2;
    unordered_map<string, vector<pair<string, int> > > gram3;
    get_data(words, 2, gram2);
    get_data(words, 3, gram3);
    cout << "Data loaded to memory!\n";

    vector <int> perm;
    for (int i = 0; i < zdania[i].size(); i++)
      perm.push_back(i);

    vector<pair<int, string> > evaluated_sentences;
    sort(words.begin(), words.end());
    // Oceń każdą permutację słów
    do {
      evaluated_sentences.push_back(make_pair(
        eval_perm(words, gram2, gram3),
        make_sentence(words)
      ));
    } while(next_permutation(words.begin(), words.end()));
    sort(evaluated_sentences.begin(), evaluated_sentences.end());

    // Wypisz permutacje od najlepszej
    ofstream output;
    stringstream fn;
    fn << "RESULT_" << time(0) << "_";
    for (string& word : words) {
      fn << word << "_";
    }
    fn << evaluation_used << ".txt";
    output.open(fn.str());

    for (int i = evaluated_sentences.size() - 1; i >= 0; i--) {
      output << evaluated_sentences[i].first << ": "
        << evaluated_sentences[i].second << "\n";
    }
  }
}
