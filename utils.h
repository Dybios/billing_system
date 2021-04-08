#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/* Macro Definitions */
#define MAX_ITEMS 1024
#define NAME_BUFFER 128
#define TMP_BUFFER 64

#define BILL_GEN 1
#define ADD_NEW 2
#define DISP_INVENTORY 3
#define EXIT 4

// Globally define the db filepaths
#define INVENTORY_FILE_PATH "Database/product_info.txt"
#define PURCHASED_FILE_PATH "Database/purchased.txt"
#define BILL_FILE_PATH "Database/bill.txt"

/* Product Structure Definition */
struct inventoryList {
	int iProductId;
	char sProductName[20];
	float fProductPrice;
	int iAvailableQuant;
};

/* Purchased Structure Definition */
struct purchasedList {
        int iProductId;
        int iPurchasedQuant;
};

/* Billed Products Structure Definition */
struct billList {
	int iProductId;
	char sProductName[20];
	int iPurchasedQuant;
	float fUnitPrice;
	float fActualPrice;
};

/* Fileutils source file prototypes */
int parseInventoryFileToStruct(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList);
int parsePurchasedFileToStruct(FILE* pPurchasedFilePointer, struct purchasedList* mDummyPurchasedList);
int refreshInventoryFile(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, int iInventoryElements);
int bufferLengthRequired(FILE* pFilePointer);
int displayInventory(FILE* pFilePointer);
int lengthOfString (char* sTempName);
int removeNewlineChar(char* sTempName);

/* Main source file prototypes */
int generateBill(FILE* pInventoryFilePointer, FILE* pPurchasedFilePointer, struct inventoryList* mDummyList, \
                                                    int iNumberOfElements, char* sCustName, char* sBillerName);
int addNewProduct(FILE* pInventoryFilePointer, struct inventoryList* mDummyInventoryList, \
                                                    int iInventoryElements, int iTempId);

#endif // UTILS_H_INCLUDED
