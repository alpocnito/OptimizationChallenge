#include "list_pointers.cpp"

int main() {
  
  List list = {};
  ListInit(&list);

  char* one = "1";
  char* two = "2";
  char* thr = "3";

  ListPush(&list, one);
  ListPush(&list, two);
  ListPush(&list, thr);
  ListDelete(&list, list.number_nodes - 1);
  ListDelete(&list, list.number_nodes - 1);

  ListPush(&list, one);
  ListPush(&list, two);
  ListPush(&list, thr);
  ListPush(&list, one);
  ListPush(&list, two);
  ListPush(&list, thr);
  PrintList(&list);
  
  ListDelete(&list, 4);
  ListDelete(&list, 3);
  ListDelete(&list, 2);
  ListDelete(&list, 1);

  PrintList(&list);
  ListDelete(&list, 0);
  
  PrintList(&list);
  
  ListDisinit(&list);
  return 0;
}
