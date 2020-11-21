// пример за асемблерен код
// намиране на минимума на елементите на масив
// и броя на минималните елементи в масива
#include <iostream>
using namespace std;
int main(){
  int ar[] = { 5, -4, 6, 3, -4, -1, -4, 20 };
  const int len = sizeof(ar) / sizeof(ar[0]);
  int min = ar[len - 1], num = 1;
  for (int i = len - 2; 0 <= i; --i)
    if (ar[i] < min) { min = ar[i]; num = 1; }
    else if (ar[i] == min) ++num;
  cout << "Minimum: " << min << "\nCount: " << num << endl;
  __asm {
    pushad
        mov ecx, len
        dec ecx
        mov eax, ar[ecx*4]
        mov esi, 1
      Next :  cmp ar[ecx*4-4], eax
              jg Step
              je Equal
              mov eax, ar[ecx*4-4]
              /*
              mov esi, 1
              jmp Step
              */
              xor esi, esi
        Equal :
              inc esi
      Step :  loop Next
        mov min, eax
        mov num, esi
    popad
  }
  cout << "Minimum: " << min << "\nCount: " << num << endl;

  system("pause");
  return 0;
}