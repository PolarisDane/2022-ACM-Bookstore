#include "Utils.h"

void ReadLine(const std::string& str, std::string* buf) {
  int pos = 0, cnt = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] <= 31 || str[i] >= 127) throw Exception("invalid argument");
    if (str[i] == ' ') {
      if (pos != i) buf[++cnt] = str.substr(pos, i - pos);
      pos = i + 1;
      if (cnt == 9) throw Exception("invalid argument");
    }
  }
  if (pos <= str.length() - 1) {
    buf[++cnt] = str.substr(pos, str.length() - pos + 1);
  }
  buf[0] = std::to_string(cnt);
}

void ValidateKeywordForSearch(const std::string& str) {
  if (str.find('|') != str.npos)
    throw Exception("error:mutiple keywords when searching");
}

bool JudgeKeyword(const std::string& str) {
  std::unordered_map<std::string, int> ret;
  std::string tmp;
  int pos = 0;
  if (str[0] == '|' || str.back() == '|') return false;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '|') {
      tmp = str.substr(pos, i - pos);
      pos = i + 1;
      if (ret.find(tmp) != ret.end()) return false;
      ret[tmp] = 1;
    }
  }
  tmp = str.substr(pos, str.length() - pos + 1);
  if (ret.find(tmp) != ret.end()) return false;
  ret[tmp] = 1;
  return true;
}//finding identical keywords

bool ValidateUserData1(const std::string& str) {
  if (str.length() > 30) return false;
  for (int i = 0; i < str.length(); i++) {
    if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
      return false;
  }
  return true;
}//UserID,Password,CurrentPassword,NewPassword

bool ValidateUserData2(const std::string& str) {
  if (str.length() > 30) return false;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] <= 32 || str[i] >= 127)
      return false;
  }
  return true;
}//Username

bool ValidateUserData3(const std::string& str) {
  if (str.length() != 1) return false;
  if (str[0] == '1' || str[0] == '3' || str[0] == '7') return true;
  return false;
}//Privilege

bool ValidateISBN(const std::string& str) {
  if (str.length() > 20 || !str.length()) return false;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] <= 32 || str[i] >= 127)
      return false;
  }
  return true;
}

bool ValidateNameAuthor(const std::string& str) {
  if (str.length() > 60) return false;
  if (str[0] != '"' || str.back() != '"') return false;
  if (str.length() <= 2) return false;
  for (int i = 1; i < str.length() - 1; i++) {
    if (str[i] <= 32 || str[i] >= 127 || str[i] == '"')
      return false;
  }
  return true;
}

bool ValidateKeyword(const std::string& str) {
  if (str.length() > 60) return false;
  if (str[0] != '"' || str.back() != '"') return false;
  if (str.length() <= 2) return false;
  for (int i = 1; i < str.length() - 1; i++) {
    if (str[i] <= 32 || str[i] >= 127 || str[i] == '"')
      return false;
    if (str[i] == '|' && str[i - 1] == '|') return false;
  }
  if (!JudgeKeyword(str.substr(1, str.length() - 2))) return false;
  return true;
}

bool ValidateQuantity(const std::string& str) {
  if (!str.length()) return true;
  if (str.length() > 10) return false;
  if (str[0] == '0' && str.length() > 1) return false;
  for (int i = 0; i < str.length(); i++) {
    if (!(str[i] >= '0' && str[i] <= '9')) return false;
  }
  if (std::stoll(str) > 2147483647 || std::stoll(str) == 0) return false;
  return true;
}

bool ValidatePrice(const std::string& str) {
  if (str.length() > 13) return false;
  if (str[0] == '0' && (str.length() > 1 && str[1] != '.')) return false;
  if (str[0] == '.' || str.back() == '.') return false;
  if (count(str.begin(), str.end(), '.') > 1) return false;
  for (int i = 0; i < str.length(); i++) {
    if (!((str[i] >= '0' && str[i] <= '9') || str[i] == '.')) return false;
  }
  return true;
}

bool ValidateCost(const std::string& str) {
  if (str.length() > 13) return false;
  if (str[0] == '0' && (str.length() > 1 && str[1] != '.')) return false;
  if (str[0] == '.' || str.back() == '.') return false;
  if (count(str.begin(), str.end(), '.') > 1) return false;
  for (int i = 0; i < str.length(); i++) {
    if (!((str[i] >= '0' && str[i] <= '9') || str[i] == '.')) return false;
  }
  if (stod(str) == 0) return false;
  return true;
}

