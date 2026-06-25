#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <vector>
#include <string>
#include <map>
#include "Entry.h"

using namespace std;

class InvertedIndex {
public:
  InvertedIndex() = default;

  void UpdateDocumentBase(vector<string> &input_docs);

  vector<Entry> GetWordCount(const string& word);

private:
  vector<string> docs;
  map<string, vector<Entry>> freq_dictionary;
};

#endif //INVERTEDINDEX_H
