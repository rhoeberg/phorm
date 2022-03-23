// #include <stdio.h>
// #include <iostream>
// #include "util.h"
// #include "util.cpp"
// #include "vm_array.h"
// #include "vm_array.cpp"
#include "tokenizer.cpp"

int main(int argc, char *argv[])
{
	TokenizedFile tk = TokenizeFile("norepo/test.cpp");

	for(int i = 0; i < tk.tokens.Count(); i++) {
		printf("TOKEN:%d\t%.*s\n", tk.tokens[i].type, tk.tokens[i].length, tk.tokens[i].at);
	}

	FreeTokenizedFile(&tk);
}
