/** Source file for generation of bill of purchased products
*   Author: Dibyajyoti Samal
*   Created: 24/02/2021
*   Modified: 3/03/2021
*/

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utils.h"

// Static Function Prototypes
static int formattedBill(struct billList* mBilledList, char* sCustName, char* sBillerName, int iPurchasedElements);

/**
* generateBill: Generate an ephemeral bill.txt file for every time a bill is generated.
*   Takes the file pointers, the structures and the customer and biller's name as arguments
*   and creates a temporary bill structure and a formatted bill (formattedBill) output to 'bill.txt' file.
*   Returns a 0 on success.
*/

int generateBill(FILE* pInventoryFilePointer, FILE* pPurchasedFilePointer, struct inventoryList* mDummyList, \
                                                    int iInventoryElements, char* sCustName, char* sBillerName) {
	struct purchasedList mPurchasedList[MAX_ITEMS];
	struct billList mBilledList[MAX_ITEMS];

	int iOutCounter = 0; // Purchased list outside counter
	int iInCounter = 0; // Inventory list inside counter
	int iPurchasedElements = 0, flag = 0;

	// Parse purchased.txt file to a purchasedList structure
	iPurchasedElements = parsePurchasedFileToStruct(pPurchasedFilePointer, mPurchasedList);

	// Iterate through the purchasedList struct over the productList struct
	// to find if the elements in purchasedList exist in productList.
	for (iOutCounter = 0; iOutCounter < iPurchasedElements; iOutCounter++) {
		for (iInCounter = 0; iInCounter < iInventoryElements; iInCounter++) {
			// If the purchased product ID if found in inventory, save the relevant data in billStruct
			if (mPurchasedList[iOutCounter].iProductId == mDummyList[iInCounter].iProductId) {
				mBilledList[iOutCounter].iProductId = mPurchasedList[iOutCounter].iProductId;
				strcpy(mBilledList[iOutCounter].sProductName, mDummyList[iInCounter].sProductName);
				mBilledList[iOutCounter].iPurchasedQuant = mPurchasedList[iOutCounter].iPurchasedQuant;
				mBilledList[iOutCounter].fUnitPrice = mDummyList[iInCounter].fProductPrice;
				mBilledList[iOutCounter].fActualPrice = mPurchasedList[iOutCounter].iPurchasedQuant * mDummyList[iInCounter].fProductPrice;

				// Update the product quantity available in the 'product_info.txt' file
				mDummyList[iInCounter].iAvailableQuant = mDummyList[iInCounter].iAvailableQuant - mPurchasedList[iOutCounter].iPurchasedQuant;
				flag = 0;
				break;
			}
			else {
				flag = 1;   // Else, set a flag.
			}
		}

		// If the flag is set, add the product to 'product_info.txt' file by calling addNewProduct.
		if (flag != 0) {
			printf("\nThe product ID %d does not exist in the inventory.", mPurchasedList[iOutCounter].iProductId);
			printf("\nAdding to inventory...\n");

			// Parse to file again to productList (inventory) structure
			addNewProduct(pInventoryFilePointer, mDummyList, iInventoryElements, mPurchasedList[iOutCounter].iProductId);

			// Also refresh the billedList structure with the updated information.
			mBilledList[iOutCounter].iProductId = mPurchasedList[iOutCounter].iProductId;
			strcpy(mBilledList[iOutCounter].sProductName, mDummyList[iInventoryElements-1].sProductName);
			mBilledList[iOutCounter].iPurchasedQuant = mPurchasedList[iOutCounter].iPurchasedQuant;
			mBilledList[iOutCounter].fUnitPrice = mDummyList[iInventoryElements-1].fProductPrice;
			mBilledList[iOutCounter].fActualPrice = mPurchasedList[iOutCounter].iPurchasedQuant * mDummyList[iInventoryElements-1].fProductPrice;
		}
	}

	refreshInventoryFile(pInventoryFilePointer, mDummyList, iInventoryElements);
	printf("\nInventory values have been refreshed.");
	printf("\nCalculated the total prices. Creating a formatted bill...");
	formattedBill(mBilledList, sCustName, sBillerName, iPurchasedElements);

	printf("\nBill has been printed to 'bill.txt'.\n");
	return 0;
} /* End of generateBill */

/**
* formattedBill: Create an internal/static function for creating a formatted ephemeral 'bill.txt' file.
*   Takes the structure, the number of elements, the customer and the biller name as arguments
*   and creates a printable version of a bill with incremental bill number (iBillNo).
*   Returns a 0 on success.
*/

static int formattedBill(struct billList* mBilledList, char* sCustName, char* sBillerName, int iPurchasedElements) {
	static int iBillNo = 1000;
	time_t mCurrentDateTime;
	FILE* pBillFilePointer = fopen(BILL_FILE_PATH, "w+");

	int iCounter, iTotalQuantity = 0;
	float fTotalPrice = 0.0;
	char cBuffer[50];

	// Total count of purchased product quantities and price
	for (iCounter = 0; iCounter < iPurchasedElements; iCounter++) {
		iTotalQuantity += mBilledList[iCounter].iPurchasedQuant;
		fTotalPrice += mBilledList[iCounter].fActualPrice;
	}

	iCounter = 0;
	mCurrentDateTime = time(NULL);                                                  // Retrieve the current time
	strftime(cBuffer, sizeof(cBuffer), "%d %B %Y", localtime(&mCurrentDateTime));   // Output the proper date to the buffer string

	fprintf(pBillFilePointer, "Customer Name: %s\t\t\t\tBiller Name: %s\n", sCustName, sBillerName);
	fprintf(pBillFilePointer, "Bill Number: %d\n", iBillNo);
	fprintf(pBillFilePointer, "Date: %s\n", cBuffer);
	fprintf(pBillFilePointer, "=======================================================================================\n");
	fprintf(pBillFilePointer, "Product ID\tName\t\t\tQuantity\tUnit Price\tActual Price\n");
	fprintf(pBillFilePointer, "=======================================================================================\n");
	while (iCounter < iPurchasedElements) {
		if ((lengthOfString(mBilledList[iCounter].sProductName)) <= 7) {
			fprintf(pBillFilePointer, "%d\t\t%s\t\t\t%d\t\t%.2f\t\t%.2f\n", mBilledList[iCounter].iProductId, \
											mBilledList[iCounter].sProductName, \
											mBilledList[iCounter].iPurchasedQuant, \
											mBilledList[iCounter].fUnitPrice, \
											mBilledList[iCounter].fActualPrice);
		}
		else {
			fprintf(pBillFilePointer, "%d\t\t%s\t\t%d\t\t%.2f\t\t%.2f\n", mBilledList[iCounter].iProductId, \
										mBilledList[iCounter].sProductName, \
										mBilledList[iCounter].iPurchasedQuant, \
										mBilledList[iCounter].fUnitPrice, \
										mBilledList[iCounter].fActualPrice);
		}
		iCounter++;
	}
	fprintf(pBillFilePointer, "======================================================================================\n");
	fprintf(pBillFilePointer, "Total\t\t\t\t\t%d\t\t\t\t%.2f\n", iTotalQuantity, fTotalPrice);

	fclose(pBillFilePointer);
	iBillNo++;
	return 0;
}
