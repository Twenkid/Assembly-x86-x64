// пример за асемблерен код
// намиране на минимума на елементите на масив
// и броя на минималните елементи в масива
// Източник на кода: ФМИ, ПУ

/*
еcx се ползва за брояч (counter)
loop Next -> ако ecx == 0 --> продължава, ако е > 0 --> jmp Next (етикета)

Бележки: Тодор "Тош" Арнаудов
http://twenkid.com
http://artificial-mind.blogspot.com
*/
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
    pushad				//push all dword ? -- вкарва в стека всички регистри
        mov ecx, len	//въвежда дължината на масива в брояча
        dec ecx			//намалява брояча с едно
        mov eax, ar[ecx*4]	//въвежда *стойността* на първия елемент на масива в акумулатора eax, *4 защото дължината на елементите е 4 байта (int 32 bit)
        mov esi, 1			//подготвя сегментния регистър esi - брой равни елементи на най-малкия
      Next :  cmp ar[ecx*4-4], eax  //сравняваш съдържанието на предишния елемент (-4 == 1 стъпка назад) с акумулатора (най-малкия елемент досега)
              jg Step //ако е по-голям - прескача
              je Equal //ако е равен - отброява в esi
              mov eax, ar[ecx*4-4]  //ако стигне дотук, значи е по-малък - запомня стойността в акумулатора
              /*
              mov esi, 1
              jmp Step
              */
              xor esi, esi   //нулира esi, броячът на повторенията
        Equal :
              inc esi  //ако е равен, увеличава броя на повтарящите се елементи на най-малкия
      Step :  loop Next  //ако ecx = 0 - край, продължава. Ако е > ==> Се връща на Next, jmp Next
        mov min, eax	//въвежда в локалната променлива, за да има достъп до нея от С++
        mov num, esi //въвежда броя на повторениятао
  }
  cout << "Minimum: " << min << "\nCount: " << num << endl;

  system("pause");
  return 0;
}