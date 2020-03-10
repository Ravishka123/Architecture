#include <iostream>
#include <random>
#include <chrono>
#define M 1000000
#include <intrin.h>

float ranfloats[M];

using namespace std;

inline float sqrt_new(float n)
{
    __asm {
        fld n
            fsqrt
    }
}

int main()
{

    double  number = 6.5;
    float temperary, sqroot;
    for (int i = 0; i < 10; i++) {
        double time1 = __rdtsc();
        sqroot = number / 2;
        temperary = 0;


        while (sqroot != temperary) {

            temperary = sqroot;


            sqroot = (number / temperary + temperary) / 2;
        }
        double time2 = __rdtsc();
        //cout << "The square root of user " << number << " is " << sqroot << '\n';
        cout << "Time Taken in user is  " << time2 - time1 << '\n';


        double time3 = __rdtsc();
        float value = sqrt(number);
        double time4 = __rdtsc();

       // cout << "The square root of built-in " << number << " is " << value << '\n';
        cout << "Time Taken in built in is  " << time4 - time3 << '\n';






        double time5 = __rdtsc();
        float value1 = sqrt_new(number);
        double time6 = __rdtsc();

        cout << "time taken for assembly sqrt is "  << time6 - time5 << endl << endl ;
        cout << "moving next" << endl;

    }
system("pause");
return 0;
}
