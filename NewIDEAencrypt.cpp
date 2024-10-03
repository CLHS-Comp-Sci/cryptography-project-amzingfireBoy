#include <iostream>
#include <bitset>
#include <string>
#include <cmath>
#include <vector>
#include <random>
using namespace std;

#define ROL32(a,n) (((a) << (n)) | ((a) >> (32 - (n))))


class IDEACipher{
public:
    IDEACipher(){};
    ~IDEACipher(){};

    // int mul(unsigned int a, unsigned int b){
    //     if(a == 0){
    //         a = 65536;
    //     }
    //     if(b == 0){
    //         b = 65536;
    //     }
    //     unsigned int c = ((a*b) % 65537);
    //     if(c == 65536){
    //         c = 0;
    //     }
    //     return c;
    // }

    int mul(unsigned int a, unsigned int b){
        int c;
    
        //Perform multiplication modulo 2^16 - 1
        c = a * b;
    
        if(c != 0)
        {
        c = ((ROL32(c, 16) - c) >> 16) + 1;
        }
        else
        {
        c = 1 - a - b;
        }
    
        //Return the result
        return c & 0xFFFF;
    }

    unsigned int ideaInv(int a){
        int b;
        int q;
        int r;
        int t;
        int u;
        int v;
    
        //Compute the multiplicative inverse modulo 2^16 - 1
        b = 0x10001;
        u = 0;
        v = 1;
    
        while(a > 0){
        q = b / a;
        r = b % a;
    
        b = a;
        a = r;
    
        t = v;
        v = u - q * v;
        u = t;
        }
    
        if(u < 0){
        u += 0x10001;
        }
    
        //Return the result
        return int(u);
    }
    
    int add(unsigned int a, unsigned int b){
        unsigned int c = (a+b) % 65536;
        return c;
    }
    
    void fullRound(int index){
        
        int k1 = mul(encBl[0],subkeys[index]);
        int k2 = add(encBl[1],subkeys[index+1]);
        int k3 = add(encBl[2],subkeys[index+2]);
        int k4 = mul(encBl[3],subkeys[index+3]);

        int k5 = k1^k3;
        int k6 = k2^k4;
        int k7 = mul(k5,subkeys[index+4]);
        int k8 = mul(add(k6,k7),subkeys[index+5]);
        int k9 = add(k7,k8);

        encBl[0] = k1^k8;
        encBl[2] = k2^k9;
        encBl[1] = k3^k8;
        encBl[3] = k4^k9;
    }

    void halfRound(){
        encBl[0] = mul(encBl[0],subkeys[50]);
        encBl[1] = add(encBl[1],subkeys[51]);
        encBl[2] = add(encBl[2],subkeys[52]);
        encBl[3] = mul(encBl[3],subkeys[53]);
    }

    void DfullRound(int index){
        
        int k1 = mul(decBl[0],Dsubkeys[index]);
        int k2 = add(decBl[1],Dsubkeys[index+1]);
        int k3 = add(decBl[2],Dsubkeys[index+2]);
        int k4 = mul(decBl[3],Dsubkeys[index+3]);

        int k5 = k1^k3;
        int k6 = k2^k4;
        int k7 = mul(k5,Dsubkeys[index+4]);
        int k8 = mul(add(k6,k7),Dsubkeys[index+5]);
        int k9 = add(k7,k8);

        decBl[0] = k1^k8;
        decBl[2] = k2^k9;
        decBl[1] = k3^k8;
        decBl[3] = k4^k9;
    }

    void DhalfRound(){
        decBl[0] = mul(encBl[0],Dsubkeys[48]);
        decBl[1] = add(encBl[1],Dsubkeys[49]);
        decBl[2] = add(encBl[2],Dsubkeys[50]);
        decBl[3] = mul(encBl[3],Dsubkeys[51]);
    }

    string encrypt(string text){
        encBl[0]= bitset<16>(bitset<8>(text[0]).to_string()+bitset<8>(text[1]).to_string()).to_ulong();
        encBl[1]= bitset<16>(bitset<8>(text[2]).to_string()+bitset<8>(text[3]).to_string()).to_ulong();
        encBl[2]= bitset<16>(bitset<8>(text[4]).to_string()+bitset<8>(text[5]).to_string()).to_ulong();
        encBl[3]= bitset<16>(bitset<8>(text[6]).to_string()+bitset<8>(text[7]).to_string()).to_ulong();


        for(int i = 0; i < 8; i++){
            fullRound(i*6);
        }
        int temp = encBl[1];
        encBl[1] = encBl[2];
        encBl[2] = temp;
        halfRound();

        string result = "";
        for(int i = 0; i < 4; i++){
            result = result + (char)(bitset<8>(bitset<16>(encBl[i]).to_string().substr(0,8)).to_ulong())+(char)(bitset<8>(bitset<16>(encBl[i]).to_string().substr(8,8)).to_ulong());
        }
        return result;
    }

