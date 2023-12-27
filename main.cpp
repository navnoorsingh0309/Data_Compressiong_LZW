#include<bits/stdc++.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<fstream>
using namespace std;

//Converting Decimal to binary according to appropriate bitsProcessed
string decToBinary(int n, int bitsProcessed) {
    int binaryNum[32];

    int i=0;
    while(n>0) {
        binaryNum[i] = n%2;
        n = n/2;
        i++;
    }
    string ss="";
    for(int j=i-1;j>=0;j--)
        ss+=to_string(binaryNum[j]);
    string data_to_send="";
    for(int i=0;i<bitsProcessed-ss.length();i++) {
        data_to_send+="0";
    }
    data_to_send += ss;
    return data_to_send;
}

//Converting Binary to Decimal with appropriate bitsProcessed
int binaryToDec(string s, int bitsProcessed) {
    int dec = 0;
    for(int i=bitsProcessed-1;i>=0;i--) {
        dec+=((int(s[bitsProcessed-1-i]))-48) * pow(2, i);
    }
    return dec;
}

//Check if certain value exists in decoing map or not
bool mapValueExists(map<int, string>& Map, string value) {
    bool a = false;
    for(auto& it:Map) {
        if(it.second == value) {
            a = true;
        }
    }
    return a;
}


//Main Function
int main() {
    int counted = 256, bitsProcessed = 9;
    //Counted: New ascii(likewise) codes
    //bitProcessed: number of bits per code
    map<string, int> dictionary;   //Encoding Map
    //Storing all characters in encoding Map
    string c = "";
    for(int i=0;i<256;i++)
    {
        c = "";
        c += char(i);
        dictionary.insert(pair<string, int>(c, i));
    }
    string line="";
    //line: To Read from file
    //Reading from file
    ifstream fin;
    fin.open("Sample.txt");
    string data_Read = "";
    //Reading Data line by line
    while(getline(fin, line)) {
        data_Read+=line+"\n";
    }
    //Closeing file
    fin.close();

    //Compressing Data and converting to ascii(likewise) codes
    int *compressed_data = (int*)malloc(sizeof(int));
    //compressed_data: A dynamic flexible array used to store ascii(likewise) codes for compressed data
    int CD_Size = 1;    //Compressed_Data_Size
    string temp = "";   //To store data temporary for performing LZWA algorithm
    for(int i=0;i<data_Read.length();i=i)
    {
        temp = data_Read[i];
        do
        {
            i++;
            temp += data_Read[i];
        }
        while(dictionary.find(temp) != dictionary.end());
        //Allocating compressed_data dynamic array size according to need
        compressed_data = (int*)realloc(compressed_data, CD_Size*sizeof(int));
        //Adding out ascii(likewise) codes to compressed_data
        compressed_data[CD_Size-1] = dictionary[temp.substr(0, temp.length()-1)];
        CD_Size++;
        //Inserting data pair to map
        dictionary.insert(pair<string, int>(temp, counted));
        counted++;
    }
    //Processed Bits
    if(counted>=256 && counted<512)
        bitsProcessed = 9;
    else if(counted>=512 && counted<1024)
        bitsProcessed = 10;
    else if(counted>=1024 && counted<2048)
        bitsProcessed = 11;
    else if(counted>=2048 && counted<4096)
        bitsProcessed = 12;

    //Converting ascii(likewise) codes to Binary
    string completebinaryString = "";
    for(int i=0;i<CD_Size-1;i++)
        //Converting Decimal to Binary with appropriate processed bits
        completebinaryString += decToBinary(compressed_data[i], bitsProcessed);
    
    //Segregating whole binary String to 8 bits combinations to convert them to bytes
    string Compressed_string = "";
    for(int i=0;i<((completebinaryString.length())/8+(((completebinaryString.length())%8!=0)?1:0));i++)
        Compressed_string += char(binaryToDec(completebinaryString.substr(i*8, 8), 8));
    //Storing whole data in a file
    ofstream fout;                                                  
    fout.open("Sample.lzwa", ios::binary);                      //Opening File
    fout<<(to_string(bitsProcessed)+"\n"+Compressed_string);    //Adding compressed bits along with bits processed to file
    fout.close();                                               //Closing File


    //Decoding from File
    fin.open("Sample.lzwa", ios::binary);                       //Opening File with binary format
    int numberoflines = 0;
    //Counting number of lines in file
    while(getline(fin, line)) {
        numberoflines++;
    }
    fin.close();                                                //Closing File 
    fin.open("Sample.lzwa", ios::binary);                       //Againg opening File to read data
    //Reading data line by line and converting them to Binary codes
    //Variables::
    string binary_Read;
    int bitsProcessed_Read;
    //temp: used to store temporary value during conversion
    //binary_Read: to store whole binary string
    //bitsProcessed_Read: contains bits processed read thorugh file
    for(int i=0;i<numberoflines;i++) {
        //Getting line from file
        getline(fin, line);
        //If not first line as first line contains bits processed
        if(i!=0) {
            for(int j=0;j<line.length();j++)
            {
                temp = "";
                //for ascii and extended ascii codes
                if(int(line[j])>=0)
                    temp+=decToBinary(int(line[j]), 8);
                else
                    temp+=decToBinary(255-int(line[j])*(-1)+1, 8);
                //Adding whole binary 8 bits to binary_Read
                binary_Read+=temp;
            }
        }
        else {
            //Reading Processed bits
            bitsProcessed_Read = stoi(line);
        }
    }

    //Covnerting data Binary to decimal
    map<int, string> decoding_dictionary;       //Decoding Map
    //Adding single character values to map
    for(int i=0;i<256;i++)
    {
        c = "";
        c += char(i);
        decoding_dictionary.insert(pair<int, string>(i, c));
    }
    temp = "";
    //temp: for temprory storing data during conversion
    counted = 256;
    //counted: to get a count of new ascii(likewise) codes
    int begin=0, end=1;
    string temp2 = "";
    //begin, end: Is used when length of temp>2 and we need to use temp2 for furthur storage
    //temp2: to furthur store segregatted data temporarily
    for(int i=0;i<binary_Read.length()/bitsProcessed_Read;i++) {
        temp+=decoding_dictionary[binaryToDec(binary_Read.substr(i*bitsProcessed_Read, bitsProcessed_Read), bitsProcessed_Read)];

        //if decoding map contains this value(print it)
        if(mapValueExists(decoding_dictionary, decoding_dictionary[binaryToDec(binary_Read.substr(i*bitsProcessed_Read, bitsProcessed_Read), bitsProcessed_Read)]))
            cout<<decoding_dictionary[binaryToDec(binary_Read.substr(i*bitsProcessed_Read, bitsProcessed_Read), bitsProcessed_Read)];

        //If temp value is missing from decoding map
        if(!mapValueExists(decoding_dictionary, temp))
        {
            //if length is only 2 as we need to segreggate otherwise
            if(temp.length()==2) {
                //Inserting temp to decoding amp
                decoding_dictionary.insert(pair<int, string>(counted, temp));
                counted++;
                //Getting last character in string
                temp = temp.back();
            }
            else {
                //Initilizing begin and end values
                begin = 0;
                end = 1;
                //getting 2 characters
                temp2 = temp.substr(begin, 2);
                for(int i=0;i<=temp.length()-2;i++) {
                    //if temp2 doesn't exist in map
                    if(!mapValueExists(decoding_dictionary, temp2)) {
                        //Inserting temp2 to decoding map
                        decoding_dictionary.insert(pair<int, string>(counted, temp2));
                        counted++;
                        //changing begin and end values accodingly
                        begin = begin + temp2.length() - 1;
                        end = begin + 1;
                        //getting last character of temp2 + one furthur chracter and store in temp2
                        temp2 = temp2.back()+temp.substr(end, 1);
                    }
                    else {
                        //if it has next character
                        if((end+1)<temp.length()) {
                            //Concatinating temp2 to next chracter
                            temp2 += temp.substr(end+1, 1);
                            //updating end values accordingly
                            end = end+1;
                        }
                    }
                }
                //updating temp value
                temp = temp2;
            }
        }
    }
}
