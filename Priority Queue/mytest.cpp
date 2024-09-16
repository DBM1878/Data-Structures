#include "squeue.h"
#include "driver.cpp"

const int NUMINSERTS = 10;

//priority functions
int myPriorityFn1(const Post &post) {
  //needs MAXHEAP
  //calculates priority based on likes and interest level
  //priority value falls in the range [1-510]
  //the highest priority would be 500+10 = 510
  //the lowest priority would be 0+1 = 1
  //the larger value means the higher priority
  int priority = post.getNumLikes() + post.getInterestLevel();
  return priority;
}
int myPriorityFn2(const Post &post) {
  //needs MINHEAP
  //calculates priority based on post time and connection level
  //priority value falls in the range [2-55]
  //the highest priority would be 1+1 = 2
  //the lowest priority would be 5+50 = 55
  //the smaller value means the higher priority
  int priority = post.getPostTime() + post.getConnectLevel();
  return priority;
}
//helper functions
void Tester::randomInsert(SQueue &queue) {
  //function is given a queue and inserts a number of posts with random values
  //int NumInserts = 10;
  
  Random idGen(MINPOSTID,MAXPOSTID);
  Random likesGen(MINLIKES,MAXLIKES);
  Random timeGen(MINTIME,MAXTIME);
  Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
  Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
  for(int i=0;i<NUMINSERTS;i++){
    Post aPost(idGen.getRandNum(),
	       likesGen.getRandNum(),
	       conLevelGen.getRandNum(),
	       timeGen.getRandNum(),
	       interestGen.getRandNum());
    queue.insertPost(aPost);
  }
}
void Tester::checkHeapPropertyMIN(Node* node) {
  if(node != nullptr) {
    if(node->m_left != nullptr and node->m_right != nullptr) {
      if(myPriorityFn2(node->m_post) > myPriorityFn2(node->m_left->m_post) or myPriorityFn2(node->m_post) > myPriorityFn2(node->m_right->m_post)) {
        throw exception();
      }
    } else if(node->m_left == nullptr and node->m_right != nullptr) {
      if(myPriorityFn2(node->m_post) > myPriorityFn2(node->m_right->m_post)) {
	throw exception();
      }
    } else if(node->m_left != nullptr and node->m_right == nullptr) {
      if(myPriorityFn2(node->m_post) > myPriorityFn2(node->m_left->m_post)) {
	throw exception();
      }
    }
    checkHeapPropertyMIN(node->m_left);
    checkHeapPropertyMIN(node->m_right);
  }
}
void Tester::checkHeapPropertyMAX(Node* node) {
  if(node != nullptr) {
    if(node->m_left != nullptr and node->m_right != nullptr) {
      if(myPriorityFn2(node->m_post) < myPriorityFn2(node->m_left->m_post) or myPriorityFn2(node->m_post) < myPriorityFn2(node->m_right->m_post)) {
	throw exception();
      }
    } else if(node->m_left == nullptr and node->m_right != nullptr) {
      if(myPriorityFn2(node->m_post) < myPriorityFn2(node->m_right->m_post)) {
	throw exception();
      }
    } else if(node->m_left != nullptr and node->m_right == nullptr) {
      if(myPriorityFn2(node->m_post) < myPriorityFn2(node->m_left->m_post)) {
	throw exception();
      }
    }
    checkHeapPropertyMAX(node->m_left);
    checkHeapPropertyMAX(node->m_right);
  }
}
//recursive function that calls search for each node in queue1, checking if that node is in queue2
void Tester::compare(Node* queue1Ptr, Node* queue2Ptr) {
  if(queue1Ptr != nullptr) {
    if(search(queue2Ptr, queue1Ptr->m_post.m_postID) != true) {
      throw exception();
    }
    compare(queue1Ptr->m_left, queue2Ptr);
    compare(queue1Ptr->m_right, queue2Ptr);
  }
}
//search function that looks for a node in the heap
bool Tester::search(Node* node, int id) {
  if(node == nullptr) {
    return false;
  }
  if(node->m_post.m_postID == id) {
    return true;
  }
  if(search(node->m_left, id) == true) {
    return true;
  }
  if(search(node->m_right, id) == true) {
    return true;
  }
  return false;
}

