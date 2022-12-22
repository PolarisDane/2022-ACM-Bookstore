#ifndef ACM_BOOKSTORE_EXCEPTION
#define ACM_BOOKSTORE_EXCEPTION

#include <iostream>
#include <cstring>

class Exception {
public:
  Exception(const std::string& str) :message(str) {}
  const std::string what() { return message; }
private:
  const std::string message;
};

#endif