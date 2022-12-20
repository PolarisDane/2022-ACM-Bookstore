#ifndef ACM_BOOKSTORE_LOGSYSTEM
#define ACM_BOOKSTORE_LOGSYSTEM

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

class LogSystem {
public:
  LogSystem();
  ~LogSystem();

  void DisplayLog();
  void DisplayFinanceLog();
  void AddLog();

private:
  std::fstream log_file, flog_file;
  int log_cnt, flog_cnt;
  int income, outcome;
};

#endif