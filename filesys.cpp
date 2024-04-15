#include "filesys.h"
#include "sdisk.h"
#include "block.h"

using namespace std;

//DONE
Filesys::Filesys(string diskname, int numberofblocks, int blocksize) : Sdisk(diskname, numberofblocks, blocksize)
{
    string buffer;

    int code = readfs();

    if (code == 0)
    {
        cout << "This is a bad drive!" << endl;
        return;
    }

    rootsize = (blocksize/10);

    fatsize = (getnumberofblocks() * 5 )/(getblocksize() + 1);

    code = readfs();

    if (buffer[0] == '#')
    {
        buildfs();
    }

    fssynch();
    cout << "Working!" << endl;
}

//DONE
int Filesys::fsclose()
{
    return 1;
}

//DONE
int Filesys::newfile(string file)
{
    for (int x = 0; x < filename.size(); x++)
    {
        if (filename[x] == file)
        {
            cout << "File exists!" << endl;
            return 0;
        }
    }

    for (int x = 0; x < filename.size(); x++)
    {
        if(filename[x] == "$$$$$")
        {
            filename[x] = file;
            firstblock[x] = 0;
            fssynch();
            return 1;
        }
    }
    return 0;
}

//DONE
int Filesys::rmfile(string file)
{
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            if (getfirstblock(file) != 0)
            {
                cout << "This file is not empty" << endl;
                return 0;
            }

            filename[i] = "$$$$$";
            return 1;
        }
    }
    //find file in filename vector;
    //if (firstblock != 0)
    //{
    //    return 0; <-this is because the file is not empty
    //}
    //change filename to $$$$$;
    //return 1;
    return 0;
}

//DONE
int Filesys::getfirstblock(string file)
{
    for(int i = 0; i < filename.size(); i++)
    {
        if(filename[i] == file)
        {
            return firstblock[i];
        }
    }
    cout << "File does not exist!" << endl;
    return 0;
}

//DONE
int Filesys::addblock(string file, string block)
{
    int allocate;

    for (int i = 0; i < filename.size(); i++)
    {

        if (filename[i] == file)
        {
            allocate = fat[0];

            fat[0] = fat[fat[0]];
            fat[allocate] = 0;

            firstblock[i] = 0;
            return 1;
        }

        else
        {
            return 0;
        }
    }

    // - check if file exists
    // - allocate block (allocate = fat[0])
    //      - fat[0] = fat[fat[0]] - updates the free list
    //      - fat[allocate] = 0 - allocated block is the last block
    // - update blocklist for the file
    //      - firstblock = 0
    //      - firstblock != 0
    return 0;
}

//DONE
int Filesys::delblock(string file, int blocknumber)
{
    int deallocate = blocknumber;
    
    //part 1
    if(not checkfileblock(file, blocknumber))
    {
        return 0;
    }

    //part 2
    int b = getfirstblock(file);

    //part 3
    if(b == blocknumber)
    {
        //deleting first block if true - change root
        for(int i = 0; i < filename.size(); i++)
        {
            if(filename[i] == file)
            {
                firstblock[i] = fat[blocknumber];
                break;
            }
        }
    }

    //part 4
    else
    {
        //deleting block from fat - change fat
        while(fat[b] != deallocate)
        {
            b = fat[b];
        }
        //fat[b] = block to be deleted
        fat[b] = fat[blocknumber];
    }

    //part 5
    //update freelist
    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    fssynch();
    return 1;
}

//DONE
int Filesys::readblock(string file, int blocknumber, string& buffer)
{
    if(checkfileblock(file, blocknumber))
    {
        getblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        return 0;
    }
}

//DONE
int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    if(checkfileblock(file, blocknumber))
    {
        putblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        return 0;
    }
}

//DONE
int Filesys::nextblock(string file, int blocknumber)
{
    if(checkfileblock(file, blocknumber))
    {
        return fat[blocknumber]; //returns next blocknumber
    }
    else
    {
        return -1;
    }
}

//DONE
int Filesys::buildfs()
{
    for (int i = 0; i < rootsize; i++)
    {
        filename.push_back("$$$$$");
        firstblock.push_back(0);
        //root created
    }

    fat.push_back(2 + fatsize);
    fat.push_back(-1);

    for(int i = 0; i < fatsize; i++)
    {
        fat.push_back(-1);
    }

    for(int i = 2 + fatsize; i < getnumberofblocks(); i++)
    {
        fat.push_back(i + 1);
    }

    fat[fat.size() - 1] = 0;
    return 1;
}

//DONE
int Filesys::readfs()
{
    //get root
    string buffer;
    getblock(1, buffer);
    istringstream instream1;
    instream1.str(buffer);
    for (int i = 0; i < rootsize; i++)
    {
        string s;
        int b;
        instream1 >> s >> b;
        filename.push_back(s);
        firstblock.push_back(b);
    }
    //get FAT
    string buffer2, b;
    for (int i = 0; i < fatsize; i++)
    {
        getblock(2 + i, b);
        buffer2 += b;
    }

    istringstream instream2;
    instream2.str(buffer2);
    for (int i = 0; i < getnumberofblocks(); i++)
    {
        int n;
        instream2 >> n;
        fat.push_back(n);
    }
    return 1;
}

//DONE
bool Filesys::checkfileblock(string file, int block)
{
    //returns true if block belongs to file
    //false
    int blockid = getfirstblock(file);
    if (blockid <= 0)
    {
        //file does not exist or is empty
        return false;
    }
    while(blockid != 0)
    {
        if(blockid == block)
        {
            return true;
        }
        else
        {
            blockid = fat[blockid];
        }
    }
    return false;
}

//DONE
int Filesys::fssynch()
{
    ostringstream ostream;

    for (int x = 0; x < rootsize; x++)
    {
        ostream << filename[x] << ' ' << firstblock[x] << ' ';
    }

    string buffer = ostream.str();

    vector <string>blocks = block(buffer, getblocksize());

    putblock(1, blocks[0]);

    ostringstream ostream2;

    for (int x = 0; x < fat.size(); x++)
    {
        ostream2 << fat[x] << ' ';
    }

    string buffer2 = ostream2.str();
    vector <string>blocks2 = block(buffer2, getblocksize());

    for (int x = 0; x < blocks.size(); x++)
    {
        putblock(x + 2, blocks[x]);
    }

    return 1;
}

//DONE
vector<string> Filesys::ls()
{
    vector<string> flist;
    for (int i=0; i<filename.size(); i++)
    {
        if (filename[i] != "XXXXX")
        {
            flist.push_back(filename[i]);
        }
    }
    return flist;
}