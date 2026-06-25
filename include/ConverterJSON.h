#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#define CONFIGFILE "config.json"
#define REQUESTFILE "requests.json"
#define ANSWERFILE "answers.json"

using namespace std;
class ConverterJSON
{
public:
    ConverterJSON ();
    ~ConverterJSON ();

    void GetConfigs();
    vector<string> getFiles();
    int  GetResponsesLimit();
    vector<string> GetRequests();
    string GetConfigName();
    string GetConfigVersion();
    	
    void putAnswers(vector<vector<pair<int, float>>> &answers);

private:
    nlohmann::json configData;

};


#endif //CONVERTERJSON_H
