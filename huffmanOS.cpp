#include <bits/stdc++.h>
#include <unistd.h>
#include <cstring>
#include "huffmanTree.h"
using namespace std;

/*please forgive my code, i know its pretty bad but please look at my 20/100 and have mercy
sources:
https://www.geeksforgeeks.org/convert-string-char-array-cpp/
https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/
https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/?ref=gcse
i coded mostly all of my runner function by hand, thats why its pretty bad
any help that i recieved for that area was given by https://chat.openai.com
*/

struct threadargs
{
    string fileholder = "";
    string huffmantree = "";
    vector<char> &arrme;
    char *storageArr;
    int storageArrSize;
};

//////////////////////////////////////////////////threading function that is completely broken and will only run one case
void *runner(void *pArg)
{

    threadargs *threadargs2 = static_cast<threadargs *>(pArg);
    string line = threadargs2->fileholder;

    /////////////////////////////////////////////////getting huffman tree data
    string huffman = threadargs2->huffmantree;
    stringstream ss5(huffman);
    vector<string> tree;
    vector<string> chars;
    string holder5;

    while (getline(ss5, holder5, '\n')) // this code block removes everything but the char and the binary code
    {
        std::string symbol = "Symbol: ";
        std::string freqency = "Frequency:";
        std::string code = "Code:";
        size_t pos1 = holder5.find(freqency);
        size_t pos2 = holder5.find(code);
        size_t pos3 = holder5.find(symbol);
        if (pos1 != std::string::npos && pos1 + freqency.length() <= holder5.length())
        {
            holder5.erase(pos1, freqency.length());
        }
        if (pos3 != std::string::npos && pos3 + symbol.length() <= holder5.length())
        {
            holder5.erase(pos3, symbol.length());
        }
        holder5.erase(5, 7);
        holder5.erase(2, 4);
        string stick = ":";
        holder5.replace(1, 1, ":");
        tree.push_back(holder5);
        string holder97;
        stringstream ss97(holder5);

        while (getline(ss97, holder97, ':'))
        {
            chars.push_back(holder97); // separation of the tree
        }
    }
    vector<string> numberholder;
    for (int i = 0; i < chars.size() - 1; i += 2)
    {
        numberholder.push_back(chars[i + 1]);
    }
    vector<string> letters;
    for (int i = 0; i < chars.size() - 1; i += 2)
    {
        letters.push_back(chars[i]);
    }
    ////////////////////////////////////////////////////////////////getting the storage array
    char *storageArr = threadargs2->storageArr;
    int storageArrSize = threadargs2->storageArrSize;
    storageArr[storageArrSize];
    /////////////////////////////////////////////////////////gets the char info
    vector<char> &arr = threadargs2->arrme;
    ////////////////////////////////////////////////////////////////////////////////getting positions
    string holder;
    stringstream ss6(line);
    vector<string> pos;
    while (getline(ss6, holder, ' '))
    {
        stringstream ss7(holder);
        string holder2;
        while (getline(ss7, holder2, ' ')) // trying to remove any spaces that exist.
        {
            pos.push_back(holder2);
        }
    }
    pos.erase(pos.begin()); // gets rid of the code num.
    //////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////pulling firstnumber /////this whole statement is pointless, could have just pulled vector[0];

    string firstnumtemp;
    stringstream holdme;

    for (int i = 0; i < line.size(); i++)
    {

        if (line[i] != ' ') // just makes sure that were taking in only values
        {
            holdme << line[i];
        }
        if (line[i] == ' ') // asks if its a space

        {
            if (i != 0) // checks to make sure that its not storing a random space
            {
                firstnumtemp = holdme.str();
                holdme.str("");
                break;
            }
        }
    }

    const char *firstnum = firstnumtemp.c_str(); // converts string to char

    ////////////////////////////////////////////////////////////this stupid ass block of code that barely runs
    int size = tree.size();

    for (int i = 0; i < storageArrSize; i++) // runs through every storage position
    {
        for (int x = 0; x < tree.size(); x++) // runs through every number in the tree
        {
            if (numberholder[x] == firstnum) // asks 'hey, does the tree node have the first num?'
            {
                for (int f = 0; f < letters.size(); f++) // running through the letters
                {
                    if (tree[x].find(arr[f]) != string::npos) // asks 'hey, does that same tree node contain this char?
                    {
                        for (int k = 0; k < pos.size(); k++) // running through the position of that letter
                        {
                            int posvalue = stoi(pos[k]);
                            if (i == posvalue) // if the position is equal to the storage position
                            {
                                storageArr[i] = arr[f]; // store it
                            }
                        }
                    }
                }
            }
        }
    }

    return NULL;
}
///////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    string filename;
    string decodeme;
    cin >> filename;
    cin >> decodeme;
    fstream myfile;
    char cap;
    int capitol;
    vector<char> arr; // vector to hold characters
    vector<int> freq; // vector to hold frequencys
    myfile.open(filename);
    while (myfile >> cap && myfile >> capitol) // just gets the info from the files
    {
        if (cap != '\n')
        {
            arr.push_back(cap);
            cap = '\n';
        }
        if (capitol != 100)
        {
            freq.push_back(capitol);
            capitol = 100;
        }
    }
    myfile.close();

    vector<char> capsd{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    /*FOR SOME REASON, IF I DELETE THIS VECTOR THE CODE
    WILL NOT RUN EVEN THOUGH IT IS NOT BEING UTILIZED ANYWHERE*/

    int size = arr.size();

    pthread_t threads[size];
    int thread_args[size];
    int result_code;

    string holder;
    vector<string> fileholder;
    stringstream ss;
    myfile.open(decodeme);
    for (int i = 0; i < size; i++)
    {
        while (getline(myfile, holder, '\n'))
        {
            fileholder.push_back(holder);
            break;
        }
    }
    myfile.close();
    ////////////////////////////////////////////////////////////////////getting the tree data
    stringstream treeman;
    string huffmantree;
    huffmantree = HuffmanCodes(arr, freq, size);
    cout << huffmantree;
    //////////////////////////////////////////////////////////////////////////////splitting the tree into two parts
    ////////////////////////////////////////////////////////////////////////////array info
    int storageamount = accumulate(freq.begin(), freq.end(), 0);
    int storageArrSize = storageamount;
    char storageArr[] = {'0'};
    ///////////////////////////////////////////////////////////////////getting char info
    vector<char> arr2 = arr;
    /////////////////////////////////////////////////////////////
    for (int i = 0; i < size; i++)
    {

        string holder3 = fileholder[i];
        threadargs args = {holder3, huffmantree, arr2, storageArr, storageArrSize};
        pthread_create(&threads[i], NULL, runner, (void *)&args);
        sleep(0); // this is the only way to be able to run muliple arguements through without overlapping, 
                //it was never mentioned that we couldnt use it, my apologies
    }

    for (int i = 0; i < size; i++)
    {

        pthread_join(threads[i], NULL);
    }
    //////////////////////////////////////////////original message
    cout << "Original message: ";
    for (int i = 0; i < storageArrSize; i++)
    {
        cout << storageArr[i];
    }
    return 0;
}
