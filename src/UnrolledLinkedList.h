#ifndef ACM_BOOKSTORE_UNROLLEDLINKEDLIST
#define ACM_BOOKSTORE_UNROLLEDLINKEDLIST

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
const int SQRT = 316;
const int MINBLOCK_SIZE = SQRT / 2;
const int MAXBLOCK_SIZE = SQRT * 2;

template <class T> class element;
template <class T> class node;
template <class T> class LinkList;

template <class T>
class element {
  friend class node<T>;
  friend class LinkList<T>;
public:

  char index[64];
  T val;

public:

  element();
  element(const std::string& str, const T& v);
  ~element() = default;

  bool operator <(const element<T>& a)const;
  bool operator ==(const element<T>& a)const;
  bool operator >(const element<T>& a)const;
  bool operator <=(const element<T>& a)const;
  bool operator >=(const element<T>& a)const;

  bool operator <(const std::string& str)const;
  bool operator ==(const std::string& str)const;
  bool operator >(const std::string& str)const;

};

template<class T>
class node {
  friend class LinkList<T>;
private:

  int siz;
  int pos, pre, nxt;

  element<T> minimum, maximum;
  element<T> data[MAXBLOCK_SIZE + 1];

public:

  node();
  node(int siz, int pos, int pre = 0, int nxt = 0);
  ~node() = default;

};

template<class T>
class LinkList {
private:

  int block_cnt;
  int space_cnt;

  static char buffer[64];

  const size_t indexSize = sizeof(buffer);
  const size_t valSize = sizeof(T);
  const size_t intSize = sizeof(int);
  const size_t elementSize = indexSize + valSize;
  const size_t nodeSize = elementSize * (MAXBLOCK_SIZE + 3) + intSize * 4;

  std::fstream file;
  std::string file_name;

public:

  LinkList() = default;

  LinkList(const std::string& str);

  ~LinkList();

private:

  void readNode(const int& pos, node<T>& p);
  void readInfo(const int& pos, node<T>& p);
  void writeNode(const int& pos, node<T>& x);

public:
  bool empty();
  void split(node<T>& p);
  void merge(node<T>& p);
  void insert(const element<T>& x);
  void del(const element<T>& x);
  std::vector<T> find(const std::string& index);

  void output();
};

#endif