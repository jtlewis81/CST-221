/**
	CST-221 Topic 4 - Memory Management

	Author: Jamie Lewis

	This program performs certain operations on a value input by the user, as directed in the assignment instructions. A decimal number from 0 to 4095 may be input, and will then be converted to binary and hexadecimal before undergoing bitwise manipulation. Those details are listed in the comment header for the transform() function below.
*/

#include <stdio.h>
#include <stdlib.h>

int input;
int binary[32];

/**
	Helper function to read the binary int array
*/
void* print_binary() {

	for(int i = 0; i < 32; i++){
		printf("%d", binary[i]);
	}
	printf("\n");
}

/**
	Convert from Decimal to Binary
	Caution: this overwrites the global variable: binary[32]
	this should be okay, since this program runs to completion after a single user input, and must then be restarted, resetting the values on next run.
*/
void* decimal_to_binary(int input) {
	int i;

	// calculate the conversion
	for(i = 31; input > 0; i--) {
		binary[i] = input % 2;
		input = input / 2;
	}
}

/**
	Convert from Decimal to Hexadecimal
*/
void* decimal_to_hexadecimal(int input) {
	// use C's built-in specifier %x to convert the decimal to hexadecimal
	printf("The hexadecimal conversion is: 0x%08x\n", input);
}

/**
	Processes the assigned transformation of the input value

	Assignment Instruction for this part:
	Transform the input by shifting the number 16 bits to the left,
	then mask out (AND) the bottom 16 bits,
	and finally add (OR) the hex number 0x07FF to produce the final resultant number.
	Display the final result in binary, hexadecimal, and decimal to the console.
*/
void* transform(int input, unsigned int hex) {

	// shift 16 bits left
	input = input << 16;

	// mask out bottom 16 bits
	// (Note: From what I could find, the "bottom" bits are the ones toward the right. Also, I am not sure what the point of this is here, since we just created a new, relatively small number and then mvoed everything 16 bits to the left, leaving the right 16 bits all 0s anyway? Commenting this line out actually does not change the end result, at least as far as I tested it.)
	input &= ~0xFFFF;

	// add the hex number given in instructions
	input = input | hex;

	// read out the final tranformed values of the input
	// binary
	decimal_to_binary(input);
	printf("The final number converted to binary is: ");
	print_binary();
	// hexadecimal
	decimal_to_hexadecimal(input);
	// decimal
	printf("The final number in decimal is: %d\n", input);
}

/**
	Main Program Method/Function
*/
int main() {

	// ask user for input
	printf("Please enter a decimal number between 0 and 4095 and press ENTER:\n");

	// get user input
	scanf("%d", &input);

	// validate input
	while (input < 0 || input > 4095) {
		printf("Please try again!\n");
		scanf("%d", &input);
	}

	// process input
	if(input >= 0 && input < 4096) { // this should always be true at this point
		decimal_to_binary(input); // convert the input value to binary
		printf("The binary conversion is: "); //
		print_binary();
		decimal_to_hexadecimal(input); // this function will print it's own output
		transform(input, 0x07FF); // this function will print it's own outputs
	}

	// end program
	return 0;
}
