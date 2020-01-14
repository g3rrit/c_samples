#include <stdio.h>

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define XSTR(s) STR(s)
#define STR(s) #s

#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)

#define ULN UNIQUE_NAME(_m__lambda)
#define UFN UNIQUE_NAME(_m__cont)

#define LAMBDA(trt, rt, name, at, b) asm("jmp "XSTR(ULN)); } rt name at b trt UFN() { asm(XSTR(ULN)":"); 

int foo() {
	printf("in foo\n");

	LAMBDA(int, void, bar, (), {
		printf("in bar\n");
		return;
	})

	printf("in foo_cont\n");

	bar();

	return 1;
}

int main() {
	printf("before foo\n");
	foo();
	printf("after foo\n");
	return 0;
}
