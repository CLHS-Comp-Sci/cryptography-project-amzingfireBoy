#include <iostream>
#include <bitset>
#include <string>
#include <cmath>
#include <vector>
#include <random>
using namespace std;

class Cipher{
public:

    void printESubkeys(){
        for(int i = 0; i < eKeys.size(); i++){
            cout << eKeys[i] << " ";
        }
        cout << endl;
    }

    void printDSubkeys(){
        for(int i = 0; i < dKeys.size(); i++){
            cout << dKeys[i] << " ";
        }
        cout << endl;
    }

    void setKey(string key){
        curKey = key;
        for(int i = 0; i < 8; i++){
            eKeys.push_back((int)key[i]);
        }
        
        dKeys.push_back(-1*eKeys[3]);
        dKeys.push_back(-1*eKeys[2]);
        dKeys.push_back(-1*eKeys[1]);
        dKeys.push_back(-1*eKeys[0]);
        
        dKeys.push_back(-1*eKeys[7]);
        dKeys.push_back(1/eKeys[6]);
        dKeys.push_back(1/eKeys[5]);
        dKeys.push_back(-1*eKeys[4]);
        

    }

    char encRound(char x){
        unsigned int let = (int)x;
        unsigned int k1 = let + eKeys[0];
        unsigned int k2 = k1 - eKeys[1];
        unsigned int k3 = k2 + eKeys[2];
        unsigned int k4 = k3 - eKeys[3];

        return (char)k4;
    }

    char decRound(char x){
        unsigned int let = (int)x;
        unsigned int k1 = let - dKeys[0];
        unsigned int k2 = k1 + dKeys[1];
        unsigned int k3 = k2 - dKeys[2];
        unsigned int k4 = k3 + dKeys[3];

        return (char)k4;
    }

    string encrypt(string text){
        string res = "";
        for(int i = 0; i < text.size(); i++){
            res = res + encRound(text[i]);
            //cout << (int)encRound(text[i]) << endl << endl;
        }
        return res;
    }

    string decrypt(string text){
        string res = "";
        for(int i = 0; i < text.size(); i++){
            res = res + decRound(text[i]);
        }
        return res;
    }

    

private:

    string curKey;
    vector<double> eKeys;
    vector<double> dKeys;

    int eBlk[4];
    int dBlk[4];

};

default_random_engine generator;
uniform_int_distribution<int> distribution(33,122);

int main(){
    string plainText = "";
    string key = "";
    //cout << bitset<64>(key) << endl;
    //cout << bitset<64>(bitset<8>(key[0]).to_string()+bitset<8>(key[1]).to_string()+bitset<8>(key[2]).to_string()+bitset<8>(key[3]).to_string()+bitset<8>(key[4]).to_string()+bitset<8>(key[5]).to_string()+bitset<8>(key[6]).to_string()+bitset<8>(key[7]).to_string()) << endl;
    Cipher ciph = Cipher();

    for(int i = 0; i < 8; i++){
        key = key + (char)(distribution(generator));
    }

    cout << "Enter the text to be encrypted: " << endl;
    getline(cin,plainText);

    ciph.setKey(key);
    string encText = ciph.encrypt(plainText);
    string decText = ciph.decrypt(encText);
    cout << endl;
    cout << "Plain text: " << plainText << endl;
    cout << "Key: " << key << endl;
    cout << "encrypted text: " << encText << endl;
    cout << "decrypted text: " << decText << endl;
    system("pause");
}