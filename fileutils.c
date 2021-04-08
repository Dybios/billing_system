/** File Utilities Funtions
*   Used by funtions in other files for some
*   important operations on files or arrays/strings.
*   Author: Dibyajyoti Samal
*   Created: 24/02/2021
*   Modified: 1/03/2021
*/

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/**
* parseInventoryFileToStruct: Parse the product_info (inventory) file to the product struct
*     that was passed from main function.
*/

int parseInventoryFileToStruct(FILE* pInventoryFilePointer, struct inventoryList* mDummyList) {
	int iMaxBuffer = bufferLengthRequired(pInventoryFilePointer);

	char cBuffer[iMaxBuffer+1];
	int iTmpCounter = 0;

	// Read the common header line and save it as a string
	fgets(cBuffer, sizeof(cBuffer), pInventoryFilePointer);

	// Add the other buffer values to struct array
	while (fgets(cBuffer, sizeof(cBuffer), pInventoryFilePointer) != NULL) {
		sscanf(cBuffer, "%d\t\t%[^\t]\t%f\t\t%d", &mDummyList[iTmpCounter].iProductId, \
							mDummyList[iTmpCounter].sProductName, \
							&mDummyList[iTmpCounter].fProductPrice, \
							&mDummyList[iTmpCounter].iAvailableQuant);
		iTmpCounter++;
	}

    // Return the total number of array elements present (iterated through)
	return iTmpCounter;
}

/**
* parsePurchasedFileToStruct: Parse the purchased.txt (purchased) file to the purchased struct
*     that was passed from the generateBill source file.
*/

int parsePurchasedFileToStruct(FILE* pPurchasedFilePointer, struct purchasedList* mDummyList) {
	int iMaxBuffer = bufferLengthRequired(pPurchasedFilePointer);

	char cBuffer[iMaxBuffer];
	int iTmpCounter = 0;

	// Parse all buffer values to the purhased struct array
	printf("\nRead values from purchased file are as follows:\n");
	while (fgets(cBuffer, sizeof(cBuffer), pPurchasedFilePointer) != NULL) {
		sscanf(cBuffer, "%d\t%d", &mDummyList[iTmpCounter].iProductId, \
					&mDummyList[iTmpCounter].iPurchasedQuant);
		printf("%s", cBuffer);
		iTmpCounter++;
	}

	// Return the total number of array elements present (iterated through)
	return iTmpCounter;
}

/**
* refreshInventoryFile: A generic function to refresh the values of the 'product_info.txt' file.
* 	Used in generateBill() and addNewProduct() functions to refresh the file
*	from updated struct values.
*/
int refreshInventoryFile(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, int iInventoryElements) {
	int iCounter = 0, err = 0;
	FILE* pTmpFile = fopen("Database/tmpfile.txt", "w");

	fprintf(pTmpFile, "Product ID\tName\t\t\tPrice\t\tAvailable QT\n"); // Header

	// Write to file with cleaner formatting
	while (iCounter < iInventoryElements) {
		if ((lengthOfString(mDummyInventoryList[iCounter].sProductName)) <= 7) {
			fprintf(pTmpFile, "%d\t\t%s\t\t\t%.2f\t\t%d\n", mDummyInventoryList[iCounter].iProductId, \
								mDummyInventoryList[iCounter].sProductName, \
								mDummyInventoryList[iCounter].fProductPrice, \
								mDummyInventoryList[iCounter].iAvailableQuant);
		}
		else {
			fprintf(pTmpFile, "%d\t\t%s\t\t%.2f\t\t%d\n", mDummyInventoryList[iCounter].iProductId, \
								mDummyInventoryList[iCounter].sProductName, \
								mDummyInventoryList[iCounter].fProductPrice, \
								mDummyInventoryList[iCounter].iAvailableQuant);
		}
		iCounter++;
	}

	fclose(pTmpFile);
	fclose(pInventoryFilePointer);

	// TODO: Fix removing file in Windows
	if ((err = remove(INVENTORY_FILE_PATH)) < 0)
		perror("Cannot remove, error");

	if ((err = rename("Database/tmpfile.txt", INVENTORY_FILE_PATH)) < 0)
		perror("Cannot rename, error");

	// Open the 'product_info.txt' file again in r/w mode
	pInventoryFilePointer = fopen(INVENTORY_FILE_PATH, "a+");
	return 0;
}

/**
* bufferLengthRequired: A generic function to find the maximum length
*       of the buffer required to read from a file.
*/
int bufferLengthRequired(FILE* pFilePointer) {
	fseek (pFilePointer, 0, SEEK_END);  // Move the file pointer to the end of file
	int length = ftell(pFilePointer);   // Store how far we have traversed from start
	fseek (pFilePointer, 0, SEEK_SET);  // Set the file pointer back to start of the file

	return (length+1);                  // Return the (length + 1) taking null character into account
}

/**
* displayInventory: A generic function to display the current inventory elements
* 	in the 'product_info.txt' file.
*/
int displayInventory(FILE* pFilePointer) {
	int iMaxBuffer = bufferLengthRequired(pFilePointer);
	char cBuffer[iMaxBuffer];

	while (fgets(cBuffer, sizeof(cBuffer), pFilePointer) != NULL)
		printf("%s", cBuffer);

	return 0;
}

/**
* lengthOfString: A generic function to find the length of any string variable.
*/
int lengthOfString(char* sTempName) {
	int iCountVar = 0;

	// Read until we find the NULL character
	while (sTempName[iCountVar] != '\0')
		iCountVar++;

	// Return the length/count
	return(iCountVar);
}

/**
* removeNewLineChar: A generic function to remove trailing newline characters when
*       a string input is taken from the fgets function.
*/
int removeNewlineChar(char* sTempName) {
	// Read until we hit newline character
	while ((sTempName != NULL) && (*sTempName != '\n'))
		sTempName++;

	// Replace newline with NULL character
	*sTempName = '\0';
	return 0;
}
