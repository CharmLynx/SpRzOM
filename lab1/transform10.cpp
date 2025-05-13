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
        
        //if(A[i]<=B[i])
        if (A[i] < B[i] + borrow){
            D[i]=(4294967296)+A[i]-B[i]-borrow;
            borrow=1;
            //cout<<"мав бути -"<<endl;
        }
        else{
            D[i]=A[i]-B[i]-borrow;
            borrow=0;
            //cout<<"все норм"<<endl;
        }
        D[i] &= 0xFFFFFFFF;
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
int bit_length_pro(uint64_t A[]) {
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            for (int j = 31; j >= 0; j--) {
                if ((A[i] >> j) & 1) {
                    return i * 32 + j + 1;
                }
            }
        }
    }
    return 0;
}
/*void long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]){
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
        /*if(long_compare(R, H, count_r) < 0){
            //cout<<" воно в if "<<endl;
            t=t-1;
            long_shift_to_high(B, H, t-k, count_b);
        }
        length(R, count_r);
        /*cout<<"count_r "<<count_r<<endl;
        //R = R-H;
        cout<<"R ";
        for (int j = count_r-1; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << R[j]<<"";
        }
        cout<<endl;*/
        /*long_sub(R, H, R, count_r);

        /*for (int j = count_r-1+t-k; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << R[j]<<"";
        }
        cout<<endl;*/
        //Q=Q+2^(t-k);
        /*bitpos = t - k;
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

    //}
//}
void bit_shift_to_high(uint64_t src[], uint64_t dst[], int shift_bits) {
    for (int i = 0; i < 64; i++) dst[i] = 0;

    int word_shift = shift_bits / 32;
    int bit_shift = shift_bits % 32;

    for (int i = 63; i >= 0; i--) {
        if (i - word_shift < 0) break;

        dst[i] |= (src[i - word_shift] << bit_shift) & 0xFFFFFFFF;

        if (bit_shift != 0 && i - word_shift - 1 >= 0) {
            dst[i] |= (src[i - word_shift - 1] >> (32 - bit_shift)) & 0xFFFFFFFF;
        }
    }
}
void long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]) {
    uint64_t H[64] = {0};
    for (int j = 0; j < count_a; j++){
        R[j] = A[j];
    }

    int k = bit_length_pro(B);
    int count_r = bit_length_pro(R);

    while (long_compare(R, B, max(count_a, count_b)) >= 0) {
        int t = bit_length_pro(R);
        int bitpos = t - k;
        if (bitpos < 0) break;

        bit_shift_to_high(B, H, bitpos);

        if (long_compare(R, H, 64) < 0) {
            bitpos--;
            if (bitpos < 0) break;
            bit_shift_to_high(B, H, bitpos);
            if (long_compare(R, H, 64) < 0) break;
        }

        long_sub(R, H, R, 64);

        if (bitpos / 32 < 64)
            Q[bitpos / 32] |= (1ULL << (bitpos % 32));

        count_r = bit_length_pro(R);
    }
}

int bit_length_proо(uint64_t A[]) {
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

    int bit_len = bit_length_proо(P);
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

        //count_k=bit_length_proо(K);
        count_a =2*bit_length(A);
        count_k=count_a;
        //count_a = count_k;

        /*cout<<endl;
        cout<<"===== A=A*A"<<endl;
        for (int j = count_k-1; j >=0; j--){
            cout <<setfill('0') << setw(8)<< hex << K[j]<<"";
        }
        //cout<<endl;
        //cout<<bit_length_proо(K)<<endl;
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

void long_div2(uint64_t A[], int count) {
    uint32_t carry = 0;
    for (int i = count - 1; i >= 0; i--) {
        uint32_t current = static_cast<uint32_t>(A[i]);
        A[i] = ((current >> 1) | (static_cast<uint64_t>(carry) << 31)) & 0xFFFFFFFF;
        carry = current & 1;
    }
}


void long_mul2(uint64_t A[], int& count) {
    uint64_t carry = 0;

    for (int i = 0; i < count; ++i) {
        uint64_t current = A[i];
        uint64_t new_carry = (current >> 31) & 1; // старший біт поточного 32-бітового значення
        A[i] = ((current << 1) | carry) & 0xFFFFFFFF; // зсув + перенос + маскування
        carry = new_carry;
    }

    // якщо після останнього елементу ще є перенос — додаємо його
    if (carry != 0) {
        A[count] = carry;
        count++;
    }
}
void long_gcd(uint64_t A[], uint64_t B[], uint64_t D[]){
    int count_a = bit_length(A);
    int count_b = bit_length(B);
    string d = "1";
    int count_d=0;
    hex_32(d, D, count_d);
    uint64_t T[64] ={0};
    while(((A[0] & 1) == 0)&&((B[0] & 1) == 0)){
        //a=a/2;
        count_a=bit_length(A);
        long_div2(A, count_a);
        /*cout<<"a/2 ";
        for (int j = count_a-1; j >=0; j--) {
            //cout << hex << A[j];
            cout << setw(8) << hex << A[j];
        }
        cout <<endl;*/
        //b=b/2;
        count_b=bit_length(B);
        long_div2(B,count_b);
        /*cout<<"b/2 ";
        for (int j = count_b-1; j >=0; j--) {
            //cout << hex << A[j];
            cout << setw(8) << hex << B[j];
        }
        cout <<endl;*/
        
        //d=d*2;
        count_d=bit_length(D);
        long_mul2(D, count_d);
        /*cout<<"d*2 ";
        for (int j = count_d-1; j >=0; j--) {
            //cout << hex << A[j];
            cout << setw(8) << hex << D[j];
        }
        cout <<endl;*/
        
    }
    /*cout<<"перевірка після 1 циклу "<<endl;
    cout<<"A ";
    count_a=bit_length(A);
    for (int j = count_a-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << A[j];
    }
    cout<<endl;
    cout<<"B ";
    count_b=bit_length(B);
    for (int j = count_b-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << B[j];
    }
    cout<<endl;
    cout<<"D ";
    count_d=bit_length(D);
    for (int j = count_d-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << D[j];
    }
    cout<<endl;
    cout<<endl;*/



    while((A[0] & 1)==0){
        //a=a/2;
        count_a=bit_length(A);
        long_div2(A, count_a);
    }
    /*cout<<"перевірка після 2 циклу "<<endl;
    cout<<"A ";
    count_a=bit_length(A);
    for (int j = count_a-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << A[j];
    }
    cout<<endl;
    cout<<endl;*/


    while(bit_length(B) != 0){
        while((B[0] & 1)==0){
            //b=b/2;
            count_b=bit_length(B);
            long_div2(B, count_b);
        }
        //(a,b)=(min{a,b}, abs(a-b))
        count_a=bit_length(A);
        count_b=bit_length(B);
        if (long_compare(A, B, max( count_a, count_b)) <= 0) {
            // A ≤ B
            /*cout<<"A<B не міняєм"<<endl;
            cout<<"все те ж А те якe min: ";
            for (int j = count_a-1; j >=0; j--) {
                //cout << hex << A[j];
                cout << setw(8) << hex << A[j];
            }
            cout<<endl;*/
            
            count_a = bit_length(A);
            count_b = bit_length(B);

            //for(int i=0; i<64; i++) T[i] = 0;
            long_sub(B, A, B, count_b);  // B = B - A
            /*for (int j = bit_length(T)-1; j >=0; j--) {
                //cout << hex << A[j];
                //cout << setw(8) << hex << D[j];
                B[j]=T[j];
            }*/
            /*cout<<"В різниця: ";
            for (int j = count_b-1; j >=0; j--) {
                //cout << hex << A[j];
                cout << setw(8) << hex << B[j];
            }
            cout <<endl;
            cout<<endl;*/
            
            count_a = bit_length(A);
            count_b = bit_length(B);

            // A залишається без змін
        } else {
            // A > B
            //swap(A, B);
            for (int i = 0; i < max(count_a, count_b); i++) {
                uint64_t temp = A[i];
                A[i] = B[i];
                B[i] = temp;
            }
            swap(count_a, count_b);
            /*cout<<"відбувся свап"<<endl;
            cout<<"нове А те якe min: ";
            for (int j = count_a-1; j >=0; j--) {
                //cout << hex << A[j];
                cout << setw(8) << hex << A[j];
            }
            cout <<endl;*/
            count_b=bit_length(B);
            //for(int i=0; i<64; i++) T[i] = 0;
            long_sub(B, A, T, count_b);  // A = A - B
            /*for (int j = bit_length(T)-1; j >=0; j--) {
                //cout << hex << A[j];
                //cout << setw(8) << hex << D[j];
                B[j]=T[j];
            }*/
            count_b=bit_length(B);
            /*cout<<"нове В різниця: ";
            for (int j = count_b-1; j >=0; j--) {
                //cout << hex << A[j];
                cout << setw(8) << hex << B[j];
            }*/
        //cout <<endl;
        //cout<<endl;
        }
    
    count_a=bit_length(A);
    count_b=bit_length(B);
        

    }
    //cout<<"вийшло з циклу"<<endl;
    //d=d*a;
    /*count_d=bit_length(D);
    cout<<"d до множення ";
    for (int j = count_d-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << D[j];
    }
    cout<<endl;
    cout<<"A ";
    for (int j = count_a-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << A[j];
    }
    cout<<endl;*/
    for(int i=0; i<64; i++) T[i] = 0;
    long_mul(D, A, T, count_a);
    for(int i=0; i<64; i++) D[i] = 0;
    for (int j = bit_length(T)-1; j >=0; j--) {
        //cout << hex << A[j];
        //cout << setw(8) << hex << D[j];
        D[j]=T[j];
    }

}
void long_lcm(uint64_t M[], uint64_t D[], uint64_t Q[]){
    uint64_t R[64] = {0};
    int count_m=bit_length(M);
    int count_d=bit_length(D);
    long_div(M, D,count_m, count_d, Q, R);
}

