/* block.h */

#ifndef BLOCK_H
#define BLOCK_H

#include <queue>
#include <string>

using namespace std;

struct block{
  queue<string> transactions;
  block * next;
  block * prev;
  int depth;

};


#endif
