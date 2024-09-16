#include "driver.cpp"
#include <random>

//recursive tree traversal function for test balanced
int Tester::traversal(Ship* aShip) {
  if(aShip == nullptr) {
    return 0;
  }
  traversal(aShip->m_left);
  if(aShip->m_left != nullptr and aShip->m_height != (aShip->m_left->m_height + 1)) {
    throw("imbalance");
  }
  traversal(aShip->m_right);
  if(aShip->m_right != nullptr and aShip->m_height != (aShip->m_right->m_height + 1)) {
    //std::cout << "imbalance" << endl;
    throw("imbalance");
  }
  return aShip->m_height;
}
//recursive tree traversal for test ordered
void Tester::traversal2(Ship* aShip) {
  if(aShip != nullptr) {
    traversal2(aShip->m_left);
    if(aShip->m_left != nullptr and aShip->m_id < aShip->m_left->m_id) {
      //std::cout << "not ordered left" << endl;
      throw("not ordered");
    }
    traversal2(aShip->m_right);
    if(aShip->m_right != nullptr and aShip->m_id > aShip->m_right->m_id) {
      //std::cout << "not ordered right" << endl;
      throw("not ordered");
    }
  }
}

//test destructor/clear
void Tester::testDestructor() {
  //uses valgrind to test
  Random idGen(MINID,MAXID);
  Random typeGen(0,4);
  
  Fleet Katana;
  
  int fleetSize = 30;
  int ID = 0;
  for(int i=0;i<fleetSize;i++){
    ID = idGen.getRandNum();
    Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
    Katana.insert(ship);
  }
  /*
  Ship* temp;
  temp = new Ship(150);
  Katana.insert(*temp);
  for(int i = 1; i < 300; i=i*2) {
    temp = new Ship(i);
    Katana.insert(*temp);
  }
  for(int i = 1; i < 300; i=i*3) {
    temp = new Ship(i);
    Katana.insert(*temp);
  }
  */
}

