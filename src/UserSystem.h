#ifndef ACM_BOOKSTORE_USERSYSTEM
#define ACM_BOOKSTORE_USERSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>

class UserSystem {
public:
  UserSystem();
  ~UserSystem();

  void UserRegister();
  void UserLogin();
  void ModifyPassword();
  void UserLogout();
private:
  BookstoreUser* currentUser;
  std::vector<BookstoreUser*> UserStack;
};

class BookstoreUser {
public:
  BookstoreUser();
  ~BookstoreUser();

private:
  std::string user_id;
  std::string user_password;
  std::string user_name;
  int privilege;

};

#endif