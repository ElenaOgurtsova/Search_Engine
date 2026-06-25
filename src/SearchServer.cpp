#include "SearchServer.h"
#include <algorithm>
#include <string>
#include <sstream>
#include "Checks.h"
#include "InvertedIndex.h"
#include <set>
#include <vector>
#include <unordered_set>
#include "ConverterJSON.h"

using namespace std;

vector<string> SearchServer::parseQuery(const string& query) {
    stringstream ss(query);
    string word;
    vector<string> words;
    while (ss >> word) {
       transform(word.begin(), word.end(), word.begin(), ::tolower);
       words.push_back(word);
    }
    return words;
}


vector<RelativeIndex> SearchServer::searchByQueryWords(const vector<string>& sorted_query) 
{
    if (sorted_query.empty()) {
        return {};
    }

    set<size_t> doc_ids;
    for (const auto& word : sorted_query) {
        for (const auto& entry : _index.GetWordCount(word)) {
            doc_ids.insert(entry.doc_id);
        }
    }

    vector<RelativeIndex> relative_indices;
    map<size_t, size_t> doc_id_absolut_rel;

    for (const auto& id : doc_ids) {
        size_t absolute_relevance = 0;
        for (const auto& query_word : sorted_query) {
            for (const auto& entry2 : _index.GetWordCount(query_word)) {
                if (id == entry2.doc_id) {
                    absolute_relevance += entry2.count;
                }
            }
        }
        doc_id_absolut_rel[id] = absolute_relevance;
    }

    size_t max_absolute_rel = 0;
    for (const auto& [doc_id, absolute_relevance] : doc_id_absolut_rel) {
        max_absolute_rel = max(max_absolute_rel, absolute_relevance);
    }

    for (const auto& [doc_id, absolute_relevance] : doc_id_absolut_rel) {
        float relative_relevance;
        if (max_absolute_rel > 0) {
            relative_relevance = static_cast<float>(absolute_relevance) / static_cast<float>(max_absolute_rel);
        } else {
            relative_relevance = 0.0f;
        }
        relative_indices.push_back({doc_id, relative_relevance});
    }

    sort(relative_indices.begin(), relative_indices.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
        if (a.rank > b.rank) {
            return true;
        }
        if (a.rank < b.rank) {
            return false;
        }
        return a.doc_id < b.doc_id;
    });

    return relative_indices;
}

vector<vector<RelativeIndex>> SearchServer::search(const vector<string>& queries_input) {
    vector<vector<RelativeIndex>> results;
    ConverterJSON convert;
    vector<vector<pair<int, float>>> answers;

    for (const auto& query : queries_input) {

        vector<string> query_words = parseQuery(query);

        unordered_set<string> unique_query_words(query_words.begin(), query_words.end());

        vector<string> sorted_query = sorting_queries(unique_query_words, _index);

        vector<RelativeIndex> relative_indices = searchByQueryWords(sorted_query);

        int count = convert.GetResponsesLimit();
        if (relative_indices.size() > count) {
            relative_indices.resize(count);

        }

        results.push_back(relative_indices);
        vector<pair<int, float>> answer_for_query;
        for (const auto& rel_index : relative_indices) {
            answer_for_query.emplace_back(rel_index.doc_id, rel_index.rank);
        }
        answers.push_back(answer_for_query);
    }
    convert.putAnswers(answers);
    return results;
}