#include "BookSystem.h"

BookSystem::BookSystem() :ISBNData("ISBN"), NameData("Name"),
                          AuthorData("Author"), KeywordData("Keyword") {
  if (ISBNData.empty()) {
    book_cnt = 0;
    std::ofstream create("Book.dat");
    create.close();
    file.open("Book.dat", std::ios::in | std::ios::out | std::ios::binary);
  }
  else {
    file.open("Book.dat", std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&book_cnt), sizeof(int));
  }
}

BookSystem::~BookSystem() {
  file.seekp(0);
  file.write(reinterpret_cast<char*>(&book_cnt), sizeof(int));
  file.close();
}

std::vector<std::string> getKeyword(const std::string& str) {
  std::vector<std::string> res;
  std::string tmp;
  if (!str.length()) {
    res.push_back("");
    return res;
  }
  int pos = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '|') {
      tmp = str.substr(pos, i - pos);
      pos = i + 1;
      res.push_back(tmp);
    }
  }
  tmp = str.substr(pos, str.length() - pos + 1);
  res.push_back(tmp);
  return res;
}

void BookSystem::selectBook(const std::string& isbn) {
  if (UserStack.empty() || UserStack.back().first.privilege < 3) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = ISBNData.find(isbn);
  if (!res.size()) {
    BookstoreBook curBook(isbn);
    ISBNData.insert(element<int>{isbn, ++book_cnt});
    NameData.insert(element<int>{"", book_cnt});
    AuthorData.insert(element<int>{"", book_cnt});
    KeywordData.insert(element<int>{"", book_cnt});
    writeBook(book_cnt, curBook);
    std::cout << "new book added" << std::endl;
  }
  UserStack.back().second = isbn;
  std::cout << "book selected\n" << "ISBN:" << isbn << std::endl;
}

void BookSystem::readBook(int& pos, BookstoreBook& p) {
  file.seekg((pos - 1) * sizeof(BookstoreBook) + sizeof(int));
  file.read(reinterpret_cast<char*>(&p), sizeof(BookstoreBook));
}
void BookSystem::writeBook(int& pos, BookstoreBook& p) {
  file.seekp((pos - 1) * sizeof(BookstoreBook) + sizeof(int));
  file.write(reinterpret_cast<char*>(&p), sizeof(BookstoreBook));
}

void BookSystem::output(const BookstoreBook& p) {
  std::cout << "search result:";
  std::cout << p.ISBN << "\t";
  std::cout << p.BookName << "\t";
  std::cout << p.Author << "\t";
  std::cout << p.Keyword << "\t";
  std::cout << p.Price << "\t";
  std::cout << p.Quantity << "\n";
}

void BookSystem::SearchISBN(const std::string& isbn) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = ISBNData.find(isbn);
  std::vector<BookstoreBook> ret;
  BookstoreBook curBook;
  for (int i = 0; i < res.size(); i++) {
    readBook(res[i], curBook);
    ret.push_back(curBook);
  }
  //sort(ret.begin(), ret.end());
  for (int i = 0; i < ret.size(); i++)
    output(ret[i]);
  if (ret.empty()) puts("");
}

void BookSystem::SearchName(const std::string& name) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = NameData.find(name);
  std::vector<BookstoreBook> ret;
  BookstoreBook curBook;
  for (int i = 0; i < res.size(); i++) {
    readBook(res[i], curBook);
    ret.push_back(curBook);
  }
  sort(ret.begin(), ret.end());
  for (int i = 0; i < ret.size(); i++)
    output(ret[i]);
  if (ret.empty()) puts("");
}

void BookSystem::SearchAuthor(const std::string& author) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = AuthorData.find(author);
  std::vector<BookstoreBook> ret;
  BookstoreBook curBook;
  for (int i = 0; i < res.size(); i++) {
    readBook(res[i], curBook);
    ret.push_back(curBook);
  }
  sort(ret.begin(), ret.end());
  for (int i = 0; i < ret.size(); i++)
    output(ret[i]);
  if (ret.empty()) puts("");
}

