#ifndef ACM_BOOKSTORE_SYSTEM
#define ACM_BOOKSTORE_SYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UserSystem.h"
#include "BookSystem.h"

class BookstoreSystem {
public:

  BookstoreSystem() = default;
  ~BookstoreSystem() = default;

  UserSystem UserSys;
  BookSystem BookSys;

};

extern BookstoreSystem BookstoreSys;

#endif