void kill_last_digits(uint64_t src[], int src_len, int digits_to_kill, uint64_t dest[]) {
    int j = 0;
    for (int i = digits_to_kill; i < src_len; i++) {
        dest[j++] = src[i];
    }
    while (j < 64) {
        dest[j++] = 0;
    }
}

void compute_mu(uint64_t n[], int count_n, uint64_t mu[], int& count_mu) {
    uint64_t beta2k[64] = {0};
    beta2k[2 * count_n] = 1; // β^{2k} — один біт у 2k-му слові

    uint64_t rem[64] = {0};
    long_div(beta2k, n, 2 * count_n + 1, count_n, mu, rem);

    count_mu = bit_length_pro(mu);  // точна кількість бітів у μ
}
void barret_reduction(uint64_t A[], uint64_t N[], uint64_t r[], uint64_t MU[]){
    uint64_t q1[64]={0};
    uint64_t q2[64]={0};
    uint64_t q3[64]={0};
    uint64_t qn[64]={0};
    int count_a =bit_length(A);
    int count_n = bit_length(N);
    kill_last_digits(A,count_a, count_n-1, q1);
    long_mul(q1, MU, q2, bit_length(q1));
    kill_last_digits(q2,bit_length(q2), count_n+1, q3);
    
    long_mul(q3, N, qn, bit_length(q3));
    long_sub(A, qn, r, count_a);
    while(long_compare(r, N, max(bit_length(r), count_n))>=0){
        long_sub(r, N,r,bit_length(r));
    }
}
void long_mod_add(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t tmp[64] = {0};
    int carry = 0;
    long_add(A, B, tmp, carry, max(bit_length(A), bit_length(B)));
    barret_reduction(tmp, N, R, MU);
}
void long_mod_sub(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[], int negative) {
    uint64_t tmp[64] = {0};
    long_sub(A, B, tmp, max(bit_length(A), bit_length(B)));
    if(negative==1){    
        barret_reduction(tmp, N, R, MU);
        long_sub(N, R, R, bit_length(N));
    }
    else{
        barret_reduction(tmp, N, R, MU);
    }
}

