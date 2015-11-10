//PUFF
//By David and Russell




#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <iomanip>
#include <new>

using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::fill;
using std::array;
using std::string;


using namespace std;

struct Node
{
    int glyph;
    int leftPointer;
    int rightPointer;
};

void main()
{
    
    //const long long MAXSIZE = 10000000;// note- I think i am too small for the large file that he is planning on giving us. maybe find another way to do this? 


    int tempLeftPointer;
    int tempRightPointer;
    char tempGlyph;
    string initialFileName;
    char originalName[64];
    int huffmanTableSize;
    int sizeOfName;
    Node mainArray[513];
    Node *next, *previous, *main;
    int* messageArray = new int[10000000];
    unsigned char byte;
    const int BITMASK1 = 1;
    int current;
    bool end = false;

    cout << "Please enter the name of file to be decompressed" << endl;

    cin >> initialFileName;

    //START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START START 


    ifstream fileIn;
    fileIn.open(initialFileName, ios::in | ios::binary); //why am I getting error messages here?
    // get rid of the test calls eventually. 
    if (fileIn.fail())
    {

        cout << " Input file failed to load properly" << endl;
        exit(1);
    }






    // reading the file into holdingBuffer
    stringstream holdingBuffer;

    holdingBuffer << fileIn.rdbuf(); // used to read in the file into the holding buffer.
    // std::string test = holdingBuffer.str(); // used to check to make sure string is working.
    //    std::cout << test << std::endl << std::endl; // ditto
    // getting the file closed
    fileIn.close();



    //reading in the header file part.
    holdingBuffer.read((char*)&sizeOfName, 4);
    holdingBuffer.get(originalName, sizeOfName + 1);
    holdingBuffer.read((char*)&huffmanTableSize, 4);

    ofstream fileOut;
    // get rid of the test calls eventually. 
    fileOut.open(originalName, ios::out | ios::binary);
    if (fileOut.fail())
    {

        cout << "file failed to load properly again" << endl;
        exit(1);
    };

    //reading in the huffmanTable
    for (int i = 0; i < huffmanTableSize; i++)
    {
        holdingBuffer.read((char*)&mainArray[i].glyph, 4);
        holdingBuffer.read((char*)&mainArray[i].leftPointer, 4);
        holdingBuffer.read((char*)&mainArray[i].rightPointer, 4);

    }

    //read in the last bytes, and decode them. 


    //use the table to figure out the bit code for each glyph.

    // effectively pointing at the first spot in the array. 
    current = 0;

    while (!holdingBuffer.eof() && !end)
    {

        holdingBuffer.read((char*)&byte, 1);

        // cout << "The bit code for " << byte << " is "; test code, delete later
        for (int i = 0; i < 8 && !end; i++)
        {
            //test to see if the high-order bit 7 is on
            if ((byte & BITMASK1))
            {
                //    cout << "1";
                current = mainArray[current].rightPointer;
            }
            else
            {
                //    cout << "0";
                current = mainArray[current].leftPointer;
            }

            if (mainArray[current].rightPointer == -1)
            {
                if (mainArray[current].glyph == 256)
                    end = true;
                else
                {
                    fileOut << (char)mainArray[current].glyph;
                    current = 0;
                }


            }
            //shift byte by one bit to the left using shift left operator
            byte = byte >> 1;
        }
    }
    //always start at mainArray[o];





    delete[] messageArray;
    fileOut.close();
}
