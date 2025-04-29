#include <iostream>

using namespace std;

int main(){
    int A[2] = {2, 1};
    int B[2] = {3, 4};
    int C[2];

    int carry = 0;
    int temp = 0;

    for(int i=0; i<2; i++){
        temp = A[i]+B[i]+carry;
        C[i]= temp & (2^16-1);
        carry = temp >> 16;
    }

    for(int i=1;i>=0;i--){
        cout<<C[i]<< " ";
    }
    int sum=0;
    /*for(int i=0;i<2;i++){
        sum = C[i]*(2^16)^
    }*/
   cout<<C[0]+C[1]*65536 <<endl;

}