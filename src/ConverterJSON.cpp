#include "ConverterJSON.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "nlohmann/json.hpp"
#include "Exceptions.h"
#include "Checks.h"


#define CONFIGFILE "config.json"
#define REQUESTFILE "requests.json"
#define ANSWERFILE "answers.json"

using namespace std;

const std::string REQUESTS_KEY = "requests";
const std::string RELEVANCE_KEY = "relevance";
const std::string RESULT_KEY = "result";

ConverterJSON::ConverterJSON(){};
ConverterJSON::~ConverterJSON(){};

void ConverterJSON::GetConfigs() {
  cout << "Loading config file..." << endl;
  ifstream configFile(CONFIGFILE);
  if(checkingFilesIsOpen(configFile, CONFIGFILE)) {
    if(configFile.is_open()) {
      try{
        nlohmann::json config;
        configFile >> config;
        configFile.close();
        if(config["config"].is_null()) {throw NoSuchConfig();}
        configData = config;
      }catch (NoSuchConfig& e) {
        cerr << e.what() << endl;
      }
    }
  }
}

vector<string> ConverterJSON::getFiles () {
  vector<string> path;
  vector<string> files;
  try {
    if(configData.contains("files") && configData["files"].is_array()) {
      for(auto& file : configData["files"]) {
        if(file.is_string()) {
          path.push_back(file);

        }else {
          cout << "File " << file << " not string" << endl;
        }
      }
    }else {
      throw NoAnyfile();
    }
  }
  catch(NoAnyfile& e) {
    cerr << e.what() << endl;
  }

  ifstream file;
  for(auto& _path : path) {
    file.open(_path);
    if(!file.is_open()) {
      cerr << "File " << _path.c_str() << " not open!" << endl;
    }else {
      stringstream buffer;
      buffer << file.rdbuf();
      if(checkingFiles(buffer)) {
        string content = buffer.str();
        files.push_back(content);
      }
    }
    file.close();
  }
  return files;
}

vector<string> ConverterJSON::GetRequests() {
  vector<string> requests;
  ifstream fileRequests(REQUESTFILE);
  checkingFilesIsOpen(fileRequests, REQUESTFILE);
  if (!fileRequests.is_open()) {
    return requests;
  }
  nlohmann::json _requests;
  fileRequests >> _requests;
  fileRequests.close();
  try {
    if(_requests.contains(REQUESTS_KEY) && _requests[REQUESTS_KEY].is_array()) {
      if(checkingRequests(_requests)) {
        for(auto& request : _requests[REQUESTS_KEY]) {
          requests.push_back(request);
        }
      }
      return requests;
    }else {
      throw NoSuchRequests();
    }
  }catch(NoSuchRequests& e) {
    cerr << e.what() << endl;
    return requests;
  }
}

void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> &answers) {
  nlohmann::json answersJson;
  nlohmann::json answersObject;
  int requestIndex = 1;
  int lenMaxCount = to_string(MAXCOUNTREQUEST).length();
  if (lenMaxCount > 1) lenMaxCount -= 1;
  int pozition = pow(10, lenMaxCount);
  if (pozition < MAXCOUNTREQUEST) {pozition *= 10;}
  for(auto& answer : answers) {
    string requestID = "request" + to_string(pozition+requestIndex).substr(1);
    nlohmann::json requestObject;

    if(answer.empty()) {
      requestObject[RELEVANCE_KEY] = {};
      requestObject[RESULT_KEY] = "false";
    }else {
      nlohmann::json relevanceArray;
      for(auto& result : answer) {
        nlohmann::json resultObject;
        resultObject["docid"] = result.first;
        resultObject["rank"] = result.second;
        relevanceArray.push_back(resultObject);
      }
      requestObject[RELEVANCE_KEY] = relevanceArray;
      requestObject[RESULT_KEY] = "true";
    }
    answersObject[requestID] = requestObject;
    requestIndex++;
  }
  answersJson["answers"] = answersObject;

  ofstream outputFile(ANSWERFILE);
  if(outputFile.is_open()) {
      outputFile << answersJson.dump(4);
      outputFile.close();
  }else cerr << "File 'answers.json' is not found!";
}

int ConverterJSON::GetResponsesLimit () {
  if(configData["config"].contains("responseLimit")) {
    return configData["config"]["max_responses"].get<int>();
  } else {
    return 5;
  }
}

string ConverterJSON::GetConfigName () {
  string name = "";
  name = configData["config"]["name"].get<string>();
  return name;
}

string ConverterJSON::GetConfigVersion () {
  string version = "";
  version = configData["config"]["version"].get<string>();
  return version;
}








