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
#include <errno.h>
#include <time.h>

#define COLORS 6  //This constant is used for the character array that holds all the colors.
#define SIZES 5 //This constant is used for the character array that holds all the sizes.


//Constants
const int CORRECTPIN = 81405; //This constant is used to set the admin pin.
const int PINATTEMPTSALLOWED = 4; //This constant is used to allow the admin a specified number of attempts at entering the correct code.
const double LOWESTPRICE = 20; //This constant is used to set the lowest price for the T-Shirt.
const double HIGHESTPRICE = 50; //This constant is used to set the highest price for the T-Shirt.
const double LOWESTPERCENT = 5; //This constant is used to set the lowest percentage of each sale to be donated.
const double HIGHESTPERCENT = 20; //This constant is used to set the highest percentage of each sale to be donated.
const char RECEIPTFILEPATH[] = "C:\\Users\\Zacka\\Documents\\GitHub\\CS2060\\IterationProject\\tshirtapp\\receipt.txt"; //file to output receipts to.
const char SHIRTFILEPATH[] = "C:\\Users\\Zacka\\Documents\\GitHub\\CS2060\\IterationProject\\tshirtapp\\tshirtfund.txt"; //file to read shirt fundraiser data.

//Organization Structure
typedef struct organization
{
	char name[51]; //name of organization
	int id;
	double price; //shirt price for the organization.
	double percentage; //donation percentage of sale for the organization. 
	double sales; //total sales from the organization.
	double donations; //total donations from the organization.
	struct organization *nextPtr; //pointer to next organization in the linked list.
} Organization;

//Function Prototypes
bool validatePinEntry(bool fundraiserRunning);
bool validateDouble(double* const number, char* buff);
bool validateName(Organization* headPtr, char name[]);
double validatePriceAndPercentage(char priceOrPercentage);
char validateYesNo(void);
char enterShirtSizeandColor(char sizeOrColorInput, char sizeOrColor, char sizesAndColors[], const char* sizeAndColorPTR[]);
void receipt(FILE* receiptFilePTR, char size, char color, char name[], Organization* headPtr);
void scanPaymentCard(void);
void fundraiserReport(Organization *headPtr, FILE *tshirtFilePTR);
void insertOrganization(Organization** headPtr, char name[]);
void printOrganizations(Organization* headPtr);