void long_mod_mul(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t A_mod[64] = {0};
    uint64_t B_mod[64] = {0};
    uint64_t product[64] = {0};

    
    barret_reduction(A, N, A_mod, MU);
    barret_reduction(B, N, B_mod, MU);
    if((bit_length(A_mod)==0)||(bit_length(A_mod)==0)){
        cout<<"0"<<endl;
    }
    else{
        int count = max(bit_length(A_mod), bit_length(B_mod));
        long_mul(A_mod, B_mod, product, count );
        barret_reduction(product, N, R, MU);
        
    }
}
/*void long_mod_pow(uint64_t A[], uint64_t E[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t base[1024] = {0};
    uint64_t result[1024] = {0};
    uint64_t tmp[1024] = {0};

    // result = 1
    result[0] = 1;

    // base = A mod N
    barret_reduction(A, N, base, MU);

    uint64_t bitlen = bit_length_pro(E);
    uint64_t len_tmp = 0;

    for (int i = 0; i < bitlen; i++) {
        if ((E[i / 32] >> (i % 32)) & 1) {
            // result = (result * base) mod N
            long_mod_mul(result, base, N, MU, tmp);
            for (int j = 0; j < 64; j++) result[j] = tmp[j];
        }

        // base = (base * base) mod N
        long_mod_mul(base, base, N, MU, tmp);
        len_tmp = bit_length(tmp);
        for (int j = 0; j < len_tmp; j++) base[j] = tmp[j];
    }

    for (int j = 0; j < 64; j++) R[j] = result[j];
}
*/
void long_mod_pow(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t R[]) {
    // 1. Ініціалізація C := 1
    string one = "1";
    uint64_t C[64] = {0};
    int count_c = 0;
    hex_32(one, C, count_c);

    // 2. Обчислити μ = β^(2k) / N
    int count_n = 0;
    length(N, count_n);

    uint64_t MU[64] = {0};
    compute_mu(N, count_n, MU, count_n);  // MU := floor(β^(2k) / N)

    // 3. Копія основи
    uint64_t A_mod[64] = {0};
    barret_reduction(A, N, A_mod, MU);

    // 4. Копія показника
    int bit_len = bit_length_proо(B);

    // 5. Основний цикл
    for (int i = 0; i < bit_len; i++) {
        if ((B[i / 32] >> (i % 32)) & 1) {
            // C := BarrettReduction(C * A, N, μ)
            uint64_t temp_mul[128] = {0};
            int count_c_now = 0, count_a_now = 0;
            length(C, count_c_now);
            length(A_mod, count_a_now);
            long_mul(C, A_mod, temp_mul, std::max(count_c_now, count_a_now));
            barret_reduction(temp_mul, N, C, MU);
        }

        // A := BarrettReduction(A * A, N, μ)
        uint64_t temp_square[128] = {0};
        int count_a_now = 0;
        length(A_mod, count_a_now);
        long_mul(A_mod, A_mod, temp_square, count_a_now);
        barret_reduction(temp_square, N, A_mod, MU);
    }

    // 6. Результат
    for (int i = 0; i < 64; i++) R[i] = C[i];
}



    







