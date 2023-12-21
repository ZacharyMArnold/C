//Zachary Arnold
//CS2060 SEC002
//Iteration #02 Implementation
//This program models a T-Shirt Sale Fundraiser.
//It will ask an Admin to start the fundraiser by entering the correct pin and the inital price and percentage to donate will be read from a file.
//The previous amount of sales and donations will also be read in an updated for this current fundraiser.
//The program will then begin taking orders from customers. The customers are required to enter the Size, Color, Payment Information, and lastly if they would like a Receipt.
//After the fundraiser is complete, the Admin can close the fundraiser and get a report on how the fundraiser went which will be saved to a file along with all receipts that were requested.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define COLORS 6  //This constant is used for the character array that holds all the colors.
#define SIZES 5 //


//Constants
const int CORRECTPIN = 81405; //This constant is used to set the admin pin.
const int PINATTEMPTSALLOWED = 4; //This constant is used to allow the admin a specified number of attempts at entering the correct code.
const float LOWESTPRICE = 20; //This constant is used to set the lowest price for the T-Shirt.
const float HIGHESTPRICE = 50; //This constant is used to set the highest price for the T-Shirt.
const int LOWESTPERCENT = 5; //This constant is used to set the lowest percentage of each sale to be donated.
const int HIGHESTPERCENT = 20; //This constant is used to set the highest percentage of each sale to be donated.
const char RECEIPTFILEPATH[] = "C:\\Users\\Zacka\\source\\repos\\CS2060IterationAndImplementation\\CS2060IterationAndImplementation\\tshirtapp\\receipt.txt"; //file to output receipts to.
const char SHIRTFILEPATH[] = "C:\\Users\\Zacka\\source\\repos\\CS2060IterationAndImplementation\\CS2060IterationAndImplementation\\tshirtapp\\tshirtfund.txt"; //file to read shirt fundraiser data.
const char INPUTFILEPATH[] = "C:\\Users\\Zacka\\source\\repos\\CS2060IterationAndImplementation\\CS2060IterationAndImplementation\\tshirtapp\\input.txt";
const char FOLDERFILEPATH[] = "C:\\Users\\Zacka\\source\\repos\\CS2060IterationAndImplementation\\CS2060IterationAndImplementation\\tshirtapp\\";

//Function Prototypes
bool validatePinEntry(bool fundraiserRunning);
float validatePriceAndPercentage(FILE* inputFilePTR, char priceOrPercentage);
void receipt(FILE *receiptFilePTR, char size, char color, float tShirtPrice,float* const totalSalesPtr, float* const totalDonationsPtr, unsigned int fundraiserPercentage);
void fundraiserReport(FILE* shirtFilePTR,float totalSales, float totalDonations);
void scanPaymentCard();
char validateYesNo();
char enterShirtSizeandColor(char sizecolorinput, char sizeorcolor, char sizesandcolors[], const char* sizeandcolorPTR[]);


