#ifndef ACM_BOOKSTORE_USERSYSTEM
#define ACM_BOOKSTORE_USERSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UnrolledLinkedList.h"

class BookstoreUser {
  friend class UserSystem;
  friend class LinkList<BookstoreUser>;
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
  std::string user_id;
  std::string user_password;
  std::string user_name;
  int privilege;
  std::string choice;//appointed using ISBN(at least for now)
};

class UserSystem {
public:
  UserSystem();
  ~UserSystem();

  void UserRegister(const std::string& id, const std::string& password, const std::string& name, const int& p);
  void UserLogin(const std::string& name, const std::string& password);
  void ModifyPassword(const std::string& name,const std::string& curPassword,const std::string& newPassword);
  void UserLogout(const std::string& name);
private:
  LinkList<BookstoreUser> UserData;
  std::vector<BookstoreUser> UserStack;
};

#endif