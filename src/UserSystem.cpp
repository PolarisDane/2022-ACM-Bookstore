#include "UserSystem.h"

std::vector<std::pair<BookstoreUser, std::string>> UserStack;

UserSystem::UserSystem() :UserData("User") {
  UserStack.clear();
  if (UserData.empty()) {
    user_cnt = 0;
    BookstoreUser root("root", "sjtu", "", 7);
    UserData.insert(element<int>{ "root", ++user_cnt });
    std::ofstream create("User.dat");
    create.close();
    file.open("User.dat", std::ios::in | std::ios::out | std::ios::binary);
    writeUser(user_cnt, root);
  }
  else {
    file.open("User.dat", std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&user_cnt), sizeof(int));
  }
}

UserSystem::~UserSystem() {
  file.seekp(0);
  file.write(reinterpret_cast<char*>(&user_cnt), sizeof(int));
  file.close();
  UserStack.clear();
}

void UserSystem::readUser(int& pos, BookstoreUser& p) {
  file.seekg((pos - 1) * sizeof(BookstoreUser) + sizeof(int));
  file.read(reinterpret_cast<char*>(&p), sizeof(BookstoreUser));
}
void UserSystem::writeUser(int& pos, BookstoreUser& p) {
  file.seekp((pos - 1) * sizeof(BookstoreUser) + sizeof(int));
  file.write(reinterpret_cast<char*>(&p), sizeof(BookstoreUser));
}

void UserSystem::UserRegister(const std::string& id, const std::string& password, const std::string& name) {
  auto res = UserData.find(id);
  if (res.size()) {
    std::cerr << "error:user id already exists" << std::endl;
    return;
  }
  BookstoreUser newUser(id, password, name, 1);
  UserData.insert(element<int>{id, ++user_cnt});
  writeUser(user_cnt, newUser);
  std::cout << "user registered:\nid:" << id << "\npassword:" << password << "\nname:" << name << std::endl;
}

void UserSystem::UserLogin(const std::string& id,const std::string& password) {
  auto res = UserData.find(id);
  if (!res.size()) {
    std::cerr << "error:user doesn't exists" << std::endl;
    return;
  }
  BookstoreUser curUser;
  readUser(res[0], curUser);
  if (!UserStack.empty() && UserStack.back().first.privilege > curUser.privilege) {
    curUser.login++;
    UserStack.push_back(std::make_pair(curUser, "0"));
    writeUser(res[0], curUser);
    std::cout << "login success by override" << std::endl;
    return;
  }
  if (curUser.user_password != password) {
    std::cerr << "error:wrong password" << std::endl;
    return;
  }
  curUser.login++;
  UserStack.push_back(std::make_pair(curUser, ""));
  writeUser(res[0], curUser);
  std::cout << "login success:" << id << std::endl;
}

void UserSystem::ModifyPassword(const std::string& id,const std::string& curPassword,const std::string& newPassword) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = UserData.find(id);
  if (!res.size()) {
    std::cerr << "error:user doesn't exists" << std::endl;
    return;
  }
  BookstoreUser curUser;
  readUser(res[0], curUser);
  if (UserStack.back().first.privilege == 7) {
    strcpy(curUser.user_password, newPassword.c_str());
    writeUser(res[0], curUser);
    std::cout << "user password modified by override:\nid:" << id << std::endl;
    return;
  }
  if (curUser.user_password != curPassword) {
    std::cerr << "error:wrong password" << std::endl;
    return;
  }
  strcpy(curUser.user_password, newPassword.c_str());
  writeUser(res[0], curUser);
  std::cout << "user password modified:\nid:" << id << std::endl;
}

void UserSystem::UserLogout() {
  if (UserStack.empty()) {
    std::cerr << "error:log out failed, no current user available" << std::endl;
    return;
  }
  BookstoreUser curUser = UserStack.back().first;
  curUser.login--;
  auto res = UserData.find(curUser.user_id);
  writeUser(res[0], curUser);
  UserStack.pop_back();
  std::cout << "logout success:" << curUser.user_id << "\nlogin times left:" << curUser.login << std::endl;
}

void UserSystem::UserAdd(const std::string& id, const std::string& password, const std::string& name, const int& p) {
  if (UserStack.empty() || p >= UserStack.back().first.privilege || UserStack.back().first.privilege < 3) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = UserData.find(id);
  if (res.size()) {
    std::cerr << "error:user id already exists" << std::endl;
    return;
  }
  BookstoreUser newUser(id, password, name, p);
  UserData.insert(element<int>{id, ++user_cnt});
  writeUser(user_cnt, newUser);
  std::cout << "user added:\nid:" << id << "\npassword:" << password << "\nname:" << name << "\nprivilege:" << p << std::endl;
}

void UserSystem::UserDelete(const std::string& id) {
  if (UserStack.empty() || UserStack.back().first.privilege != 7) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = UserData.find(id);
  if (!res.size()) {
    std::cerr << "error:user doesn't exists" << std::endl;
    return;
  }
  BookstoreUser curUser;
  readUser(res[0], curUser);
  if (curUser.login) {
    std::cerr << "error:delete failed, user logined in" << std::endl;
    return;
  }
  UserData.del(element<int>{id, res[0]});
  std::cout << "delete success:" << id << std::endl;
}

BookstoreUser::BookstoreUser() {
  memset(user_id, 0, sizeof(user_id));
  memset(user_name, 0, sizeof(user_name));
  memset(user_password, 0, sizeof(user_password));
  privilege = 0; login = 0;
}

BookstoreUser::BookstoreUser(const std::string& id, const std::string& password, const std::string& name, const int& p) {
  strcpy(user_id, id.c_str());
  strcpy(user_password, password.c_str());
  strcpy(user_name, name.c_str());
  privilege = p; login = 0;
}

bool BookstoreUser::operator<(const BookstoreUser& a)const {
  return user_id < a.user_id;
}

bool BookstoreUser::operator>(const BookstoreUser& a)const {
  return user_id > a.user_id;
}

bool BookstoreUser::operator==(const BookstoreUser& a)const {
  return user_id == a.user_id;
}

