// примери за асемблерен код
// намиране на сумата на елементите на масив 
#include <iostream>
using namespace std;
void View(int *a, int L) {
  for (int i = L - 1; 0 <= i; --i) cout << a[i] << "  ";
  cout << endl;
}
int Sum(int *a, int L) {
  int s = 0;
  for (int i = L - 1; 0 <= i; --i) s += a[i];
  return s;
}
int SumAsm(int *a, int L) {
  __asm {
    push esi
        xor eax, eax
        mov ecx, L
        mov esi, a
      NEXT : add eax, [esi + ecx * 4 - 4]
             loop NEXT
    pop esi
  }
}
int main(){
  int ar[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const int len = sizeof(ar) / sizeof(ar[0]);
  View(ar, len);
  cout << "Sum: " << Sum(ar, len) << endl;
  int sum;
  __asm {
    push eax
        xor eax, eax
        mov ecx, len
      NEXT : add eax, ar[ecx*4-4]
             loop NEXT
        mov sum, eax
    pop eax
  }
  cout << "Sum: " << sum << endl;
  cout << "Sum: " << SumAsm(ar, len) << endl;

  system("pause");
  return 0;
}