int main() {	
	bool fundraiserRunning = false; //fundraiserRunning is used for determine whether fundraiser is currently running. Used with PinEntry to start or end the fundraiser.
	float tShirtPrice = 0; //tShirtPrice represents the price of the T-Shirts for sale.
	float totalSales = 0; //totalSales represents the Total Sale Dollars from the fundraiser.
	float totalDonations = 0; //totalDonations represents the Total Donation Dollars from the fundraiser.
	int fundraiserPercentage = 0; //fundraiserPercentage represents the percentage of each T-Shirt sold that will be donated.
	char totalDonationsInput[75]; //string for previous donations.
	char totalSalesInput[75]; //string for previous sales.
	char size = 'a'; //size is used for the size of the T-Shirt being purchased.
	char sizes[SIZES+1] = "smlxq"; //array to check if a valid size char was inputted.
	const char* sizePTR[SIZES] = {"small", "medium", "large", "extra-large", "admin pin entry"}; //array of pointers for the available sizes.
	char color = 'a'; //color is used for the color of the T-Shirt being purchased.
	char colors[COLORS+1] = "kwrobp"; //array to check if a valid color char was inputted.
	const char* colorPTR[COLORS] = {"black", "white", "red", "orange", "blue", "purple"}; //array of pointers for the available colors.
	char* end; //pointer for getting the sales and donations from last event.
	FILE* inputFilePTR = fopen(INPUTFILEPATH, "r"); //file for reading in data.
	FILE *shirtFilePTR = fopen(SHIRTFILEPATH, "r+"); //file for reading previous fundraiser data.
	FILE *receiptFilePTR = fopen(RECEIPTFILEPATH, "a"); //file for receipts.

	//To begin the fundraiser, the validatePinEntry is called to ensure the Admin is the one that is starting the fundraiser. 
	//Should the correct Pin be entered, the fundraiser is started and intial T-shirt price (set by calling setTShirtPrice) 
	//and donation percentages(set by calling setFundraiserPercentage) are set by the Admin.
	if (validatePinEntry(fundraiserRunning) == true) {
		if ((shirtFilePTR == NULL) && (receiptFilePTR == NULL)) {
			puts("One or more files could not be opened. Exiting.");
		}
		else {
			//fgets reads in the values for totalSales and totalDonations from the last event and updates current values to continue the fundraiser.
			fundraiserRunning = true;
			fgets(totalSalesInput, 75, shirtFilePTR);
			fgets(totalDonationsInput, 75, shirtFilePTR);
			totalSales = (float)strtod(totalSalesInput, &end);
			totalDonations = (float)strtod(totalDonationsInput, &end);
			tShirtPrice = validatePriceAndPercentage(inputFilePTR, '$');
			fundraiserPercentage = (int)validatePriceAndPercentage(inputFilePTR, '%');
		}
	}
	
	//After the fundraiser is started, a Fundraiser header is displayed to indicate the start of a new purchase.
	//The customer begins inputting details regarding the T-Shirt they intend to purchase, starting with size (set by calling enterShirtSize).
	while (fundraiserRunning == true) {
		puts("\nWelcome to the Fundraiser! Thank you for participating!");
		size = enterShirtSizeandColor('s', size, sizes, sizePTR);
		
		//If the admin wishes to end the fundraiser,the enterShirtSize function allows entry of the letter q.
		//Should the admin want to end the program, he must validate his pin again. If successful, the fundraiser is closed, and a report is generated.
		//Should the admin fail to enter the pin correctly, The fundraiser continues.
		if (size == 'q') {
			if (validatePinEntry(fundraiserRunning) == true) {
				fundraiserRunning = false;
				//After the fundraiser is closed, a Report is generated for the Admin, which includes totalSales and totalDonations.
				if ((shirtFilePTR = fopen(SHIRTFILEPATH, "w")) == NULL) {
					puts("Files could not be opened. Exiting.");
				}
				else {
					fundraiserReport(shirtFilePTR, totalSales, totalDonations);
					fclose(shirtFilePTR);
					fclose(receiptFilePTR);
				}
			}
		}

		//In all other cases (a shirt size is entered), a color must then be selected and payment entered.
		//Lastly the customer is offered a receipt.
		else {
			color = enterShirtSizeandColor('c', color, colors, colorPTR);
			scanPaymentCard();
			receipt(receiptFilePTR ,size, color, tShirtPrice, &totalSales, &totalDonations, fundraiserPercentage);
		}
	}
	return 0;
} //End Main

