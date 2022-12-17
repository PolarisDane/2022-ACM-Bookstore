#include "UserSystem.h"

UserSystem::UserSystem() :UserData("User") {
  BookstoreUser root("", "sjtu", "root", 7);
  UserData.insert(element<BookstoreUser>{ "root", root });
}

UserSystem::~UserSystem() {
  UserStack.clear();
}

void UserSystem::UserRegister(const std::string& id, const std::string& password, const std::string& name, const int& p) {
  BookstoreUser cur(id, password, name, p);
  UserData.insert(element<BookstoreUser>{name, cur});
}

void UserSystem::UserLogin(const std::string& name,const std::string& password) {
  auto res = UserData.find(name);
  if (res[0].val.user_password == password) UserStack.push_back(res[0].val);
  else std::cerr << "wrong password" << std::endl;
}

void UserSystem::ModifyPassword(const std::string& name,const std::string& curPassword,const std::string& newPassword) {
  UserData.modify(name, curPassword, newPassword);
}

void UserSystem::UserLogout(const std::string& name) {
  if (UserStack.back().user_name != name) std::cerr << "undefined behaviour" << std::endl;
  UserStack.back().choice = "";
  UserStack.pop_back();
}

BookstoreUser::BookstoreUser() {
  user_id = user_password = user_name = choice = "";
  privilege = 0;
}

BookstoreUser::BookstoreUser(const std::string& id, const std::string& password, const std::string& name, const int& p) {
  user_id = id; user_password = password; user_name = name;
  privilege = p; choice = "";
}

bool BookstoreUser::operator<(const BookstoreUser& a)const {
  return user_name < a.user_name;
}

bool BookstoreUser::operator>(const BookstoreUser& a)const {
  return user_name > a.user_name;
}

bool BookstoreUser::operator<=(const BookstoreUser& a)const {
  return user_name <= a.user_name;
}

bool BookstoreUser::operator>=(const BookstoreUser& a)const {
  return user_name >= a.user_name;
}

bool BookstoreUser::operator==(const BookstoreUser& a)const {
  return user_name == a.user_name;
}