#include "BookstoreSystem.h"

BookstoreSystem::BookstoreSystem() {
  log_file.open("log", std::ios::in | std::ios::out);
  if (!log_file.good()) {
    row_cnt = 0;
    std::ofstream create("log");
    create.close();
    log_file.open("log", std::ios::in | std::ios::out);
  }
  else {
    log_file.seekg(0);
    log_file >> row_cnt;
  }
}

BookstoreSystem::~BookstoreSystem() {
  log_file.seekp(0);
  log_file << row_cnt;
  log_file.close();
}

void BookstoreSystem::write_log(const std::string& str, int type) {
  log_file.seekp(0, std::ios::end);
  row_cnt += 3;
  if (UserStack.empty()) log_file << "no user logined\n";
  else log_file << "USER ID:" << UserStack.back().first.user_id << "\n";
  log_file << "COMMAND:" << str << "\n";
  long double val;
  if (type == 1) {
    if (BookSys.income.size() == 1) val = BookSys.income.back();
    else val = BookSys.income.back() - BookSys.income[BookSys.income.size() - 2];
    log_file << "FINANCE:+" << std::fixed << std::setprecision(2) << val << "\n";
    row_cnt++;
  }
  else if (type == -1) {
    if (BookSys.outcome.size() == 1) val = BookSys.outcome.back();
    else val = BookSys.outcome.back() - BookSys.outcome[BookSys.outcome.size() - 2];
    log_file << "FINANCE:-" << std::fixed << std::setprecision(2) << val << "\n";
    row_cnt++;
  }
  log_file << "\n";
  log_file.flush();
}//type=0:no finance
//type=1:finance income
//type=-1:finance outcome

void BookstoreSystem::show_log() {
  if (UserStack.empty() || UserStack.back().first.privilege < 7) {
    throw Exception("authority not enough");
  }
  std::cout << "---------------------------------------------------------------------\n\n";
  log_file.seekg(0, std::ios::beg);
  std::string str;
  for (int i = 1; i <= row_cnt; i++) {
    getline(log_file, str);
    std::cout << str << "\n";
  }//按照行数输出，防止文件流溢出错误
  std::cout << "---------------------------------------------------------------------\n";
  log_file.flush();
}

BookstoreSystem BookstoreSys;
