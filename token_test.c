#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <tokenizer_Filby_Cody.c>



int main()

{

  char str[100];

  while(str[0] != 'q'){

    printf("$");

    fgets(str, 100, stdin);

    count_tokens(str);

    char** tokens = tokenize(str);

    count_tokens(tokens);

    print_all_tokens(tokens);

  }

}
