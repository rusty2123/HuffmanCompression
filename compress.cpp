//HUFF
//By Russell and David

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifndef _WIN32
// TODO: Move this to a header
char* strcpy_s(char* dest, const char* src) {
	return strcpy(dest, src);
}

char* strncpy_s(char* dest, const char* src, std::size_t num) {
	return strncpy(dest, src, num);
}

char* strcat_s(char* dest, const char* src) {
	return strcat(dest, src);
}
#endif

using namespace std;

const  int CODE_LEN = 32;
struct node
{
	int f = 0;
	int g;
	int lt;
	int rt;
	char code[CODE_LEN];
};

int compare(const void * a, const void * b)
{
	struct node *elem1 = (struct node *)a;
	struct node *elem2 = (struct node *)b;

	if (elem1->f < elem2->f)
		return -1;
	else if (elem1->f > elem2->f)
		return 1;
	else
		return 0;
}

void traverse(int c, string code, node A[])
{
	if (A[c].lt != -1)
	{
		traverse(A[c].lt, code + "0", A);
	}

	strcpy_s(A[c].code, code.c_str());

	if (A[c].rt != -1)
	{
		traverse(A[c].rt, code + "1", A);
	}

}

unsigned char encodeByte(char bitstring[])
{
	//building an encoded byte from left to right
	int cnt = 0;

	//byte to be encoded
	unsigned char byte = '\0';

	//length of huffman code
	int bitstringLength = strlen(bitstring);

	byte = '\0';

	cnt = 7;
	for (int i = 0; i < bitstringLength; i++)
	{
		//is the bit "on"?
		if (bitstring[i] == '1')
			//turn the bit on by using the OR bitwise manipulator
			byte = byte | (int)pow(2.0, cnt);
		cnt--;
	}
	return byte;
}

void createOutputFileName(char* inputFileName, char outputFileName[])
{
	for (int i = 0; i < (int)strlen(inputFileName) + 1; i++)
	{
		if (inputFileName[i] == '.' || inputFileName[i] == '\0')
		{
			strncpy(outputFileName, inputFileName, i);
			strcat(outputFileName, ".huf");
			break;
		}
	}
}

void getFrequencies(stringstream &buffer, node A[], int &count)
{
	char symbol;
	bool found = false;

	do
	{
		buffer >> noskipws >> symbol;
		for (int i = 0; i < 256; i++)
		{
			if ((int)symbol == A[i].g)
			{
				++A[i].f;
				found = true;
				break;
			}
		}
		if (found == false)
		{
			for (int j = 0; j < 256; j++)
			{
				if (A[j].g == -1)
				{
					A[j].g = symbol;
					++A[j].f;
					++count;
					break;
				}
			}
		}
		found = false;

	} while (!buffer.eof());
}

