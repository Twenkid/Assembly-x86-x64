#include <iostream>
using namespace std;
int Sum(int v, int* p, int& r) {
	//return v+*p+r;
	int s;
	__asm {
		push eax
		push esi
			mov eax, v
			mov esi, p
			add eax, [esi]
			mov esi, r
			add eax, [esi]
			mov s, eax
		pop esi
		pop eax
	}
	return s;
}
int main(){
  system("chcp 1251");
  int x, y, z, s;
  cout << "три цели числа: ";
  cin >> x >> y >> z;
  s = x + y + z;
  cout << "сума: " << s << endl;
  __asm {
    push eax
      mov eax, x
      add eax, y
      add eax, z
      mov s, eax
    pop eax
  }
  cout << "сума: " << s << endl;
  cout << "сума: " << Sum(x,&y,z) << endl;

  cout << "\n\n";
  system("pause");
  return 0;
}