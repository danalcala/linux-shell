#include "table.h"

//DONE
Table::Table(string diskname,int blocksize,int numberofblocks, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    this->flatfile = flatfile.substr(0,5);
    this->indexfile = indexfile.substr(0,5);
}

//DONE
int Table::Build_Table(string input_file)
{
    ifstream infile;
    infile.open(input_file.c_str());
    newfile(flatfile);
    newfile(indexfile);
    ostringstream ostream;
    string record, key;
    getline(infile, record);

    while(infile.good())
    {
        key = record.substr(0,5);
        vector<string> blocks = block(record, getblocksize());
        int b = addblock(flatfile, blocks[0]);
        ostream << key << " " << b << " ";
        getline(infile, record);
    }

    ostream << "kangaroo"<<" "<< 0 << " ";
    string buffer = ostream.str();
    vector<string> blocks2 = block(buffer, getblocksize());
    for (int i = 0; i < blocks2.size(); i++)
    {
        addblock(indexfile, blocks2[i]);
    }
    return 1;
}

//DONE
void Table::Search(string key)
{
    int b = indexSearch(key);
    if (b == -1)
    {
        cout << "Record not found!" << endl;
        return;
    }
    else
    {
        string buffer;
        readblock(flatfile, b, buffer);
        cout << buffer;
    }
    return;
}

//DONE(returns -1 if record not found)
int Table::indexSearch(string key)
{
    string buffer;
    string buffer2;
    int b = getfirstblock(indexfile);
    //error checking is in order here
    while (b != 0)
    {
        readblock(indexfile, b, buffer);
        buffer2 += buffer;
        b = nextblock(indexfile, b);
    }

    istringstream istream;
    istream.str(buffer2);
    string ikey;
    int iblock;
    istream >> key >> iblock;
    while (ikey != "kangaroo")
    {
        if(ikey == key)
        {
            return iblock;
        }
        istream >> ikey >> iblock;
    }
    return -1;
}