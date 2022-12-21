#include <iostream>
#include "UnrolledLinkedList.h"
#include "UserSystem.h"
#include "BookSystem.h"
#include "BookstoreSystem.h"
#include "Utils.h"

signed main() {
  //BookstoreSys.UserSys.UserLogin("root", "sjtu");
  //UserSystem b;
  //b.UserLogin("root", "sjtu");
  BookstoreWork();
  return 0;
}
/*
  a.UserAdd("Polaris_Dane", "qweasdzxc", "", 0);
  a.UserDelete("Polaris_Dane");
  a.UserLogin("Polaris_Dane", "qweasdzxc");
*/

/*
  b.selectBook("1");
  b.ModifyBook(2, "clorftxdy");
  b.ModifyBook(3, "clorf");
  b.ModifyBook(4, "clorf|Polaris");
  b.ModifyBook(5, "3");
  b.ImportBook(2, 4);
  b.BuyBook("1", 1);
  b.ImportBook(2, 100);
  b.BuyBook("1", 3);
  b.ShowFinanceLog(-1);
  b.SearchName("clorftxdy");
  b.SearchAuthor("clorf");
  b.SearchKeyword("clorf");
  b.SearchKeyword("Polaris");
  //a.UserDelete("Polaris_Dane");
*/