//testing functions
bool Tester::testMinInsertNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //test if heap property is maintained
  try {
    std::cout << queue1.m_heap << endl;
    checkHeapPropertyMIN(queue1.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testMaxInsertNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn1, MAXHEAP);
  randomInsert(queue1);
  //test if heap property is maintained
  try {
    checkHeapPropertyMAX(queue1.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testMinRemoveNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //test if heap property is maintained
  try {
    for(int i = 0; i < NUMINSERTS; i++) {
      //removes then checks if heap property is maintained ensuring correct order of removal
      queue1.getNextPost();
      checkHeapPropertyMIN(queue1.m_heap);
    }
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testMaxRemoveNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn1, MAXHEAP);
  randomInsert(queue1);
  //test if heap property is maintained
  try {
    for(int i = 0; i < NUMINSERTS; i++) {
      //removes then checks if heap property is maintained ensuring correct order of removal
      queue1.getNextPost();
      checkHeapPropertyMAX(queue1.m_heap);
    }
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::newPriorityNewHeapSameData() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //creates copy of queue1 to check if it has the same data
  SQueue queue2(queue1);
  //new priority function and checking if same data is there
  try {
    queue1.setPriorityFn(myPriorityFn1, MAXHEAP);
    checkHeapPropertyMAX(queue1.m_heap);
    compare(queue1.m_heap, queue2.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::mergeEmptyQueueWithNonEmpty() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //create empty queue
  SQueue queue2(myPriorityFn2, MINHEAP);
  //try merge
  try {
    queue2.mergeWithQueue(queue1);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testCopyNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //creates copy of queue1 and checks if it has the same data
  try {
    SQueue queue2(queue1);
    compare(queue1.m_heap, queue2.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testCopyEdge() {
  bool success = true;
  //create empty queue
  SQueue queue1(myPriorityFn2, MINHEAP);
  //randomInsert(queue1);
  //creates copy of queue1 and checks if it has the same data
  try {
    SQueue queue2(queue1);
    compare(queue1.m_heap, queue2.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testAssignmentNormal() {
  bool success = true;
  //create queue and insert a number of posts with random values
  SQueue queue1(myPriorityFn2, MINHEAP);
  randomInsert(queue1);
  //creates copy of queue1 and checks if it has the same data
  try {
    SQueue queue2(myPriorityFn2, MINHEAP);
    queue2 = queue1;
    compare(queue1.m_heap, queue2.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::testAssignmentEdge() {
  bool success = true;
  //create empty queue
  SQueue queue1(myPriorityFn2, MINHEAP);
  //randomInsert(queue1);
  //creates copy of queue1 and checks if it has the same data
  try {
    SQueue queue2(myPriorityFn2, MINHEAP);
    queue2 = queue1;
    compare(queue1.m_heap, queue2.m_heap);
  } catch(...) {
    success = false;
  }
  return success;
}
bool Tester::dequeueOnEmptyThrowsException() {
  bool success = false;
  //create empty queue
  SQueue queue1(myPriorityFn2, MINHEAP);
  try {
    queue1.getNextPost();
  } catch(...) {
    success = true;
  }
  return success;
}
bool Tester::mergeQueuesWithDifferentPriorityThrowException() {
  bool success = false;
  //create 2 queues with different priorityFn's
  SQueue queue1(myPriorityFn2, MINHEAP);
  SQueue queue2(myPriorityFn1, MAXHEAP);
  try {
    queue1.mergeWithQueue(queue2);
  } catch(...) {
    success = true;
  }
  return success;
}

int main() {
  //Tester test;

  //std::cout << "test insert" << endl;
  //std::cout << test.testMinInsertNormal() << endl;
  //std::cout << test.testMaxInsertNormal() << endl;

  //std::cout << "test remove" << endl;
  //std::cout << test.testMinRemoveNormal() << endl;
  //std::cout << test.testMaxRemoveNormal() << endl;

  //std::cout << "test new priority function, new heap, same data" << endl;
  //std::cout << test.newPriorityNewHeapSameData() << endl;

  //std::cout << "test merege empty queue with non-empty queue" << endl;
  //std::cout << test.mergeEmptyQueueWithNonEmpty() << endl;

  //std::cout << "test copy constructor" << endl;
  //std::cout << test.testCopyNormal() << endl;
  //std::cout << test.testCopyEdge() << endl;

  //std::cout << "test assignment operator" << endl;
  //std::cout << test.testAssignmentNormal() << endl;
  //std::cout << test.testAssignmentEdge() << endl;

  //std::cout << "test dequeue on empty heap" << endl;
  //std::cout << test.dequeueOnEmptyThrowsException() << endl;

  //std::cout << "test merge queues with different priority dunctions" << endl;
  //std::cout << test.mergeQueuesWithDifferentPriorityThrowException() << endl;

  return 0;
}
