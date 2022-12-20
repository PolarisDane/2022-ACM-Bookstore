#ifndef ACM_BOOKSTORE_SYSTEM
#define ACM_BOOKSTORE_SYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UserSystem.h"
#include "BookSystem.h"

class BookstoreSystem {
public:
  BookstoreSystem();
  ~BookstoreSystem();

private:
  UserSystem users;
  BookSystem books;
  LogSystem log;

};

#endif