int main(){

    //string b = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    //string a = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string b = "123456789abcdef9";
    //string a = "abcdef9012345678";
    //string b = "123";
    //string a = "369";
    //string a = "fbcdef90";
    //string b = "12345678";
    string a = "12345678123456781234567812345699";
    //string b = "12345678123456781234567812345679";
    string b = "145afcd961278435aaacfdba12345678";
    //string a = "4";
    //string b = "1";
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


    uint64_t S[64];
    long_sub(A, B, S, count_a);
    cout<<"sub ";
    if(negative==1) cout<<"-";
    for (int j = count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << S[j]<<"";
    }
    cout<<endl;
    cout<<endl;
    if (negative==1) swap(A, B);

    //множення
    uint64_t M[128]={0};
    long_mul(A, B, M, count_a);
    cout<<"mull"<<endl;

    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << M[j]<<"";
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
    uint64_t R[64] = {0};
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
    cout<<endl;

    //перезаписались тому
    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    hex_32(a,A,count_a);
    hex_32(b,B,count_b);
    

    
    //gcd
    uint64_t D[64]={0};
    long_gcd(A, B, D);
    int count_d=bit_length(D);
    cout<<"gcd ";
    for (int j = count_d-1; j >=0; j--) {
        //cout << hex << A[j];
        cout << setw(8) << hex << D[j];
    }
    cout<<endl;
    cout<<endl;
    

    //перезаписались тому
    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    hex_32(a,A,count_a);
    hex_32(b,B,count_b);

    //lcm
    uint64_t Ql[64] = {0};
    long_lcm(M, D, Ql);
    cout<<"lcm ";
    for (int j = bit_length(Ql)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << Ql[j];
    }
    cout<<endl;
    cout<<endl;

    //перезаписались тому
    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    hex_32(a,A,count_a);
    hex_32(b,B,count_b);

    
    
    //мю
    string n = "1234567812345678";
    uint64_t N[64] = {0};      
    int count_n=0;
    hex_32(n, N, count_n);
    uint64_t MU[128] = {0};            
    int count_mu = 0;
    compute_mu(N, count_n, MU, count_mu);


    //barret reduction
    /*
    uint64_t r[64]={0};
    barret_reduction(A, N, r, MU);


    cout<<"r "<<bit_length(r)<<endl;
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    */


    //+ mod
    uint64_t r[64]={0};
    long_mod_add(A, B, N, MU, r);
    cout<<"(a+b) mod n "<<endl;
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //-mod
    if(a<b){
        swap(a,b);
        negative=1;
    }
    long_mod_sub(A, B, N, MU, r, negative);
    if (negative==1){
        swap(A, B);
        swap(count_a, count_b);
    }
    cout<<"(a-b) mod n "<<endl;
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //*mod
    cout<<"(a*b) mod n "<<endl;
    long_mod_mul(A, B, N, MU, r);
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //^2mod
    cout<<"(a*a) mod n "<<endl;
    long_mod_mul(A, A, N, MU, r);
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //перезаписались тому
    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    hex_32(a,A,count_a);
    hex_32(b,B,count_b);

    //^n mod
    long_mod_pow(A, B, N, r);
    cout<<"(a^b) mod n "<<endl;
    for (int j = bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;

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