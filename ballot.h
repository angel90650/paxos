/* ballot.h */
#ifndef BALLOT_H
#define BALLOT_H

#include <tuple>
#include <mutex>
using namespace std;

class Ballot{

 public:
  tuple<int, int, int> getCurrent();
  tuple<int, int, int> getNew(int block);
  string str();
  void update(int higherNum);
  void setID(int ID);
  void setNum(int balNum);
  int num();
  
  
 private:
  int id;
  int blockDepth;
  int ballotNum;
  int highestNum;
  mutex mtx;
};

#endif
  
