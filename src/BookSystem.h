#ifndef ACM_BOOKSTORE_BOOKSYSTEM
#define ACM_BOOKSTORE_BOOKSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>
#include "UnrolledLinkedList.h"
#include "UserSystem.h"

class BookstoreBook {
  friend class BookSystem;
public:

  BookstoreBook();
  BookstoreBook(std::string isbn, std::string name="",
                std::string author="", std::string keyword="",
                int q=0, double p=0);
  ~BookstoreBook() = default;

  bool operator <(const BookstoreBook& a)const;
  bool operator >(const BookstoreBook& a)const;
  bool operator ==(const BookstoreBook& a)const;

private:

  char ISBN[21];
  char BookName[61];
  char Author[61];
  char Keyword[61];

  int Quantity;
  double Price;

};

class BookSystem {
public:

  BookSystem();
  ~BookSystem();

  void readBook(int& pos, BookstoreBook& p);
  void writeBook(int& pos, BookstoreBook& p);

  void output(const BookstoreBook& p);

  void SearchISBN(const std::string& isbn);
  void SearchName(const std::string& name);
  void SearchAuthor(const std::string& author);
  void SearchKeyword(const std::string& keyword);

  void selectBook(const std::string& isbn);

  void BuyBook(const std::string& isbn, const int& q);
  void ImportBook(const int& q, const double& totalcost);

  void ModifyBook(const int& type, const std::string& str);

  friend std::vector<std::string> getKeyword(const std::string& str);

private:

  std::fstream file;

  int book_cnt;
  LinkList<int> ISBNData, NameData, AuthorData, KeywordData;

};

#endif