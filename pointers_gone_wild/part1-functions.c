/**
 * Pointers Gone Wild
 * CS 241 - Spring 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Assign the sum of a^2 and b^2 to c and print the results. If a = 3,
 * b = 4, then you should print "3^2 + 4^2 = 25" on its own line.
 *
 * @param a
 *     Input parameter a.
 *
 * @param b
 *     Input parameter b.
 */
void one(const int a, const int b) {
  int c = (a * a) + (b * b);
  //cout << a << "^2 + " << b << "^2 = " << c << endl;
  printf("%d^2 + %d^2 = %d\n", a,b,c);
}

/**
 * Checks to see if the input parameter is a passing grade and prints out
 * if the grade is passing using the provided format specifiers. Note that
 * 'grade' is a char pointer pointing to the beginning of a C string. A grade
 * is considered passing iff the numeric interpretation (defined by strtof) is
 * above a 70. Suppose
 * that 'grade' pointed to the string "73.58", then 'grade' would be passing in
 * this example. If 'grade' pointed to "it's over 9000!!!!", then that is still
 * not
 * a passing grade.
 *
 * @param grade
 *     The grade to check.
 */
void two(const char *grade) {
//  char buffer[strlen(grade)];
//  if(sscanf(grade, "%s", buffer) == EOF || sscanf(grade, "%s", buffer) == 0){
//char * temp;
  float gra = strtof(grade,NULL);
  if (gra > 70)
    printf("%f passed!\n", gra);
  else
    printf("%s not passed!\n", grade);
//}
//  else
//  {
//    char * temp;
//    float gra = strtof(grade,&temp);
//    printf("%f not passed!\n", gra);
 // }
}

/**
 * Have the integer pointer p point to the integer x which is on the stack.
 * Then print the value that 'p' points to and format it as an integer
 */
void three() {
  int x = 4;
  int *p;
  p = &x;

  printf("The value of p is: %d\n", *p);
}

/**
 * Prints out a specific message based on if the number is
 * between 0 and 1 (exclusive for both).
 *
 * @param value
 *     Value to test.
 */
void four(const float value) {
  if (0 > value || value < 1)
    printf("The value is between zero and one.\n");
  else
    printf("The value is not between zero and one.\n");
}

/**
 * Prints "x and y are equal." iff the values x and y point to are equal
 * where equality is defined by integer equality.
 * Else print "x and y are different".
 *
 * @param x
 *     First input parameter.
 *
 * @param y
 *     Second input parameter.
 */
void five(const int *x, const int *y) {
  if (*x == *y)
    printf("x and y are equal.\n");
  else
    printf("x and y are different.\n");
}

/**
 * Returns a pointer to a float that points to memory on the heap,
 * which contains a copy of the value that the integer pointer 'x' is pointing
 * to.
 *
 * @param x
 *     Input parameter, whose value will be returned as a (float *).
 *
 * @returns
 *     A new pointer, allocated on the heap and not freed, that
 *     contains the value of the input parameter.
 */
float *six(const int *x) {
  float *p = malloc(sizeof(float));
  *p = *x;
  return p;
}

/**
 * Takes a char pointer 'a' and checks to see if the first char
 * that it points to is alphabetic (upper or lower case).
 *
 * @param a
 *     Input parameter a, which is a char*
 *
 */
void seven(const char *a) {
  if ((a[0] >= 'A' && a[0] <= 'Z') ||(a[0]>= 'a'&& a[0]<= 'z'))
    printf("a is a letter.\n");
  else
    printf("a is not a letter.\n");
}

/**
 * Allocates memory on the heap large enough to hold the C-string "Hello",
 * assigns the string character by character, prints out the full string "Hello"
 * on it's own line, then frees the memory used on the heap.
 */
void eight() {
  char *s = malloc(6);

  *s = 'H';
  *(s+1) = 'e';
  *(s+2) = 'l';
  *(s+3) = 'l';
  *(s+4) = 'o';
  *(s+5) = '\0';
  fprintf(stdout,"%s\n", s);
  fflush(stdout);

  free(s);
  s = NULL;
}

/**
 * Creates a float pointer 'p' that points to memory on the heap and
 * writes the numeric value 12.5 into it, prints out the float representation
 * of the value that p points to, and finally frees p.
 */
void nine() {
  float *p = malloc(sizeof(float));
  *p = 12.5;

  printf("The value of p is: %f\n", *p);
  free (p);
  p = NULL;
}

/**
 * Zeros out the value that x points to.
 *
 * @param x
 *     Pointer to reset to 0.
 */
void ten(int *x) { *x = 0; }

/**
 * Concatenates "Hello " and the parameter str, which is guaranteed to be a
 * valid c string, and
 * prints the concatenated string.
 */
