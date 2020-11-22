// примери за асемблерен код
// намиране на максимума на елементите на масив
// Източник на кода: ФМИ, ПУ

// Допълнителни коментари в кода по-долу:
// Тодор "Twenkid" Арнаудов
// http://artificial-mind.blogspot.com
// http://twenkid.com
//je - jump Equal
//jge -- jump Greater or Equal
//jb - jump Below
//jnb - jump Not below
//ja - jump Above
//jnle - jump Not Less or Equal ( > )
//и т.н. 

#include <iostream>
using namespace std;
void View(int *a, int L) {
  for (int i = 0; i < L; ++i) cout << a[i] << "  ";
  cout << endl;
}
int Max(int *a, int L) {
  int max = a[0];
  for (int i = 1; i < L; ++i) if(max < a[i]) max = a[i];
  return max;
}
int MaxAsm(int *a, int L) { //указател към a, дължина на масива Length
  __asm {
    push esi			//вкарват се esi и ecx в стека, защото програмната среда ги използва, а ще се променят
    push ecx
        mov ecx, a		//началния адрес на масива (на елемент a[0])
        mov eax, [ecx] //зарежда
        mov esi, 0
      NEXT2: inc esi	// esi += 1
             cmp esi, L // if (esi == L) 
             je END2                    //ако е стигнат края на масива, goto END2
                cmp eax, [ecx + esi*4]  //if eax >= a[esi] goto NEXT2 ... ecx е базов адрес (на елемент 0), esi е номер на елемент, *4 заради размера на елемента - 4 байта (DWORD, int)
                jge NEXT2
                mov eax, [ecx + esi*4]    //esle if eax < a[esi] ==> eax = 
             jmp NEXT2
      END2 :
    pop ecx
    pop esi
  }
}
int main(){
  int ar[] = { 1, -2, 80, -2, 3, 80, 7, 80, 6, -5, 4 };
  const int len = sizeof(ar) / sizeof(ar[0]);
  View(ar, len);
  cout << "Maximum: " << Max(ar, len) << endl;
  int max;
  __asm {
    push eax
    push esi
        mov eax, ar
        mov esi, 1
      NEXT : cmp esi, len
             je END
                cmp eax, ar[esi*4]
                jge STEP
                mov eax, ar[esi*4]
              STEP :
             inc esi
             jmp NEXT
      END :
        mov max, eax
    pop esi
    pop eax
  }
  cout << "Maximum: " << max << endl;
  __asm {
    push eax
    push esi
        mov eax, ar
        mov esi, 0
      NEXT2: inc esi
             cmp esi, len
             je END2
                cmp eax, ar[esi*4]
                jge NEXT2
                mov eax, ar[esi*4]
             jmp NEXT2
      END2 :
        mov max, eax
    pop esi
    pop eax
  }
  cout << "Maximum: " << max << endl;
  cout << "Maximum: " << MaxAsm(ar, len) << endl;

  system("pause");
  return 0;
}