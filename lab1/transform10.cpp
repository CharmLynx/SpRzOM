#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <iomanip>

using namespace std;

void hex_32(const string& hex_string, uint64_t digits[], int& count){

    int hex_length = hex_string.length();
    string part(8,'0'); //для перетворення
    //перезапис аби можна було з цим працювати
    while(hex_length>0){
        if(hex_length>=8){
            //вибираю останні 8
            for(int i=0;i<8;i++){
                part.at(i)=hex_string.at(hex_length-8+i);
            }
            hex_length=hex_length-8;
        }
        else{
            //нулі на потенційно порожні
            for(int i=0;i<8-hex_length;i++){
                part.at(i)='0';
            }
            //дописуються з hex що там лишилося
            for (int j=0; j<hex_length; j++) {
                part.at(8-hex_length+j) = hex_string.at(j);
            }
            hex_length=0;

        }
        //конвертувати ці 8 символів у число типу uint32_t
        uint32_t value = stoul(part, nullptr, 16);
        //записати його в digits[i]
        digits[count]=value;
        count++;
    }
}

void long_add(uint64_t A[], uint64_t B[], uint64_t C[], int& carry, int count){
    carry=0;   
    uint64_t temp; 
    
    for(int i=0;i<=count-1;i++){
        temp=A[i]+B[i]+carry;
        //cout<<"A[i] "<<A[i]<<endl;
        //cout<<"B[i] "<<B[i]<<endl;
        //cout<<"після додавання "<<temp<<endl;
        C[i]=temp&(4294967296-1);
        C[i] = temp & 0xFFFFFFFF;
        //cout<<"c[i] "<<C[i]<<endl;
        carry=temp >> 32;
        //cout<<"перенос "<<carry<<endl;
        //cout<<"-----------------"<<endl;
        
    }
}
void long_sub(uint64_t A[], uint64_t B[], uint32_t D[],int count){
    int borrow=0;
    int32_t temp;
    for(int i=0; i<=count-1; i++){
        //cout<<"A[i] "<<A[i]<<endl;
        //cout<<"B[i] "<<B[i]<<endl;
        
        if(A[i]<=B[i]){
            D[i]=(4294967296)+A[i]-B[i]-borrow;
            borrow=1;
            //cout<<"мав бути -"<<endl;
        }
        else{
            D[i]=A[i]-B[i]-borrow;
            borrow=0;
            //cout<<"все норм"<<endl;
        }
        //cout<<"borrow "<<borrow<<endl;
        //cout<<"D[i] "<<D[i]<<endl;
        //cout<<"---------------------"<<endl;
    }

}


int main(){
    
    string a = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    string b = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string a = "123456789abcdef9";
    //string b = "abcdef9012345678";
    //string a = "abcdef90";
    //string b = "12345678";
    int negative=0;
    if(a<b){
        swap(a,b);
        negative=1;
    }

    uint64_t A[64];
    int count_a=0;

    uint64_t B[64];
    int count_b=0;
    
    hex_32(a, A, count_a);
    hex_32(b, B, count_b);

    /*for (int j = 0; j < count_a; j++) {
        cout << hex << A[j] << endl;
    }
    cout<<"-------------------------"<<endl;
    for (int j = 0; j < count_b; j++) {
        cout << hex << B[j] << endl;
    }*/

    int carry;
    uint64_t C[64];
    long_add(A, B, C, carry, count_a);

    //cout<<"carry "<<carry<<endl;
    //треба придумати як передбачати розмір масиву
    cout<<"add ";
    if(carry!=0) cout<<carry;
    for (int j = count_a-1; j >=0; j--){
        cout << hex << C[j]<<"";
    }
    
    cout<<endl;
   

    

    uint32_t D[32];
    long_sub(A, B, D, count_a);
    cout<<"sub ";
    if(negative=1) cout<<"-";
    for (int j = count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << D[j]<<"";
    }
    cout<<endl;


    
    
    return 0;
} 