//test insertion
bool Tester::insertNormal() {
  bool success = true;
  Fleet Katana;

  Random idGen(MINID,MAXID);
  Random typeGen(0,4);
  int fleetSize = 300;
  int ID = 0;
  try {
    for(int i=0;i<fleetSize;i++){
      ID = idGen.getRandNum();
      Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
      //Katana.listShips();
      Katana.insert(ship);
    }
  }
  catch(...) {
    success = false;
  }
  /*
  Ship* temp;
  temp = new Ship(150);
  Katana.insert(*temp);
  for(int i = 1; i < 300; i=i*2) {
    temp = new Ship(i);
    Katana.insert(*temp);
  }
  try {
    for(int i = 1; i < 300; i=i*3) {
      temp = new Ship(i);
      Katana.insert(*temp);
    }
  }
  catch(...) {
    success = false;
  }
  */
  return success;
}
bool Tester::insertEdge() {
  bool success = false;
  Fleet Katana;
  Ship ship(0);
  try {
    Katana.insert(ship);
  }
  catch(...) {
    success = true;
  }
  return success;
}
bool Tester::insertError() {
  bool success = false;
  Fleet Katana;
  Ship ship(100000);
  try {
    Katana.insert(ship);
  }
  catch(...) {
    success = true;
  }
  return success;
}
//balance
bool Tester::insertBalance() {
  bool success = true;
  Fleet Katana;

  Random idGen(MINID,MAXID);
  Random typeGen(0,4);
  int fleetSize = 300;
  int ID = 0;
  for(int i=0;i<fleetSize;i++){
    ID = idGen.getRandNum();
    Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
    Katana.insert(ship);
  }
  try {
    traversal(Katana.m_root);
  }
  catch(...) {
    success = false;
  }
  return success;
}
//ordered
bool Tester::insertOrdered() {
  bool success = true;
  Fleet Katana;

  Random idGen(MINID,MAXID);
  Random typeGen(0,4);
  int fleetSize = 30;
  int ID = 0;
  for(int i=0;i<fleetSize;i++){
    ID = idGen.getRandNum();
    Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
    Katana.insert(ship);
  }
  /*
  Ship* temp;
  temp = new Ship(150);
  Katana.insert(temp);
  for(int i = 1; i < 300; i=i*2) {
    temp = new Ship(i);
    Katana.insert(temp);
  }
  for(int i = 1; i < 300; i=i*3) {
    temp = new Ship(i);
    Katana.insert(temp);
  }
  */
  try {
    traversal2(Katana.m_root);
  }
  catch(...) {
    success = false;
  }
  return success;
}
//test remove
bool Tester::removeNormal() {
  bool success = true;
  Fleet Katana;
  Ship ship(15000);
  Katana.insert(ship);
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  //Katana.listShips();
  try {
    for(int i = 10000; i < 30000; i=i+2000) {
      Katana.remove(i);
    }
    //std::cout << endl;
    //Katana.listShips();
  }
  catch(...) {
    success = false;
  }
  return success;
}
bool Tester::removeEdge() {
  //removing everything
  bool success = true;
  Fleet Katana;
  Ship temp(150);
  Katana.insert(temp);
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  try {
    for(int i = 10000; i < 30000; i=i+1000) {
      Katana.remove(i);
    }
  }
  catch(...) {
    success = false;
  }
  return success;
}
bool Tester::removeError1() {
  bool success = true;
  Fleet Katana;
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  try {
    Katana.remove(-4);
  }
  catch(...) {
    success = false;
  }
  return success;
}
bool Tester::removeError2() {
  bool success = false;
  Fleet Katana;
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  try {
    Katana.remove(12300);
  }
  catch(...) {
    success = true;
  }
  return success;
}
//balance
bool Tester::removeBalance() {
  bool success = true;
  Fleet Katana;
  Ship temp(15000);
  Katana.insert(temp);
  for(int i = 10000; i < 90000; i=i+250) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  for(int i = 10000; i < 90000; i=i+2000) {
    Katana.remove(i);
  }
  try {
    traversal(Katana.m_root);
  }
  catch(...) {
    success = false;
  }
  return success;
}
//ordered
bool Tester::removeOrdered() {
  bool success = true;
  Fleet Katana;
  Ship temp(15000);
  Katana.insert(temp);
  for(int i = 10000; i < 90000; i=i+250) {
    Ship temp(i);
    Katana.insert(temp);
  }
  //remove
  for(int i = 10000; i < 90000; i=i+2000) {
    Katana.remove(i);
  }
  try {
    traversal2(Katana.m_root);
  }
  catch(...) {
    success = false;
  }
  return success;
}

//test removeLost
bool Tester::testRemoveLost() {
  bool success = true;
  Fleet Katana;
  Ship temp(15000);
  Katana.insert(temp);
  for(int i = 10000; i < 90000; i=i+2000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  removeLostHelper(Katana.m_root);
  //Katana.listShips();
  Katana.removeLost();
  try {
    removeLostHelper2(Katana.m_root);
    //std::cout << endl;
    //Katana.listShips();
  }
  catch(...) {
    success = false;
  }
  return success;
}
void Tester::removeLostHelper(Ship* aShip) {
  if(aShip == nullptr) {
  } else {
    removeLostHelper(aShip->m_left);
    removeLostHelper(aShip->m_right);
    if(aShip->m_id % 6 == 0) {
      STATE state = LOST;
      aShip->setState(state);
    }
  }
}
void Tester::removeLostHelper2(Ship* aShip) {
  if(aShip == nullptr) {
  } else {
    removeLostHelper(aShip->m_left);
    removeLostHelper(aShip->m_right);
    if(aShip->m_id % 6 == 0) {
      throw;
    }
  }
}

//time test
bool Tester::insertTime() {
  return true;
}
bool Tester::removeTime() {
  return true;
}

//test checkImbalance
bool Tester::testCheckImbalance() {
  bool success = true;
  Fleet Katana;
  Ship* temp;
  Ship temp2(15000);
  Katana.insert(temp2);
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp2(i);
    Katana.insert(temp2);
  }
  temp = Katana.m_root;
  for(int i = 0; i < 3; i++) {
    //temp = temp->m_left;
    temp = temp->m_right;
  }
  if(Katana.checkImbalance(temp) == traversal(temp)) {
    success = false;
  }
  return success;
}

