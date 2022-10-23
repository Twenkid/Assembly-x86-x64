# Assembly-Examples

## 2022

AVX2, AVX, SSE ... intrinsics, built-in ASM in C++, GCC/G++ tests etc.

* asm -- incluse asm in MS Visual Studio, call a function printf  23-10-2022
* intrinsics - AVX2 examples etc.  23-10-2022


Compile with command line compiler of MS Visual Studio:
x86 Native Tools ...

x86_64 Cross Tools ...

Developer Command Prompt... 

![image](https://user-images.githubusercontent.com/23367640/197369736-881c720c-915f-4f50-8ff9-fcaad2215150.png)

``` 
Z:\c>cl asm.cpp /link /out:program1.exe
```

```batch
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30133 for x86
Copyright (C) Microsoft Corporation.  All rights reserved.
asm.cpp

Microsoft (R) Incremental Linker Version 14.29.30133.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:asm.exe
/out:program1.exe
asm.obj

Z:\c>program1
Hello world

Z:\c>
```

## 2017

Примерен код на Асемблер за x86 с обяснения на български език (вбъдеще може би и x64 и др.)

Източник на кода: ФМИ, ПУ

Допълнителни коментари и обяснения: Тош, 2017 г.


Кодът е вграден в С, писан на Visual Studio:

```
int Sum(int *a, int L) {
  int s = 0;
  for (int i = L - 1; 0 <= i; --i) s += a[i];
  return s;
}
int SumAsm(int *a, int L) {
  __asm {
    push esi	//праща в стека
        xor eax, eax		//нулира eax -- xor reg,reg е равносилно на mov reg, 0
                        //(mov eax,0  ... mov ecx,0) но е по-ефективно
        mov ecx, L			//броячът ecx = дължината на масива
        mov esi, a			//esi - адресът на нулевия елемент на масива
      NEXT : add eax, [esi + ecx * 4 - 4]
				//натрупва стойността в акумулатора eax = eax + a[ecx] ... 
				//Изразите като този в скобите са адресна аритметика, esi е базов регистър
				//(нулевия елемент),
				//ecx е номерът на елемента, *4 - защото 4 байта е размерът на елемент;
				//-4 е защото масивите започват от [0], но дължината се очаква да е поне 1
				//И дължината е с едно по-голяма от последния валиден елемент.
				//if L==1, валидно е само a[0]
				//Затова първата стойност на ecx се очаква да е поне 1.
				//Ако се прати L == 0 може да се получи грешка, защото процесорът
				//ще опита да прочете един адрес (4 байта) преди началото на масива.
				//Ако пък няма " - 4" и се прати коректна дължина, процесорът
				//ще прочете с един 4-байтов адрес напред, извън масива.
						
             loop NEXT	//loop проверява дали ecx == 0, ако е --> край, продължава
						//if ecx > 0 ==> { ecx -= 1; jmp NEXT }
						//T.e. събирането започва от последния елемент

    pop esi		//записва в esi старата стойност, от преди асемблерния блок
  }
}
```
