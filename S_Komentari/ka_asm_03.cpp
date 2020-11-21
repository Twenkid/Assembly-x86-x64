// примери за асемблерен код
// намиране на максимума на три числа
// Източник на кода: ФМИ, ПУ
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

* Допълнителни коментари в кода по-долу:
* Тодор "Тош" Арнаудов
 http://artificial-mind.blogspot.com
 http://twenkid.com
 
[eax], [esi], ...[reg] --> стойността, която се намира на адреса, записан в регистъра
Ако в рег. има 1000, чете от адрес 1000 - както адресиране на масив, [0], [1], ...
*/

#include <iostream>
using namespace std;
int MaxAsm(int v, int * p, int & r) { 
/*
v се праща по стойност, p и r - по адрес
p е указател, &r е референция (синоним), което на ниско ниво е едно и също (на С++ е различно, с референциите има по-малко права, мисля че няма адресна аритметика - с указатели може да пишеш *p++ което значи да увеличиш адреса с размера на един елемент, в случаи - с 4 в байтове; референциите са по-ограничаващи

Тук &r се ползва като променлива, в която се записва стойността на по-големия елемент от v и *p,

Така както е дефинирана функцията обаче е грешна -- дадена е че връща int, а не връща нищо. 
Може да се ползва хем да връща, хем да записва резултата и в r; 

Затова добавям:

*/

 int result; //ДОБАВЕНО
 
  __asm {
    push esi  //вкарва в стека настоящата стойност на esi, защото ще се ползва в кода по-долу, а е нужен на средата
        mov esi, p
        mov eax, [esi]  //зарежда стойността в адреса на esi. 
        cmp v, eax //сравнява стойността на v с
        jng STEP1 //if v <= eax ---> goto STEP 1  
        mov eax, v //else eax = v  (max = v) 
      STEP1 : 
        mov esi, r    //въвежда r в сегментни регистър esi, който позволява нужното непряко адресиране
        cmp eax, [esi] //сравнява eax със стойността на адреса esi (както ако се чете от масив - [0] е нулевия адрес, [1] - първия и т.н.)
        jge STEP2  //if eax >= r STEP2
        mov eax, [esi] //else [esi], т.е. r = eax (max); записва стойността на адреса esi в акумулатора eax
		mov result, eax //ДОБАВЕНО
      STEP2 :
    pop esi //връщаме старото esi
  }
  
  return  result //ДОБАВЕНО
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
        cmp x, eax //if x < eax
        jng STEP1 //            goto STEP1
        mov eax, x //else eax = x  (max = x)
      STEP1 :
        cmp eax, z  //if eax >= z
        jge STEP2   //           goto STEP2
        mov eax, z  //else eax = z (max = z)
      STEP2 :
        mov max, eax
    pop eax
  }
  cout << "Maximum: " << max << endl;
  cout << "Maximum: " << MaxAsm(x,&y,z) << endl;

  system("pause");
  return 0;
}