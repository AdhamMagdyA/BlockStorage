#include <iostream>
#include <fstream>
using namespace std;

int blockSize = 5;

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

int GetBlockIndex (char *cIndexFile, int iToken){
    // open the file
    ifstream file(cIndexFile);
    // read the header record
    Record record;
    file.read( reinterpret_cast<char *>(&record), sizeof(record) );
    Block block(blockSize);
    // save the next block index
    int next = record.iKey;
    // traversing the list of blocks
    while (next != -1 ){
        file.seekg(0+sizeof(record) + (next-1)*sizeof(block));
        file.read( reinterpret_cast<char *>(&block), sizeof(block) );
        // might the key exist in this block
        if(block.record[0].iVal >= iToken){
            // check if the block contains that key
            for(int i=1; i<blockSize; i++){
                if(block.record[i].iKey == iToken)
                    return next;
            }
            // does not exist
            return -1;
        }else{
            // traverse rest of the list
            next = block.record[0].iKey;
        }
    }
    // reached end of list without finding the key
    return -1;
}
int GetRecordIndex (char *cIndexFile, int iToken){
    // open the file
    ifstream file(cIndexFile);
    // read the header record
    Record record;
    file.read( reinterpret_cast<char *>(&record), sizeof(record) );
    Block block(blockSize);
    // save the next block index
    int next = record.iKey;
    // traversing the list of blocks
    while (next != -1 ){
        file.seekg(0+sizeof(record) + (next-1)*sizeof(block));
        file.read( reinterpret_cast<char *>(&block), sizeof(block) );
        // might the key exist in this block
        if(block.record[0].iVal >= iToken){
            // check if the block contains that key
            for(int i=1; i<blockSize; i++){
                if(block.record[i].iKey == iToken)
                    return i;
            }
            // does not exist
            return -1;
        }else{
            // traverse rest of the list
            next = block.record[0].iKey;
        }
    }
    // reached end of list without finding the key
    return -1;
}
int FirstEmptyBlock(char *cIndexFile){
    // open the file
    ifstream file(cIndexFile);
    // read the header record
    Record record;
    file.read( reinterpret_cast<char *>(&record), sizeof(record) );
    return record.iVal;
}

 // Insertion Function
    int InsertNewRecordAtIndex (string FileName, int Key, int Address)
    {
        Node Nodes[NumOfPages] ; // Array to Read the Binary File ("MyFile.bin)
        FILE *file, *file2 ; // Two FILE pointers for read and write at the file
        static bool Flag1 = false ; // Flag to determine if the root is created or not
        static bool Flag2 = false ; // Flag to determine if a page is need to split or not
        bool Flag3 = true ; // Flag to determine if the root is need to split or not
        bool Flag4 = true ; // Flag to determine if we insert into more one root
        file = fopen("MyFile.bin", "rb") ;
        fread(&Nodes, sizeof(Nodes), 1, file) ;
        if(Nodes[Nodes[0].descendant[0].value].leafStatus == -1 && Flag1 == false && Flag2 == false)
        {
            Nodes[Nodes[0].descendant[0].value].leafStatus = 0 ;
            for(int i = 0 ; i<5 ; i++)
            {
                if(Nodes[Nodes[0].descendant[0].value].descendant[i].address == -1)
                {
                    Nodes[Nodes[0].descendant[0].value].descendant[i].value = Key ;
                    Nodes[Nodes[0].descendant[0].value].descendant[i].address = Address ;
                    break ;
                }
            }
        }
        else if(Nodes[Nodes[0].descendant[0].value].leafStatus == 0 && Flag1 == false && Flag2 == false)
        {
            for(int i = 0 ; i<5 ; i++)
            {
                if(Nodes[Nodes[0].descendant[0].value].descendant[i].address == -1)
                {
                    Nodes[Nodes[0].descendant[0].value].descendant[i].value = Key ;
                    Nodes[Nodes[0].descendant[0].value].descendant[i].address = Address ;
                    if (i == 4)
                    {
                        Sort(Nodes[Nodes[0].descendant[0].value].descendant, 5) ;
                        Nodes[0].descendant[0].value++;
                        Flag1 = true ;
                    }
                    Sort(Nodes[Nodes[0].descendant[0].value].descendant, i+1) ;
                    break ;
                }
            }
        }
        else if(Flag1 == true && Flag2 == false)
        {
            Descendant d [6] ;
            for(int i = 0 ; i<5 ; i++)
            {
                d[i] = Nodes[Nodes[0].descendant[0].value - 1].descendant[i] ;
            }
            d[5].value = Key ;
            d[5].address = Address ;
            Sort(d, 6) ;
            Descendant d1[3] ;
            Descendant d2[3] ;
            for(int i = 0 ; i<3 ; i++)
            {
                d1[i] = d[i] ;
                d2[i] = d[i+3] ;
            }
            for(int i = 0 ; i<3 ; i++)
            {
                Nodes[Nodes[0].descendant[0].value].descendant[i] = d1[i];
                Nodes[Nodes[0].descendant[0].value + 1].descendant[i] = d2[i] ;
            }
            Descendant root [5] ;
            root[0] = d1[2] ;
            root[0].address = Nodes[0].descendant[0].value;
            root[1] = d2[2] ;
            root[1].address = Nodes[0].descendant[0].value + 1;
            for(int i = 2 ; i<5 ; i++)
            {
                root[i].value = -1;
                root[i].address = -1;
            }
            for(int i = 0  ; i<5 ; i++)
            {
                Nodes[1].descendant[i] = root[i] ;
            }
            Nodes[Nodes[0].descendant[0].value].leafStatus = 0 ;
            Nodes[Nodes[0].descendant[0].value+1].leafStatus = 0 ;
            Nodes[0].descendant[0].value+= 2 ;
            Nodes[1].leafStatus = 1;
            Flag1 = false ;
            Flag2 = true ;
        }
        else if(Flag2 == true)
        {
            if(Max(Key, Address))
            {
                return 1;
            }
            for(int i = 0 ; i<5 ; i++)
            {
                if(Key <= Nodes[1].descendant[i].value)
                {
                    for(int j = 0 ; j<5 ; j++)
                    {
                        if(Nodes[Nodes[1].descendant[i].address].descendant[j].address == -1 && Nodes[Nodes[1].descendant[i].address].leafStatus == 0)
                        {
                            Nodes[Nodes[1].descendant[i].address].descendant[j].address = Address ;
                            Nodes[Nodes[1].descendant[i].address].descendant[j].value = Key ;
                            Sort(Nodes[Nodes[1].descendant[i].address].descendant, j+1);
                            Flag3 = false ;
                            break ;
                        }
                        else if(Flag3 == true && j==4 && Nodes[0].descendant[0].value != -1)
                        {
                            Descendant D[6] ;
                            for(int k = 0 ; k<5 ; k++)
                            {
                                D[k] = Nodes[Nodes[1].descendant[i].address].descendant[k] ;
                            }
                            D[5].value = Key ;
                            D[5].address = Address ;
                            Sort(D, 6) ;
                            Descendant D1[3], D2[3] ;
                            for(int m = 0 ; m<3 ; m++)
                            {
                                D1[m] = D[m] ;
                                D2[m] = D[m+3] ;
                            }
                            for(int q = 0 ; q<5 ; q++)
                            {
                                if(Nodes[1].descendant[q].address == -1)
                                {
                                    Nodes[1].descendant[q].value = D1[2].value ;
                                    Nodes[1].descendant[q].address = Nodes[0].descendant[0].value ;
                                    Nodes[Nodes[0].descendant[0].value].leafStatus = 0 ;
                                    Sort(Nodes[1].descendant, q+1) ;
                                    Flag4 = false ;
                                    break ;
                                }
                            }
                            for(int i = 0 ; i<3 ; i++)
                            {
                                Nodes[Nodes[0].descendant[0].value].descendant[i] = D1[i] ;
                            }
                            int id ;
                            for(int i = 0 ; i<5 ; i++)
                            {
                                if(Nodes[1].descendant[i].value == D2[2].value)
                                {
                                    id = Nodes[1].descendant[i].address ;
                                    break ;
                                }
                            }
                            Nodes[id].descendant[0] = D2[0] ;
                            Nodes[id].descendant[1] = D2[1] ;
                            Nodes[id].descendant[2] = D2[2] ;
                            for(int i = 3 ; i<5 ; i++)
                            {
                                Nodes[id].descendant[i].value = -1 ;
                                Nodes[id].descendant[i].address = -1 ;
                            }
                            Nodes[0].descendant[0].value++ ;
                            if(Flag4 == true)
                            {
                                Nodes[Nodes[0].descendant[0].value - 1].leafStatus = 0 ;
                                Descendant Root [6] ;
                                for(int i = 0 ; i<5 ; i++)
                                {
                                    Root[i] = Nodes[1].descendant[i] ;
                                }
                                Root[5].value = D1[2].value ;
                                Root[5].address = Nodes[0].descendant[0].value - 1 ;
                                Sort(Root, 6) ;
                                Descendant R1[3], R2[3] ;
                                for(int i = 0 ; i<3 ; i++)
                                {
                                    R1[i] = Root[i] ;
                                    R2[i] = Root[i+3] ;
                                }
                                for(int i = 0  ; i<3 ; i++)
                                {
                                    Nodes[Nodes[0].descendant[0].value].descendant[i] = R1[i] ;
                                    Nodes[Nodes[0].descendant[0].value + 1].descendant[i] = R2[i] ;
                                }
                                Nodes[Nodes[0].descendant[0].value].leafStatus = 1 ;
                                Nodes[Nodes[0].descendant[0].value + 1].leafStatus = 1 ;
                                Nodes[1].descendant[0].value = R1[2].value ;
                                Nodes[1].descendant[0].address = Nodes[0].descendant[0].value ;
                                Nodes[1].descendant[1].value = R2[2].value ;
                                Nodes[1].descendant[1].address = Nodes[0].descendant[0].value + 1 ;
                                for(int i = 2 ; i<5 ; i++)
                                {
                                    Nodes[1].descendant[i].value = -1 ;
                                    Nodes[1].descendant[i].address = -1 ;
                                }
                                Nodes[0].descendant[0].value += 2 ;
                                if(Nodes[0].descendant[0].value == NumOfPages)
                                {
                                    Nodes[0].descendant[0].value = -1 ;
                                }
                            }
                        }
                        else if (Nodes[0].descendant[0].value == -1)
                        {
                            int ID ;
                            if(Key < Nodes[1].descendant[i].value)
                            {
                                for(int j = 0 ; j<5 ; j++)
                                {
                                    if(Key < Nodes[Nodes[1].descendant[i].address].descendant[j].value)
                                    {
                                        ID =  Nodes[Nodes[1].descendant[i].address].descendant[j].address ;
                                        break ;
                                    }
                                }
                                for(int j = 0 ; j<5 ; j++)
                                {
                                    if(Nodes[ID].descendant[j].value == -1)
                                    {
                                        Nodes[ID].descendant[j].value = Key ;
                                        Nodes[ID].descendant[j].address = Address ;
                                        Sort(Nodes[ID].descendant, j+1) ;
                                        break ;
                                    }
                                    else
                                    {
                                        continue ;
                                    }
                                }
                            }
                            break ;
                        }
                    }
                    break ;
                }
                else
                {
                    continue ;
                }
            }
        }
        file2 = fopen("MyFile.bin", "w+b") ;
        for(int i = 0 ; i<NumOfPages ; i++)
        {
            fwrite(&Nodes[i], sizeof(Nodes[i]), 1, file2) ;
        }
        fclose(file) ;
        rewind(file2) ;
        fclose(file2) ;
        return -1 ;
    }
