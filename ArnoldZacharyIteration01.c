//Zachary Arnold
//CS2060 SEC002
//Iteration #01 Implementation
//This program models a T-Shirt Sale Fundraiser.
//It will ask an Admin to start the fundraiser by entering the correct pin and setting inital price and percentages to donate.
//The program will then begin taking orders from customers. The customers are required to enter the Size, Color, Payment Information, and lastly if they would like a Receipt.
//After the fundraiser is complete, the Admin can close the fundraiser and get a report on how the fundraiser went.

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

//Constants
const int CORRECTPIN = 81405; //This constant is used to set the admin pin.
const int PINATTEMPTSALLOWED = 3; //This constant is used to allow the admin a specified number of attempts at entering the correct code.
const int LOWESTPRICE = 10; //This constant is used to set the lowest price for the T-Shirt.
const int HIGHESTPRICE = 50; //This constant is used to set the highest price for the T-Shirt.
const int LOWESTPERCENT = 1; //This constant is used to set the lowest percentage of each sale to be donated.
const int HIGHESTPERCENT = 100; //This constant is used to set the highest percentage of each sale to be donated.

//Function Prototypes

bool validatePinEntry(bool fundraiserRunning);
float setTShirtPrice();
unsigned int setFundraiserPercentage();
void receipt(char size, char color, float tShirtPrice,float* const totalSalesPtr, float* const totalDonationsPtr, unsigned int fundraiserPercentage);
void fundraiserReport(float totalSales, float totalDonations);
void scanPaymentCard();
char validateYesNo();
char enterShirtSize();
char enterShirtColor();


int main()
{	bool fundraiserRunning = false; //fundraiserRunning is used for determine whether fundraiser is currently running. Used with PinEntry to start or end the fundraiser.
	float tShirtPrice = 0; //tShirtPrice represents the price of the T-Shirts for sale.
	float totalSales = 0; //totalSales represents the Total Sale Dollars from the fundraiser.
	float totalDonations = 0; //totalDonations represents the Total Donation Dollars from the fundraiser.
	unsigned int fundraiserPercentage = 0; //fundraiserPercentage represents the percentage of each T-Shirt sold that will be donated.
	char size; //size is used for the size of the T-Shirt being purchased.
	char color; //color is used for the color of the T-Shirt being purchased.
	
	//To begin the fundraiser, the validatePinEntry is called to ensure the Admin is the one that is starting the fundraiser. 
	//Should the correct Pin be entered, the fundraiser is started and intial T-shirt price (set by calling setTShirtPrice) 
	//and donation percentages(set by calling setFundraiserPercentage) are set by the Admin.
	if (validatePinEntry(fundraiserRunning) == true) {
		fundraiserRunning = true;
		tShirtPrice = setTShirtPrice();
		fundraiserPercentage = setFundraiserPercentage();
	}
	
	//After the fundraiser is started, a Fundraiser header is displayed to indicate the start of a new purchase.
	//The customer begins inputting details regarding the T-Shirt they intend to purchase, starting with size (set by calling enterShirtSize).
	while (fundraiserRunning == true) {
		puts("\nWelcome to the Fundraiser! Thank you for participating!");
		size = enterShirtSize();
		
		//If the admin wishes to end the fundraiser,the enterShirtSize function allows entry of the letter q.
		//Should the admin want to end the program, he must validate his pin again. If successful, the fundraiser is closed, and a report is generated.
		//Should the admin fail to enter the pin correctly, The fundraiser continues.
		if (size == 'q') {
			if (validatePinEntry(fundraiserRunning) == true) {
				fundraiserRunning = false;
			}
		}

		//In all other cases (a shirt size is entered), a color must then be selected and payment entered.
		//Lastly the customer is offered a receipt.
		else {
			color = enterShirtColor();
			scanPaymentCard();
			receipt(size, color, tShirtPrice, &totalSales, &totalDonations, fundraiserPercentage);
		}
	}
	
	//After the fundraiser is closed, a Report is generated for the Admin, which includes totalSales and totalDonations.
	fundraiserReport(totalSales, totalDonations);
	return 0;
} //End Main