int main() {	
	bool inputOrganizations = true;
	bool fundraiserRunning = false; //fundraiserRunning is used for determine whether fundraiser is currently running. Used with PinEntry to start or end the fundraiser.
	char name[100] = "";
	char size = 'a'; //size is used for the size of the T-Shirt being purchased.
	char sizes[SIZES+1] = "smlxq"; //array to check if a valid size char was inputted.
	char color = 'a'; //color is used for the color of the T-Shirt being purchased.
	char colors[COLORS+1] = "kwrobp"; //array to check if a valid color char was inputted.
	const char* colorPTR[COLORS] = {"black", "white", "red", "orange", "blue", "purple"}; //array of pointers for the available colors.
	const char* sizePTR[SIZES] = { "small", "medium", "large", "extra-large", "admin pin entry" }; //array of pointers for the available sizes.

	Organization* headPtr = NULL;

	FILE* shirtFilePTR = fopen(SHIRTFILEPATH, "w"); //file for reading previous fundraiser data.
	FILE* receiptFilePTR = fopen(RECEIPTFILEPATH, "a"); //file for receipts.

	//To begin the fundraiser, the validatePinEntry is called to ensure the Admin is the one that is starting the fundraiser. 
	//Should the correct Pin be entered, the fundraiser is started and intial T-shirt price (set by calling setTShirtPrice) 
	//and donation percentages(set by calling setFundraiserPercentage) are set by the Admin.
	
	if (validatePinEntry(fundraiserRunning) == true) {
		if (receiptFilePTR == NULL) {
			receiptFilePTR = fopen(RECEIPTFILEPATH, "w"); //creates the file for shirts.
		}
		fundraiserRunning = true;
		do {

			// The admin is prompted to enter the organization's name and then the insertOrganization function is called. The admin can enter multiple organizations.
			do {
				puts("Please enter the name of the organization you start a fundraiser for.");
				fgets(name, 100, stdin);
				if (strlen(name) > 50) {
					puts("The name is to long, please reduce the length to less than 50 characters.");
				}
			} while (strlen(name) > 50);
			insertOrganization(&headPtr, name);
			puts("Would you like to enter another organization?");
			if (validateYesNo() == 'n') {
				inputOrganizations = false;
			}
		} while (inputOrganizations);
	}
	
	//After the fundraiser is started, a Fundraiser header is displayed to indicate the start of a new purchase.
	//The customer begins inputting details regarding the T-Shirt they intend to purchase, starting with size (set by calling enterShirtSize).
	if (headPtr == NULL) {
		puts("There are no organizations participating to start the fundraiser.");
	}
	else {
		while (fundraiserRunning == true) {
			puts("\nWelcome to the Fundraiser! Thank you for participating!");
			printOrganizations(headPtr);
			do {
				puts("Please enter the name of the organization you would like to purchase a shirt from.");
				fgets(name, 100, stdin);
			} while (!validateName(headPtr, name));
			size = enterShirtSizeandColor('s', size, sizes, sizePTR);

			//If the admin wishes to end the fundraiser,the enterShirtSize function allows entry of the letter q.
			//Should the admin want to end the program, he must validate his pin again. If successful, the fundraiser is closed, and a report is generated.
			//Should the admin fail to enter the pin correctly, The fundraiser continues.
			if (size == 'q') {
				if (validatePinEntry(fundraiserRunning) == true) {
					fundraiserRunning = false;
					//After the fundraiser is closed, a Report is generated for the Admin, which includes totalSales and totalDonations.
					fundraiserReport(headPtr, shirtFilePTR);
					fclose(shirtFilePTR);
					fclose(receiptFilePTR);
				}
			}

			//In all other cases (a shirt size is entered), a color must then be selected and payment entered.
			//Lastly the customer is offered a receipt.
			else {
				color = enterShirtSizeandColor('c', color, colors, colorPTR);
				scanPaymentCard();
				receipt(receiptFilePTR, size, color, name, headPtr);
			}
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


//validate float ensures the value that is being validated is in fact an float and is within the of an float. It returns an float value.
bool validateDouble(double *const numberPtr, char* buff) {
	char* end;
	errno = 0;
	buff[strcspn(buff, "\n")] = 0;
	strcat(buff, "\0");
	bool validDbl = false;
	double dblTest = strtod(buff, &end);
	if (end == buff) {
		fprintf(stderr, "%s: not a floating-point number\n", buff);
	}
	else if ('\0' != *end) {
		fprintf(stderr, "%s: this value has extra characters at end of input: %s\n", buff, end);
	}
	else if ((DBL_MIN == dblTest || DBL_MAX == dblTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	else {
		*numberPtr = dblTest;
		validDbl = true;
	}
	return validDbl;
}

//validatePriceAndPercentage intakes a string and character to determine what validation is neccessary. 
//Depending on what it needs to validate it will compare the value found to the lowest and highest values allowed for the price or percentage.
//It returns the number found.
double validatePriceAndPercentage(char priceOrPercentage) {
	char numberString[100];
	double number = 0;
	do {
		if (priceOrPercentage == '%') {
			do
			{
				printf("Please enter the percentage to donate. Range: %.2lf%% - %.2lf%%\n", LOWESTPERCENT, HIGHESTPERCENT);
				fgets(numberString, 100, stdin);
				if (validateDouble(&number, numberString)) {
					if (number > HIGHESTPERCENT) {
						printf("%.2lf is to high to donate.\n", number);
					}
					else if (number < LOWESTPERCENT && number > 0) {
						printf("%.2lf is to low to donate.\n", number);
					}
				}
				else {
					puts("That is not a valid number.");
				}
			} while (number < LOWESTPERCENT || number > HIGHESTPERCENT);
			printf("Is %.2lf%% the correct percentage to donate?\n", number);
		}
		else if (priceOrPercentage == '$') {
			do
			{
				printf("Please enter the t-shirt price. Range: $%.2lf - $%.2lf\n", LOWESTPRICE, HIGHESTPRICE);
				fgets(numberString, 100, stdin);
				if (validateDouble(&number, numberString)) {
					if (number > HIGHESTPRICE) {
						printf("%.2lf is to high for the price.\n", number);
					}
					else if (number < LOWESTPRICE && number > 0) {
						printf("%.2lf is to low for the price.\n", number);
					}
				}
				else {
					puts("That is not a valid number.");
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
char enterShirtSizeandColor(char sizeOrColorInput, char sizeOrColor, char sizesAndColors[], const char* sizeAndColorPTR[]) {
	int validInput = 0;  //Used to validate input from scanf
	
	do {
		if (sizeOrColorInput == 's') {
			puts("Enter the shirt size (s)mall, (m)edium, (l)arge, or e(x)tra-large:");
		}
		else if (sizeOrColorInput == 'c') {
			puts("Enter the shirt color blac(k), (w)hite, (r)ed, (o)range, (b)lue, or (p)urple.");
		}
		validInput = scanf("%c", &sizeOrColor);
		sizeOrColor = tolower(sizeOrColor);
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) {
			puts("Invalid value entered.");
		}
		else if (strchr(sizesAndColors, sizeOrColor) == NULL && sizeOrColorInput == 's') {
			puts("Please enter s for (s)mall, m for (m)edium, l for (l)arge, or x for e(x)tra-large.");
		}
		else if (strchr(sizesAndColors, sizeOrColor) == NULL && sizeOrColorInput == 'c') {
			puts("Please enter k for blac(k), w for (w)hite, r for (r)ed, o for (o)range, b for (b)lue, or p for(p)urple.");
		}
	} while (strchr(sizesAndColors, sizeOrColor) == NULL);

	if (sizeOrColorInput == 's') {
		printf("Shirt size is %c, %s.\n\n", sizeOrColor, sizeAndColorPTR[(int)(strchr(sizesAndColors, sizeOrColor) - sizesAndColors)]);
	}
	else if (sizeOrColorInput == 'c') {
		printf("Shirt color is %c, %s.\n\n", sizeOrColor, sizeAndColorPTR[(int)(strchr(sizesAndColors, sizeOrColor) - sizesAndColors)]);
	}

	return sizeOrColor;
}

//scanPaymentCard is used to input the customers payment information and validate it to ensure it is a 5 digit card number and is an actual number.
void scanPaymentCard() {
	bool validCard = true;
	char cardNumber[30] = ""; //Represents the customers card number being entered.
	char* token;

	//This do while loop validates the card number being entered to ensure it is a 5 digit number and an actual number.
	puts("Enter your payment card number:");
	do {
		validCard = true;
		fgets(cardNumber, 30, stdin);
		if (strlen(cardNumber) != 20) {
			validCard = false;
		}
		if (strlen(cardNumber) > 30) {
			while (getchar() != '\n');
		}
		if (cardNumber[4] != '-' || cardNumber[9] != '-' || cardNumber[14] != '-') {
			validCard = false;
		}
		token = strtok(cardNumber, "-");
		while (token != NULL && validCard != false) {
			for (int i = 0; i < 4; i++) {
				if (!isdigit(token[i])) {
					validCard = false;
				} 
			}
			token = strtok(NULL, "-");
		}
		

		if (validCard == false) {
			puts("Please enter a valid card number.");
		}
	} while (validCard == false);
} //End scanPaymentCard

//receipt is used to calculate the new totalSales, totalDonations, and offer the customer a receipt.
void receipt(FILE* receiptFilePTR, char size, char color, char name[], Organization* headPtr) {
	static int orderNumber = 1000;
	Organization* currentPtr = headPtr;
	while (strcmp(name, currentPtr->name) != 0) {
		currentPtr = currentPtr->nextPtr;
	}
	
	currentPtr->sales = currentPtr->sales + currentPtr->price;
	currentPtr->donations = currentPtr->donations + (currentPtr->price * currentPtr->percentage / 100);

	//The customer is offered a receipt. Should they want one, a receipt is printed with details about the shirt they purchased and the overall fundraising efforts.
	puts("\nWould you like a receipt with your order?");
	if (validateYesNo() == 'y') {
		fprintf(receiptFilePTR, "Receipt #%d\n", orderNumber);
		fprintf(receiptFilePTR, "Organization: %s", currentPtr->name);
		fprintf(receiptFilePTR, "T-Shirt Size: %c, Color: %c, Cost: $%.2f\n", toupper(size), toupper(color), currentPtr->price);
		fprintf(receiptFilePTR, "%.2lf%s", currentPtr->percentage, "% of your cost will be donated to charity.\nThank you for your support.\n");
		fprintf(receiptFilePTR, "Current amount raised for charity is $%.2lf\n\n", currentPtr->donations);
	}
	orderNumber++;
} //End receipt

//fundraiserReport is used to print a Report for the admin after the fundraiser is completed/closed.
void fundraiserReport(Organization* headPtr, FILE* tshirtFilePTR) {

	Organization* currentPtr = headPtr;
	if (headPtr != NULL) {
		fprintf(tshirtFilePTR, "Fundraiser Reports for each organization:\n\n");
		while (currentPtr != NULL) {
			fprintf(tshirtFilePTR, "Organization: %s", currentPtr->name);
			fprintf(tshirtFilePTR, "%.2lf$ were the total sales for this organization.\n", currentPtr->sales);
			fprintf(tshirtFilePTR, "%.2lf$ were the total donations for the organization.\n", currentPtr->donations);
			currentPtr = currentPtr->nextPtr;
		}
	}
	else {
		printf("There were no fundraisers to print for the report.");
	}
}

//The insertOrganization function starts and adds nodes to the linkedlist of organization structures.
void insertOrganization(Organization** headPtr, char name[])
{
	//First allocate memory to the new Organization.
	Organization* organizationPtr = malloc(sizeof(Organization));

	//Ensure that memory was allocated
	if (organizationPtr != NULL)
	{
		//Input all data into the new Organization from the incoming data.
		
		strcpy(organizationPtr->name,name);
		organizationPtr->price = validatePriceAndPercentage('$');
		organizationPtr->percentage = validatePriceAndPercentage('%');
		organizationPtr->sales = 0;
		organizationPtr->donations = 0;
		organizationPtr->nextPtr = NULL;

		//Two new pointers to Organizations(nodes) in the linked list to allow traversal through the list.
		Organization* previousPtr = NULL;
		Organization* currentPtr = *headPtr;

		//While loop iterates to the end of the list and adds the new organization.
		while (currentPtr != NULL)
		{
			previousPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
		}
		if (previousPtr == NULL)
		{
			*headPtr = organizationPtr;
		}
		else
		{
			previousPtr->nextPtr = organizationPtr;
		}
		organizationPtr->nextPtr = currentPtr;
	}
	//If memory allocation was unsucessful, displays message for user.
	else
	{
		puts("No memory to create a new Organization.");
	}

} //End insertOrganization

//printOrganizations function iterates through the linked list and prints out each orgnaization
void printOrganizations(Organization* headPtr)
{
	//New pointer to traverse the list without losing the list.
	Organization* currentPtr = headPtr;

	//If there is a list, it is printed.
	if (headPtr != NULL)
	{
		puts("The organizations that are participating are :");
		//While loop iterates through the entire list.
		while (currentPtr != NULL)
		{
			printf("%s", currentPtr->name);
			currentPtr = currentPtr->nextPtr;
		}
	}
	else
	{
		puts("There are no organizations participating in this fundraiser.");
	}
} //End printPets

//validateName ensure the ogranization the user wants to buy from exists within the organizations that are participating. returns a boolean to whether the organization exists.
bool validateName(Organization* headPtr, char name[]) {
	bool foundName = false;
	Organization* currentPtr = headPtr;
	
	//while loop looks for an exact match of the name the user inputted and and the name within the linked list.
	while (currentPtr != NULL && strcmp(name, currentPtr->name) != 0 ) {
		currentPtr = currentPtr->nextPtr;
	}
	if (currentPtr != NULL) {
		foundName = true;
	}
	return foundName;
}



//Reflection
//I am extremely happy with my work. I worked hard to get updates done to iteration 2 and make the program function and then I charged right into iteration 3.
//I ran into an issue when writing my algorithm invlolving multiple fundraisers. My first assumption was there was going to be a definitive number of organizations.
//Upon further review I realized the assignment did not specify, so I chalked up a new idea and worked with yes/no (which I already had modularized).
//With that in mind, I am extremely pleased because my program can handle as many organizations the admin wants to input rather than having to know ahead of time. 

