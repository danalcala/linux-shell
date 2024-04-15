#include "sdisk.h"
using namespace std;

//DONE
Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
	this -> diskname = diskname;
	this -> numberofblocks = numberofblocks;
	this -> blocksize = blocksize;
	
	fstream infile;
	infile.open(diskname.c_str(), ios::in|ios::out);
	
	if(!infile.good())
	{
		infile.close();
		ofstream infile;
		cout << "Disk does not exist, creating new Sdisk" << endl;
		infile.open(diskname.c_str());
		for (int i = 0; i < (blocksize * numberofblocks); i++)
		{
			infile.put('#');
		}
		cout << "Sdisk created!" << endl;
	}
	
	if(infile.good())
	{
		cout << diskname << " exists.\n";
	}
}

//DONE
int Sdisk::getblock(int blocknumber, string &buffer)
{
	fstream iodisk;
	iodisk.open(diskname.c_str(), ios::in|ios::out);
	
	iodisk.seekg(blocknumber * blocksize);

	
	if (blocknumber <= 0 || blocknumber > numberofblocks)
	{
		return 0;
	}
	
	for (int i = 0; i < blocksize; i++)
	{
		char c = iodisk.get();
		buffer.push_back(c);
	}
	return 1;
}

//DONE
int Sdisk::putblock(int blocknumber, string buffer)
{
	fstream iofile;
	iofile.open(diskname.c_str(), ios::in|ios::out);

	iofile.seekp(blocknumber * blocksize);
	
	if (blocknumber > 0 && blocknumber < numberofblocks && buffer.length() == blocksize)
	{
		for (int i = 0; i < buffer.length(); i++)
		{
			iofile.put(buffer[i]);
		}
		iofile.close();
		return 1;
	}
	
	else
	{
		return 0;
	}
}

//DONE
int Sdisk::getnumberofblocks()
{
	return numberofblocks;
}

//DONE
int Sdisk::getblocksize()
{
	return blocksize;
}