//validatePinEntry is used to validate the Pin that is entered into the program. It ensures that only PINATTEMPTSALLOWED attempts are given to enter the Pin correctly. 
//It also compares the entered Pin to CORRECTPIN. The function returns a bool to say whether the correct Pin was entered or not.
//It is important to note that only 5-digit pin entries count as an attempt.
bool validatePinEntry(bool fundraiserRunning) {
	bool validPin = false; //validPin represents whether the correct Pin was entered or not. Initally set to false to ensure the return doesnt start the program unintentionally.
	int validInput = 0; //Used to validate input from scanf.
	int pin = 0; //Represents the Pin the user enters.
	int entryCount = 0; //Counts the attempts the user has made to enter the pin correctly. It is important to note that only 5-digit pins count as an attempt.

	//The user is asked to enter the 5-digit Pin. A do while loop is used to ensure at least one iteration through.
	puts("Please enter your 5-digit Pin.");
	do {
		validInput = scanf("%d", &pin);
		while (getchar() != '\n'); //Clear the input buffer.
		
		//validInput is used to ensure that a number is entered into pin.
		if (validInput != 1) {
			puts("Please enter a valid 5-digit Pin.");
		}

		//pin is compared to ensure it is at least a 5 digit pin but not greater than a 5 digit pin.
		else if (pin < 10000 || pin > 99999) {
			puts("Please enter 5 digits for the Pin.");
		}

		//Lastly pin is compared to the CORRECTPIN, if it is not correct, the count is incremented.
		//If the count reaches PINATTEMPTSALLOWED attempts, the program is exited if the fundraiser isnt started or resume if it is started.
		else if (pin != CORRECTPIN) {
			entryCount++;
			if (entryCount < PINATTEMPTSALLOWED) {
				puts("That is not correct. Please enter your 5-digit Pin.");
			}
			else if (entryCount == PINATTEMPTSALLOWED && fundraiserRunning == false) {
				printf("You have attempted %d times. Exiting.", PINATTEMPTSALLOWED);
			}
			else if (entryCount == PINATTEMPTSALLOWED && fundraiserRunning == true) {
				printf("You have attempted %d times. Resuming Sale.", PINATTEMPTSALLOWED);
			}
		}

		//If the pin is entered correct, validPin is set to true so the program can start the fundraiser or close the fundraiser.
		else if (pin == CORRECTPIN) {
			puts("That is the correct Pin.\n");
			validPin = true;
		}

	//Loop is done until the correct pin is entered or PINATTEMPTSALLOWED is reached.
	} while (pin != CORRECTPIN && entryCount < PINATTEMPTSALLOWED);
	return validPin;
} //End validPinEntry

//setTShirtPrice is used to set the inital price of the T-Shirt to be sold. It also validates to ensure that the price entered is within a range as well as an actual number.
float setTShirtPrice() {
	int validInput = 0;  //Used to validate input from scanf
	float tShirtPrice = 0; //Represents the T-Shirt price to be returned.

	//This do while loop is used to validate that the price entered is correct.
	do {
		puts("Enter the selling price of the t-shirt:");
		//This do while loop validates the price being entered to ensure it is within range and an actual number.
		do {
			validInput = scanf("%f", &tShirtPrice);
			while (getchar() != '\n'); //Clear the input buffer.
			if (validInput != 1) {
				puts("Please enter a valid price.");
			}
			else if (tShirtPrice < LOWESTPRICE || tShirtPrice > HIGHESTPRICE) {
				puts("Please enter a reasonable price for the Tshirt.");
			}
		} while (tShirtPrice < LOWESTPRICE || tShirtPrice > HIGHESTPRICE);

		//Asks the user to verify the price they entered is correct.
		printf("Is $%.2f the correct price of the shirt?\n", tShirtPrice);
	} while (validateYesNo() == 'n');
	return tShirtPrice;
} //End setTShirtPrice

//setFundraiserPercentage is used to set the percentage of each t-shirt sold that will be donated. 
//It also validates to ensure that the percentage entered is within a range as well as an actual number.
unsigned int setFundraiserPercentage() {
	int validInput = 0;  //Used to validate input from scanf
	unsigned int fundraiserPercentage = 0; //Represents the fundraiser donation percentage to be returned.

	//This do while loop is used to validate that the percentage entered is correct.
	do {
		puts("\nEnter the fundraiser percentage of the t-shirt sales:");
		//This do while loop validates the percentage being entered to ensure it is within range and an actual number.
		do {
			validInput = scanf("%d", &fundraiserPercentage);
			while (getchar() != '\n'); //Clear the input buffer.
			if (validInput != 1) {
				puts("Please enter a valid percentage.");
			}
			else if (fundraiserPercentage < LOWESTPERCENT || fundraiserPercentage > HIGHESTPERCENT) {
				puts("Please enter a percentage between 1-100.");
			}
		} while ( fundraiserPercentage < LOWESTPERCENT || fundraiserPercentage > HIGHESTPERCENT);

		//Asks the user to verify the percentage they entered is correct.
		printf("Is %d%s\n", fundraiserPercentage, "% the correct fundraiser percentage?");
	} while (validateYesNo() == 'n');
	return fundraiserPercentage;
} //End setFundraiserPercentage

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

