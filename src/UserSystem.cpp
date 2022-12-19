#include "UserSystem.h"

UserSystem::UserSystem() :UserData("User") {
  if (UserData.empty()) {
    user_cnt = 0;
    file.open(+"");
    BookstoreUser root("root", "sjtu", "", 7);
    UserData.insert(element<int>{ "root", ++user_cnt });
    std::ofstream create("User");
    create.close();
    file.open("User", std::ios::in | std::ios::out | std::ios::binary);
  }
  else {
    file.open("User", std::ios::in | std::ios::out | std::ios::binary);
    file.read(reinterpret_cast<char*>(&user_cnt), sizeof(int));
  }
}

UserSystem::~UserSystem() {

  UserStack.clear();
}

void UserSystem::readUser(int& pos, BookstoreUser& p) {
  auto res = UserData.find(p.user_id); pos = res[0];
  file.seekg((pos - 1) * sizeof(BookstoreUser) + sizeof(user_cnt));
  file.read(reinterpret_cast<char*>(&p), sizeof(BookstoreUser));
}
void UserSystem::writeUser(int& pos, BookstoreUser& p) {
  file.seekp((pos - 1) * sizeof(BookstoreUser) + sizeof(user_cnt));
  file.write(reinterpret_cast<char*>(&p), sizeof(BookstoreUser));
}

void UserSystem::UserRegister(const std::string& id, const std::string& password, const std::string& name, const int& p) {
}

void UserSystem::UserLogin(const std::string& name,const std::string& password) {
}

void UserSystem::ModifyPassword(const std::string& name,const std::string& curPassword,const std::string& newPassword) {

}

void UserSystem::UserLogout(const std::string& name) {
  if (UserStack.back().first.user_name != name) std::cerr << "undefined behaviour" << std::endl;
  UserStack.pop_back();
}

BookstoreUser::BookstoreUser() {
  privilege = 0;
}

BookstoreUser::BookstoreUser(const std::string& id, const std::string& password, const std::string& name, const int& p) {
  privilege = p;
}

bool BookstoreUser::operator<(const BookstoreUser& a)const {
}

bool BookstoreUser::operator>(const BookstoreUser& a)const {
}

bool BookstoreUser::operator<=(const BookstoreUser& a)const {
}

bool BookstoreUser::operator>=(const BookstoreUser& a)const {
}

bool BookstoreUser::operator==(const BookstoreUser& a)const {
}