void eleven(const char *str) {
  char *s = "Hello ";
  char *s2  = malloc(strlen(str)+strlen(s)+1);
  strcpy(s2,s);
  strcat(s2, str);
  printf("%s\n", s2);
  free(s2);//>>????
}

/**
 * Creates an array of values containing the values {0.0, 0.1, ..., 0.9}.
 */
void twelve() {
  float values[10];

  int i, n = 10;
  for (i = 0; i < n; i++)
    values[i] = ((float)i) / (float)n;

  for (i = 0; i < n; i++)
    printf("%f ", values[i]);
  printf("\n");
}

/**
 * Creates a 2D array of values and prints out the values on the diagonal.
 */
void thirteen(int a) {
  int values[10][10];

  int i, j;
  //values = malloc(10 * sizeof(int));
  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++)
      values[i][j] = i * j * a;

  for (i = 0; i < 10; i++)
    printf("%d ", values[i][i]);
  printf("\n");
}

/**
 * If s points to the string "blue", then print "Orange and BLUE!". If s points
 * to the string
 * "orange", then print "ORANGE and blue!" else just print "orange and blue!".
 * Use strcmp() and
 * friends to test for string equality.
 *
 * @param s
 *     Input parameter, used to determine which string is printed.
 */
void fourteen(const char *s) {
  int flag = 0;
  if(!strcmp(s,"orange"))
    flag = 2;
  if(!strcmp(s,"blue"))
    flag = 1;
  switch (flag) {
  case 1:
    printf("Orange and BLUE!\n");
    break;

  case 2:
    printf("ORANGE and blue!\n");
    break;

  case 0:
    printf("orange and blue!\n");
    break;
  }
}

/**
 * Prints out a specific string based on the input parameter (value).
 *
 * @param value
 *     Input parameter, used to determine which string is printed.
 */
void fifteen(const int value) {
  switch (value) {
  case 1:
    printf("You passed in the value of one!\n");
    break;      
  case 2:
    printf("You passed in the value of two!\n");
    break;
  default:
    printf("You passed in some other value!\n");
  }
}

/**
 * Returns a newly allocated string on the heap with the value of "Hello".
 * This should not be freed.
 *
 * @returns
 *     A newly allocated string, stored on the heap, with the value "Hello".
 */
char *sixteen() {
  char *s = malloc(6);
  strcpy(s, "Hello");
  return s;
}

/**
 * Prints out the radius of a circle, given its diameter.
 *
 * @param d
 *     The diameter of the circle.
 */
void seventeen(const int d) {
  printf("The radius of the circle is: %f.\n", ((float)d / 2));
}

/**
 * Manipulates the input parameter (k) and prints the result.
 *
 * @param k
 *     The input parameter to manipulate.
 */
void eighteen(const int k) {
  int temp = k;
  temp = temp * temp;
  temp += temp;
  temp *= temp;
  temp -= 1;

  printf("Result: %d\n", temp);
}

/**
 * @brief
 *     Clears the bits in "value" that are set in "flag".
 *
 * This function will apply the following rules to the number stored
 * in the input parameter "value":
 * (1): If a specific bit is set in BOTH "value" and "flag", that
 *      bit is NOT SET in the result.
 * (2): If a specific bit is set ONLY in "value", that bit IS SET
 *      in the result.
 * (3): All other bits are NOT SET in the result.
 *
 * Examples:
 *    clear_bits(value = 0xFF, flag = 0x55): 0xAA
 *    clear_bits(value = 0x00, flag = 0xF0): 0x00
 *    clear_bits(value = 0xAB, flag = 0x00): 0xAB
 *
 * @param value
 *     The numeric value to manipulate.
 *
 * @param flag
 *     The flag (or mask) used in order to clear bits from "value".
 */
long int clear_bits(long int value, long int flag) {
  value = (value^flag)&value;// TODO clear_bits
  return value;
}

/**
 * @brief
 *     A little finite automaton in C.
 *
 * This function will use a provided transition function to simulate a DFA over
 * an input string. The function returns the final state.
 *
 * The DFA starts in state 0. For each character in input_string, call the
 * transition function with the current state and the current character. The
 * current state changes to the state the transition function returns.
 *
 * So, in pseudo code:
 *
 * state = 0
 * for character in input_string:
 *     state = transition_function(state, character)
 *
 * NOTE: the transition_function parameter is a "function pointer." For more
 * information about these fun creatures, see this:
 * http://www.cprogramming.com/tutorial/function-pointers.html
 *
 * @param transition - function pointer to a transition function to use
 * @param input_string - string to run the automaton over
 *
 * @return the final state
 */
int little_automaton(int (*transition)(int, char), const char *input_string) {
  int state = 0;
  int size = strlen(input_string);
  for(int i = 0; i < size ; i++)
  state = transition(state,input_string[i]);// put something here

  return state;
}
