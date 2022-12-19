#ifndef ACM_BOOKSTORE_USERSYSTEM
#define ACM_BOOKSTORE_USERSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UnrolledLinkedList.h"

class BookstoreUser {
  friend class UserSystem;
public:
  BookstoreUser();
  BookstoreUser(const std::string& id, const std::string& password, const std::string& name, const int& p);
  ~BookstoreUser() = default;

  bool operator <(const BookstoreUser& a)const;
  bool operator >(const BookstoreUser& a)const;
  bool operator <=(const BookstoreUser& a)const;
  bool operator >=(const BookstoreUser& a)const;
  bool operator ==(const BookstoreUser& a)const;

private:
  char user_id[31];
  char user_password[31];
  char user_name[31];
  int privilege;
};

class UserSystem {
public:
  UserSystem();
  ~UserSystem();

  void readUser(int& pos, BookstoreUser& p);
  void writeUser(int& pos, BookstoreUser& p);

  void UserRegister(const std::string& id, const std::string& password, const std::string& name, const int& p);
  void UserLogin(const std::string& name, const std::string& password);
  void ModifyPassword(const std::string& name,const std::string& curPassword,const std::string& newPassword);
  void UserLogout(const std::string& name);
private:
  std::fstream file;

  int user_cnt;
  LinkList<int> UserData;
  std::vector<std::pair<BookstoreUser, std::string>> UserStack;
};

#endif