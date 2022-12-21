#ifndef ACM_BOOKSTORE_UNROLLEDLINKEDLIST
#define ACM_BOOKSTORE_UNROLLEDLINKEDLIST

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
static const int SQRT = 316;
static const int MINBLOCK_SIZE = SQRT / 2;
static const int MAXBLOCK_SIZE = SQRT * 2;

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
  std::vector<T> listAll();
};

template <class T>
element<T>::element() {
  memset(index, 0, sizeof(index)); val = 0;
}
template <class T>
element<T>::element(const std::string& str, const T& v) {
  strcpy(index, str.c_str()); val = v;
}

template <class T>
bool element<T>::operator <(const element<T>& a)const {
  int res = strcmp(index, a.index);
  return res < 0 || (!res && val < a.val);
}
template <class T>
bool element<T>::operator ==(const element<T>& a)const {
  int res = strcmp(index, a.index);
  return !res && val == a.val;
}
template <class T>
bool element<T>::operator >(const element<T>& a)const {
  int res = strcmp(index, a.index);
  return res > 0 || (!res && val > a.val);
}
template <class T>
bool element<T>::operator <=(const element<T>& a)const {
  int res = strcmp(index, a.index);
  return res < 0 || (!res && val <= a.val);
}
template <class T>
bool element<T>::operator >=(const element<T>& a)const {
  int res = strcmp(index, a.index);
  return res > 0 || (!res && val >= a.val);
}
template <class T>
bool element<T>::operator <(const std::string& str)const {
  return strcmp(index, str.c_str()) < 0;
}
template <class T>
bool element<T>::operator ==(const std::string& str)const {
  return !strcmp(index, str.c_str());
}
template <class T>
bool element<T>::operator >(const std::string& str)const {
  return strcmp(index, str.c_str()) > 0;
}

template <class T>
node<T>::node() {
  siz = pos = pre = nxt = 0;
}
template <class T>
node<T>::node(int siz, int pos, int pre, int nxt)
  :siz(siz), pos(pos), pre(pre), nxt(nxt) {}

template <class T>
LinkList<T>::LinkList(const std::string& str) {
  block_cnt = 0; space_cnt = 0; file_name = str;
  file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
  if (file.good()) {
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&block_cnt), intSize);
    file.read(reinterpret_cast<char*>(&space_cnt), intSize);
  }
  else {
    std::ofstream create(file_name);
    create.close();
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
  }//create the file needed if not avaliable
}
template <class T>
LinkList<T>::~LinkList() {
  file.seekp(0);
  file.write(reinterpret_cast<char*>(&block_cnt), intSize);
  file.write(reinterpret_cast<char*>(&space_cnt), intSize);
  file.close();
}

template <class T>
void LinkList<T>::readNode(const int& pos, node<T>& p) {
  if (pos < 1) std::cout << "233" << std::endl;
  file.seekg((pos - 1) * nodeSize + intSize * 2);
  file.read(reinterpret_cast<char*>(&p), nodeSize);
}

template <class T>
void LinkList<T>::readInfo(const int& pos, node<T>& p) {
  if (pos < 1) std::cout << "233" << std::endl;
  file.seekg((pos - 1) * nodeSize + intSize * 2);
  file.read(reinterpret_cast<char*>(&p.siz), valSize);
  file.read(reinterpret_cast<char*>(&p.pos), valSize);
  file.read(reinterpret_cast<char*>(&p.pre), valSize);
  file.read(reinterpret_cast<char*>(&p.nxt), valSize);
  file.read(reinterpret_cast<char*>(&p.minimum), elementSize);
  file.read(reinterpret_cast<char*>(&p.maximum), elementSize);
}

template <class T>
void LinkList<T>::writeNode(const int& pos, node<T>& x) {
  if (pos < 1) std::cout << "233" << std::endl;
  x.minimum = x.data[1]; x.maximum = x.data[x.siz];
  file.seekp((pos - 1) * nodeSize + intSize * 2);
  file.write(reinterpret_cast<char*>(&x), nodeSize);
}

template <class T>
bool LinkList<T>::empty() { return block_cnt == 0; }

