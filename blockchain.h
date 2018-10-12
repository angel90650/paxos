/* blockchain.h */

#ifndef BLOCKCHAIN_H
#define  BLOCKCHAIN_H

#include "block.h"
using namespace std;

class BlockChain{

 public:
  
 
  void insert(block * block);
  void save();
  void load();
  
 private:
  
  block * head;
  block * tail;
  int index;
  
};

#endif
