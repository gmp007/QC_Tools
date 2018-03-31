
#include "argumentparser.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "../MATRIX/matrix.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentParser" << endl;
  cerr << "Testing: constructor" << endl;
  {
    vector<string> flag = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    set<vector<string>> flags;
    flags.insert(flag);
    ArgumentParser ArgPars(flags);
  }

  cerr << "Testing: showUsage" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
    ArgPars.showUsage();
  }  

  cerr << "Testing: setFlagArgOpt" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",val);
    string val2 = ".7";
    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",val2);
    string val3 = ".orb";
    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",val3);
  }

  cerr << "Testing: parse" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    string val2 = ".7";
    string val3 = ".orb";
    set<string> exts = {val,val2,val3};
    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
    ArgPars.setFlagArgOpt("--punfile-mon1","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
    ArgPars.setFlagArgOpt("--punfile-mon2","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);

    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",true);
    ArgPars.setFlagArgOpt("--punfile-mon1","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",false);
    ArgPars.setFlagArgOpt("--punfile-mon2","ARGUMENT_FILE","PROPERTY_FILE_EXIST","FILE_MUST_EXIST",true);



    int argc = 5;
    const char * argv[argc];// = {"calc_J","--punfile-pair", "testfile.pun", "--punfile-mon1","file.orb"};
    argv[0] = "calc_J";
    argv[1] = "--punfile-pair";
    argv[2] = "testfile.pun";
    argv[3] = "--punfile-mon1";
    argv[4] = "file.orb";
    ArgPars.parse(argv,argc); 

    argc = 7;
    const char * argv2[argc];
    argv2[0] = "calc_J";
    argv2[1] = "--punfile-pair";
    argv2[2] = "testfile.pun";
    argv2[3] = "--punfile-mon1";
    argv2[4] = "file.orb";
    argv2[5] = "--punfile-mon2";
    argv2[6] = "fort.7";
    
    bool excep = false;
    try{
      ArgPars.parse(argv2,argc); 
    }catch(...){
      excep = true;
    }
    assert(excep);
  }

  cerr << "Testing: get" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    string val2 = ".7";
    string val3 = ".orb";
    set<string> exts = {val,val2,val3};
    ArgPars.setFlagArgOpt("--punfile-pair","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);
    ArgPars.setFlagArgOpt("--punfile-mon1","ARGUMENT_FILE","PROPERTY_FILE_EXT","ALLOWED_FILE_EXT",exts);

    int argc = 5;
    const char * argv[argc];
    argv[0] = "calc_J";
    argv[1] = "--punfile-pair";
    argv[2] = "file.pun";
    argv[3] = "--punfile-mon1";
    argv[4] = "file.orb";
    ArgPars.parse(argv,argc); 
  
    string fileName = ArgPars.getStr("--punfile-pair");
    assert(fileName.compare("file.pun")==0);
    string fileName2 = ArgPars.getStr("--punfile-mon1");
    assert(fileName2.compare("file.orb")==0);

  }

  return 0;
}