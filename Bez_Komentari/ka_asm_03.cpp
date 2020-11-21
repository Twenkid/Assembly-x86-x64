// примери за асемблерен код
// намиране на максимума на три числа
/*

Условни преходи
J#

За #:

Първо, при стойност на флаг:
jc когато CF==1
jnc когато CF != 1

Второ, след cmp x, y
за x, y цели без знак преход при:
x<y   команда jb или jnae
x==y  команда je
x!=y  команда jne
x>y   команда ja или jnbe
x>=y  команда jae

Второ, след cmp x, y
за x, y в допълнителен код преход при:
x<y   команда jl или jnge
x==y  команда je
x!=y  команда jne
x>y   команда jg или jnle
x>=y  команда jge

*/

#include <iostream>
using namespace std;
int MaxAsm(int v, int * p, int & r) {
  __asm {
    push esi
        mov esi, p
        mov eax, [esi]
        cmp v, eax
        jng STEP1
        mov eax, v
      STEP1 :
        mov esi, r
        cmp eax, [esi]
        jge STEP2
        mov eax, [esi]
      STEP2 :
    pop esi
  }
}
int main(){
  int x, y, z, max;
  cout << "x y z: ";
  cin >> x >> y >> z;
  if (x < y) max = y;
  else max = x;
  if (max < z) max = z;
  cout << "Maximum: " << max << endl;
  max -= 12345;
  __asm {
    push eax
        mov eax, y
        cmp x, eax
        jng STEP1
        mov eax, x
      STEP1 :
        cmp eax, z
        jge STEP2
        mov eax, z
      STEP2 :
        mov max, eax
    pop eax
  }
  cout << "Maximum: " << max << endl;
  cout << "Maximum: " << MaxAsm(x,&y,z) << endl;

  system("pause");
  return 0;
}