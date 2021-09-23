#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>


int main()

{

  char* str;

  while(str != 'q'){

    printf("$");
    fgets(str, 100, stdin);

  }

  count_tokens(*str);

  char** tokens = tokenize(str);

  count_tokens(tokens);

  print_all_tokens(tokens);

}



/* Return true (non-zero) if c is a whitespace characer

   ('\t' or ' ').

   Zero terminators are not printable (therefore false) */

bool delim_character(char c){

  if(c == '\t' || c == ' ')

    return true;

  return false;

}



/* Return true (non-zero) if c is a non-whitespace

   character (not tab or space).

   Zero terminators are not printable (therefore false) */

bool non_delim_character(char c){

  if(c == "\t" || c == ' ')

    return false;

  return true;

}



/* Returns a pointer to the first character of the next

   space-separated word*/

char *word_start(char* str){

  if (count_tokens(str) > 0) { // count_tokens works with tokens or words

    char *temp = str;

    while (*temp) {

      if (delim_character(*temp)) {

	*temp++;

	return temp; // if a space is found, return the pointer to the first character

      }

      *temp++;

    }

  }

  return str; // return original if none found

}



/* Returns a pointer to the first space character of the zero

   terminated string*/

char *end_word(char* str){

  while (non_delim_character(*str)){

    str++;

    if (delim_character(*str)) {

      return str;

    }

  }

  return str;

}



// counts the number of words or tokens

int count_tokens(char* str){

  int count = 0;

  int pos = 0;

  while(str[pos]) {

    count++;

    pos++;

  }

  return count;

}



/* Returns a freshly allocated zero-terminated vector of freshly allocated

   space-separated tokens from zero-terminated str.

   For example, tokenize("hello world string") would result in:

     tokens[0] = "hello"

     tokens[1] = "world"

     tokens[2] = "string"

     tokens[3] = 0
*/

char *copy_str(char *inStr, short len){

  int pos = 0;

  char* copy;

  while(inStr[pos]){

    if (pos < len)

      copy[pos] = inStr[pos];

    if (pos == len)

      return copy;

    pos++;

  }

  return inStr;

}



char** tokenize(char* str){

  int pos = 0;

  int token_len = count_tokens(str);

  char** tokens = (char**) malloc((token_len+1) * sizeof(char*));

  while(pos < token_len) {

    int len = word_start(str) - end_word(str);

    str = word_start(str);

    char* token = copy_str(str, len);

    str = end_word(str);

    tokens[pos] = token;

    pos++;

  }

  return tokens;

}





void print_all_tokens(char** tokens) {

  int pos = 0;

  while (tokens[pos]) {

    printf("The token at %d is: %s\n", pos, tokens[pos]);

    pos++;

  }

}

