#ifndef ACM_BOOKSTORE_BOOKSYSTEM
#define ACM_BOOKSTORE_BOOKSYSTEM

#include <iostream>
#include <cmath>
#include <algorithm>

class BookSystem {
public:
  BookSystem();
  ~BookSystem();

  BookstoreBook* SearchISBN();
  BookstoreBook* SearchName();
  BookstoreBook* SearchAuthor();
  BookstoreBook* SearchKeyword();

  void BuyBook();
  void ImportBook();

private:
  std::string selected_ISBN;
};


class BookstoreBook {
public:
  BookstoreBook();
  ~BookstoreBook();

private:
  std::string ISBN;
  std::string BookName;
  std::string Author;

  int Quantity;
  int Price;
  int TotalCost;
};

#endif