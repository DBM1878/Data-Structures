// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType) {
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
}

SQueue::~SQueue() {
  recursiveDelete(m_heap);
  m_heap = nullptr;
}

SQueue::SQueue(const SQueue& rhs) {
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  recursiveDataCopy(rhs.m_heap);
}

SQueue& SQueue::operator=(const SQueue& rhs) {
  recursiveDelete(m_heap);
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  recursiveDataCopy(rhs.m_heap);
  return *this;
}

void SQueue::insertPost(const Post& post) {
  if(m_heap == nullptr) {
    //if heap is empty, creates node of the post and sets root to nodeInsert
    Node* nodeInsert = new Node(post);
    m_heap = nodeInsert;
    m_size++;
  } else {
    //creates temp queue and inserts post in temp queue.
    //then merges the temp queue with current queue
    SQueue tempQueue(m_priorFunc, m_heapType);
    tempQueue.insertPost(post);
    mergeWithQueue(tempQueue);
    //set tempQueue.m_heap to nullptr as to not delete inserted node when destructor is called
    tempQueue.m_heap = nullptr;
  }
}

Post SQueue::getNextPost() {
  //if heap is empty throw out_of_range exception
  if(m_heap == nullptr) {
    throw std::out_of_range("out_of_range");
  }
  //set temp pointer equal to root, then merge the left and right children and return temp
  Node* temp = m_heap;
  m_heap = m_heap->m_left;
  SQueue tempQueue(m_priorFunc, m_heapType);
  tempQueue.m_heap = temp->m_right;
  mergeWithQueue(tempQueue);
  m_size--;
  return temp->m_post;
}

void SQueue::mergeWithQueue(SQueue& rhs) {
  //checks for self-assignment
  if(m_heap == rhs.m_heap) {
    //throw exception("self_assignment");
    throw std::invalid_argument("self_assignment");
  }
  //checks if the priority functions of the merging queues are the same
  if(m_priorFunc != rhs.m_priorFunc) {
    throw std::invalid_argument("domain_error");
  }
  //if rhs is empty nothing happens
  if(rhs.m_heap != nullptr) {
    //if heap is empty and rhs is not, sets m_heap equal to rhs.m_heap
    if(m_heap == nullptr) {
      m_heap = rhs.m_heap;
    } else {
      //swaps subtrees of heap and then merges with rhs with left child of m_heap
      swapSubtree(m_heap);
      m_heap->m_left = mergeQueues(m_heap->m_left, rhs.m_heap, m_heapType);
    }
  }
}

void SQueue::clear() {
  recursiveDelete(m_heap);
  m_heap = nullptr;
  m_size = 0;
}

int SQueue::numPosts() const {
  return m_size;
}

void SQueue::printPostsQueue() const {
  printHeapHelper(m_heap);
}

prifn_t SQueue::getPriorityFn() const {
  return *m_priorFunc;
}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  Node* oldHeap = m_heap;
  m_heap = nullptr;
  recursiveDataCopy(oldHeap);
  recursiveDelete(oldHeap);
}

void SQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

void SQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Post& post) {
  sout << "Post ID: " << post.getPostID() 
        << ", number of likes: " << post.getNumLikes()
        << ", interest level: " << post.getInterestLevel() 
        << ", connection level: " << post.getConnectLevel()
        << ", post time: " << post.getPostTime();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPost();
  return sout;
}

//fucntion that recursively deletes all nodes in heap
void SQueue::recursiveDelete(Node* node) {
  if(node != nullptr) {
    recursiveDelete(node->m_left);
    recursiveDelete(node->m_right);
    delete node;
  }
}

//recursive function that prints out nodes in heap
void SQueue::printHeapHelper(Node* node) const {
  if(node != nullptr) {
    std::cout << "[" << (*m_priorFunc)(node->m_post) << "]";
    std::cout << "Post ID: " << node->m_post.m_postID;
    std::cout << ", number of likes: " << node->m_post.m_likes;
    std::cout << ", interest level: " << node->m_post.m_interestLevel;
    std::cout << ", connection level: " << node->m_post.m_connectLevel;
    std::cout << ", post time: " << node->m_post.m_postTime << endl;

    printHeapHelper(node->m_left);
    printHeapHelper(node->m_right);
  }
}

//recursive function that copies data from one queue to another
void SQueue::recursiveDataCopy(Node* node) {
  if(node != nullptr) {
    Post aPost(node->m_post.m_postID, node->m_post.m_likes, node->m_post.m_interestLevel, node->m_post.m_connectLevel, node->m_post.m_postTime);
    insertPost(aPost);
    recursiveDataCopy(node->m_left);
    recursiveDataCopy(node->m_right);
  }
}

//function that handles swapping subtrees when mergeing queues
void SQueue::swapSubtree(Node* root) {
  Node* temp = root->m_left;
  root->m_left = root->m_right;
  root->m_right = temp;
}

//recursive function that merges two queues
Node* SQueue::mergeQueues(Node* subtree, Node* rhsHeap, HEAPTYPE heapType) {
  //if subtree is nullptr, return rhsHeap
  if(subtree == nullptr) {
    return rhsHeap;
  } else {
    //if heapType is MINHEAP
    if(heapType == MINHEAP) {
      //check if subtree has higher priority
      //if yes, merge rhsHeap with subtree's left child
      if((*m_priorFunc)(subtree->m_post) < (*m_priorFunc)(rhsHeap->m_post)) {
        subtree->m_left = mergeQueues(subtree->m_left, rhsHeap, heapType);
      } else {
	//swap subtree's children then merge rhsHeap with subtree's left child
        swapSubtree(subtree);
        subtree->m_left = mergeQueues(subtree->m_left, rhsHeap, heapType);
      }
    } else {
      //if heapType is MAXHEAP
      //check if subtree has higher priority
      //if yes, merge rhsHeap with subtree's left child
      if((*m_priorFunc)(subtree->m_post) > (*m_priorFunc)(rhsHeap->m_post)) {
	subtree->m_left = mergeQueues(subtree->m_left, rhsHeap, heapType);
      } else {
	//swap subtree's children then merge rhsHeap with subtree's left child
	swapSubtree(subtree);
	subtree->m_left = mergeQueues(subtree->m_left, rhsHeap, heapType);
      }
    }
  }
}
