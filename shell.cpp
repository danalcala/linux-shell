#include "shell.h"

//DONE
Shell::Shell(string filename, int numberofblocks, int blocksize):Filesys(filename, numberofblocks, blocksize)
{

}

//DONE
int Shell::dir()
{
    vector<string> flist=ls();

    for (int i=0; i<flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
    return 1;
}

//DONE
int Shell::add(string file,string buffer)
{
    int errorcode = newfile(file);
    if (errorcode == 0)
    {
        //file cannot be created
        return 0;
    }

    vector<string> blocks = block(buffer, getblocksize());
    for (int i = 0; i <blocks.size(); i++)
    {
        addblock(file, blocks[i]);
    }

    return 1;
}

//DONE
int Shell::del(string file)
{
    //remove blocks
    int b = getfirstblock(file);
    if (b == -1)
    {
        return 0;
    }
    if (b == 0)
    {
        return 1;
    }

    while(b != 0)
    {
        delblock(file, b);
        b = getfirstblock(file);
    }
    rmfile(file);
    return 1;

}

//DONE
int Shell::type(string file)
{
    string buffer1, buffer2;
    int b = getfirstblock(file);

    if (b == -1)
    {
        cout << "No such file!" << endl;
        return 0; //no file
    }

    if (b == 0)
    {
        return 1;
    }

    while (b != 0)
    {
        readblock(file, b, buffer1);
        buffer2 += buffer1;
        b = nextblock(file, b);
    }
    cout << buffer2;
    return 1;
}

//DONE
int Shell::copy(string file1, string file2)
{
    int b1 = getfirstblock(file1);
    if (b1 == -1)
    {
        return 0; //no file
    }
    int b2 = getfirstblock(file2);
    //check if file exists  -> exit

    string buffer;
    while (b1 != 0)
    {
        readblock(file1, b1, buffer);
        b1 = nextblock(file1, b1);
        addblock(file2, buffer);
    }
    return 1;
}