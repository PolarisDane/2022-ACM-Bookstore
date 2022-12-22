#include "Utils.h"

void ReadLine(const std::string& str, std::string* buf) {
  int pos = 0, cnt = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ' ') {
      buf[++cnt] = str.substr(pos, i - pos);
      pos = i + 1;
    }
  }
  if (pos <= str.length() - 1) {
    buf[++cnt] = str.substr(pos, str.length() - pos + 1);
  }
  buf[0] = std::to_string(cnt);
}

void BookstoreWork() {
  std::string order;
  std::string buffer[10];
  std::string str;
  while (getline(std::cin, order)) {
    for (int i = 0; i < 10; i++) buffer[i].clear();
    ReadLine(order, buffer);
    int cnt = std::stoi(buffer[0]);
    //for (int i = 1; i <= cnt; i++) {
    //  std::cout << "keyword" << i << ":" << buffer[i] << std::endl;
    //}
    try {
      if (buffer[1] == "su") {
        BookstoreSys.UserSys.UserLogin(buffer[2], buffer[3]);
      }
      else if (buffer[1] == "logout") {
        BookstoreSys.UserSys.UserLogout();
      }
      else if (buffer[1] == "register") {
        BookstoreSys.UserSys.UserRegister(buffer[2], buffer[3], buffer[4]);
      }
      else if (buffer[1] == "passwd") {
        if (cnt == 4) BookstoreSys.UserSys.ModifyPassword(buffer[2], buffer[3], buffer[4]);
        else BookstoreSys.UserSys.ModifyPassword(buffer[2], "", buffer[3]);
      }
      else if (buffer[1] == "useradd") {
        BookstoreSys.UserSys.UserAdd(buffer[2], buffer[3], buffer[5], std::stoi(buffer[4]));
      }
      else if (buffer[1] == "delete") {
        BookstoreSys.UserSys.UserDelete(buffer[2]);
      }
      else if (buffer[1] == "show") {
        if (buffer[2] == "finance") {
          if (cnt == 2) BookstoreSys.BookSys.ShowFinanceLog(-1);
          else BookstoreSys.BookSys.ShowFinanceLog(std::stoi(buffer[3]));
          continue;
        }
        if (cnt == 1) {
          BookstoreSys.BookSys.ListBook();
          continue;
        }
        for (int i = 2; i <= cnt; i++) {
          if (buffer[i][1] == 'I') { BookstoreSys.BookSys.SearchISBN(buffer[i].substr(6)); }
          else if (buffer[i][1] == 'n') { BookstoreSys.BookSys.SearchName(buffer[i].substr(7, buffer[i].length() - 8)); }
          else if (buffer[i][1] == 'a') { BookstoreSys.BookSys.SearchAuthor(buffer[i].substr(9, buffer[i].length() - 10)); }
          else if (buffer[i][1] == 'k') { BookstoreSys.BookSys.SearchKeyword(buffer[i].substr(10, buffer[i].length() - 11)); }
        }
      }
      else if (buffer[1] == "buy") {
        if (buffer[3].find(".") != buffer[3].npos || buffer[3].find("-") != buffer[3].npos || !std::stoi(buffer[3])) {
          throw Exception("error:invalid argument");
        }
        BookstoreSys.BookSys.BuyBook(buffer[2], std::stoi(buffer[3]));
      }
      else if (buffer[1] == "select") {
        BookstoreSys.BookSys.selectBook(buffer[2]);
      }
      else if (buffer[1] == "modify") {
        int type = 0;
        if (cnt == 1) throw Exception("error:no argument provided");
        int hit[6] = { 0,0,0,0,0,0 };
        bool flag = false;
        for (int i = 2; i <= cnt; i++) {
          if (buffer[i][1] == 'I') { hit[1]++; flag |= (hit[1] > 1); }
          else if (buffer[i][1] == 'n') { hit[2]++; flag |= (hit[2] > 1); }
          else if (buffer[i][1] == 'a') { hit[3]++; flag |= (hit[3] > 1); }
          else if (buffer[i][1] == 'k') { hit[4]++; flag |= (hit[4] > 1); }
          else if (buffer[i][1] == 'p') { hit[5]++; flag |= (hit[5] > 1); }
        }
        if (flag) throw Exception("error:identical arguments");
        for (int i = 2; i <= cnt; i++) {
          if (buffer[i][1] == 'I') { type = 1; str = buffer[i].substr(6); }
          else if (buffer[i][1] == 'n') { type = 2; str = buffer[i].substr(7, buffer[i].length() - 8); }
          else if (buffer[i][1] == 'a') { type = 3; str = buffer[i].substr(9, buffer[i].length() - 10); }
          else if (buffer[i][1] == 'k') { type = 4; str = buffer[i].substr(10, buffer[i].length() - 11); }
          else if (buffer[i][1] == 'p') { type = 5; str = buffer[i].substr(7); }
          BookstoreSys.BookSys.ModifyBook(type, str);
        }
      }
      else if (buffer[1] == "import") {
        if (buffer[2].find(".") != buffer[2].npos || buffer[2].find("-") != buffer[2].npos || !std::stoi(buffer[2])) {
          throw Exception("error:invalid argument");
        }
        if (buffer[3].find("-") != buffer[3].npos || !std::stoi(buffer[3])) {
          throw Exception("error:invalid argument");
        }
        BookstoreSys.BookSys.ImportBook(std::stoi(buffer[2]), std::stod(buffer[3]));
      }
      else if (buffer[1] == "log") {}//incomplete
      else if (buffer[1] == "exit") break;
      else if (buffer[1] == "quit") break;
    }
    catch (Exception error) { std::cerr << "Invalid" << std::endl << error.what() << std::endl; }
  }
  ExitSystem();
}

void ExitSystem() {
  std::cout << "exiting system" << std::endl;
  return;
}