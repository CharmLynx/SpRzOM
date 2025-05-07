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
void t32_hex(uint64_t A[], string& a, int count){
    for(int i =count;i>=0;i--){
        stringstream ss;
        ss << setfill('0') << setw(8) << hex << A[i];
        a += ss.str();
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
void long_sub(uint64_t A[], uint64_t B[], uint64_t D[],int count){
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
    uint64_t E[128] ={0};
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
        //cout<<endl;
        //F=F+shifted;//а тут склепати багаторозрядне додавання би, інакше воно недодасться
        int carry_mm=0;
        long_add(F, shifted, F, carry_mm, count_sh);
        //cout<<"-------------"<<endl;
    }
}

int long_compare(uint64_t A[], uint64_t B[], int count) {
    for (int i = count - 1; i >= 0; i--) {
        if (A[i] < B[i]){
            return -1;
            break;
        }
        if (A[i] > B[i]){
            return 1;
            break;
        }
    }
    return 0;
}

void length(uint64_t A[], int& count){
    count=0;
    for(int i=0;i<64;i++){//максимальна довжина числа 2048 біт,а отже у масиву - 64
        if(A[i]!=0){
            count++;
        }
        else{
            break;
        }
    }

}
int bit_length(uint64_t A[]) {
    int count=0;
    /*for(int i=0;i<64;i++){//максимальна довжина числа 2048 біт,а отже у масиву - 64
        if(A[i]!=0){
            count++;
        }
        else{
            return count;
        }
    }*/
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            return i + 1;
        }
    }
    return 0;
}

void long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]){
    uint64_t H[64] = {0};
    for (int j = 0; j < count_a; j++) {
        R[j]=A[j];
    }
    int count_r = 0;
    length(R, count_r);
    int k = bit_length(B);
    int t;
    int bitpos;
    int word_index;
    int bit_index;
    while((long_compare(R, B, count_r))>=0){
        //cout<<"увійшло в цикл"<<endl;
        t=bit_length(R);
        //cout<<"t "<<t<<endl;
        long_shift_to_high(B, H, t-k, count_b);
        /*cout<<"H ";
        for (int j = count_a-1+t-k; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << H[j]<<"";
        }
        cout<<endl;*/
        if(long_compare(R, H, count_r) < 0){
            //cout<<" воно в if "<<endl;
            t=t-1;
            long_shift_to_high(B, H, t-k, count_b);
        }
        length(R, count_r); 
        //count_r=count_r;
        //cout<<"count_r "<<count_r<<endl;
        //R = R-H;
        //cout<<"R ";
        /*for (int j = count_r-1; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << R[j]<<"";
        }*/
        //cout<<endl;
        long_sub(R, H, R, count_r);
        /*for (int j = count_r-1+t-k; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << R[j]<<"";
        }
        cout<<endl;*/
        //Q=Q+2^(t-k);
        bitpos = t - k;
        word_index = bitpos / 32;
        bit_index  = bitpos % 32;
        uint64_t carry_q = 1ULL << bit_index;

        for (int i = word_index; i < 64 && carry_q != 0; i++) {
            uint64_t temp = Q[i] + carry_q;
            carry_q = (temp < Q[i]) ? 1 : 0; // якщо було переповнення
            Q[i] = temp;
        }
        /*cout << "Q: ";
        for (int j = bit_length(Q)-1; j >= 0; j--) {
            cout <<setfill('0') << setw(8)<< hex << Q[j];
        }
        cout << endl;
        cout<<"-----------------------------"<<endl;*/

    }
}
int bit_length_pro(uint64_t A[]) {
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            for (int j = 63; j >= 0; j--) {
                if ((A[i] >> j) & 1) {
                    return i * 64 + j + 1;
                }
            }
        }
    }
    return 0;
}

void long_power(uint64_t A[], uint64_t P[], uint64_t J[], int& count_a, int count_p){
    string j = "1";
    
    int count_j=0;
    hex_32(j, J,count_j);
    uint64_t H[128]={0};
    
    uint64_t K[128]={0};
    int count_k;

    int bit_len = bit_length_pro(P);
    for (int i = 0; i < bit_len; i++) {
        if ((P[i / 32] >> (i % 32)) & 1) {
            //cout<<"тут один"<<endl;
            /*cout<<"===== J"<<endl;
            for (int j = bit_length(J)-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << J[j]<<"";
            }
            cout<<"====="<<endl;*/
            for(int i=0; i<64; i++) H[i] = 0;
            /*cout<<"===== A"<<endl;
            for (int j = count_a-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << A[j]<<"";
            }
            cout<<endl;
            cout<<"aaaaaaaaaaaaaaa "<<count_a<<endl;*/
            //long_mul(J, A, H, count_a);
            long_mul(A, J, H, bit_length(A));
            /*cout<<"hhhhhhhhhhh "<<bit_length(H)<<endl;
            cout<<bit_length(H)<<endl;
            cout<<"===== J=A*J"<<endl;
            for (int j = 2*bit_length(H)-1; j >=0; j--){
                cout <<setfill('0') << setw(8)<< hex << H[j]<<"";
            }*/
            for(int i=0; i<128; i++) J[i] = 0;    
            for(int i=0; i<=2*count_a-1; i++){
                J[i]=H[i];
                //cout<<"є"<<endl;
            }
            //cout<<endl;
            //cout<<"====="<<endl;
        }
        for(int i=0; i<128; i++) K[i] = 0;
        long_mul(A, A, K, count_a);

        //count_k=bit_length_pro(K);
        count_a =2*bit_length(A);
        count_k=count_a;
        //count_a = count_k;

        /*cout<<endl;
        cout<<"===== A=A*A"<<endl;
        for (int j = count_k-1; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << K[j]<<"";
        }
        //cout<<endl;
        //cout<<bit_length_pro(K)<<endl;
        cout<<"====="<<endl;*/
        for(int i=0; i<64; i++) A[i] = 0;
        //cout<<"занулили"<<endl;
        
        //cout<<count_k<<endl;
        for(int i=0; i<count_k; i++){
            A[i]=K[i];
            //cout<<"є"<<endl;
        }
        count_a=count_k;
        //cout<<"----------------------------------------"<<endl;
    }
    //cout<<endl;
}

