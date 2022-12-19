#include "UnrolledLinkedList.h"

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
  file.open(file_name + ".dat", std::ios::in | std::ios::out);
  if (file.good()) {
    file >> block_cnt >> space_cnt;
    file.close();
    file.open(file_name + ".bin", std::ios::in | std::ios::out | std::ios::binary);
  }
  else {
    std::ofstream create(file_name + ".bin");
    create.close();
    file.open(file_name + ".bin", std::ios::in | std::ios::out | std::ios::binary);
  }//create the file needed if not avaliable
}
template <class T>
LinkList<T>::~LinkList() {
  file.close();
  std::fstream mainData(file_name + ".dat", std::ios::out | std::ios::trunc);
  mainData << block_cnt << " " << space_cnt;
  mainData.close();
}

template <class T>
void LinkList<T>::readNode(const int& pos, node<T>& p) {
  file.seekg((pos - 1) * nodeSize);
  file.read(reinterpret_cast<char*>(&p), nodeSize);
}

template <class T>
void LinkList<T>::readInfo(const int& pos, node<T>& p) {
  file.seekg((pos - 1) * nodeSize);
  file.read(reinterpret_cast<char*>(&p.siz), valSize);
  file.read(reinterpret_cast<char*>(&p.pos), valSize);
  file.read(reinterpret_cast<char*>(&p.pre), valSize);
  file.read(reinterpret_cast<char*>(&p.nxt), valSize);
  file.read(reinterpret_cast<char*>(&p.minimum), elementSize);
  file.read(reinterpret_cast<char*>(&p.maximum), elementSize);
}

template <class T>
void LinkList<T>::writeNode(const int& pos, node<T>& x) {
  x.minimum = x.data[1]; x.maximum = x.data[x.siz];
  file.seekp((pos - 1) * nodeSize);
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
    block_cnt--;
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
    readNode(p.nxt, p);
  }
}//for test only

template <class T>
char LinkList<T>::buffer[64] = "";