int main()
{
	stringstream bitStringBuffer;
	stringstream finBuffer;
	node frequencies[513];
	char symbol;
	int m, h, f, p;
	node root;
	node temp;
	char bitString[9] = "";
	unsigned char byteBuffer[8];
	char reversed[8] = "";
	unsigned char byte;
	int glyphCount = 0;
	ifstream fin;
	ofstream fout;
	char fName[32] = "";
	char hufName[32] = "";
	int fileLength;
	string fileName;
	int entries = 0;
	int arrayLength = 0;

	//initialize all possible glyph nodes to -1 to speed up program during runtime
	for (int i = 0; i < 255; i++)
	{
		frequencies[i].g = -1;
		frequencies[i].lt = -1;
		frequencies[i].rt = -1;
		strcpy_s(frequencies[i].code, "");
	}

	//initialize the byte buffer to null
	for (int i = 0; i < 8; i++)
	{
		byteBuffer[i] = '\0';
	}

	std::cout << "File to be compressed: ";
	cin >> fName;

	//START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START 

	createOutputFileName(fName, hufName);
	
	//read input file into a string stream
	fin.open(fName, ios::in | ios::binary);
	finBuffer << fin.rdbuf();

	getFrequencies(finBuffer, frequencies, glyphCount);

	//insert eof indicator as 0A

	for (int i = 0; i < 256; i++)
	{
		if (frequencies[i].g == -1)
		{
			frequencies[i].g = 256;
			frequencies[i].f = 1;
			++glyphCount;
			break;
		}
	}
	/************************************************/





	/************************************************/

	//children of the node at A[n] are found at A[(2*n)+1] and A[(2*n)+2]

	//sort symbols from the map into an array based on lowest to highest frequency
	qsort(frequencies, glyphCount, sizeof(node), compare);

	/************************************************/
	//construct max heap from min ascending list. 

	// this is looking for the first free (empty) slot in the heap. and setting h and f accordingly.
	for (int i = 0; i < 256; i++)
	{
		if (frequencies[i].f == 0)
		{
			f = i;
			h = i - 1;
			//i have no idea why the last glyph has one frequency higher then it should
			--frequencies[h].f;
			break;
		}
		else
		{
			//get total # of symbols
			entries = entries + frequencies[i].f;
		}
	}

	--entries;

	for (int i = 0; i < glyphCount - 1; i++)
	{
		//mark(m) low of relative slots 2 and 3 (pick the merge node)
		if ((frequencies[1].f <= frequencies[2].f) || (2 > h))
			m = 1;
		else
			m = 2;

		//move m to next free slot f
		frequencies[f] = frequencies[m];


		// what is p? 
		// this compares frequencies[m].f to frequencies[h].f
		if (m < h)
		{
			// move h to m
			frequencies[m] = frequencies[h];
			p = m;
		}

		//reheap if necessary  

		while ((frequencies[p].f > frequencies[2 * p + 1].f
			|| frequencies[p].f > frequencies[2 * p + 2].f))
		{
			if ((frequencies[2 * p + 1].f <= frequencies[2 * p + 2].f) &&
				(2 * p + 1 < h))
			{
				//switching the variables of frequency p and frequencies [(2*p) +1] 
				temp = frequencies[p];
				frequencies[p] = frequencies[2 * p + 1];
				frequencies[2 * p + 1] = temp;
				//making p = to it's new value
				p = 2 * p + 1;
			}
			else if ((2 * p) + 2 < h)
			{
				//switching the variables of frequency p and frequencies [(2*p) +2] 
				temp = frequencies[p];
				frequencies[p] = frequencies[2 * p + 2];
				frequencies[2 * p + 2] = temp;
				//making p = to it's new value
				p = 2 * p + 2;
			}
			else
				break;
		}

		//move lowest freq node [slot zero] to (h) (merge other node)
		frequencies[h] = frequencies[0];

		//create new freq node at [slot zero]
		frequencies[0].g = -1;
		frequencies[0].f = frequencies[h].f + frequencies[f].f;
		frequencies[0].lt = h;
		frequencies[0].rt = f;

		//reheap if necessary
		p = 0;
		// checking to see if either child is smaller than the parent.
		while ((frequencies[p].f > frequencies[2 * p + 1].f
			|| frequencies[p].f > frequencies[2 * p + 2].f))

		{

			// checking if the children nodes are in scope, and if the first one is less than the second one. 
			if ((frequencies[2 * p + 1].f <= frequencies[2 * p + 2].f) &&
				(2 * p + 1 < h))
			{
				//switching the variables of frequency p and frequencies [(2*p) +1] 
				temp = frequencies[p];
				frequencies[p] = frequencies[2 * p + 1];
				frequencies[2 * p + 1] = temp;
				//making p = to it's new value
				p = 2 * p + 1;
			}
			else if ((frequencies[2 * p + 1].f < frequencies[p].f) && (2 * p + 2 >= h) &&
				(2 * p + 1 < h))
			{
				//switching the variables of frequency p and frequencies [(2*p) +1] 
				temp = frequencies[p];
				frequencies[p] = frequencies[2 * p + 1];
				frequencies[2 * p + 1] = temp;
				//making p = to it's new value
				p = 2 * p + 1;
			}
			// checking to see if the second child is in scope 
			else if (2 * p + 2 < h)
			{
				//switching the variables of frequency p and frequencies [(2*p) +2] 
				temp = frequencies[p];
				frequencies[p] = frequencies[2 * p + 2];
				frequencies[2 * p + 2] = temp;
				//making p = to it's new value
				p = 2 * p + 2;
			}
			else
				break;
		}
		//move (h) and (f)
		--h;
		++f;
	}


	/************************************************/

	/************************************************/
	//assign bitcodes


	traverse(0, "", frequencies);




	//read through file again and output bitcodes
	finBuffer.clear();
	finBuffer.seekg(0);

	fout.open(hufName, ios::out | ios::binary);
	fout.clear();
	fout.seekp(0);

	//output header file size, filename, and table size
	arrayLength = (2 * glyphCount) - 1;
	fileLength = strlen(fName);
	fileName.assign(fName);


	fout.write((char*)&fileLength, sizeof fileLength);
	fout << hex << fileName;
	fout.write((char*)&arrayLength, sizeof arrayLength);

	for (int i = 0; i < arrayLength; i++)
	{
		fout.write((char*)&frequencies[i].g, sizeof(int));
		fout.write((char*)&frequencies[i].lt, sizeof(int));
		fout.write((char*)&frequencies[i].rt, sizeof(int));
	}

	for (int i = 0; i < entries - 1; i++) //do
	{
		finBuffer >> noskipws >> symbol;

		for (int i = 0; i < 513; i++)
		{
			if (frequencies[i].g == symbol)
			{
				bitStringBuffer << frequencies[i].code;
				break;
			}
		}
	} //while (!finBuffer.eof());

	for (int i = 0; i < 513; i++)
	{
		if (frequencies[i].g == 256)
		{
			bitStringBuffer << frequencies[i].code;
			break;
		}
	}

	while (!bitStringBuffer.eof())
	{
		strcpy_s(bitString, "00000000");
		//reading in the first 8 bits.
		for (int i = 0; i < 8; i++)
			bitStringBuffer.get(bitString[i]);

		if (strlen(bitString) < 8)
		{
			while (strlen(bitString) < 8)
			{
				strcat_s(bitString, "0");
			}
		}

		// reversing the bits. 
		for (int i = 7; i >= 0; i--)
			reversed[7 - i] = bitString[i];

		// this is changing the 8 bits into a recognized byte, and then clearing out the reverse c style string. 
		byte = encodeByte(reversed);
		strcpy_s(reversed, "");

		fout.write((char*)&byte, sizeof byte);
	}

	fin.close();
	fout.close();
	return 0;
}
