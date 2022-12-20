#include "LogSystem.h"

LogSystem::LogSystem() {
  log_file.open("");
  if (log_file.good()) {
    flog_file.open("finance log");
    log_file.seekg(0); flog_file.seekg(0);
    log_file.read(reinterpret_cast<char*>(&log_cnt), sizeof(int));
    flog_file.read(reinterpret_cast<char*>(&flog_cnt), sizeof(int));
    flog_file.read(reinterpret_cast<char*>(&income), sizeof(int));
    flog_file.read(reinterpret_cast<char*>(&outcome), sizeof(int));
  }
  else {
    std::ofstream create("log");
    create.open("finance log");
    create.close();
    log_file.open("log"); flog_file.open("finance log");
  }
}

LogSystem::~LogSystem() {
  log_file.seekp(0); flog_file.seekp(0);
  log_file.write(reinterpret_cast<char*>(&log_cnt), sizeof(int));
  flog_file.write(reinterpret_cast<char*>(&flog_cnt), sizeof(int));
  flog_file.write(reinterpret_cast<char*>(&income), sizeof(int));
  flog_file.write(reinterpret_cast<char*>(&outcome), sizeof(int));
  log_file.close(); flog_file.close();
}

LogSystem::DisplayLog() {

}

LogSystem::DisplayFinanceLog() {

}

LogSystem::AddLog() {

}
