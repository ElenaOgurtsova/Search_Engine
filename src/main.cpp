#include "ConverterJSON.h"
#include "SearchServer.h"
#include "InvertedIndex.h"
#include <iostream>
#include <fstream>

#if !defined PROJECT_NAME
#define PROJECT_NAME "Search_Engine"
#endif
#define PROJECT_VERSION "1.0"

using namespace std;

int main()
{
    ConverterJSON json;
    InvertedIndex invertedIndex;
    SearchServer search_server(invertedIndex);
    json.GetConfigs();

    string name =json.GetConfigName();
    string version = json.GetConfigVersion();
    if (version!=PROJECT_VERSION) {
        cerr << "Project version is incorrect."<< endl;
        return 1;
    }
    if (name != PROJECT_NAME) {
        cerr << "Project name is incorrect."<< endl;
        return 1;
    }

    vector<string> files = json.getFiles();
    if (files.size() == 0) {
        cerr << "No files for analysis"<< endl;
        return 1;
    }

    invertedIndex.UpdateDocumentBase(files);

    vector<string> requests = json.GetRequests();
    if (requests.size() == 0) {
        cerr << "No requests for analysis"<< endl;
        return 1;
    }
    search_server.search(requests);

    return 0;
}