// UMBC - CMSC 341 - Spring 2022 - Proj2
#include "fleet.h"
Fleet::Fleet(){
  m_root = nullptr;
}
Fleet::~Fleet(){
  recursiveDelete(m_root);
  m_root = nullptr;
}
void Fleet::insert(const Ship& ship){
  if(ship.m_id < MINID or ship.m_id > MAXID) {
    //throw("Ship ID is out of range, cannot insert into fleet");
    std::cout << "Ship ID is out of range, cannot insert into fleet" << endl;
  } else {
    Ship* temp = new Ship(ship.m_id, ship.m_type, ship.m_state);
    if(m_root == nullptr) {
      m_root = temp;
    } else {
      insertHelper(m_root, temp);
    }
  }
}

void Fleet::clear(){
  recursiveDelete(m_root);
  m_root = nullptr;
}

void Fleet::remove(int id){
  if(id < MINID or id > MAXID) {
    std::cout << "Ship id out of range" << endl;
  } else if(findShip(id) != true) {
    std::cout << "Ship is not in fleet" << endl;
  } else {
    if(m_root->m_id == id) {
      removeHelperRoot();
    } else {
      removeHelperOther(m_root, id);
    }
  }
}

void Fleet::updateHeight(Ship* aShip){
  if(aShip->m_left == nullptr and aShip->m_right == nullptr) {
    aShip->m_height = 0;
  } else if(aShip->m_left == nullptr and aShip->m_right != nullptr) {
    aShip->m_height = aShip->m_right->m_height + 1;
  } else if(aShip->m_left != nullptr and aShip->m_right == nullptr) {
    aShip->m_height = aShip->m_left->m_height + 1;
  } else {
    if(aShip->m_left->m_height > aShip->m_right->m_height) {
      aShip->m_height = aShip->m_left->m_height + 1;
    } else {
      aShip->m_height = aShip->m_right->m_height + 1;
    }
  }
}
int Fleet::checkImbalance(Ship* aShip){
  if(aShip != nullptr) {
    if(aShip->m_left == nullptr and aShip->m_right == nullptr) {
      return 0;
    } else if(aShip->m_left != nullptr and aShip->m_right == nullptr) {
      return aShip->m_left->m_height;
    } else if(aShip->m_left == nullptr and aShip->m_right != nullptr) {
      return aShip->m_right->m_height;
    } else {
      if(aShip->m_left->m_height > aShip->m_right->m_height) {
	return (aShip->m_left->m_height - aShip->m_right->m_height);
      } else {
	return (aShip->m_right->m_height - aShip->m_left->m_height);
      }
    }
  }
  return 0;
}
Ship* Fleet::rebalance(Ship* aShip){
  //std::cout << "start" << endl;
  if(aShip->m_right == nullptr) {
    //std::cout << "right nullptr" << endl;
    if(aShip->m_left->m_height > 1) {
      //std::cout << "right rotate" << endl;
      rightRotate(aShip);
    }
  } else if(aShip->m_left == nullptr) {
    std::cout << "left nullptr" << endl;
    if(aShip->m_right->m_height > 1) {
      std::cout << "left rotate" << endl;
      leftRotate(aShip);
    }
  } else if(aShip->m_left->m_height > (aShip->m_right->m_height + 1)) {
    //std::cout << "no nullptr" << endl;
    //std::cout << "left greater than right, right rotate" << endl;
    rightRotate(aShip);
  } else {
    //std::cout << "no nullptr" << endl;
    //std::cout << "right greater than left, left rotate" << endl;
    leftRotate(aShip);
  }
  return aShip;
}

