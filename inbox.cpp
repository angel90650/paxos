/* inbox.cpp */
#include "inbox.h"

void Inbox::insert(string msg){
  mtx.lock();
  inbox.push(msg);
  mtx.unlock();
}

string Inbox::popMsg(){
  string msg;
  mtx.lock();
  msg = inbox.front();
  inbox.pop();
  mtx.unlock();
  return msg;
}

bool Inbox::isEmpty(){
  return inbox.empty();
}

int Inbox::size(){
  return inbox.size();
}

