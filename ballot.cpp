/* ballot.cpp */

#include "ballot.h"

tuple<int, int, int> Ballot::getCurrent(){
  mtx.lock();
  tuple<int, int, int> ballot (ballotNum, id, blockDepth);
  mtx.unlock();
  return ballot;
}

tuple<int, int, int> Ballot::getNew(int block){
  mtx.lock();
  tuple<int, int, int> ballot (highestNum + 1, id, block);
  highestNum = highestNum + 1;
  blockDepth = block;
  ballotNum = highestNum;
  mtx.unlock();
  return ballot;
}

string Ballot::str(){
  string ballot;
  mtx.lock();
  ballot = to_string(ballotNum) + " " + to_string(id) + " " + to_string(blockDepth);
  mtx.unlock();
  return ballot;
}

void Ballot::setID(int ID){
  id = ID;
}

void Ballot::setNum(int balNum){
  mtx.lock();
  ballotNum = balNum;
  mtx.unlock();
}

int Ballot::num(){
  int balNum;
  mtx.lock();
  balNum = ballotNum;
  mtx.unlock();
  return balNum;
}


  
  
