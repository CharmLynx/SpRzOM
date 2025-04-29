#include <iostream>
#include <cmath>

using namespace std;

int main(){
    long int a=123123123123123123;
    int b=458458458;

    int n;
    n=log(1203123212321)/log(4294967296)+1;
    cout<<n<<endl;

    int t=0;
    t=a%((4294967296));
    cout<<t<<endl;
    t=(a-t)%((4294967296)^(n-2));
    cout<<t;

    return 0;
}