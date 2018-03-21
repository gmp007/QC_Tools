#include <cassert> 
#include <iostream>
#include <vector>
#include "qc_functions.hpp"
#include "../MATRIX/matrix.hpp"

using namespace std;

int main(void){

  cout << "Testing: unscramble_P_Coef" << endl;
  {
    Matrix * dimerCoef = new Matrix(8,8);
    for(int i=1;i<=8;++i){
      dimerCoef->set_elem(1.3,i,1);
      dimerCoef->set_elem(0.2,i,3);
      dimerCoef->set_elem(82.4,i,4);
      dimerCoef->set_elem(9.4,i,5);
      dimerCoef->set_elem(3.4,i,6);
      dimerCoef->set_elem(-3.0,i,8);
    }
  
    // Our coefficient matrix should look like this
    //
    //         col 1  col 2  col 3  col 4  col 5  col 6  col 7  col 8
    //        _______________________________________________________
    // row 1 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 2 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 3 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 4 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 5 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 6 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 7 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    // row 8 |  1.3    0.0    0.2   82.4    9.4    3.4    0.0   -3.0
    //         atm3   atm1   atm1   atm5   atm4   atm4   atm2   atm2


    // Contains the x y and z position of atoms in monomer A
    Matrix * coordA = new Matrix(2,3);
    // Atom 1 at (1.0,1.0,1.0)
    // Atom 2 at (0.0,0.0,0.0)
    coordA->set_elem(1.0,1,1);
    coordA->set_elem(1.0,1,2);
    coordA->set_elem(1.0,1,3);
    

    // Contains the x y and z position of atoms in monomer B
    Matrix * coordB = new Matrix(3,3);
    // Atom 3 at (1.0,0.0,0.0)
    // Atom 4 at (2.0,0.0,0.0)
    // Atom 5 at (3.0,0.0,0.0)
    coordB->set_elem(1.0,1,1);
    coordB->set_elem(2.0,2,1);
    coordB->set_elem(3.0,3,1);

    Matrix * coordDimer = new Matrix(5,3);
    // Arrange atoms in the dimer so they do not appear in the 
    // same order as the monomers
    
    // row1 Atom 3
    coordDimer->set_elem(1.0,1,1);
    // row2 Atom 1
    coordDimer->set_elem(1.0,2,1);
    coordDimer->set_elem(1.0,2,2);
    coordDimer->set_elem(1.0,2,3);
    // row3 Atom 5 
    coordDimer->set_elem(3.0,3,1);
    // row4 Atom 4
    coordDimer->set_elem(2.0,4,1);
    // row5 Atom 2 (0.0,0.0,0.0)
    
    vector<int> matchA = coordA->matchRow(*coordDimer,2);
    vector<int> matchB = coordB->matchRow(*coordDimer,2);

    assert(matchA.at(0)==2);
    assert(matchA.at(1)==5);

    assert(matchB.at(0)==1);
    assert(matchB.at(1)==4);
    assert(matchB.at(2)==3);

    vector<int> basisFuncDimer;
    // Basis functions per atom
    //
    // Atom 1 2
    // Atom 2 2
    // Atom 3 1
    // Atom 4 2
    // Atom 5 1

    // According to the dimer coordinates they will appear in the following
    // order
    // Atom 3 1
    // Atom 1 2
    // Atom 5 1
    // Atom 4 2
    // Atom 2 2

    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);
    
    cerr << "Before passing in" << endl;
    for( auto it : matchA ) cerr << it << endl;
    for( auto it : matchB ) cerr << it << endl;
    for( auto it : basisFuncDimer ) cerr << it << endl;
    // If we correct the dimer coefficient matrix to line up with the
    // coefficients of the monomers it should look like this 
    //
    //         col 1  col 2  col 3  col 4  col 5  col 6  col 7  col 8
    //        _______________________________________________________
    // row 1 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 2 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 3 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 4 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 5 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 6 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 7 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    // row 8 |  0.0    0.2    0.0   -3.0    1.3    9.4    3.4   82.4
    //         atm1   atm1   atm2   atm2   atm3   atm4   atm4   atm5    
    cerr << "Calling unscramble" << endl;
    auto NewCoef = unscramble_P_Coef( matchA, matchB, basisFuncDimer,dimerCoef);

    cerr << *NewCoef << endl; 

    for(int i=1;i<=8;++i){
      assert(static_cast<int>(NewCoef->get_elem(i,1))==0);
      assert(static_cast<int>(NewCoef->get_elem(i,2)*10)==2);
      assert(static_cast<int>(NewCoef->get_elem(i,3))==0);
      assert(static_cast<int>(NewCoef->get_elem(i,4))==-3);
      assert(static_cast<int>(NewCoef->get_elem(i,5)*10)==13);
      assert(static_cast<int>(NewCoef->get_elem(i,6)*10)==94);
      assert(static_cast<int>(NewCoef->get_elem(i,7)*10)==34);
      assert(static_cast<int>(NewCoef->get_elem(i,8)*10)==824);
    }

  }

  cout << "Testing: unscramble_S_Coef" << endl;
  {
    Matrix * SCoef = new Matrix(8,8);
    for(int i=1;i<=8;++i){
      SCoef->set_elem(1.3,i,2);
      SCoef->set_elem(1.3,i,3);
      SCoef->set_elem(4.0,i,4);
    }

    for(int i=1;i<=8;++i){
      SCoef->set_elem(1.3,2,i);
      SCoef->set_elem(1.3,3,i);
      SCoef->set_elem(4.0,4,i);
    }
  
    SCoef->set_elem(6.0,2,4);
    SCoef->set_elem(6.0,3,4);
    SCoef->set_elem(6.0,4,2);
    SCoef->set_elem(6.0,4,3);

    // Our coefficient matrix should look like this
    //
    //               col 1  col 2  col 3  col 4  col 5  col 6  col 7  col 8
    //              _______________________________________________________
    // atm 3 row 1 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 1 row 2 |  1.3    1.3    1.3    6.0    1.3    1.3    1.3    1.3
    // atm 1 row 3 |  1.3    1.3    1.3    6.0    1.3    1.3    1.3    1.3
    // atm 5 row 4 |  4.0    6.0    6.0    4.0    4.0    4.0    4.0    4.0
    // atm 4 row 5 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 4 row 6 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 2 row 7 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    // atm 2 row 8 |  0.0    1.3    1.3    4.0    0.0    0.0    0.0    0.0
    //                atm3   atm1   atm1   atm5   atm4   atm4   atm2   atm2


    // Contains the x y and z position of atoms in monomer A
    Matrix * coordA = new Matrix(2,3);
    // Atom 1 at (1.0,1.0,1.0)
    // Atom 2 at (0.0,0.0,0.0)
    coordA->set_elem(1.0,1,1);
    coordA->set_elem(1.0,1,2);
    coordA->set_elem(1.0,1,3);
    

    // Contains the x y and z position of atoms in monomer B
    Matrix * coordB = new Matrix(3,3);
    // Atom 3 at (1.0,0.0,0.0)
    // Atom 4 at (2.0,0.0,0.0)
    // Atom 5 at (3.0,0.0,0.0)
    coordB->set_elem(1.0,1,1);
    coordB->set_elem(2.0,2,1);
    coordB->set_elem(3.0,3,1);

    Matrix * coordDimer = new Matrix(5,3);
    // Arrange atoms in the dimer so they do not appear in the 
    // same order as the monomers
    
    // row1 Atom 3
    coordDimer->set_elem(1.0,1,1);
    // row2 Atom 1
    coordDimer->set_elem(1.0,2,1);
    coordDimer->set_elem(1.0,2,2);
    coordDimer->set_elem(1.0,2,3);
    // row3 Atom 5 
    coordDimer->set_elem(3.0,3,1);
    // row4 Atom 4
    coordDimer->set_elem(2.0,4,1);
    // row5 Atom 2 (0.0,0.0,0.0)
    
    vector<int> matchA = coordA->matchRow(*coordDimer,2);
    vector<int> matchB = coordB->matchRow(*coordDimer,2);

    assert(matchA.at(0)==2);
    assert(matchA.at(1)==5);

    assert(matchB.at(0)==1);
    assert(matchB.at(1)==4);
    assert(matchB.at(2)==3);

    vector<int> basisFuncDimer;
    // Basis functions per atom
    //
    // Atom 1 2
    // Atom 2 2
    // Atom 3 1
    // Atom 4 2
    // Atom 5 1

    // According to the dimer coordinates they will appear in the following
    // order
    // Atom 3 1
    // Atom 1 2
    // Atom 5 1
    // Atom 4 2
    // Atom 2 2

    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);
    
    cerr << "Before passing in" << endl;
    for( auto it : matchA ) cerr << it << endl;
    for( auto it : matchB ) cerr << it << endl;
    for( auto it : basisFuncDimer ) cerr << it << endl;
    // If we correct the dimer coefficient matrix to line up with the
    // coefficients of the monomers it should look like this 
    //
    //         col 1  col 2  col 3  col 4  col 5  col 6  col 7  col 8
    //        _______________________________________________________
    // row 1 |  1.3    1.3    1.3   1.3    1.3    1.3    1.3   6.0
    // row 2 |  1.3    1.3    1.3   1.3    1.3    1.3    1.3   6.0
    // row 3 |  1.3    1.3    0.0   0.0    0.0    0.0    0.0   4.0
    // row 4 |  1.3    1.3    0.0   0.0    0.0    0.0    0.0   4.0
    // row 5 |  1.3    1.3    0.0   0.0    0.0    0.0    0.0   4.0
    // row 6 |  1.3    1.3    0.0   0.0    0.0    0.0    0.0   4.0
    // row 7 |  1.3    1.3    0.0   0.0    0.0    0.0    0.0   4.0
    // row 8 |  6.0    6.0    4.0   4.0    4.0    4.0    4.0   4.0
    //         atm1   atm1   atm2   atm2   atm3   atm4   atm4   atm5    
    cerr << "Calling unscramble" << endl;
    auto NewCoef = unscramble_S( matchA, matchB, basisFuncDimer,SCoef);

    cerr << *NewCoef << endl; 

    for(int i=1;i<=7;++i){
      assert(static_cast<int>(NewCoef->get_elem(i,1)*10)==13);
      assert(static_cast<int>(NewCoef->get_elem(i,2)*10)==13);

      assert(static_cast<int>(NewCoef->get_elem(1,i)*10)==13);
      assert(static_cast<int>(NewCoef->get_elem(2,i)*10)==13);
    }

    for(int i=3;i<=8;++i){
      assert(static_cast<int>(NewCoef->get_elem(8,i))==4);
      assert(static_cast<int>(NewCoef->get_elem(i,8))==4);
    }
    assert(static_cast<int>(NewCoef->get_elem(1,8))==6);
    assert(static_cast<int>(NewCoef->get_elem(2,8))==6);
    assert(static_cast<int>(NewCoef->get_elem(8,2))==6);
    assert(static_cast<int>(NewCoef->get_elem(8,1))==6);
  }

  cout << "Testing: unscramble_OE" << endl;
  {
    Matrix * OECoef = new Matrix(8,1);
    OECoef->set_elem(1.3,1,1);
    OECoef->set_elem(0.2,3,1);
    OECoef->set_elem(82.4,4,1);
    OECoef->set_elem(9.4,5,1);
    OECoef->set_elem(3.4,6,1);
    OECoef->set_elem(-3.0,8,1);
  
    // Our coefficient matrix should look like this
    //
    //              col 1 
    //             _______
    // atm3 row 1 |  1.3
    // atm1 row 2 |  0.0
    // atm1 row 3 |  0.2
    // atm5 row 4 | 82.4
    // atm4 row 5 |  9.4
    // atm4 row 6 |  3.4
    // atm2 row 7 |  0.0
    // atm2 row 8 | -3.0


    // Contains the x y and z position of atoms in monomer A
    Matrix * coordA = new Matrix(2,3);
    // Atom 1 at (1.0,1.0,1.0)
    // Atom 2 at (0.0,0.0,0.0)
    coordA->set_elem(1.0,1,1);
    coordA->set_elem(1.0,1,2);
    coordA->set_elem(1.0,1,3);
    

    // Contains the x y and z position of atoms in monomer B
    Matrix * coordB = new Matrix(3,3);
    // Atom 3 at (1.0,0.0,0.0)
    // Atom 4 at (2.0,0.0,0.0)
    // Atom 5 at (3.0,0.0,0.0)
    coordB->set_elem(1.0,1,1);
    coordB->set_elem(2.0,2,1);
    coordB->set_elem(3.0,3,1);

    Matrix * coordDimer = new Matrix(5,3);
    // Arrange atoms in the dimer so they do not appear in the 
    // same order as the monomers
    
    // row1 Atom 3
    coordDimer->set_elem(1.0,1,1);
    // row2 Atom 1
    coordDimer->set_elem(1.0,2,1);
    coordDimer->set_elem(1.0,2,2);
    coordDimer->set_elem(1.0,2,3);
    // row3 Atom 5 
    coordDimer->set_elem(3.0,3,1);
    // row4 Atom 4
    coordDimer->set_elem(2.0,4,1);
    // row5 Atom 2 (0.0,0.0,0.0)
    
    vector<int> matchA = coordA->matchRow(*coordDimer,2);
    vector<int> matchB = coordB->matchRow(*coordDimer,2);

    assert(matchA.at(0)==2);
    assert(matchA.at(1)==5);

    assert(matchB.at(0)==1);
    assert(matchB.at(1)==4);
    assert(matchB.at(2)==3);

    vector<int> basisFuncDimer;
    // Basis functions per atom
    //
    // Atom 1 2
    // Atom 2 2
    // Atom 3 1
    // Atom 4 2
    // Atom 5 1

    // According to the dimer coordinates they will appear in the following
    // order
    // Atom 3 1
    // Atom 1 2
    // Atom 5 1
    // Atom 4 2
    // Atom 2 2

    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(1);
    basisFuncDimer.push_back(2);
    basisFuncDimer.push_back(2);
    
    cerr << "Before passing in" << endl;
    for( auto it : matchA ) cerr << it << endl;
    for( auto it : matchB ) cerr << it << endl;
    for( auto it : basisFuncDimer ) cerr << it << endl;
    // If we correct the dimer coefficient matrix to line up with the
    // coefficients of the monomers it should look like this 
    //
    //              col 1 
    //             _______
    // atm1 row 1 |  0.0
    // atm1 row 2 |  0.2
    // atm2 row 3 |  0.0
    // atm2 row 4 | -3.0
    // atm3 row 5 |  1.3
    // atm4 row 6 |  9.4
    // atm4 row 7 |  3.4
    // atm5 row 8 | 82.4

    cerr << "Calling unscramble" << endl;
    auto NewCoef = unscramble_OE( matchA, matchB, basisFuncDimer,OECoef);

    cerr << *NewCoef << endl; 

    assert(static_cast<int>(NewCoef->get_elem(1,1))==0);
    assert(static_cast<int>(NewCoef->get_elem(2,1)*10)==2);
    assert(static_cast<int>(NewCoef->get_elem(3,1))==0);
    assert(static_cast<int>(NewCoef->get_elem(4,1))==-3);
    assert(static_cast<int>(NewCoef->get_elem(5,1)*10)==13);
    assert(static_cast<int>(NewCoef->get_elem(6,1)*10)==94);
    assert(static_cast<int>(NewCoef->get_elem(7,1)*10)==34);
    assert(static_cast<int>(NewCoef->get_elem(8,1)*10)==824);

  }

  return 0;
}