    string decrypt(string text){
        decBl[0]= bitset<16>(bitset<8>(text[0]).to_string()+bitset<8>(text[1]).to_string()).to_ulong();
        decBl[1]= bitset<16>(bitset<8>(text[2]).to_string()+bitset<8>(text[3]).to_string()).to_ulong();
        decBl[2]= bitset<16>(bitset<8>(text[4]).to_string()+bitset<8>(text[5]).to_string()).to_ulong();
        decBl[3]= bitset<16>(bitset<8>(text[6]).to_string()+bitset<8>(text[7]).to_string()).to_ulong();


        
        

        for(int i = 0; i < 8; i++){
            DfullRound(i*6);
        }
        int temp = decBl[1];
        decBl[1] = decBl[2];
        decBl[2] = temp;
        
        DhalfRound();
        
        

        string result = "";
        for(int i = 0; i < 4; i++){
            result = result + (char)(bitset<8>(bitset<16>(decBl[i]).to_string().substr(0,8)).to_ulong())+(char)(bitset<8>(bitset<16>(decBl[i]).to_string().substr(8,8)).to_ulong());
        }
        return result;
    }

    void test(string text){
        
        string res = encrypt(text);
        cout << res << endl;
        cout << decrypt(res) << endl; 

    }

    void setKey(bitset<128> k){
        key = k;
        for(int i = 0; i < (9*6); i++){
            subkeys.push_back((bitset<16>(k.to_string().substr((16*(i%8)),16)).to_ulong()));
            Dsubkeys.push_back(0);
            //cout << bitset<16>(subkeys[i]) << dec << endl;
            //cout << bitset<16>(Dsubkeys[i]) << dec << endl;
            if(i%8 == 7){
                k = ((k << 25) | (k >> 103));
                //cout << "0x" << hex << k << dec << endl;
            }
        }

        for(int i = 0; i < 52; i += 6){
            Dsubkeys[i] = ideaInv(subkeys[48 - i]);
        
            if(i == 0 || i == 48)
            {
                Dsubkeys[i + 1] = -subkeys[49 - i];
                Dsubkeys[i + 2] = -subkeys[50 - i];
            }
            else
            {
                Dsubkeys[i + 1] = -subkeys[50 - i];
                Dsubkeys[i + 2] = -subkeys[49 - i];
            }
        
            Dsubkeys[i + 3] = ideaInv(subkeys[51 - i]);
        
            if(i < 48)
            {
                Dsubkeys[i + 4] = subkeys[46 - i];
                Dsubkeys[i + 5] = subkeys[47 - i];
            }
        }
        
        // for(int i = 0; i < 8; i++){
        //     Dsubkeys.push_back(bitset<16>(subkeys[(6*(8-i))-1]).to_ulong());
        //     Dsubkeys.push_back(~bitset<16>(subkeys[(6*(8-i))-2]).to_ulong());
        //     Dsubkeys.push_back(~bitset<16>(subkeys[(6*(8-i))-3]).to_ulong());
        //     Dsubkeys.push_back(bitset<16>(subkeys[(6*(8-i))-4]).to_ulong());
        //     Dsubkeys.push_back(bitset<16>(subkeys[(6*(8-i))-5]).to_ulong());
        //     Dsubkeys.push_back(bitset<16>(subkeys[(6*(8-i))-6]).to_ulong());
        // }
        // Dsubkeys.push_back(bitset<16>(subkeys[51]).to_ulong());
        // Dsubkeys.push_back(~bitset<16>(subkeys[50]).to_ulong());
        // Dsubkeys.push_back(~bitset<16>(subkeys[49]).to_ulong());
        // Dsubkeys.push_back(bitset<16>(subkeys[48]).to_ulong());
        
    }

private:
    int a = 0xFFFF;
    bitset<128> key;
    vector<unsigned int> subkeys;
    vector<unsigned int> Dsubkeys;

    int encBl[4];
    int decBl[4];

};

default_random_engine generator;
uniform_int_distribution<int> distribution(33,122);

int main(){
    IDEACipher cipher;


    string key = "";

    for(int i = 0; i < 16; i++){
        key = key + (char)(distribution(generator));
    }
    string res = "";
    for(int i = 0; i < 16; i++){
        res = res + bitset<8>(key[i]).to_string();
    }

    bitset<128> bitKey(res);
    cipher.setKey(bitKey);

    string plainText = "";

    cout << "Enter the text to be encrypted: " << endl;
    getline(cin, plainText);
    //cin >> plainText;
    int len = plainText.length();
    
    if(len % 8 != 0){
        for(int i = 0; i < (8-(len % 8)); i++){
            plainText = plainText + " ";
        }
    }

    int index = 0;
    string cipherText = "";

    while(index < plainText.length()){
        cipherText = cipherText + cipher.encrypt(plainText.substr(index,8));
        index += 8;
    }


    cout << "key: " << key << endl;
    cout << "Plain Text: " << plainText << "-" << endl;
    cout << "Cipher Text: " << cipherText << endl;

    system("pause");
}