//validatePinEntry is used to validate the Pin that is entered into the program. It ensures that only PINATTEMPTSALLOWED attempts are given to enter the Pin correctly. 
//It also compares the entered Pin to CORRECTPIN. The function returns a bool to say whether the correct Pin was entered or not.
bool validatePinEntry(bool fundraiserRunning) {
	bool validPin = false; //validPin represents whether the correct Pin was entered or not. Initally set to false to ensure the return doesnt start the program unintentionally.
	int validInput = 0; //Used to validate input from scanf.
	int pin = 0; //Represents the Pin the user enters.
	int entryCount = 0; //Counts the attempts the user has made to enter the pin correctly.

	
	//The user is asked to enter the 5-digit Pin. A do while loop is used to ensure at least one iteration through.
	do {
		puts("Please enter your Pin.");
		validInput = scanf("%d", &pin);
		while (getchar() != '\n'); //Clear the input buffer.
		
		puts("");

		//validInput is used to ensure that a number is entered into pin.
		if (validInput != 1) {
			puts("Invalid Pin entered.");
		}

		//pin is compared to ensure it is at least a 5 digit pin but not greater than a 5 digit pin.
		else if (pin < 10000 || pin > 99999) {
			puts("Invalid Pin entered.");
		}
		//Lastly pin is compared to the CORRECTPIN, if it is not correct, the count is incremented.
		//If the count reaches PINATTEMPTSALLOWED attempts, the program is exited if the fundraiser isnt started or resume if it is started.
		else if (pin != CORRECTPIN || entryCount == 3) {
			if (entryCount < PINATTEMPTSALLOWED) {
				puts("Invalid pin entered.");
			}
		}
		else if (pin == CORRECTPIN) {
			puts("That is the correct Pin.\n");
			validPin = true;
		}
		entryCount++;
		if (entryCount == PINATTEMPTSALLOWED && fundraiserRunning == false) {
				printf("\nYou have attempted %d times. Exiting.", PINATTEMPTSALLOWED);
		}
		else if (entryCount == PINATTEMPTSALLOWED && fundraiserRunning == true) {
				printf("\nYou have attempted %d times. Resuming Sale.", PINATTEMPTSALLOWED);
		}
		//If the pin is entered correct, validPin is set to true so the program can start the fundraiser or close the fundraiser.
		
	//Loop is done until the correct pin is entered or PINATTEMPTSALLOWED is reached.
	} while (pin != CORRECTPIN && entryCount < PINATTEMPTSALLOWED);
	return validPin;
} //End validPinEntry

//validate int ensures the value that is being validated is in fact an integer and is within the of an integer. It returns an integer value.
int validateInt(const char* buff) {
	char* end;
	errno = 0;
	int validInt = 0;
	long intTest = strtol(buff, &end, 10);
	if (end == buff) {
		fprintf(stderr, "%s: not a decimal number\n", buff);
	}
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	else if (intTest > INT_MAX) {
		fprintf(stderr, "%ld: greater than INT_MAX\n", intTest);
	}
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%ld less than INT_MIN\n", intTest);
	}
	else {
		validInt = (int)intTest;
	}
	return validInt;
}

