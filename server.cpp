/* server.cpp
 * Angel Castro, Nery Lara
 * CS 171 Final Project
 * Paxos System
 * 
 *
 *
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cerrno>
#include <thread>
#include <system_error>
#include <mutex>
#include <sstream>
#include <vector>
#include <strings.h>
#include "ballot.h"
#include "inbox.h"
#include "blockchain.h"
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;

#define SIZE 1024
#define SERVERS 5
int PORT, ID;
int sockfd;
Ballot ballot;
Inbox proposals, values, decisions, acks, promises;
struct sockaddr_in addr[SERVERS];
sem_t proposeLock;
bool leader = false;



/* send buffer to all */
void sendAll(const char * buf);
/* send proposal to all */
void sendProposalAll();
/* send promise to proposer */
void sendPromise();
/* send value to be accepted to all*/
void sendValueAll();
/* send acknowledgement for value to proposer*/
void sendAck();
/* send decision to all */
void sendDecisionAll();
/* send promise to proposer  */

/* send ack for value to proposer */

/* Print error message and exit */
void error(string msg);
void parsePromise(string promise);
bool recvProposal();
bool recvDecision();
bool recvValue();
bool recvAckMajority();
bool recvPromiseMajority();
void setAddresses();


/* threads */
void messageHandler();
void proposeHandler();
void acceptHandler();
void learnHandler();
void userHandler();

int main(int argc, char ** argv){
  /* serverID */
  if(argc != 2)
    error("< ID >");
  ID = atoi(argv[1]);
  if(ID < 1 || ID > 5)
    error("Invalid ID.");

  int optval = 1;
  char buf[SIZE];
  ballot.setID(ID);
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
  setAddresses();
  if(bind(sockfd, (struct sockaddr *) &addr[ID], sizeof(addr[ID])) < 0)
    error("bind failed");

  sem_init(&proposeLock, 0, 0);
  thread messageThread(messageHandler);
  thread proposeThread(proposeHandler);
  thread acceptThread(acceptHandler);
  thread learnThread(learnHandler);
  thread userThread(userHandler);

  messageThread.join();
  proposeThread.join();
  acceptThread.join();
  learnThread.join();
  userThread.join();
}

void messageHandler(){
  socklen_t clientlen;
  int counter = 1;
  struct sockaddr_in clientaddr;
  char buf[SIZE];
  char msg_type;
  string tempStr;
  while(1){
    memset(buf, 0, SIZE);
    recvfrom(sockfd, buf, SIZE, 0, (struct sockaddr *)&clientaddr, &clientlen);
    tempStr = buf;
    msg_type = buf[0];
    switch(msg_type){
    case 'p':
      cout << "received proposal: " << to_string(counter) << " " << tempStr << endl;
      proposals.insert(to_string(counter) + " " +tempStr);
     
      break;
    case 'v':
      values.insert(to_string(counter) + " " +tempStr);
      break;
    case 'd':
      decisions.insert(to_string(counter) + " " +tempStr);
      break;
    case 'a':
      acks.insert(to_string(counter) + " " +tempStr);
      break;
    case 'r':
      promises.insert(to_string(counter) + " " +tempStr);
      break;
    }
    counter++;
  }
}

void proposeHandler(){

  while(1){
    sem_wait(&proposeLock);
    if(!leader){
      sendProposalAll();
      if(recvPromiseMajority()){
	leader = true;
      }
    }else{ /* leader */
      sendValueAll();
      if(recvAckMajority()){
	sendDecisionAll();
      }
    }
  }
}

void acceptHandler(){

  while(1){
    if(recvProposal()){
      sendPromise();
    }
    if(recvValue()){
      sendAck();
    }
  }
}

void learnHandler(){
  while(1){
    recvDecision();
  }
}

void userHandler(){
  
  sem_post(&proposeLock);

}






void sendAll(const char * buf){
  for(int i = 0; i < 5; i++){
    sendto(sockfd, buf, SIZE, 0, (struct sockaddr *)&addr[i], sizeof(addr[i]));    
  } 
}

void sendProposalAll(){
  /* set up proposal */
  string proposal;
  char * msg;
  ballot.getNew(1);
  proposal = "p " + ballot.str();
  cout << "send proposal: " << proposal << endl;
  sendAll(proposal.c_str());
}

void sendPromise(){

}

void sendValueAll(){
  
}

void sendAck(){

}

void sendDecisionAll(){

}

bool recvPromiseMajority(){
  int count;
  string promise;
  
  while (count < 3){
    if(promises.isEmpty()){}
    else{
      promise = promises.popMsg();
      parsePromise(promise);
    }
  }
}

bool recvProposal(){
  string proposal;
  stringstream ss;
  int balNum;
  int id;
  int blockDepth;
  int counter;
  bool rtnVal = false;
 
  while(!proposals.isEmpty()){
    proposal = proposals.popMsg();
    cout << "rcvProposal string: " << proposal << endl; 
    ss << proposal;
    ss >> counter;
    ss >> balNum;
    ss >> balNum;
    ss >> id;
    ss >> blockDepth;

    if(balNum >= ballot.num()){
      ballot.setNum(balNum);
      rtnVal = true;
      cout << "return val: " << rtnVal << endl; 
    }
  }
  return rtnVal;
}
bool recvValue(){

}

bool recvAckMajority(){
  
}

bool recvDecision(){

}

void setAddresses(){
  string fileName = "config.txt";
  ifstream file;
  struct hostent * server;
  string ip;
  int port;
  file.open(fileName);
  for(int i = 0; i < 5; i++){
    file>>ip;
    file>>port;
    server = gethostbyname(ip.c_str());
    bzero((char *) &addr[i], sizeof(addr[i]));
    addr[i].sin_family = AF_INET;
    inet_pton(AF_INET, server->h_addr, &addr[i].sin_addr);
    addr[i].sin_port = htons((unsigned short)port);
  }
  file.close();
}

void error(string msg) {
  cout << msg << endl;
  exit(EXIT_FAILURE);
}

void parsePromise(string promise){

}