void BookSystem::SearchKeyword(const std::string& keyword) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = KeywordData.find(keyword);
  std::vector<BookstoreBook> ret;
  BookstoreBook curBook;
  for (int i = 0; i < res.size(); i++) {
    readBook(res[i], curBook);
    ret.push_back(curBook);
  }
  sort(ret.begin(), ret.end());
  for (int i = 0; i < ret.size(); i++)
    output(ret[i]);
  if (ret.empty()) puts("");
}

void BookSystem::BuyBook(const std::string& isbn, const int& q) {
  if (UserStack.empty()) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  auto res = ISBNData.find(isbn);
  if (res.empty()) {
    std::cerr << "error:book doesn't exist" << std::endl;
    return;
  }
  BookstoreBook curBook;
  readBook(res[0], curBook);
  if (curBook.Quantity < q) {
    std::cerr << "error:quantity not enough" << std::endl;
    return;
  }
  curBook.Quantity -= q;
  writeBook(res[0], curBook);
  std::cout << "books bought with a total cost of " << q * curBook.Price << std::endl;
}

void BookSystem::ImportBook(const int& q, const double& totalcost) {
  if (UserStack.empty() || UserStack.back().first.privilege < 3) {
    std::cerr << "error:authority not enough" << std::endl;
    return;
  }
  if (UserStack.back().second == "") {
    std::cerr << "error:no selected book" << std::endl;
    return;
  }
  auto res = ISBNData.find(UserStack.back().second);
  BookstoreBook curBook;
  readBook(res[0], curBook);
  curBook.Quantity += q;
  writeBook(res[0], curBook);
  std::cout << q << " books imported costing " << totalcost << std::endl;
  //此处应该有log记录
}

void BookSystem::ModifyBook(const int& type, const std::string& str) {
  if (UserStack.empty() || UserStack.back().second == "") {
    std::cerr << "error:no selected book" << std::endl;
    return;
  }
  std::string curISBN = UserStack.back().second;
  auto res = ISBNData.find(curISBN);
  BookstoreBook curBook;
  readBook(res[0], curBook);
  if (type == 1) {
    if (curISBN == str) {
      std::cerr << "error:ISBN not changed" << std::endl;
      return;
    }
    ISBNData.del(element<int>{curBook.ISBN, res[0]});
    strcpy(curBook.ISBN, str.c_str());
    ISBNData.insert(element<int>{curBook.ISBN, res[0]});
  }
  else if (type == 2) {
    strcpy(curBook.BookName, str.c_str());
    NameData.del(element<int>{curBook.BookName, res[0]});
    NameData.insert(element<int>{curBook.BookName, res[0]});
  }
  else if (type == 3) {
    strcpy(curBook.Author, str.c_str());
    AuthorData.del(element<int>{curBook.Author, res[0]});
    AuthorData.insert(element<int>{curBook.Author, res[0]});
  }
  else if (type == 4) {
    auto ret = getKeyword(curBook.Keyword);
    for (int i = 0; i < ret.size(); i++)
      KeywordData.del(element<int>{ret[i], res[0]});
    strcpy(curBook.Keyword, str.c_str());
    ret = getKeyword(str);
    for (int i = 0; i < ret.size(); i++)
      KeywordData.insert(element<int>{ret[i], res[0]});
  }
  else if (type == 5) curBook.Price = std::stod(str);
  writeBook(res[0], curBook);
  std::cout << "info modified" << std::endl;
}

BookstoreBook::BookstoreBook() {
  memset(ISBN, 0, sizeof(ISBN));
  memset(BookName, 0, sizeof(BookName));
  memset(Author, 0, sizeof(Author));
  memset(Keyword, 0, sizeof(Keyword));
  Quantity = Price = 0;
}

BookstoreBook::BookstoreBook(std::string isbn, std::string name,
                             std::string author, std::string keyword,
                             int q, double p) {
  strcpy(ISBN, isbn.c_str()); strcpy(BookName, name.c_str());
  strcpy(Author, author.c_str()); strcpy(Keyword, keyword.c_str());
  Quantity = q; Price = p;
}

bool BookstoreBook::operator<(const BookstoreBook& a)const {
  return strcmp(ISBN, a.ISBN) < 0;
}

bool BookstoreBook::operator>(const BookstoreBook& a)const {
  return strcmp(ISBN, a.ISBN) > 0;
}

bool BookstoreBook::operator==(const BookstoreBook& a)const {
  return strcmp(ISBN, a.ISBN) == 0;
}
