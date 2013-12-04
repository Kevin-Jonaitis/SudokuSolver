#include <iostream>;
using namespace std;


/* Convert an odomoter digit, a character 1...9A...Z, to an unsigned fixnum digit, 0...34. */
int odometer_digit_to_int(char odigit);

/* Convert an unsigned fixnum digit, 0...34, to an odomoter digit, a character 1...9A...Z. */
char int_digit_to_odometer(int digit);

/* Count the odometer numbers that can be expressed using fewer than LENGTH digits. */
int odometer_count_below_length(int length);

/* Treat ONUMBER as a base 35 number using ODOMETER_DIGIT_TO_FIXNUM. */
int odometer_as_base35(string odometer);

/* The number of odometer digits that will be required to represent FNUMBER. */
int number_of_odometer_digits(int number);

/* Return a string that represents BASE35_NUMBER in base 35 with odometer digits. */
string base35_as_odometer(int base35_number, int length);

/* Convert an odometer number, represented as a string, to an unsigned fixnum. */
int odometer_number_to_int(string oNumber);

/* Convert an unsigned fixnum to an odometer number, represented as a string. */
string int_to_odometer_number(int number);