//test rebalace
bool Tester::testRebalance() {
  bool success = true;
  Fleet Katana;
  for(int i = 10001; i < 10004; i++) {
    Ship temp(i);
    Katana.insert(temp);
  }
  Katana.checkImbalance(Katana.m_root);
  if(Katana.m_root->m_id == 10002) {
    success = false;
  }
  return success;
}

//test listShips
bool Tester::testListShips() {
  bool success = true;
  Fleet Katana;

  Random idGen(MINID,MAXID);
  Random typeGen(0,4);
  int fleetSize = 10;
  int ID = 0;
  for(int i=0;i<fleetSize;i++){
    ID = idGen.getRandNum();
    Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
    Katana.insert(ship);
  }
  try {
    Katana.listShips();
  }
  catch(...) {
    success = false;
  }
  return success;
}

//test setState
bool Tester::testSetState() {
  bool success = false;
  Fleet Katana;
  Ship ship(15000);
  Katana.insert(ship);
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp2(i);
    Katana.insert(temp2);
  }
  STATE state = LOST;
  Katana.setState(18000, state);
  try {
    setStateHelper(Katana.m_root, 18000, state);
  }
  catch(...) {
    success = true;
  }
  return success;
}
void Tester::setStateHelper(Ship* aShip, int ID, STATE state) {
  if(aShip != nullptr) {
    setStateHelper(aShip->m_left, ID, state);
    if(aShip->getID() == ID and aShip->getState() == state) {
      throw("found");
    }
    setStateHelper(aShip->m_right, ID, state);
    if(aShip->getID() == ID and aShip->getState() == state) {
      throw("found");
    }
  }
}

//test updateHeight
bool Tester::testUpdateHeight() {
  bool success = true;
  Fleet Katana;
  Ship* temp;
  Ship ship(15);
  Katana.insert(ship);
  for(int i = 1; i < 30; i=i*2) {
    Ship temp3(i);
    Katana.insert(temp3);
  }
  temp = Katana.m_root;
  while(temp->m_right != nullptr) {
    temp = temp->m_right;
  }
  temp->m_right = new Ship(40);
  Katana.updateHeight(temp);
  if(temp->m_height != 1) {
    success = false;
  }
  return success;
}

//test findShip
bool Tester::testFindShip() {
  bool success = true;
  Fleet Katana;
  Ship ship(15000);
  Katana.insert(ship);
  for(int i = 10000; i < 30000; i=i+1000) {
    Ship temp(i);
    Katana.insert(temp);
  }
  if(Katana.findShip(24000) != true) {
    success = false;
  }
  return success;
}

int main() {
  Tester Test;
  
  //test destructor
  //Test.testDestructor();

  //test insert
  //std::cout << "Test Insert" << endl;
  std::cout << Test.insertNormal() << endl;
  //std::cout << Test.insertEdge() << endl;
  //std::cout << Test.insertError() << endl;
  //std::cout << Test.insertBalance() << endl;
  //std::cout << Test.insertOrdered() << endl;

  //test Remove
  //std::cout << "Test Remove" << endl;
  //std::cout << Test.removeNormal() << endl;
  //std::cout << Test.removeEdge() << endl;
  //std::cout << Test.removeError1() << endl;
  //std::cout << Test.removeError2() << endl;
  //std::cout << Test.removeBalance() << endl;
  //std::cout << Test.removeOrdered() << endl;

  //test removeLost
  //std::cout << "test removeLost" << endl;
  //std::cout << Test.testRemoveLost() << endl;

  //time test
  //std::cout << Test.insertTime() << endl;
  //std::cout << Test.removeTime() << endl;

  //test check imbalance
  //std::cout << "test checkImbalance" << endl;
  //std::cout << Test.testCheckImbalance() << endl;

  //test rebalance
  //std::cout << "test rebalance" << endl;
  //std::cout << Test.testRebalance() << endl;

  //test list ships
  //std::cout << "test listShips" << endl;
  //std::cout << Test.testListShips() << endl;

  //test set state
  //std::cout << "Test setState" << endl;
  //std::cout << Test.testSetState() << endl;

  //test update height
  //std::cout << "test updateHeight" << endl;
  //std::cout << Test.testUpdateHeight() << endl;

  //test find ship
  //std::cout << "test findShip" << endl;
  //std::cout << Test.testFindShip() << endl;
  
  return 0;
}
