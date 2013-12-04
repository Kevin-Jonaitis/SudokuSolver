#include <iostream>
using namespace std;

string odometer = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int odometer_digit_to_int(char odigit) {
	return odometer.find(odigit);
}

char int_digit_to_odometer(int digit){
	return odometer.at(digit);
}

int odometer_count_below_length(int length){
	int base = 35;
	int power = 1;
	int answer = 0;
	int temp = length - 1;
	while (temp > 0){
		power = power * base;
		answer = answer + power;
		temp = temp - 1;
	}
	return answer;
}

int odometer_as_base35(string odometer) {
	int base = 35;
	int power = 1;
	int answer = 0;
	int ndx = odometer.length() - 1;
	while (ndx >= 0) {
		answer = answer + odometer_digit_to_int(odometer.at(ndx)) * power;
		power = power * base;
		ndx = ndx - 1;
	}
	return answer;
}

int number_of_odometer_digits(int number) {
	int answer = 1;
	while (odometer_count_below_length(answer + 1) < number + 1) {
		answer = answer + 1;
	}
	return answer;
}

string base35_as_odometer(int base35_number, int length) {
	int base = 35;
	//create a new string of specified length;
	string answer = "a";
	answer.resize(length);
	double power = pow(base, length - 1);
	int temp = base35_number;
	for (int i = 0; i < length; i++) {
		int fdigit = floor(temp / power);
		answer[i] = int_digit_to_odometer(fdigit);
		temp = temp - fdigit * power;
		power = power / base;
	}
	return answer;
}

int odometer_number_to_int(string oNumber) {
	int count_below = odometer_count_below_length(oNumber.length());
	int answer = count_below + odometer_as_base35(oNumber);
	return answer;
}

string int_to_odometer_number(int number) {
	int ndigits = number_of_odometer_digits(number);
	int count_below = odometer_count_below_length(ndigits);
	string answer = base35_as_odometer(number - count_below, ndigits);
	return answer;
}
