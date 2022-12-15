#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
const int SQRT = 316;
const int MINBLOCK_SIZE = SQRT / 2;
const int MAXBLOCK_SIZE = SQRT * 2;

class element {
  friend class node;
  friend class LinkList;
private:
  char index[64];
  int val;
public:
  element() {
    memset(index, 0, sizeof(index)); val = 0;
  }
  element(const std::string& str, const int& v) {
    strcpy(index, str.c_str()); val = v;
  }
  ~element() = default;

  bool operator <(const element& a)const {
    int res = strcmp(index, a.index);
    return res < 0 || (!res && val < a.val);
  }
  bool operator ==(const element& a)const {
    int res = strcmp(index, a.index);
    return !res && val == a.val;
  }
  bool operator >(const element& a)const {
    int res = strcmp(index, a.index);
    return res > 0 || (!res && val > a.val);
  }
  bool operator <=(const element& a)const {
    int res = strcmp(index, a.index);
    return res < 0 || (!res && val <= a.val);
  }
  bool operator >=(const element& a)const {
    int res = strcmp(index, a.index);
    return res > 0 || (!res && val >= a.val);
  }
  bool operator <(const std::string& str)const {
    return strcmp(index, str.c_str()) < 0;
  }
  bool operator ==(const std::string& str)const {
    return !strcmp(index, str.c_str());
  }
  bool operator >(const std::string& str)const {
    return strcmp(index, str.c_str()) > 0;
  }
};

class node {
  friend class LinkList;
private:

  int siz;
  int pos, pre, nxt;
  element minimum, maximum;
  element data[MAXBLOCK_SIZE + 1];

public:
  node() {
    siz = pos = pre = nxt = 0;
  }
  node(int siz, int pos, int pre = 0, int nxt = 0)
    :siz(siz), pos(pos), pre(pre), nxt(nxt) {}
};

class LinkList {
private:

  int block_cnt;
  int space_cnt;

  static char buffer[64];
  //static int reuse[1000];
  //static int top;

  const size_t indexSize = sizeof(buffer);
  const size_t valSize = sizeof(int);
  const size_t elementSize = indexSize + valSize;
  const size_t nodeSize = elementSize * (MAXBLOCK_SIZE + 3) + valSize * 4;

  std::fstream file;
  std::string file_name;
public:

  LinkList(const std::string& str) {
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

  ~LinkList() {
    file.close();
    std::fstream mainData(file_name + ".dat", std::ios::out | std::ios::trunc);
    mainData << block_cnt << " " << space_cnt;
    mainData.close();
  }

private:

  void readNode(const int& pos, node& p) {
    if (pos < 1) exit(233);
    file.seekg((pos - 1) * nodeSize);
    file.read(reinterpret_cast<char*>(&p), nodeSize);
  }

  void readInfo(const int& pos, node& p) {
    file.seekg((pos - 1) * nodeSize);
    file.read(reinterpret_cast<char*>(&p.siz), valSize);
    file.read(reinterpret_cast<char*>(&p.pos), valSize);
    file.read(reinterpret_cast<char*>(&p.pre), valSize);
    file.read(reinterpret_cast<char*>(&p.nxt), valSize);
    file.read(reinterpret_cast<char*>(&p.minimum), elementSize);
    file.read(reinterpret_cast<char*>(&p.maximum), elementSize);
  }

  void writeNode(const int& pos, node& x) {
    x.minimum = x.data[1]; x.maximum = x.data[x.siz];
    file.seekp((pos - 1) * nodeSize);
    file.write(reinterpret_cast<char*>(&x), nodeSize);
  }

public:
  void split(node& p) {
    node tmp, nxtNode; tmp.siz = p.siz / 2;
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
  void merge(node& p) {
    if (!p.nxt) return;
    node nxtNode;
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
  void insert(const element& x) {
    if (block_cnt == 0) {
      block_cnt++; node tmp;
      tmp.siz = 1; tmp.pos = ++space_cnt;
      tmp.data[1] = x;
      writeNode(1, tmp);
      return;
    }
    node p;
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
  void del(const element& x) {
    if (!block_cnt) return;
    node p, nxtNode;
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
  void find(const std::string& index) {
    if (!block_cnt) {
      std::cout << "null" << std::endl;
      return;
    }
    bool find_flag = false;
    node p;
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
          std::cout << p.data[j].val << " ";
          find_flag = true;
        }
        if (index < p.data[j].index) break;
      }
      if (i == block_cnt) break;
      readInfo(p.nxt, p);
    }
    if (!find_flag) std::cout << "null";
    std::cout << std::endl;
  }
  void output() {
    node p;
    readNode(1, p);
    for (int i = 1; i <= block_cnt; i++) {
      std::cout << "node" << i << ":" << std::endl;
      for (int j = 1; j <= p.siz; j++) {
        std::cout << p.data[j].index << ":" << p.data[j].val << std::endl;
      }
      readNode(p.nxt, p);
    }
  }//for test only
};

char LinkList::buffer[64] = "";
//int LinkList::reuse[1000] = { 0 };
//int LinkList::top = 0;

signed main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); std::cout.tie(0);
  LinkList myList("data");
  int n;
  std::cin >> n;
  std::string op, index;
  int val;
  for (int i = 1; i <= n; i++) {
    std::cin >> op;
    if (op == "insert") {
      std::cin >> index >> val;
      myList.insert(element{ index, val });
    }
    else if (op == "delete") {
      std::cin >> index >> val;
      myList.del(element{ index, val });
    }
    else {
      std::cin >> index;
      myList.find(index);
    }
  }
  return 0;
}
