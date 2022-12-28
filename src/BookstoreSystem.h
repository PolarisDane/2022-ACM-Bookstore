#ifndef ACM_BOOKSTORE_SYSTEM
#define ACM_BOOKSTORE_SYSTEM

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include "UserSystem.h"
#include "BookSystem.h"

class BookstoreSystem {
public:

  BookstoreSystem();
  ~BookstoreSystem();

  void write_log(const std::string& str, int type);
  void show_log();

  UserSystem UserSys;
  BookSystem BookSys;

  int row_cnt;

  std::fstream log_file;

};

extern BookstoreSystem BookstoreSys;

#endif