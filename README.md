# Assembly-Examples Exercises Projects x86 x64 
# Disassembly - Debugging - ... 

###
x64Dbg ... plugin
Multiline Ultimate Assembly 
@label1:
   MOV EAX, 4
   RETN

The nearest preceding anonymous label is referred to as: @b  (before) or @r
The nearest following anonymous label is referred to as:  @f

(Apply the idea for Vsy Assembly: when BEQ -1 f ..? when adding ... --> see... (or/and first add some data structures to simplify the jobs! at least a stack and or a graph?, note 18.7.2025)

..
```
cpp\2025
asm6a X64
-- xmm0, xmm1, xmm2, xmm3 ...
printf calling convention Win64 ...
...
asmijit library
asmjit1...

x64dbg
Z:\snapshot_2025-03-15_15-57\release\x64>
x64dbg
```

..

## 2025, Note 2.7.2025

* MASM, Native ... ml, ml64 command line
2019 ... Dev tools ... Native console ... VS2019 etc.

https://learn.microsoft.com/en-us/cpp/assembler/masm/ml-and-ml64-command-line-reference?view=msvc-170

ml.exe /Fo a1.obj /c a.asm /Fo b1.obj /c b.asm

env.vars:
INCLUDE	Specifies search path for include files.
ML	Specifies default command-line options.
TMP	Specifies path for temporary files.

/Zs	Performs a syntax check only.
/help
/I pathname	Sets path for include file. A maximum of 10 /I options is allowed.
/F hexnum	Sets stack size to hexnum bytes (the same as /link /STACK:<number>). The value must be expressed in hexadecimal notation. There must be a space between /F and hexnum.
/EP	Generates a preprocessed source listing (sent to STDOUT). See /Sf.
/AT	Enables tiny-memory-model support. Enables error messages for code constructs that violate the requirements for .com format files. This option isn't equivalent to the .MODEL TINY directive.
Not available in ml64.exe.
/Bl filename	Selects an alternate linker in filename.

/coff	Generates common object file format (COFF) type of object module. Required for Win32 assembly language development.
Not available in ml64.exe.


* Bit manipulation instructions x86

https://claude.ai/chat/08d46e2d-6e2f-469d-987e-9bd4d61a2ecf
https://claude.ai/share/0fc3cd0f-5699-433d-af44-095ce0265601

*  Online Assembler & Disassembler 
https://defuse.ca/online-x86-assembler.htm#disassembly

https://defuse.ca/online-x86-assembler.htm#disassembly


* Understanding Windows x64 Assembly - a long tutorial
https://sonictk.github.io/asm_tutorial/

NASM syntax:
"""
bits 64
default rel

segment .data
    msg db "Hello world!", 0xd, 0xa, 0

segment .text
global main
extern ExitProcess

extern printf

main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32

    lea     rcx, [msg]
    call    printf

    xor     rax, rax
    call    ExitProcess
    
nasm -f win64 -o hello_world.obj hello_world.asm
link hello_world.obj /subsystem:console /entry:main /out:hello_world_basic.exe
hello_world_basic.exe
Hello world!

"""

## 2022

AVX2, AVX, SSE ... intrinsics, built-in ASM in C++, GCC/G++ tests etc.

* asm -- incluse asm in MS Visual Studio, call a function printf  23-10-2022
* intrinsics - AVX2 examples etc.  23-10-2022+

x86 Native Tools Command Prompt

#W -- warnings

```
Z:\c>cl /W4 m.cpp /link /out:m.exe
m -->

cl file1.cpp file2.cpp file3.cpp /link /out:p.exe

cl /EHsc file1.cpp file2.cpp file3.cpp /link /out:p.exe   //Exception handling, standard

cl file1.cpp file2.cpp file3.cpp /link /out:p.exe

cl file1.cpp file2.cpp file3.cpp /link /out:p.exe

cl haha.cpp jsd.cpp efjewf.h /link  --> haha.exe
cl /?

```
General cl:

```bash
Compile and link:
cl /c main.cpp
cl /c odr1.cpp
cl /c odr2.cpp
cl /Feodr.exe main.obj odr1.obj odr2.obj

/Fe - force exe ... odr.exe
```


## GCC/G++

```
g++ -S m.cpp --> Assembly
g++ m.cpp --> a.out  ./a.out
g++ -o run m.cpp --> ./run
g++ --version
-Os  - optimize for size
-O2  - optimize for speed
- f... align ... 
gcc asm.cpp -mavx2 -lstdc++ -o asm
g++ asm.cpp -mavx2 -o asm
```


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
