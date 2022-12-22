#include "BookSystem.h"

BookSystem::BookSystem() :ISBNData("ISBN"), NameData("Name"),
                          AuthorData("Author"), KeywordData("Keyword") {
  if (ISBNData.empty()) {
    book_cnt = fin_log_cnt = 0;
    std::ofstream create("Book.dat");
    create.open("fin.dat");
    create.close();
    file.open("Book.dat", std::ios::in | std::ios::out | std::ios::binary);
    fin_log_file.open("fin.dat", std::ios::in | std::ios::out | std::ios::binary);
  }
  else {
    file.open("Book.dat", std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&book_cnt), sizeof(int));
    fin_log_file.open("fin.dat", std::ios::in | std::ios::out | std::ios::binary);
    fin_log_file.seekg(0);
    fin_log_file.read(reinterpret_cast<char*>(&fin_log_cnt), sizeof(int));
    income.resize(fin_log_cnt); outcome.resize(fin_log_cnt);
    for (int i = 0; i < fin_log_cnt; i++) {
      fin_log_file.read(reinterpret_cast<char*>(&income[i]), sizeof(double));
      fin_log_file.read(reinterpret_cast<char*>(&outcome[i]), sizeof(double));
    }
  }
}

BookSystem::~BookSystem() {
  file.seekp(0);
  file.write(reinterpret_cast<char*>(&book_cnt), sizeof(int));
  file.close();
  fin_log_file.seekp(0);
  fin_log_file.write(reinterpret_cast<char*>(&fin_log_cnt), sizeof(int));
  for (int i = 0; i < fin_log_cnt; i++) {
    fin_log_file.write(reinterpret_cast<char*>(&income[i]), sizeof(double));
    fin_log_file.write(reinterpret_cast<char*>(&outcome[i]), sizeof(double));
  }
  fin_log_file.close();
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
    throw Exception("error:authority not enough");
  }
  auto res = ISBNData.find(isbn);
  if (!res.size()) {
    BookstoreBook curBook(isbn);
    ISBNData.insert(element<int>{isbn, ++book_cnt});
    NameData.insert(element<int>{"", book_cnt});
    AuthorData.insert(element<int>{"", book_cnt});
    KeywordData.insert(element<int>{"", book_cnt});
    res.push_back(book_cnt);
    writeBook(book_cnt, curBook);
    std::cout << "new book added" << std::endl;
  }
  UserStack.back().second = res[0];
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
  std::cout << std::fixed << std::setprecision(2) << p.Price << "\t";
  std::cout << p.Quantity << "\n";
}

void BookSystem::ListBook() {
  if (!book_cnt) {
    puts(""); return;
  }
  BookstoreBook curBook;
  auto res = ISBNData.listAll();
  for (int i = 0; i < book_cnt; i++) {
    readBook(res[i], curBook);
    output(curBook);
  }
}

void BookSystem::SearchISBN(const std::string& isbn) {
  if (UserStack.empty()) throw Exception("error:authority not enough");
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
  if (UserStack.empty()) throw Exception("error:authority not enough");
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
  if (UserStack.empty()) throw Exception("error:authority not enough");
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
  if (UserStack.empty()) throw Exception("error:authority not enough");
  if (keyword.find('|') != keyword.npos) throw Exception("error:mutiple keywords when searching");
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
  if (UserStack.empty()) throw Exception("error:authority not enough");
  auto res = ISBNData.find(isbn);
  if (res.empty()) throw Exception("error:book doesn't exist");
  BookstoreBook curBook;
  readBook(res[0], curBook);
  if (curBook.Quantity < q) throw Exception("error:quantity not enough");
  curBook.Quantity -= q;
  writeBook(res[0], curBook);
  fin_log_cnt++;
  if (income.empty()) {
    income.push_back(q * curBook.Price);
    outcome.push_back(0);
  }
  else {
    income.push_back(income.back() + q * curBook.Price);
    outcome.push_back(outcome.back());
  }
  std::cout << std::fixed << std::setprecision(2) << "books bought with a total cost of " << q * curBook.Price << std::endl;
}

void BookSystem::ImportBook(const int& q, const double& totalcost) {
  if (UserStack.empty() || UserStack.back().first.privilege < 3) {
    throw Exception("error:authority not enough");
  }
  if (!UserStack.back().second) throw Exception("error:no selected book");
  BookstoreBook curBook;
  readBook(UserStack.back().second, curBook);
  curBook.Quantity += q;
  writeBook(UserStack.back().second, curBook);
  fin_log_cnt++;
  if (income.empty()) {
    income.push_back(0);
    outcome.push_back(totalcost);
  }
  else {
    income.push_back(income.back());
    outcome.push_back(outcome.back() + totalcost);
  }
  std::cout << std::fixed << std::setprecision(2) << q << " books imported costing " << totalcost << std::endl;
}

void BookSystem::ModifyBook(const int& type, const std::string& str) {
  if (UserStack.empty() || UserStack.back().first.privilege < 3) {
    throw Exception("error:authority not enough");
  }
  if (!UserStack.back().second) throw Exception("error:no selected book");
  int pos = UserStack.back().second;
  BookstoreBook curBook;
  readBook(pos, curBook);
  if (type == 1) {
    if (curBook.ISBN == str) throw Exception("error:ISBN not changed");
    auto ret = ISBNData.find(str);
    if (!ret.empty()) throw Exception("error:identical ISBN");
    ISBNData.del(element<int>{curBook.ISBN, pos});
    strcpy(curBook.ISBN, str.c_str());
    ISBNData.insert(element<int>{curBook.ISBN, pos});
  }
  else if (type == 2) {
    NameData.del(element<int>{curBook.BookName, pos});
    strcpy(curBook.BookName, str.c_str());
    NameData.insert(element<int>{curBook.BookName, pos});
  }
  else if (type == 3) {
    AuthorData.del(element<int>{curBook.Author, pos});
    strcpy(curBook.Author, str.c_str());
    AuthorData.insert(element<int>{curBook.Author, pos});
  }
  else if (type == 4) {
    auto ret1 = getKeyword(str);
    std::unordered_map<std::string, int> keyword_table;
    for (int i = 0; i < ret1.size(); i++) {
      if (keyword_table.find(ret1[i]) != keyword_table.end()) throw Exception("error:identical keyword");
      keyword_table[ret1[i]] = 1;
    }
    auto ret2 = getKeyword(curBook.Keyword);
    for (int i = 0; i < ret2.size(); i++)
      KeywordData.del(element<int>{ret2[i], pos});
    strcpy(curBook.Keyword, str.c_str());
    for (int i = 0; i < ret1.size(); i++)
      KeywordData.insert(element<int>{ret1[i], pos});
  }
  else if (type == 5) curBook.Price = std::stod(str);
  writeBook(pos, curBook);
  std::cout << "info modified" << std::endl;
}

void BookSystem::ShowFinanceLog(int count) {
  if (UserStack.back().first.privilege != 7) throw Exception("error:authority not enough");
  if (!count) {
    puts(""); return;
  }
  if (count > fin_log_cnt) throw Exception("error:invalid count of finance log");
  std::cout << "finance log:" << std::endl;
  if (count == -1) {
    std::cout << std::fixed << std::setprecision(2) << "+" << income.back() << "-" << outcome.back() << std::endl;
    return;
  }
  double incomeVal = income.back() - income[fin_log_cnt - count - 1];
  double outcomeVal = outcome.back() - outcome[fin_log_cnt - count - 1];
  std::cout << std::fixed << std::setprecision(2) << "+" << incomeVal << "-" << outcomeVal << std::endl;
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
