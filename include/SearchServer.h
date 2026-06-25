#ifndef SEARHSERVER_H
#define SEARHSERVER_H
#include "InvertedIndex.h"

using namespace std;

struct RelativeIndex {
  size_t doc_id;
  float rank;

  bool operator == (const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
  }
};

class SearchServer {
  public:
    SearchServer(InvertedIndex& idx) : _index(idx) {};

    vector<vector<RelativeIndex>> search(const vector<string>& queries_input);
    vector<string> parseQuery(const string& query);
    vector<RelativeIndex> searchByQueryWords(const vector<string>& sorted_query);
    private:
      InvertedIndex& _index;
};

#endif //SEARHSERVER_H