void Fleet::dumpTree() const {
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const{
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}
void Fleet::listShips() const {
  listHelper(m_root);
}

bool Fleet::setState(int id, STATE state){
  Ship* temp = m_root;
  while(temp->m_id != id and temp != nullptr) {
    if(temp->m_id > id) {
      temp = temp->m_left;
    } else {
      temp = temp->m_right;
    }
  }
  if(temp == nullptr) {
    return false;
  } else {
    temp->m_state = state;
    return true;
  }
}

void Fleet::removeLost(){
  removeLostHelper(m_root);
}

bool Fleet::findShip(int id) const {
  Ship* temp = m_root;
  while(temp != nullptr and temp->m_id != id) {
    if(temp->m_id > id) {
      temp = temp->m_left;
    } else {
      temp = temp->m_right;
    }
  }
  if(temp == nullptr) {
    return false;
  } else {
    return true;
  }
}
void Fleet::recursiveDelete(Ship* aShip) {
  if(aShip == nullptr) {
  } else {
    recursiveDelete(aShip->m_left);
    recursiveDelete(aShip->m_right);
    delete aShip;
  }
}
void Fleet::insertHelper(Ship* current, Ship* ship) {
    if(ship->m_id < current->m_id) {
      if(current->m_left == nullptr) {
	current->m_left = ship;
      } else {
        insertHelper(current->m_left, ship);
      }
    } else if(ship->m_id > current->m_id) {
      if(current->m_right == nullptr) {
	current->m_right = ship;
      } else {
        insertHelper(current->m_right, ship);
      }
    } else if(ship->m_id == current->m_id) {
      //throw("Ship ID already in fleet");
      //std::cout << "Ship ID " << ship->m_id << " already in fleet" << endl;
    }
    updateHeight(current);
    if(checkImbalance(current) > 1) {
      Ship* parent = m_root;
      if(parent != ship) {
	std::cout << "ship address: " << ship << endl;
	std::cout << "ship id: " << ship->m_id << endl;
	while(parent->m_left != ship and parent->m_right != ship) {
	  if(parent->m_id < ship->m_id) {
	    parent = parent->m_right;
	  } else {
	    parent = parent->m_left;
	  }
	  
	  if(parent->m_left != nullptr) {
	  std::cout << "parent left address: " << parent->m_left << endl;
	  std::cout << "parent left id: " << parent->m_left->m_id << endl;
	  }
	  if(parent->m_right != nullptr) {
	  std::cout << "parent right address: " << parent->m_right << endl;
	  std::cout << "parent right id: " << parent->m_right->m_id << endl;
	  }
	  std::cout << endl;
	  
	  /*
	  if(parent->m_id < ship->m_id) {
	    parent = parent->m_right;
	  } else {
	    parent = parent->m_left;
	  }
	  */
	}
      }
      std::cout << "rebalance" << endl;
      rebalance(current);
    }
}
void Fleet::listHelper(Ship* aShip) const {
  if(aShip != nullptr) {
    listHelper(aShip->m_left);
    //std::cout << aShip->m_id << ":" << STATE(aShip->m_state) << ":" << SHIPTYPE(aShip->m_type) << endl;
    std::cout << aShip->m_id << ":";
    if(aShip->m_state == 0) {
      std::cout << "ALIVE:";
    } else {
      std::cout << "LOST:";
    }
    if(aShip->m_type == 0) {
      std::cout << "CARGO" << endl;
    } else if(aShip->m_type == 1) {
      std::cout << "TELESCOPE" << endl;
    } else if(aShip->m_type == 2) {
      std::cout << "COMMUNICATOR" << endl;
    } else if(aShip->m_type == 3) {
      std::cout << "FULECARRIER" << endl;
    } else if(aShip->m_type == 4) {
      std::cout << "ROBOCARRIER" << endl;
    }
    listHelper(aShip->m_right);
  }
}
void Fleet::removeHelperRoot() {
  if(m_root->m_left == nullptr and m_root->m_right == nullptr) {
    delete m_root;
    m_root = nullptr;
  } else if(m_root->m_left == nullptr and m_root->m_right != nullptr) {
    delete m_root;
    m_root = m_root->m_right;
  } else if(m_root->m_left != nullptr and m_root->m_right == nullptr) {
    delete m_root;
    m_root = m_root->m_left;
  } else {
    Ship* curr = m_root->m_left;
    Ship* prev = m_root->m_left;
    while(curr->m_right != nullptr) {
      curr = curr->m_right;
    }
    while(prev->m_right != curr) {
      prev = prev->m_right;
    }
    curr->m_right = m_root->m_right;
    if(curr->m_left != nullptr) {
      prev->m_right = curr->m_left;
    } else {
      prev->m_right = nullptr;
      curr->m_left = m_root->m_left;
    }
    delete m_root;
    m_root = curr;
  }
}
void Fleet::removeHelperOther(Ship* curr, int id) {
  if(curr->m_id == id) {
    if(curr->m_left == nullptr and curr->m_right == nullptr) {
      Ship* temp = m_root;
      while(temp->m_left != curr and temp->m_right != curr) {
	if(id < temp->m_id) {
	  temp = temp->m_left;
	} else {
	  temp = temp->m_right;
	}
      }
      if(temp->m_left == curr) {
	temp->m_left = nullptr;
      } else {
	temp->m_right = nullptr;
      }
      delete curr;
    } else if(curr->m_left == nullptr and curr->m_right != nullptr) {
      Ship* temp = m_root;
      while(temp->m_left != curr and temp->m_right != curr) {
	if(id < temp->m_id) {
	  temp = temp->m_left;
	} else {
	  temp = temp->m_right;
	}
      }
      if(temp->m_left == curr) {
	temp->m_left = curr->m_right;
      } else {
	temp->m_right = curr->m_right;
      }
      delete curr;
    } else if(curr->m_left != nullptr and curr->m_right == nullptr) {
      Ship* temp = m_root;
      while(temp->m_left != curr and temp->m_right != curr) {
	if(id < temp->m_id) {
	  temp = temp->m_left;
	} else {
	  temp = temp->m_right;
	}
      }
      if(temp->m_left == curr) {
	temp->m_left = curr->m_left;
      } else {
	temp->m_right = curr->m_left;
      }
      delete curr;
    } else {
      Ship* temp = curr->m_left;
      Ship* tempParent = curr->m_left;
      while(temp->m_right != nullptr) {
	temp = temp->m_right;
      }
      while(tempParent->m_right != temp) {
	tempParent = tempParent->m_right;
      }
      curr->m_id = temp->m_id;
      curr->m_type = temp->m_type;
      curr->m_state = temp->m_state;
      if(temp->m_left != nullptr) {
	tempParent->m_right = temp->m_left;
      } else {
        tempParent->m_right = nullptr;
      }
      delete temp;
    }
  } else {
    if(id < curr->m_id) {
      removeHelperOther(curr->m_left, id);
    } else {
      removeHelperOther(curr->m_right, id);
    }
  }
  updateHeight(curr);
  if(checkImbalance(curr) > 1) {
    rebalance(curr);
  }
}
void Fleet::removeLostHelper(Ship* aShip) {
  if(aShip != nullptr) {
    STATE state = LOST;
    removeLostHelper(aShip->m_left);
    removeLostHelper(aShip->m_right);
    if(aShip->m_state == state) {
      remove(aShip->m_id);
    }
  }
}
void Fleet::leftRotate(Ship* aShip) {
  //if(aShip->m_left == nullptr) {
  std::cout << "left rotate start" << endl;
    Ship* parent = m_root;
    if(parent != aShip) {
      std::cout << "aShip address: " << aShip << endl;
      std::cout << "aShip id: " << aShip->m_id << endl;
      while(parent->m_left != aShip and parent->m_right != aShip) {
	if(parent->m_left != nullptr) {
	  std::cout << "parent left address: " << parent->m_left << endl;
	  std::cout << "parent left id: " << parent->m_left->m_id << endl;
	}
	if(parent->m_right != nullptr) {
	  std::cout << "parent right address: " << parent->m_right << endl;
	  std::cout << "parent right id: " << parent->m_right->m_id << endl;
	}
	std::cout << endl;
        if(parent->m_id < aShip->m_id) {
	  parent = parent->m_right;
        } else {
	  parent = parent->m_left;
        }
	/*
	if(parent->m_left != nullptr) {
	  std::cout << "parent left address: " << parent->m_left << endl;
	  std::cout << "parent left id: " << parent->m_left->m_id << endl;
	}
	if(parent->m_right != nullptr) {
	  std::cout << "parent right address: " << parent->m_right << endl;
	  std::cout << "parent right id: " << parent->m_right->m_id << endl;
	}
	std::cout << endl;
	*/
      }
    }
    if(aShip->m_right->m_left == nullptr) {
      aShip->m_right->m_left = aShip;
      if(parent->m_left == aShip) {
	parent->m_left = aShip->m_right;
      } else {
	parent->m_right = aShip->m_right;
      }
      aShip->m_right = nullptr;
    } else {
      aShip->m_right->m_left->m_left = aShip;
      aShip->m_right->m_left->m_right = aShip->m_right;
      if(parent->m_left == aShip) {
	parent->m_left = aShip->m_right->m_left;
      } else {
	parent->m_right = aShip->m_right->m_left;
      }
      aShip->m_right->m_left = nullptr;
      aShip->m_right = nullptr;
    }
  //}
  /*
  else {
    Ship* parent = m_root;
    while(parent->m_left != aShip or parent->m_right != aShip) {
      if(parent->m_id < aShip->m_id) {
	parent = parent->m_right;
      } else {
	parent = parent->m_left;
      }
    }
    if(aShip->m_left->m_left == nullptr) {
      aShip->m_left->m_left = aShip;
      if(parent->m_left == aShip) {
	parent->m_left = aShip->m_left;
      } else {
	parent->m_right = aShip->m_left;
      }
      aShip->m_right = nullptr;
    } else {
      aShip->m_left->m_left->m_left = aShip;
      aShip->m_left->m_left->m_right = aShip->m_left;
      if(parent->m_left == aShip) {
	parent->m_left = aShip->m_left->m_left;
      } else {
	parent->m_right = aShip->m_left->m_left;
      }
      aShip->m_left->m_left = nullptr;
      aShip->m_left = nullptr;
    }
  }
  */
}
void Fleet::rightRotate(Ship* aShip) {
  Ship* parent = m_root;
  if(parent != aShip) {
    while(parent->m_left != aShip and parent->m_right != aShip) {
      if(parent->m_id < aShip->m_id) {
        parent = parent->m_right;
      } else {
        parent = parent->m_left;
      }
    }
  }
  if(aShip->m_left->m_right == nullptr) {
    aShip->m_left->m_right = aShip;
    if(parent->m_left == aShip) {
      parent->m_left = aShip->m_left;
    } else {
      parent->m_right = aShip->m_left;
    }
    aShip->m_left = nullptr;
  } else {
    aShip->m_left->m_left->m_left = aShip;
    aShip->m_left->m_left->m_right = aShip->m_left;
    if(parent->m_left == aShip) {
      parent->m_left = aShip->m_left->m_left;
    } else {
      parent->m_right = aShip->m_left->m_left;
    }
    aShip->m_left->m_left = nullptr;
    aShip->m_left = nullptr;
  }
}
