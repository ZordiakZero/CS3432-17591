#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#include "string.h"

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
  int reg_amount = 32;
  reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
  for(int i = 0; i < 32; i++)
    reg[i] = i;
}


bool strEqual(char* str1, char* str2){ // checks if two strings are equivalent
  char* temp1 = str1;
  char* temp2 = str2;
  int count1 = 0;
  int count2 = 0;

  while (*(temp1+count1) != '\0'){
    count1++;
  }
  while (*(temp2+count2) != '\0'){
    count2++;
  }
  int i = 0;
  while (*(str1+i) != '\0' && *(str2+i) != '\0'){
    if (*(str1+i) != *(str2+i))
      return false;
    i++;
  }
  if (count1 != count2)
      return false;
  return true;
}

int getLocation(char* str){ //change x## to ##
  int i = 0;
  char pos[2];
  while(*(str+i) != '\0'){
    i++;
  }
  if(i>=3){
    pos[0] = *(str+1);
    pos[1] = *(str+2);
  }
  else
    pos[0] = *(str+1); //for single digit register

  return atoi(pos); //string to int
}


int instructionCases(char* str){ //makes cases for instructions
  if (strEqual(str, "LW")) {
    return 0;
  }
  else if (strEqual(str, "SW")) {
    return 1;
  }
  else if (strEqual(str, "ADD")) {
    return 2;
  }
  else if (strEqual(str, "ADDI")) {
    return 3;
  }
  else
    return 4;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char *instr){
  char *txt = "mem.txt"; //loads the mem.txt file so we can read it
  char **tokens;
  tokens = tokenize(instr, ' ');//gets the tokens seperated by space

  int instructNum = instructionCases(tokens[0]);
  //implement a switch case for each instruction and act accordording to the instruction
  switch(instructNum){
  case 0:; //LW - Need to end stateement after case to avoid error from assignments
    int saveLoc = getLocation( *(tokens+1) );  //location where info is loaded
    char** tokenSplit = tokenize( *(tokens+2), '('); //removing ()
    char** tokensFinal = tokenize( *(tokenSplit+1), ')');

    int regLocation = getLocation(*tokensFinal); //location of register
    int32_t location = atoi(*tokenSplit) + regLocation;

    reg[saveLoc] = read_address(location, txt); //update loaction on array
    break;
  case 1:; //SW
    saveLoc = getLocation(*(tokens+1));
    tokenSplit = tokenize(*(tokens+2),'(');
    tokensFinal = tokenize(*(tokenSplit+1),')');
    regLocation = getLocation(*tokensFinal);

    int32_t write = reg[saveLoc]; //where info will be written
    location = atoi(*tokenSplit)+regLocation;
    int32_t Write = write_address(write, location, txt); //writes info
    break;
  case 2:; //Add
    saveLoc = getLocation(*(tokens+1));
    int r1 = getLocation(*(tokens+2)); //int for 1st value
    int r2 = getLocation(*(tokens+3)); //int for 2nd value
    reg[saveLoc] = reg[r1] + reg[r2];
    printf("ADD Output: %d\n", reg[saveLoc]);
    break;
  case 3:;//Addi
    saveLoc = getLocation(*(tokens+1));
    r1 = getLocation(*(tokens+2));
    r2 = atoi(*(tokens+3)); //convert to int for 2nd value

    reg[saveLoc] = reg[r1] + r2;
    printf("ADDi Output: %d\n",reg[saveLoc]);
    break;
  default://everything else
    printf("Invalid Instruction...\n");
    return false;
  }
  for(int i = 0; i< 16; i++){
    printf("X%02i:%.*lld ", i, 8, (long long int) reg[i]);
    printf("X%02i:%.*lld ", i+16, 8, (long long int) reg[i+16]);
    printf("\n");
  }
  return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
  int32_t data_to_write = 0xFFF; // strEqual to 4095
  int32_t address = 0x98; // strEqual to 152
  char* mem_file = "mem.txt";

  // Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
  int32_t write = write_address(data_to_write, address, mem_file);
  if(write == (int32_t) NULL)
    printf("ERROR: Unsucessful write to address %0X\n", 0x40);
  int32_t read = read_address(address, mem_file);

  printf("Read addrss %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Your code goes in the main
 *
 */
int main(){
  // Do not write any code between init_regs
  init_regs(); // DO NOT REMOVE THIS LINE

  // Below is a sample program to a write-read. Overwrite this with your own code.
  //write_read_demo();
  char input[100];
  while(1){
    printf("Enter riscv instruction... \n(To end enter: q)\n$");
    fgets(input,sizeof(input), stdin);
    if(*input == 'q' || *input == 'Q'){
      break;
    }
    interpret(input);
    printf("\n");
  }
  printf("Final Register:\n");
  for(int i = 0; i< 16; i++){
    printf("X%02i:%.*lld ", i, 8, (long long int) reg[i]);
    printf("X%02i:%.*lld ", i+16, 8, (long long int) reg[i+16]);
    printf("\n");
  }
  printf("\n");
}
