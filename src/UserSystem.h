#ifndef ACM_BOOKSTORE_USERSYSTEM
#define ACM_BOOKSTORE_USERSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UnrolledLinkedList.h"
#include "Exception.h"

class BookstoreUser {
  friend class UserSystem;
  friend class BookSystem;
  friend class BookstoreSystem;
public:

  BookstoreUser();
  BookstoreUser(const std::string& id, const std::string& password, const std::string& name, const int& p);
  ~BookstoreUser() = default;

  bool operator <(const BookstoreUser& a)const;
  bool operator >(const BookstoreUser& a)const;
  bool operator ==(const BookstoreUser& a)const;

private:

  char user_id[31];
  char user_password[31];
  char user_name[31];
  int privilege, login;

};

extern std::vector<std::pair<BookstoreUser, int>> UserStack;

class UserSystem {
  friend class BookSystem;
  friend class BookstoreSystem;
public:

  UserSystem();
  ~UserSystem();

  void readUser(int& pos, BookstoreUser& p);
  void writeUser(int& pos, BookstoreUser& p);

  void UserRegister(const std::string& id, const std::string& password, const std::string& name);
  void UserLogin(const std::string& id, const std::string& password);
  void ModifyPassword(const std::string& id,const std::string& curPassword,const std::string& newPassword);
  void UserLogout();
  void UserAdd(const std::string& id, const std::string& password, const std::string& name, const int& p);
  void UserDelete(const std::string& id);

private:

  std::fstream file;

  int user_cnt;
  LinkList<int> UserData;

};

#endif