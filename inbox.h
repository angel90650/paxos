/* inbox.h */

#ifndef INBOX_H
#define INBOX_H

#include <queue>
#include <string>
#include <mutex>

using namespace std;

class Inbox{

 public:

  void insert(string msg);
  bool isEmpty();
  int size();
  string popMsg();

 private:
  queue<string> inbox;
  mutex mtx;
};
  
#endif

  
