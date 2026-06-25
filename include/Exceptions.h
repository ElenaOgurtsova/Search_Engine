#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class NoSuchFile : public std::exception {
public:
  NoSuchFile (const std::string& message): message{message}
  {}
  const char* what() const noexcept override
  {
    return message.c_str();
  }
private:
  std::string message;
};

class NoSuchConfig : public std::exception {
public:
  const char* what() const noexcept override {
    return "'config' no such in config file";
  }
};

class NoSuchFiles : public std::exception {
public:
  const char* what() const noexcept override {
    return "'files' no such in config file";
  }
};

class NoSuchRequests : public std::exception {
public:
  const char* what() const noexcept override {
    return "Requests not found";
  }
};

class NoAnyfile : public std::exception {
public:
  const char* what() const noexcept override {
    return "'config' no such in file";
  }
};

#endif //EXCEPTIONS_H
