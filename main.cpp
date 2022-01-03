#include <iostream>
#include <fstream>
using namespace std;

struct Record{
    int iKey;
    int iVal;
    // default constructor
    Record(){}
    // parameterized constructor
    Record(int key, int val){
        iKey = key;
        iVal = val;
    }
};

struct Block{
    Record *record;
    Block(int n){
        record = new Record[n];
    }
};


int main() {
    Record r1(10,2);
    //writing to a file
    ofstream outfile("blocks.bin", ios::app | ios::binary);
    outfile.seekp(0);
    outfile.write( reinterpret_cast<char *>(&r1), sizeof(r1) );

    Record r;
    //reading from a file
    ifstream infile("blocks.bin", ios::binary);
    infile.seekg(0);
    infile.read( reinterpret_cast<char *>(&r), sizeof(r) );

    cout << r.iKey << " " << r.iVal;


}