// Delete Function
    int DeleteARecord (string FileName, int recordID){
        Node Nodes[NumOfPages] ;
        FILE *file , *file2 ;
        file = fopen("MyFile.bin", "rb") ;
        fread(&Nodes, sizeof(Nodes), 1, file) ;
        Node *record;
        Node *root;
        vector<int> indexroot;
        int indexleaf;
        int maxleafvalue=0;
        int indexofdeletedrecord;
        int numofdesinleaf=0;
        int numofdesinroot=0;
        Descendant desunderflow;
        Descendant desunderflow1;
        int maxvalueofnextleafindex=0;
        int rootindex;
        int nextleafindex;
        int indexoflastdes=0;
        bool foundroot=false;
        bool foundleaf=false;
        int i;
        int index=0;
        for( i=1; i<NumOfPages; i++)
        {
            if(Nodes[i].leafStatus==1)
            {
label1:
                for(int j=0; j<5; j++)
                {
                    if(recordID<=Nodes[i].descendant[j].value&&foundleaf==false)
                    {
                        index=Nodes[i].descendant[j].address;
                        record=&Nodes[index];
                        foundroot=true;
                        break;
                    }
                }
            }
            if(foundroot==false)
            {
                return -1;
            }
            else
            {
                if(record->leafStatus==1)
                {
                    i=index;
                    indexroot.push_back(index);
                    goto label1;
                }
                else if(record->leafStatus==0)
                {
                    for(int j=0; j<5; j++)
                    {
                        if(recordID==record->descendant[j].value)
                        {
                            foundleaf=true;
                            break;
                        }
                    }
                    if(foundleaf==false)
                    {
                        return -1;
                    }
                    else if(foundleaf==true)
                    {
                        for(int i=0; i<NumOfPages; i++)
                        {
                            if(record->leafStatus==Nodes[i].leafStatus&&record->descendant[0].value==Nodes[i].descendant[0].value&&record->descendant[0].address==Nodes[i].descendant[0].address)
                            {
                                indexleaf=i;
                                break;
                            }
                        }
                        for(int i=0; i<5; i++) //to find the maxvalueindex and the number of des in leaf
                        {
                            if(Nodes[indexleaf].descendant[i].value!=-1)
                            {
                                maxleafvalue+=1;
                            }
                        }
                        numofdesinleaf=maxleafvalue+1;
                        maxleafvalue=Nodes[indexleaf].descendant[maxleafvalue].value;
                    }
                }
            }
        }
        for(int i=0; i<5; i++) //to find the maxvalueindex and the number of des in leaf
        {
            if(Nodes[indexroot.back()].descendant[i].value!=-1)
            {
                numofdesinroot+=i;
            }
        }
        int maxrootvalue=Nodes[indexroot.back()].descendant[numofdesinroot-1].value;
        for(int i=0; i<5; i++)
        {
            if(Nodes[indexleaf].descendant[i].value==recordID)
            {
                indexofdeletedrecord=i;
                break;
            }
        }
        if(recordID!=maxleafvalue&&numofdesinleaf>2)//deleting withouteffect
        {
            for(int i=indexofdeletedrecord; i<5; i++)
            {
                Nodes[indexleaf].descendant[i].value=Nodes[indexleaf].descendant[i+1].value;
                Nodes[indexleaf].descendant[i].address=Nodes[indexleaf].descendant[i+1].address;
            }
            Nodes[indexleaf].descendant[4].value=-1;
            Nodes[indexleaf].descendant[4].address=-1;
            return 1;
        }
        else if(recordID==maxleafvalue&&numofdesinleaf>2)//deletingwithmaxvalueproblem
        {
            for(int i=indexofdeletedrecord; i<5; i++)
            {
                Nodes[indexleaf].descendant[i].value=Nodes[indexleaf].descendant[i+1].value;
                Nodes[indexleaf].descendant[i].address=Nodes[indexleaf].descendant[i+1].address;
            }
            Nodes[indexleaf].descendant[4].value=-1;
            Nodes[indexleaf].descendant[4].address=-1;
            for(int j=indexroot.back(); j>-1; j--)
            {
                for(int k=0; k<5; k++)
                {
                    if( Nodes[j].descendant[k].value==recordID)
                    {
                        Nodes[j].descendant[k].value=Nodes[indexleaf].descendant[indexofdeletedrecord-1].value;
                    }
                }
            }
            return 1;
        }
        else if(numofdesinleaf==2&&maxleafvalue==maxrootvalue)
        {
            for(int i=0; i<5; i++)
            {
                if(Nodes[indexroot.back()].descendant[i].value>=recordID)
                {
                    rootindex=i;
                    break;
                }
            }
            Nodes[indexleaf].descendant[0].value=-1;
            Nodes[indexleaf].descendant[0].address=-1;
            nextleafindex=Nodes[indexroot.back()].descendant[rootindex-1].address;
            for(int i=0; i<5; i++)
            {
                if(Nodes[nextleafindex].descendant[i].value!=-1)
                {
                    maxvalueofnextleafindex+=1;
                }
            }
            desunderflow1.value=Nodes[nextleafindex].descendant[maxvalueofnextleafindex-1].value;
            desunderflow1.address=Nodes[nextleafindex].descendant[maxvalueofnextleafindex-1].address;
            for(int i=0; i<5; i++)
            {
                Nodes[indexleaf].descendant[i].value=Nodes[indexleaf].descendant[i].value;
                Nodes[indexleaf].descendant[i].address=Nodes[indexleaf].descendant[i].address;
            }
            Nodes[indexleaf].descendant[4].value=-1;
            Nodes[indexleaf].descendant[4].address=-1;
            Nodes[indexleaf].descendant[0].value=desunderflow1.value;
            Nodes[indexleaf].descendant[0].address=desunderflow1.address;
            Nodes[nextleafindex].descendant[maxvalueofnextleafindex-1].value=-1;
            Nodes[nextleafindex].descendant[maxvalueofnextleafindex-1].address=-1;
            Nodes[indexroot.back()].descendant[rootindex-1].value=Nodes[nextleafindex].descendant[maxvalueofnextleafindex-2].value;
            return 1;
        }
        else if(numofdesinleaf==2&&maxleafvalue!=maxrootvalue&&recordID<=maxleafvalue)
        {
            for(int i=0; i<5; i++)
            {
                if(Nodes[indexroot.back()].descendant[i].value>=recordID)
                {
                    rootindex=i;
                    break;
                }
            }
            for(int i=indexofdeletedrecord; i<5; i++)
            {
                Nodes[indexleaf].descendant[i].value=Nodes[indexleaf].descendant[i+1].value;
                Nodes[indexleaf].descendant[i].address=Nodes[indexleaf].descendant[i+1].address;
            }
            Nodes[indexleaf].descendant[4].value=-1;
            Nodes[indexleaf].descendant[4].address=-1;
            nextleafindex=Nodes[indexroot.back()].descendant[rootindex+1].address;
            desunderflow.value=Nodes[nextleafindex].descendant[0].value;
            desunderflow.address=Nodes[nextleafindex].descendant[0].address;
            for(int i=0; i<5; i++)
            {
                Nodes[nextleafindex].descendant[i].value=Nodes[nextleafindex].descendant[i+1].value;
                Nodes[nextleafindex].descendant[i].address=Nodes[nextleafindex].descendant[i+1].address;
            }
            Nodes[nextleafindex].descendant[4].value=-1;
            Nodes[nextleafindex].descendant[4].address=-1;
            Nodes[indexleaf].descendant[1].value=desunderflow.value;
            Nodes[indexleaf].descendant[1].address=desunderflow.address;
            Nodes[indexroot.back()].descendant[rootindex].value=desunderflow.value;
            return 1;
        }
        file2 = fopen("MyFile.bin", "w+b") ;
        for(int i = 0 ; i<NumOfPages ; i++)
        {
            fwrite(&Nodes[i], sizeof(Nodes[i]), 1, file2) ;
        }
        fclose(file) ;
        rewind(file2) ;
        fclose(file2) ;
        return -1 ;
    }
/*------------------------------------------------------------------------------ */

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

//    cout << GetKey("blocks.bin",1,2) << endl;
//    cout << GetVal("blocks.bin",1,2) << endl;

    cout << "element in block: " << GetBlockIndex("blocks.bin",5) << " and record: " << GetRecordIndex("blocks.bin",5) << endl;
    cout << "first empty block is at " << FirstEmptyBlock("blocks.bin");
}