//enterShirtSize allows the customer to input the size of the T-Shirt they wish to purchase. It also validates the inputted character.
char enterShirtSize() {
	int validInput = 0;  //Used to validate input from scanf
	char size; //Represents the size the T-Shirt being purchased.

	//This do while loop validates the size being entered to ensure it is one of the selections available.
	puts("Enter the shirt size (s)mall, (m)edium, (l)arge, or e(x)tra-large:");
	do {
		validInput = scanf("%c", &size);
		//tolower forces uppercases entries into lowercase to simplify cases.
		size = tolower(size);
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) {
			puts("Invalid value entered. Enter s, m, l, or x.");
		}
		else if (size != 's' && size != 'm' && size != 'l' && size != 'x' && size != 'q') {
			puts("Please enter s, m, l, or x.");
		}
	//Loop is ran until a size that is available is selected or the admin enters q.
	} while (size != 's' && size != 'm' && size != 'l' && size != 'x' && size != 'q');
	
	//The selected size is printed.
	printf("Shirt size is %c\n\n", size);
	return size;
} //End enterShirtSize

//enterShirtSize allows the customer to input the color of the T-Shirt they wish to purchase. It also validates the inputted character.
char enterShirtColor() {
	int validInput = 0;  //Used to validate input from scanf
	char color; //Represents the color the T-Shirt being purchased.
	
	//This do while loop validates the color being entered to ensure it is one of the selections available.
	puts("Enter the shirt color (b)lack or (w)hite");
	do {
		validInput = scanf("%c", &color);
		//tolower forces uppercases entries into lowercase to simplify cases.
		color = tolower(color); 
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) {
			puts("Invalid value entered. Enter b or w.");
		}
		else if (color != 'b' && color != 'w') {
			puts("Please enter b or w.");
		}
	//Loop is ran until a color that is available is selected.
	} while (color != 'b' && color != 'w');

	//The selected color is printed.
	printf("Shirt color is %c\n\n", color);
	return color;
} //End enterShirtColor

//scanPaymentCard is used to input the customers payment information and validate it to ensure it is a 5 digit card number and is an actual number.
void scanPaymentCard() {
	int validInput = 0;  //Used to validate input from scanf
	int cardNumber = 0; //Represents the customers card number being entered.
	
	//This do while loop validates the card number being entered to ensure it is a 5 digit number and an actual number.
	puts("Enter your payment card number:");
	do {
		validInput = scanf("%d", &cardNumber);
		while (getchar() != '\n'); //Clear the input buffer.
		if (validInput != 1) {
			puts("Please enter a valid payment card.");
		}
		else if (cardNumber < 10000 || cardNumber > 99999) {
			puts("Please enter a valid 5 digits for the card number.");
		}
	} while (cardNumber < 10000 || cardNumber > 99999);
} //End scanPaymentCard

//receipt is used to calculate the new totalSales, totalDonations, and offer the customer a receipt.
void receipt(char size, char color, float tShirtPrice, float* const totalSalesPtr,float* const totalDonationsPtr,unsigned int fundraiserPercentage) {
	static int orderNumber = 1000;
	*totalSalesPtr = *totalSalesPtr + tShirtPrice;
	*totalDonationsPtr = *totalDonationsPtr + (tShirtPrice * fundraiserPercentage / 100);
	
	//The customer is offered a receipt. Should they want one, a receipt is printed with details about the shirt they purchased and the overall fundraising efforts.
	puts("\nWould you like a receipt with your order?");
	if (validateYesNo() == 'y') {
		printf("\nReceipt #%d\n", orderNumber);
		printf("T-Shirt Size: %c, Color: %c, Cost: $%.2f\n", toupper(size), toupper(color), tShirtPrice);
		printf("%d%s", fundraiserPercentage, "% of your cost will be donated to charity.\nThank you for your support.\n");
		printf("Current amount raised for charity is $%.2f\n", *totalDonationsPtr);
	}
	orderNumber++;
} //End receipt

//fundraiserReport is used to print a Report for the admin after the fundraiser is completed/closed.
void fundraiserReport(float totalSales, float totalDonations) {
	//Prints totalSales and totalDonations for the admin.
	printf("The total sales for the Fundraiser were $%.2f\n", totalSales);
	printf("The total donations for the Fundraiser were $%.2f\n", totalDonations);
} //End funraiserReport