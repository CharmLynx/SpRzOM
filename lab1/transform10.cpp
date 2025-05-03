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
    int k=0;
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
        k++;
    }
    if(carry!=0)C[k]=carry;
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
void long_mul_one(uint64_t A[], uint64_t E[], uint64_t& carry, int count, uint64_t e){
    uint64_t temp; 
    int k=0;   
    //cout<<"B "<<e<<endl;
    for(int i=0; i<=count-1;i++){
        //cout<<"carry "<<carry<<endl;
        //cout<<"A[i] "<<A[i]<<endl;
        temp=A[i]*e+carry;
        E[i]=temp&(4294967296-1);
        carry = temp >> 32;
        //cout<<"E[i] "<<E[i]<<endl;
        //cout<<"---================--------"<<endl;
        k++;
    }
    //cout<<"carry "<<carry<<endl;
    if(carry!=0)E[k]=carry;
    //cout<<"k "<<k<<endl;
}

void long_shift_to_high(uint64_t A[], uint64_t L[], int i, int count){
    uint64_t n = 00000000;
    for(int j=0; j<i; j++){
        L[j]=n;
    }
    for(int j=i;j<=count+i;j++){
        L[j]=A[j-i];
    }
}

void long_mul(uint64_t A[], uint64_t B[], uint64_t F[], int count_a){
    uint64_t shifted[128] = {0};    // зсунуте значення
    uint64_t E[64] ={0};
    int count_sh=0;
    int count_e;
    int carry_mm;
    uint64_t carry_m;
    int carry;
    for(int i=0; i<=count_a-1;i++){
        carry_m=0;
        carry_mm=0;
        long_mul_one(A, E, carry_m, count_a, B[i]);
        /*cout<<"A ";
        for (int j = count_a-1; j >=0; j--) {
            cout << hex << A[j];
        }
        cout<<endl;
        cout<<"B[i] "<<B[i]<<endl;*/
        if(carry_m!=0){
            /*for (int j = count_a; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << E[j];
            }
            cout<<endl;*/
            long_shift_to_high(E, shifted, i, count_a);
            /*for (int j = count_a+i; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << shifted[j]<<"";
            }*/
            long_add(F, shifted, F, carry_mm, count_a+i+1);
            /*cout<<endl;
            cout<<"result ";
            for (int j = count_a+i; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << F[j]<<"";
            }*/
        }
        else{
            /*for (int j = count_a-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << E[j];
            }*/
            long_shift_to_high(E, shifted, i, count_a-1);
            /*for (int j = count_a+i-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << shifted[j]<<"";
            }*/
            long_add(F, shifted, F, carry_mm, count_a+i);
            /*cout<<endl;
            cout<<"result ";
            for (int j = count_a+i-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << F[j]<<"";
            }*/
        }
        //cout<<endl;
        
        //count_e=0;
        //long_shift_to_high(E, shifted, i, count_e);
        /*for (int j = count_a-1; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << shifted[j]<<"";
        }*/
        cout<<endl;
        //F=F+shifted;//а тут склепати багаторозрядне додавання би, інакше воно недодасться
        int carry_mm=0;
        long_add(F, shifted, F, carry_mm, count_sh);
        //cout<<"-------------"<<endl;
    }
}



int main(){
    
    //string a = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    //string b = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string a = "123456789abcdef9";
    //string b = "abcdef9012345678";
    //string a = "fbcdef90";
    //string b = "12345678";
    string a = "12345678123456781234568712345678";
    string b = "145afcd961278435aaacfdba12345678";
    int negative=0;
    //це для віднімання
    if(a<b){
        swap(a,b);
        negative=1;
    }

    uint64_t A[64]={0};
    int count_a=0;

    uint64_t B[64]={0};
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

    cout<<"add ";
    if(carry!=0){
        for (int j = count_a; j >=0; j--){
            cout << hex << C[j]<<"";
        }
    }
    else{
        for (int j = count_a-1; j >=0; j--){
            cout << hex << C[j]<<"";
        }
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

    //множення
    uint64_t F[64]={0};
    long_mul(A, B, F, count_a);

    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << F[j]<<"";
    }
    



    
    return 0;
} 