//validate float ensures the value that is being validated is in fact an float and is within the of an float. It returns an float value.
float validateFloat(const char* buff) {
	char* end;
	errno = 0;
	float validFlt = 0;
	double fltTest = strtod(buff, &end);
	if (end == buff) {
		fprintf(stderr, "%s: not a floating-point number\n", buff);
	}
	else if ((DBL_MIN == fltTest || DBL_MAX == fltTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	else if (fltTest > FLT_MAX) {
		fprintf(stderr, "%lf greater than DBL_MAX\n", fltTest);
	}
	else if (fltTest < FLT_MIN) {
		fprintf(stderr, "%lf less than DBL_MIN\n", fltTest);
	}
	else {
		validFlt = (float)fltTest;
	}
	return validFlt;
}

//validatePriceAndPercentage intakes a string and character to determine what validation is neccessary. 
//Depending on what it needs to validate it will compare the value found to the lowest and highest values allowed for the price or percentage.
//It returns the number found.
float validatePriceAndPercentage(FILE* inputFilePTR, char priceOrPercentage) {
	char inputString[100];
	float number = 0;
	do {
		if (priceOrPercentage == '%') {
			do
			{
				fgets(inputString, 100, inputFilePTR);
				number = (float)validateInt(inputString);
				if (number > HIGHESTPERCENT) {
					printf("%d is to high to donate.\n", (int)number);
				}
				else if (number < LOWESTPERCENT && number > 0) {
					printf("%d is to low to donate.\n", (int)number);
				}
			} while (number < LOWESTPERCENT || number > HIGHESTPERCENT);
			printf("Is %%%d the correct percentage to donate?\n", (int)number);
		}
		else if (priceOrPercentage == '$') {
			do
			{
				fgets(inputString, 100, inputFilePTR);
				number = validateFloat(inputString);
				if (number > HIGHESTPRICE) {
					printf("%.2lf is to high for the price.\n", number);
				}
				else if (number < LOWESTPRICE && number > 0) {
					printf("%.2lf is to low for the price.\n", number);
				}
			} while (number < LOWESTPRICE || number > HIGHESTPRICE);
			printf("Is $%.2lf the correct price of the shirt?\n", number);
		}
		
	} while (validateYesNo() == 'n');
	return number;
}

//validateYesNo is used allows the user to input a yes or no in response to a question that was asked.
char validateYesNo() {
	int validInput = 0;  //Used to validate input from scanf
	char validYesNo; //Represents yes or no via a character. This will be returned.

	//The user is asked to enter y or n. A do while loop is used to ensure the value inputted is either a y/Y or n/N.
	puts("Please enter (y)es or (n)o:");
	do {
		validInput = scanf("%c", &validYesNo);
		//tolower forces uppercases entries into lowercase to simplify cases.
		validYesNo = tolower(validYesNo);
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) { 
			puts("Invalid value entered. Enter y or n.");
		}
		else if (validYesNo != 'y' && validYesNo != 'n') {
			puts("Invalid value entered. Enter y or n.");
		}
	//Loop is repeated until a y or a n is entered.
	} while (validYesNo != 'y' && validYesNo != 'n');
	return  validYesNo;
} //End validateYesNo

//enterShirtSizeandColor looks at what needs to be checked and compares the input to an array of available colors. If found, the input is valid and the program continues otherwise,
//the user is asked to reinput the color they desire.
char enterShirtSizeandColor(char sizeorcolorinput, char sizeorcolor, char sizesandcolors[], const char* sizeandcolorPTR[]) {
	int validInput = 0;  //Used to validate input from scanf
	
	do {
		if (sizeorcolorinput == 's') {
			puts("Enter the shirt size (s)mall, (m)edium, (l)arge, or e(x)tra-large:");
		}
		else if (sizeorcolorinput == 'c') {
			puts("Enter the shirt color blac(k), (w)hite, (r)ed, (o)range, (b)lue, or (p)urple.");
		}
		validInput = scanf("%c", &sizeorcolor);
		sizeorcolor = tolower(sizeorcolor);
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) {
			puts("Invalid value entered.");
		}
		else if (strchr(sizesandcolors, sizeorcolor) == NULL && sizeorcolorinput == 's') {
			puts("Please enter s for (s)mall, m for (m)edium, l for (l)arge, or x for e(x)tra-large.");
		}
		else if (strchr(sizesandcolors, sizeorcolor) == NULL && sizeorcolorinput == 'c') {
			puts("Please enter k for blac(k), w for (w)hite, r for (r)ed, o for (o)range, b for (b)lue, or p for(p)urple.");
		}
	} while (strchr(sizesandcolors, sizeorcolor) == NULL);

	if (sizeorcolorinput == 's') {
		printf("Shirt size is %c, %s.\n\n", sizeorcolor, sizeandcolorPTR[(int)(strchr(sizesandcolors, sizeorcolor) - sizesandcolors)]);
	}
	else if (sizeorcolorinput == 'c') {
		printf("Shirt color is %c, %s.\n\n", sizeorcolor, sizeandcolorPTR[(int)(strchr(sizesandcolors, sizeorcolor) - sizesandcolors)]);
	}

	return sizeorcolor;
}

