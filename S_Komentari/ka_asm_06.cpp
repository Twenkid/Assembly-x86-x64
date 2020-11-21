// примери за асемблерен код
// намиране на броя на единиците в кода на цяло число 
// Източник на кода: ФМИ, ПУ

//* Допълнителни коментари в кода по-долу:
//* Тодор Арнаудов - Тош: http://artificial-mind.blogspot.com
//* http://twenkid.com
//* Виж особените инструкции: rcr, bt, bsf
//* http://x86.renejeschke.de/html/file_module_x86_id_273.html
//* R - rotate, C - Carry (flag), R/L - right, left
//* Използва и съдържанието на флага за пренос CF
//*  BT - bit test - чете стойността на бита на дадена позиция и го записва в CF
//*  http://x86.renejeschke.de/html/file_module_x86_id_22.html
//*  BSF - bit scan forward - сканира източника докато намери бит "1" и записва в кой поред разряд е първата единица:
//* https://courses.engr.illinois.edu/ece390/archive/spr2002/books/labmanual/inst-ref-bsf.html 
//* BTC: http://x86.renejeschke.de/html/file_module_x86_id_23.html
//* Записва стойността на посочен бит във флага CF, и го обръща в операнда (not ... )

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
        mov ecx, 32   //* 32 = 0x00000020 --> '1' на шеста позиция в двоичния код 
        mov eax, n   //* брой стъпки
        xor esi, esi // esi - брой на единиците в n  //* записва 0 за начало,и нулира флага CF? 
      Cyc : rcr eax, 1 //стъпка ротация надясно
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
        xor ecx, ecx // ecx - текущият сканиран разред, започва от 0
        xor esi, esi // esi - брой на единиците в n, започва от 0
       Cyc2 : bt n, ecx //* BT - bit test --> стойността на бита се поставя в CF
              adc esi, 0 //* ADC -- Add with Carry -> добавя единица, ако CF е 1 (т.е. ако даденият бит е 1)
              inc ecx    //* Минава на следващия бит за сканиране
              cmp ecx, 32 //* Броят разреди - дали е стигнало до края на сканирането
              jne Cyc2    //* Докато ecx не е 32 - последният разред - goto Cyc2 
        mov cn, esi  //* esi съдържа броя единици, прехвърляме в локалната променлива
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