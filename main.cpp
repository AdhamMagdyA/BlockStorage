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

int GetKey(char *cIndexFile, int iBlock, int iRecord){
    // open the file
    ifstream indexFile(cIndexFile);
    // define record and block to hold the data
    Record r;
    Block block(5);
    // seek to the specified block
    indexFile.seekg(0 + sizeof(r) + (iBlock-1)*sizeof(block)) ;
    // read the specified block
    indexFile.read( reinterpret_cast<char *>(&block), sizeof(block) );
    // read the specified record
    r = block.record[iRecord];
    // return key
    return r.iKey;
}

int GetVal(char *cIndexFile, int iBlock, int iRecord){
    // open the file
    ifstream indexFile(cIndexFile);
    // define record and block to hold the data
    Record r;
    Block block(5);
    // seek to the specified block
    indexFile.seekg(0 + sizeof(r) + (iBlock-1)*sizeof(block)) ;
    // read the specified block
    indexFile.read( reinterpret_cast<char *>(&block), sizeof(block) );
    // read the specified record
    r = block.record[iRecord];
    // return value
    return r.iVal;
}




int main() {

    //this main shows how the writing and reading should be done

    ofstream outfile("blocks.bin", ios::binary);

    // write header record
    Record header(1,2);
    outfile.write( reinterpret_cast<char *>(&header), sizeof(header) );

    //write 1st block
    Record r1(3,5);
    Record r2(1,5);
    Record r3(3,20);
    Record r4(5,27);
    Block block1(5);
    block1.record[0] = r1;
    block1.record[1] = r2;
    block1.record[2] = r3;
    block1.record[3] = r4;
    outfile.write( reinterpret_cast<char *>(&block1), sizeof(block1) );

    //write 2nd block
    Record rt(-1,-1);
    Block block2(5);
    block2.record[0] = rt;
    outfile.write( reinterpret_cast<char *>(&block2), sizeof(block2) );

    //write 3rd block
    Record r5(-1,9);
    Record r6(8,4);
    Record r7(9,11);
    Block block3(5);
    block3.record[0] = r5;
    block3.record[1] = r6;
    block3.record[2] = r7;
    outfile.write( reinterpret_cast<char *>(&block3), sizeof(block3) );

    outfile.close();

    Block block(5);
    //reading from a file
    ifstream infile("blocks.bin");
    // seeking to the 3rd block (from beginning of the file skip header record + 2 blocks)
    infile.seekg(0 + sizeof(r1) + 2*sizeof(block)) ;
    infile.read( reinterpret_cast<char *>(&block), sizeof(block) );

    cout << GetKey("blocks.bin",1,2) << endl;
    cout << GetVal("blocks.bin",1,2) << endl;

}