//scanPaymentCard is used to input the customers payment information and validate it to ensure it is a 5 digit card number and is an actual number.
void scanPaymentCard() {
	int validInput = 0;  //Used to validate input from scanf
	char cardNumber[20] = ""; //Represents the customers card number being entered.
	int firstFour = 0, secondFour = 0, thirdFour = 0, lastFour = 0;
	char* token;

	//This do while loop validates the card number being entered to ensure it is a 5 digit number and an actual number.
	puts("Enter your payment card number:");
	do {
		validInput = scanf("%19s", cardNumber);
		while (getchar() != '\n'); //Clear the input buffer.
		token = strtok(cardNumber, "-");
		firstFour = atoi(token);
		printf("%c", token);
		token = strtok(NULL, "-");
		secondFour = atoi(token);
		token = strtok(NULL, "-");
		thirdFour = atoi(token);
		token = strtok(NULL, "-");
		lastFour = atoi(token);
		if (validInput != 1) {
			puts("Please enter a valid payment card.");
		}
		else if (firstFour < 1000 || firstFour > 9999 || secondFour < 1000 || secondFour > 9999 || thirdFour < 1000 || thirdFour > 9999 || lastFour < 1000 || lastFour > 9999) {
			puts("Please enter a valid card number.");
		}
	} while ((firstFour < 1000 || firstFour > 9999) && (secondFour < 1000 || secondFour > 9999) && (thirdFour < 1000 || thirdFour > 9999) && (lastFour < 1000 || lastFour > 9999));
} //End scanPaymentCard

//receipt is used to calculate the new totalSales, totalDonations, and offer the customer a receipt.
void receipt(FILE *receiptFilePTR, char size, char color, float tShirtPrice, float* const totalSalesPtr,float* const totalDonationsPtr,unsigned int fundraiserPercentage) {
	static int orderNumber = 1000;
	*totalSalesPtr = *totalSalesPtr + tShirtPrice;
	*totalDonationsPtr = *totalDonationsPtr + (tShirtPrice * fundraiserPercentage / 100);
	
	//The customer is offered a receipt. Should they want one, a receipt is printed with details about the shirt they purchased and the overall fundraising efforts.
	puts("\nWould you like a receipt with your order?");
	if (validateYesNo() == 'y') {
		fprintf(receiptFilePTR, "Receipt #%d\n", orderNumber);
		fprintf(receiptFilePTR, "T-Shirt Size: %c, Color: %c, Cost: $%.2f\n", toupper(size), toupper(color), tShirtPrice);
		fprintf(receiptFilePTR, "%d%s", fundraiserPercentage, "% of your cost will be donated to charity.\nThank you for your support.\n");
		fprintf(receiptFilePTR, "Current amount raised for charity is $%.2f\n\n", *totalDonationsPtr);
	}
	orderNumber++;
} //End receipt

//fundraiserReport is used to print a Report for the admin after the fundraiser is completed/closed.
void fundraiserReport(FILE *shirtFilePTR,float totalSales, float totalDonations) {
	//Prints totalSales and totalDonations for the admin.
	fprintf(shirtFilePTR,"%.2f$ were the total sales for the Fundraiser.\n", totalSales);
	fprintf(shirtFilePTR,"%.2f$ were the total donations for the Fundraiser.\n", totalDonations);
} //End funraiserReport


//Reflection
//1. I am extremely happy with the size and color function. I like that it compares it to an array which can easily be updated in the future without changing to much code.

//2. I used debugging a bunch when trying to read in values from the previous fundraising results.
//2. I hope I did it correctly, because I had to change the output of the report to have the number in the front of the string.

//3. The program really helped me understand comparison of a character to an array of characters. I like how the code looks as well.

//4. The first thing I did was ensure my variables made sense, each name is indicative to what it will be doing. 
//4. I made my code as modular as possible.
//4. I used many constants in my code to ensure future updating and changing will be easy.
//4. With this iteration we also reduced our code duplication and enhanced code reuse with the validateInt and validateFloat functions.
