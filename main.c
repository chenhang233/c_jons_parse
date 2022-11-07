#include <stdio.h>

static const char* skip(const char* in) { 
	char* ptr = in + 1;
	printf("ptr=%s\n", ptr);
	int len = 0;
	while (*ptr != '\"' && *ptr && ++len) {
		printf("*ptr=%d=%c\n", *ptr, *ptr);
		if (*ptr++ == '\\') ptr++;
	}
	printf("len=%d\n", len);
		 return in; 
}

int main(int argc, const char * argv) {
	char* p = "(\\\"Bee\\\"asd) N \n}";
	char * p2 = skip(p);
	//printf("%s\n", p);
	printf("--next--%d\n", *p2);
}