bool ValidateCount(const std::string& str) {
  if (!str.length()) return true;
  if (str.length() > 10) return false;
  if (str[0] == '0' && str.length() > 1) return false;
  for (int i = 0; i < str.length(); i++) {
    if (!(str[i] >= '0' && str[i] <= '9')) return false;
  }
  if (std::stoll(str) > 2147483647) return false;
  return true;
}

void BookstoreWork() {
  std::string order;
  std::string buffer[10];
  std::string str;
  for (int i = 0; i < 10; i++) buffer[i].clear();
  //freopen("my.out", "w", stdout);
  while (getline(std::cin, order)) {
    for (int i = 0; i < 10; i++) buffer[i].clear();
    try {
      ReadLine(order, buffer);
      int cnt = std::stoi(buffer[0]);
      //for (int i = 1; i <= cnt; i++) {
      //  std::cout << "keyword" << i << ":" << buffer[i] << std::endl;
      //}
      if (buffer[1] == "su") {
        if (cnt != 2 && cnt != 3) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[3])) throw Exception("error:invalid argument");
        if (cnt == 3) BookstoreSys.UserSys.UserLogin(buffer[2], buffer[3]);
        else BookstoreSys.UserSys.UserLogin(buffer[2], "");
      }
      else if (buffer[1] == "logout") {
        if (cnt != 1) throw Exception("error:invalid argument");
        BookstoreSys.UserSys.UserLogout();
      }
      else if (buffer[1] == "register") {
        if (cnt != 4)throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[3])) throw Exception("error:invalid argument");
        if (!ValidateUserData2(buffer[4])) throw Exception("error:invalid argument");
        BookstoreSys.UserSys.UserRegister(buffer[2], buffer[3], buffer[4]);
      }
      else if (buffer[1] == "passwd") {
        if (cnt != 3 && cnt != 4)throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[3])) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[4])) throw Exception("error:invalid argument");
        if (cnt == 4) BookstoreSys.UserSys.ModifyPassword(buffer[2], buffer[3], buffer[4]);
        else BookstoreSys.UserSys.ModifyPassword(buffer[2], "", buffer[3]);
      }
      else if (buffer[1] == "useradd") {
        if (cnt != 5) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[3])) throw Exception("error:invalid argument");
        if (!ValidateUserData3(buffer[4])) throw Exception("error:invalid argument");
        if (!ValidateUserData2(buffer[5])) throw Exception("error:invalid argument");
        BookstoreSys.UserSys.UserAdd(buffer[2], buffer[3], buffer[5], std::stoi(buffer[4]));
      }
      else if (buffer[1] == "delete") {
        if (cnt != 2) throw Exception("error:invalid argument");
        if (!ValidateUserData1(buffer[2])) throw Exception("error:invalid argument");
        BookstoreSys.UserSys.UserDelete(buffer[2]);
      }
      else if (buffer[1] == "show") {
        if (buffer[2] == "finance") {
          if (cnt != 2 && cnt != 3) throw Exception("error:invalid argument");
          if (!ValidateCount(buffer[3])) throw Exception("error:invalid argument");
          if (cnt == 2) BookstoreSys.BookSys.ShowFinanceLog(-1);
          else BookstoreSys.BookSys.ShowFinanceLog(std::stoi(buffer[3]));
          continue;
        }
        if (cnt == 1) {
          BookstoreSys.BookSys.ListBook();
          continue;
        }
        sort(buffer->begin() + 2, buffer->begin() + cnt + 1);
        int hit[5] = { 0,0,0,0,0 };
        bool flag = false;
        for (int i = 2; i <= cnt; i++) {
          int len = buffer[i].length();
          if (len >= 6 && buffer[i].substr(0, 6) == "-ISBN=") {
            hit[1]++; flag |= (hit[1] > 1);
            if (!ValidateISBN(buffer[i].substr(6))) throw Exception("error:invalid argument");
          }
          else if (len >= 7 && buffer[i].substr(0, 6) == "-name=") {
            hit[2]++; flag |= (hit[2] > 1);
            if (!ValidateNameAuthor(buffer[i].substr(6))) throw Exception("error:invalid argument");
          }
          else if (len >= 9 && buffer[i].substr(0, 8) == "-author=") {
            hit[3]++; flag |= (hit[3] > 1);
            if (!ValidateNameAuthor(buffer[i].substr(8))) throw Exception("error:invalid argument");
          }
          else if (len >= 10 && buffer[i].substr(0, 9) == "-keyword=") {
            hit[4]++; flag |= (hit[4] > 1);
            if (!ValidateKeyword(buffer[i].substr(9))) throw Exception("error:invalid argument");
            ValidateKeywordForSearch(buffer[i].substr(10, buffer[i].length() - 1));
          }
          else throw Exception("error:invalid argument");
        }
        if (flag) throw Exception("error:identical arguments");
        for (int i = 2; i <= cnt; i++) {
          if (buffer[i][1] == 'I') { BookstoreSys.BookSys.SearchISBN(buffer[i].substr(6)); }
          else if (buffer[i][1] == 'n') { BookstoreSys.BookSys.SearchName(buffer[i].substr(7, buffer[i].length() - 8)); }
          else if (buffer[i][1] == 'a') { BookstoreSys.BookSys.SearchAuthor(buffer[i].substr(9, buffer[i].length() - 10)); }
          else if (buffer[i][1] == 'k') { BookstoreSys.BookSys.SearchKeyword(buffer[i].substr(10, buffer[i].length() - 11)); }
        }
      }
      else if (buffer[1] == "buy") {
        if (cnt != 3) throw Exception("error:invalid argument");
        if (!ValidateISBN(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateQuantity(buffer[3])) throw Exception("error:invalid argument");
        BookstoreSys.BookSys.BuyBook(buffer[2], std::stoi(buffer[3]));
      }
      else if (buffer[1] == "select") {
        if (cnt != 2) throw Exception("error:invalid argument");
        if (!ValidateISBN(buffer[2])) throw Exception("error:invalid argument");
        BookstoreSys.BookSys.selectBook(buffer[2]);
      }
      else if (buffer[1] == "modify") {
        int type = 0;
        if (cnt == 1) throw Exception("error:no argument provided");
        sort(buffer->begin() + 2, buffer->begin() + cnt + 1);
        int hit[6] = { 0,0,0,0,0,0 };
        bool flag = false;
        for (int i = 2; i <= cnt; i++) {
          int len = buffer[i].length();
          if (len >= 6 && buffer[i].substr(0, 6) == "-ISBN=") {
            hit[1]++; flag |= (hit[1] > 1);
            if (!ValidateISBN(buffer[i].substr(6))) throw Exception("error:invalid argument");
          }
          else if (len >= 7 && buffer[i].substr(0, 6) == "-name=") {
            hit[2]++; flag |= (hit[2] > 1);
            if (!ValidateNameAuthor(buffer[i].substr(6))) throw Exception("error:invalid argument");
          }
          else if (len >= 9 && buffer[i].substr(0, 8) == "-author=") {
            hit[3]++; flag |= (hit[3] > 1);
            if (!ValidateNameAuthor(buffer[i].substr(8))) throw Exception("error:invalid argument");
          }
          else if (len >= 10 && buffer[i].substr(0, 9) == "-keyword=") {
            hit[4]++; flag |= (hit[4] > 1);
            if (!ValidateKeyword(buffer[i].substr(9))) throw Exception("error:invalid argument");
          }
          else if (len >= 7 && buffer[i].substr(0, 7) == "-price=") {
            hit[5]++; flag |= (hit[5] > 1);
            if (!ValidatePrice(buffer[i].substr(7))) throw Exception("error:invalid argument");
          }
          else throw Exception("error:invalid argument");
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
        if (cnt != 3) throw Exception("error:invalid argument");
        if (!ValidateQuantity(buffer[2])) throw Exception("error:invalid argument");
        if (!ValidateCost(buffer[3])) throw Exception("error:invalid argument");
        BookstoreSys.BookSys.ImportBook(std::stoi(buffer[2]), std::stod(buffer[3]));
      }
      else if (buffer[1] == "log") {}//incomplete
      else if (buffer[1] == "exit") {
        if (cnt != 1) throw Exception("error:invalid argument");
        break;
      }
      else if (buffer[1] == "quit") {
        if (cnt != 1) throw Exception("error:invalid argument");
        break;
      }
      else if (buffer[1].empty()) continue;
      else throw Exception("error:unknown input");
    }
    catch (Exception error) {
      std::cout << "Invalid" << std::endl;
      //std::cout << error.what() << std::endl;
    }
  }
  ExitSystem();
}//检查输入字符合法性未完成

void ExitSystem() {
  //std::cout << "exiting system" << std::endl;
  return;
}