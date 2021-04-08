/** Source file for adding new products
*   Author: Dibyajyoti Samal
*   Created: 24/02/2021
*   Modified: 3/03/2021
*/

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Static Function Prototypes */
static int updateStructValues(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, int iArrayElement, int iNumberOfElements);

/**
* addNewProduct: Adds new product in the 'product_info.txt' file if a new product ID is found.
*   Takes the 'product_info.txt' file pointer, inventoryList struct, the number of elements and
*   the entered product ID as arguments. If product ID is found to exist, it updates fields.
*   Returns 0 on success.
*/
int addNewProduct(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, \
                    int iInventoryElements, int iTempId) {
	char sTempName[NAME_BUFFER];
	char sTempBuffer[TMP_BUFFER];
	float fTempPrice = 0;
	int iTempQuant = 0;

	short int flag = 0, iArrayElement = 0;

	// Check if given product ID exists
 	for (int iCount = 0; iCount < iInventoryElements; iCount++) {
		if (iTempId == mDummyInventoryList[iCount].iProductId) {
			flag = 1;
			printf("\nEntry Detected. Updating Values...");
			iArrayElement = iCount;
		}
	}

	// If product ID does not exist, trigger new input fields
	if (flag == 0) {
		printf("\nEnter following values for Product %d:", iTempId);

		printf("\nName: ");
		fflush(stdin);
		fgets(sTempName, sizeof(sTempName), stdin);
		removeNewlineChar(sTempName);

		// Read both values as strings and convert to appropriate formats.
		// This is to avoid any stray buffer overruns.
		printf("Unit Price: ");
		fflush(stdin);
		fgets(sTempBuffer, sizeof(sTempBuffer), stdin);
		fTempPrice = atof(sTempBuffer);

		printf("Current Quantity: ");
		fflush(stdin);
		fgets(sTempBuffer, sizeof(sTempBuffer), stdin);
		iTempQuant = atoi(sTempBuffer);

		// Clean formatting and write to 'product_info.txt' file
		if ((lengthOfString(sTempName)) <= 7)
			fprintf(pInventoryFilePointer, "%d\t\t%s\t\t\t%.2f\t\t%d\n", iTempId, sTempName, fTempPrice, iTempQuant);
		else
			fprintf(pInventoryFilePointer, "%d\t\t%s\t\t%.2f\t\t%d\n", iTempId, sTempName, fTempPrice, iTempQuant);

		printf("\nWritten/Added to file 'product_info.txt'.\n");
	}
	else {
		updateStructValues(pInventoryFilePointer, mDummyInventoryList, iArrayElement, iInventoryElements);
	}

	// Update the product_info.txt file struct values
	parseInventoryFileToStruct(pInventoryFilePointer, mDummyInventoryList);
	return 0;
} /* End of addNewProduct */

/**
* updateStructValues: Create an internal/static function for updating inventoryList struct.
*   Takes the 'product_info.txt' file pointer, inventoryList struct, the array element to modify,
*   and the total number of elements in the inventory structure as elements.
*   Returns 0 on success.
*/
static int updateStructValues(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, \
                                                            int iArrayElement, int iInventoryElements) {
	int iChoice = 0;
	char sTempBuffer[TMP_BUFFER];

	printf("\nChoose the field you want to update:");
	printf("\n1) Name\t\t2) Unit Price\t\t3) Quantity\n");

	// Take choice input in fgets to avoid unwanted buffer overruns later
	fflush(stdin);
	fgets(sTempBuffer, sizeof(sTempBuffer), stdin);
	iChoice = atoi(sTempBuffer);

	switch (iChoice) {
	case 1:
		printf("\nEnter updated product name: ");
		fflush(stdin);
		fgets(mDummyInventoryList[iArrayElement].sProductName, sizeof(mDummyInventoryList[iArrayElement].sProductName), stdin);
		removeNewlineChar(mDummyInventoryList[iArrayElement].sProductName);
		printf("\nName updated successfully.\n");
		break;
	case 2:
		printf("\nEnter updated unit price: ");
		fflush(stdin);
		fgets(sTempBuffer, sizeof(sTempBuffer), stdin);
		mDummyInventoryList[iArrayElement].fProductPrice = atof(sTempBuffer);
		printf("\nPrice updated successfully.\n");
		break;
	case 3:
		printf("\nEnter updated product quantity: ");
		fflush(stdin);
		fgets(sTempBuffer, sizeof(sTempBuffer), stdin);
		mDummyInventoryList[iArrayElement].iAvailableQuant = atoi(sTempBuffer);
		printf("\nQuantity updated successfully.\n");
		break;
	default:
		printf("\nInvalid Option. Try again.\n");
		break;
	}

	refreshInventoryFile(pInventoryFilePointer, mDummyInventoryList, iInventoryElements);
	printf("\nValues have been updated.\n");

	return 0;
}