template <class T>
void LinkList<T>::split(node<T>& p) {
  node<T> tmp, nxtNode; tmp.siz = p.siz / 2;
  tmp.nxt = p.nxt; tmp.pre = p.pos;
  tmp.pos = p.nxt = ++space_cnt;
  p.siz = p.siz - tmp.siz;
  for (int i = 1; i <= tmp.siz; i++) tmp.data[i] = p.data[i + p.siz];
  if (tmp.nxt) {
    readNode(tmp.nxt, nxtNode);
    nxtNode.pre = tmp.pos;
    writeNode(nxtNode.pos, nxtNode);
  }
  writeNode(tmp.pos, tmp);
  block_cnt++;
}
template <class T>
void LinkList<T>::merge(node<T>& p) {
  if (!p.nxt) return;
  node<T> nxtNode;
  readNode(p.nxt, nxtNode);
  if (p.siz + nxtNode.siz >= MAXBLOCK_SIZE) return;
  for (int i = 1; i <= nxtNode.siz; i++) p.data[i + p.siz] = nxtNode.data[i];
  p.siz += nxtNode.siz; p.nxt = nxtNode.nxt;
  if (nxtNode.nxt) {
    readNode(nxtNode.nxt, nxtNode);
    nxtNode.pre = p.pos;
    writeNode(nxtNode.pos, nxtNode);
  }
  block_cnt--;
}
template <class T>
void LinkList<T>::insert(const element<T>& x) {
  if (block_cnt == 0) {
    block_cnt++; node<T> tmp;
    tmp.siz = 1; tmp.pos = ++space_cnt;
    tmp.data[1] = x;
    writeNode(1, tmp);
    return;
  }
  node<T> p;
  readNode(1, p);
  if (x < p.data[1]) {
    p.siz++;
    for (int j = p.siz; j >= 1; j--) p.data[j] = p.data[j - 1];
    p.data[1] = x;
    if (p.siz == MAXBLOCK_SIZE) split(p);
    writeNode(p.pos, p);
    return;
  }
  bool flag = false;
  while (x >= p.minimum) {
    if (p.nxt) readInfo(p.nxt, p);
    else {
      flag = 1; break;
    }
  }
  if (p.pre && !flag) readNode(p.pre, p);
  else readNode(p.pos, p);
  //find the suitable place for insert
  if (x < p.data[1]) {
    p.siz++;
    for (int j = p.siz; j >= 1; j--) p.data[j] = p.data[j - 1];
    p.data[1] = x;
  }
  else {
    int pos = std::lower_bound(p.data + 1, p.data + p.siz + 1, x) - p.data;
    p.siz++;
    for (int j = p.siz; j > pos; j--) p.data[j] = p.data[j - 1];
    p.data[pos] = x;
  }
  if (p.siz == MAXBLOCK_SIZE) split(p);
  writeNode(p.pos, p);
  return;
}
template <class T>
void LinkList<T>::del(const element<T>& x) {
  if (!block_cnt) return;
  node<T> p, nxtNode;
  readInfo(1, p);
  bool flag = false;
  while (x >= p.minimum) {
    if (p.nxt) readInfo(p.nxt, p);
    else {
      flag = 1; break;
    }
  }
  if (p.pre && !flag) readNode(p.pre, p);
  else readNode(p.pos, p);
  bool find_flag = false;
  int pos = std::lower_bound(p.data + 1, p.data + p.siz + 1, x) - p.data;
  if (x == p.data[pos]) {
    find_flag = true;
    for (int i = pos; i <= p.siz; i++) p.data[i] = p.data[i + 1];
    p.siz--;
  }
  if (find_flag && p.siz == 0 && !p.nxt && !p.pre) {
    block_cnt--; space_cnt = 0;
    return;
  }
  if (find_flag) {
    merge(p); writeNode(p.pos, p);
  }
  return;
}
template <class T>
std::vector<T> LinkList<T>::find(const std::string& index) {
  std::vector<T> res; res.clear();
  if (!block_cnt) {
    return res;
  }
  node<T> p;
  readInfo(1, p);
  for (int i = 1; i <= block_cnt; i++) {
    if (index < p.minimum.index) break;
    if (index > p.maximum.index) {
      if (i == block_cnt) break;
      readInfo(p.nxt, p); continue;
    }
    readNode(p.pos, p);
    for (int j = 1; j <= p.siz; j++) {
      if (index == p.data[j].index) {
        res.push_back(p.data[j].val);
      }
      if (index < p.data[j].index) break;
    }
    if (i == block_cnt) break;
    readInfo(p.nxt, p);
  }
  return res;
}
template <class T>
void LinkList<T>::output() {
  node<T> p;
  readNode(1, p);
  for (int i = 1; i <= block_cnt; i++) {
    std::cout << "node" << i << ":" << std::endl;
    for (int j = 1; j <= p.siz; j++) {
      std::cout << p.data[j].index << ":" << p.data[j].val << std::endl;
    }
    if (!p.nxt) break;
    readNode(p.nxt, p);
  }
}//for test only
template <class T>
std::vector<T> LinkList<T>::listAll() {
  std::vector<T> res;
  node<T> p;
  readNode(1, p);
  for (int i = 1; i <= block_cnt; i++) {
    for (int j = 1; j <= p.siz; j++)
      res.push_back(p.data[j].val);
    if (!p.nxt) break;
    readNode(p.nxt, p);
  }
  return res;
}

template <class T>
char LinkList<T>::buffer[64] = "";

#endif