int main(){

    //string a ="fffffffe26019bbe3021d17424135fe8cfe865df3c00aa342bfe30aacef58e91266dce2714434c546b9932a0926496b5db81da8eaf31a899151348d12b347fa6d132f5e8db99007fbff5063f1452c4a3760f426863b4a7492a7d2214acffd25b90b635a1723e6b5e849b556d809a5d9bbcd9a72e7047b9419199327eb80b4d4af130a556f430e51723d999f39ebc9e81b48f3cf120dec7f22ef495613393a531bcc57d18f6f85f0fed5c74c84346b749a8b0f78b0715ff547c120b7a0bb76248c828042070e6b57334b29526aa11c61a33eb3cc6a1f461e08116f07767773375c5b474253f4b9291e4d2281f171644e80198cdf86cac333f394abd21c1c8fc04";

    //string a = "fffffffe26019bbe3021d17424135fe8cfe865df3c00aa342bfe30aacef58e91266dce2714434c546b9932a0926496b5db81da8eaf31a899151348d12b347fa6d132f5e8db99007fbff5063f1452c4a3760f426863b4a7492a7d2214acffd25b90b635a1723e6b5e849b556d809a5d9bbcd9a72e7047b9419199327eb80b4d4af130a556f430e51723d999f39ebc9e81b48f3cf120dec7f22ef495613393a531bcc57d18f6f85f0fed5c74c84346b749a8b0f78b0715ff547c120b7a0bb76248c828042070e6b57334b29526aa11c61a33eb3cc6a1f461e08116f07767773375c5b474253f4b9291e4d2281f171644e80198cdf86cac333f394abd21c1c8fc04";
    //string b = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";

    string b = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    string a = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string b = "123456789abcdef9";
    //string a = "abcdef9012345678";
    //string a = "fbcdef90";
    //string b = "12345678";
    //string a = "12345678123456781234567812345679";
    //string b = "12345678123456781234567812345679";
    //string b = "145afcd961278435aaacfdba12345678";
    //string a = "1";
    //string b = "0";
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
    string c;
    
    long_add(A, B, C, carry, count_a);

    cout<<"add ";
    if(carry!=0){
        for (int j = count_a; j >=0; j--){
            cout << hex << C[j]<<"";
        }
        //t32_hex(C, c, count_a);
        //cout<<c<<endl;
    }
    else{
        for (int j = count_a-1; j >=0; j--){
            cout << hex << C[j]<<"";
        }
        cout<<endl;
        //t32_hex(C, c, count_a-1);
        //cout<<c<<endl;
    }
    
    cout<<endl;
    cout<<endl;


    uint64_t D[64];
    long_sub(A, B, D, count_a);
    cout<<"sub ";
    if(negative==1) cout<<"-";
    for (int j = count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << D[j]<<"";
    }
    cout<<endl;
    cout<<endl;
    if (negative==1) swap(A, B);

    //множення
    uint64_t F[128]={0};
    long_mul(A, B, F, count_a);
    cout<<"mull"<<endl;

    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << F[j]<<"";
    }
    cout<<endl;
    cout<<endl;

    //квадрат
    uint64_t G[128]={0};
    long_mul(A, A, G, count_a);
    cout<<"^2"<<endl;
    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << G[j]<<"";
    }
    cout<<endl;
    cout<<endl;

    //ділення
    uint64_t R[64]={0};
    uint64_t Q[64] = {0};
    long_div(A, B, count_a, count_b, Q, R);


    
    cout<<"div "<<endl;
    cout << "q ";
    for (int j = bit_length(Q)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << Q[j];
    }
    cout << endl;
    cout << "r ";
    for (int j = count_a - 1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << R[j];
    }
    cout << endl;
    cout<<endl;

    //багаторозрядний степінь
    string p = "3";
    int count_p;
    uint64_t P[64] = {0};
    hex_32(p, P, count_p);
    uint64_t J[128] ={0};
    int count_a1 =count_a;
    long_power(A, P, J, count_a1, count_p);
    cout<<"a^p "<<endl;
    
    for (int j = bit_length(J)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << J[j];
    }
    cout << endl;

    


    return 0;
} 

/*#include <chrono>
using namespace std;
using namespace std::chrono;

auto start = high_resolution_clock::now();

// 🔁 Виклик функції
long_mul(A, B, C, count);

auto end = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(end - start);
cout << "Час виконання: " << duration.count() << " мкс" << endl;
*/