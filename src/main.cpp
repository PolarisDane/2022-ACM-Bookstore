#include <iostream>
#include "UnrolledLinkedList.h"
#include "UserSystem.h"
#include "BookSystem.h"

signed main() {
  UserSystem a;
  BookSystem b;
  a.UserLogin("root", "sjtu");
  b.selectBook("1");
  b.ModifyBook(2, "clorftxdy");
  b.ModifyBook(3, "clorf");
  b.ModifyBook(4, "clorf|Polaris");
  b.SearchName("clorftxdy");
  b.SearchAuthor("clorf");
  b.SearchKeyword("clorf");
  b.SearchKeyword("Polaris");
  //a.UserDelete("Polaris_Dane");
  return 0;
}
/*
  a.UserAdd("Polaris_Dane", "qweasdzxc", "", 0);
  a.UserDelete("Polaris_Dane");
  a.UserLogin("Polaris_Dane", "qweasdzxc");
*/