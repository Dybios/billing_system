/** Source for billing software used in Billdesk
*   Author: Dibyajyoti Samal
*   Created: 24/02/2021
*   Modified: 3/03/2021
*
**/

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/** Main Program
* main: Enables a menu for user to select the required billing option for the user.
*   Returns a 0 on successful operation.
*/
int main () {
	struct inventoryList mProductList[MAX_ITEMS];
	int iInventoryElements = 0, iTempId = 0;
	char sCustName[NAME_BUFFER];    // Customer name variable
	char sBillerName[NAME_BUFFER];  // Biller name variable

	char sTempChoiceBuffer[TMP_BUFFER];
	char sTempIdBuffer[TMP_BUFFER];
	int iMenuChoice = 0;

	// Open 'product_info.txt' file in r/o mode and 'puchased.txt' in r/w mode.
	puts("\nWelcome to store Billdesk.\nLoading Product Database...");
	FILE* pInventoryFilePointer = fopen(INVENTORY_FILE_PATH, "r");
	FILE* pPurchasedFilePointer = fopen(PURCHASED_FILE_PATH, "a+");

	// Check if inventory file exists
	if (pInventoryFilePointer != NULL) {
		puts("Database Loaded!\n");
	}
	else {
		puts("Could not initialise inventory file. Creating required files...\n");
		pInventoryFilePointer = fopen(INVENTORY_FILE_PATH, "w");
		fprintf(pInventoryFilePointer, "Product ID\tName\t\t\tPrice\t\tAvailable QT\n");    // Create file with predefined header
	}

	// Open inventory files again in r/w mode
	pInventoryFilePointer = fopen(INVENTORY_FILE_PATH, "a+");

	// Parse the product_info.txt file to the inventoryList struct
	iInventoryElements = parseInventoryFileToStruct(pInventoryFilePointer, mProductList);

	printf("\n\nEnter the biller's name: ");
	fflush(stdin);
	fgets(sBillerName, sizeof(sBillerName), stdin);
	removeNewlineChar(sBillerName);
	printf("Hi %s!\n", sBillerName);

	// Loop menu options until user specifically decides to exit
	do {
		printf("\nSelect Your Choice:\n1) Generate Bill\t2) Add New Product\n3) Display\t\t4) Exit\n");

		// Take choice input in fgets to avoid buffer overruns later
		fflush(stdin);
		fgets(sTempChoiceBuffer, sizeof(sTempChoiceBuffer), stdin);
		iMenuChoice = atoi(sTempChoiceBuffer);

		switch (iMenuChoice) {
		case BILL_GEN:
			printf("\nEnter Customer Name: ");
			fflush(stdin);
			fgets(sCustName, sizeof(sCustName), stdin);
			removeNewlineChar(sCustName);

			generateBill(pInventoryFilePointer, pPurchasedFilePointer, mProductList, \
							iInventoryElements, sCustName, sBillerName);
			break;

		case ADD_NEW:
			printf("\nEnter the product ID to add/update: ");
			fflush(stdin);
			fgets(sTempIdBuffer, sizeof(sTempIdBuffer), stdin);
			iTempId = atoi(sTempIdBuffer);

			addNewProduct(pInventoryFilePointer, mProductList, iInventoryElements, iTempId);
			break;

		case DISP_INVENTORY:
			displayInventory(pInventoryFilePointer);
			break;

		case EXIT:
			printf("\nThank you for using Billdesk.\n");
			break;

		default:
			printf("\nInvalid Option, try again.\n");
			break;
		}
	} while (iMenuChoice != EXIT);

	fclose(pInventoryFilePointer);
	fclose(pPurchasedFilePointer);
	return 0;
} /* End of main */
