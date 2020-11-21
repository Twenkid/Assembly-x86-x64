// примери за асемблерен код
// намиране на броя на единиците в кода на цяло число 
#include <iostream>
using namespace std;
int main(){
  unsigned int n;
  cout << "n (>= 0) : ";
  cin >> n;
  
  int cn = 0, t = n;
  for (int i = 1; i <= 32; ++i)
  {
    cn += t % 2;
    t /= 2;
  }
  cout << "Count: " << cn << endl;
  __asm { // с изместване циклично през флага CF
    push eax
    push ecx
    push esi
        mov ecx, 32
        mov eax, n
        xor esi, esi // esi - брой на единиците в n
      Cyc : rcr eax, 1
            adc esi, 0
            loop Cyc
        mov cn, esi
    pop esi
    pop ecx
    pop eax
  }
  cout << "Count: " << cn << endl;
  
  cn = 0;
  for (int i = 0; i < 32; ++i)
    cn += (n >> i) % 2;
  cout << "Count: " << cn << endl;
  __asm { // с директно извличане на разряда и записванве във флага CF
    push ecx
    push esi
        xor ecx, ecx
        xor esi, esi // esi - брой на единиците в n
       Cyc2 : bt n, ecx
              adc esi, 0
              inc ecx
              cmp ecx, 32
              jne Cyc2
        mov cn, esi
    pop esi
    pop ecx
  }
  cout << "Count: " << cn << endl;
  
  // Следващият алгоритъм няма аналог на високо ниво на програмиране,
  // защото командата bsf няма съответен аналог.
  __asm { // с повторение на цикъла толкова пъти, колкото единици има
    pushad
        xor esi, esi // esi - брой на единиците в n
        mov eax, n
      Cyc3 :  bsf ecx, eax
              jz END
              inc esi
              btc eax, ecx
              jmp Cyc3
      END :
        mov cn, esi
    popad
  }
  cout << "Count: " << cn << endl;

  system("pause");
  return 0;
}