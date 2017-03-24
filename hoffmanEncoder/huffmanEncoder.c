#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>

//#include <io.h>

#include <string.h>

#include <errno.h>

#include <stdlib.h>

#include <assert.h>

#include <stddef.h>


//#define SIZE(x) (arraySizeCounter(x))

//int code[1255][1255] = { 0, 0 };

//char codeVaultNew[10024];

char tempcode[1024] = "";

char uniqueString[1024] = "";

char tempBuff[1024] = "";

int frequency[1024] = { 0 };

//int codeSize;

long dataSize, fileSize, dictSize;

;

//char *buff;

//char origInput[255];

//////////////////////////STRUCT Creation////////////////////////////

typedef struct input{
	unsigned char letter;
	struct input *next;
} input;

typedef struct minInput{
	unsigned char letter;
	struct minInput *next;
} minInput;

struct input *inputHead = NULL;
struct input *inputCurrent = NULL;
struct minInput *minInputHead = NULL;
struct minInput *minInputCurrent = NULL;

typedef struct huffTree{
	char letter;
	int weight;
	struct huffTree *leftNode; //pointer to left node
	struct huffTree *rightNode; // pointer to right node
} huffTree;

struct huffTree *head = NULL; // Initializing huffTree head
struct huffTree *current = NULL; // Initializing a current pointer

typedef struct dictionary{
	int zeros;
	char letter;
	struct dictionary *next;
	//	struct dictionary *prev;
} dictionary;

struct dictionary *dictHead = NULL;
struct dictionary *dicCurr = NULL;
struct dictionary *diclast = NULL;

typedef struct codeVault {
	char bit;
	struct codeVault *next;
} codeVault;

struct codeVault *codeHead = NULL;
struct codeVault *codeCurr = NULL;

//////////////////////////STRUCT Functions////////////////////////////

////////////////NODES CREATION///////////////

dictionary *getdicNode()

{

	struct dictionary *temp = ((dictionary *)malloc(sizeof(dictionary)));

	if (temp == NULL)

	{

		abort();

	}

	return temp;

}

input *getinputNode()

{

	struct input *temp = ((input *)malloc(sizeof(input)));

	if (temp == NULL)

	{

		abort();

	}

	return temp;

}

minInput *getMinInputNode()

{

	struct minInput *temp = ((minInput *)malloc(sizeof(minInput)));

	if (temp == NULL)

	{

		abort();

	}

	return temp;

}

huffTree *getNode()

{

	struct huffTree *temp = ((huffTree *)malloc(sizeof(huffTree)));

	if (temp == NULL)

	{

		abort();

	}

	return temp;

}

codeVault *getcodeNode(){

	struct codeVault *temp = malloc(sizeof(codeVault));

	if (temp == NULL)

	{

		abort();

	}

	return temp;

}
///////////////////////////////////////



///////////TREE CREATION///////////////////////

huffTree *insertHuffNode(char letter, int weight, char type) {

	struct huffTree *node = getNode();



	if (type == 'n') {

		if (head == NULL) { //Empty Tree

			node->rightNode = getNode();

			node->leftNode = getNode();

			//node = node->rightNode;

			node->rightNode->letter = letter;

			node->rightNode->weight = weight;

			node->rightNode->leftNode = NULL;

			node->rightNode->rightNode = NULL;

			head = current = node;

			node = node->leftNode;

		}

		else

		{

			node->rightNode = getNode();

			node->leftNode = getNode();

			//node = node->rightNode;

			node->rightNode->letter = letter;

			node->rightNode->weight = weight;

			node->rightNode->leftNode = NULL;

			node->rightNode->rightNode = NULL;

			current->leftNode = node;

			current = node;

			node = node->leftNode;

		}

	}

	else {

		node->letter = letter;

		node->weight = weight;

		//node->leftNode = NULL;

		//node->rightNode = NULL;

		current->leftNode = node;

		current = node;

		node = node->leftNode;

		current->leftNode = NULL;

		current->rightNode = NULL;

	}

	return current;

}

huffTree *createHuffTree(char uniqChars[], int frequency[], int count)

{

	head = current = NULL;

	//printf("Checking the uniqe string: %s\n\n", uniqChars);

	//count = (charArraySizeCounter(uniqChars) - 1); //getting size-1 to populate the huffTree except

	count--;

	head = current = NULL;

	for (int i = 0; i < count; i++)

	{ //Populating the tree.....

		insertHuffNode(uniqChars[i], frequency[i], 'n');

	}

	current = insertHuffNode(uniqChars[count], frequency[count], 'l');

	return current;

}

void treePrinter(huffTree *origTree)

{

	origTree = head;

	while (origTree != NULL)

	{

		if (origTree->rightNode == NULL)

		{

			printf("The Node is: %c\tand\t%d\n\n", origTree->letter, origTree->weight);

			origTree = origTree->leftNode;

		}

		else

		{

			printf("The Node is: %c\tand\t%d\n\n", origTree->rightNode->letter, origTree->rightNode->weight);

			origTree = origTree->leftNode;

		}

	}

}

void addNodeToDic(dictionary *node)

{

	if (dictHead == NULL)

	{

		dictHead = dicCurr = node;

	}

	else

	{

		dicCurr->next = node;

		dicCurr = node;

	}

}

void printList() {

	dictionary *node;

	node = dictHead;

	printf("\nThe List contains: \n");

	while (node->next != NULL) {

		printf("%c\t", node->letter);

		for (int i = 0; i < node->zeros; i++) {
			printf("%c ", '0');
		}
		printf("%c ", '1');
		node = node->next;

		printf("\n");
	}

	//last node - all zeroes
	printf("%c\t", node->letter);

	for (int i = 0; i < node->zeros; i++) {
		printf("%c ", '0');
	}


	//free(node);
}

void printCodeList() {

	codeVault *node;

	node = codeHead;

	printf("\nThe Code contains: \n");

	while (node != NULL) {

		printf("%d", node->bit);	
		node = node->next;
	}

	printf("\n\n\n");
	//free(node);
}

//////////////////LinkedList Functions////////////////////////




//////////////////////////Functions////////////////////////////

void fileReader(char inputFile[])

{

	FILE *fp;

	int c;

	int n = 0;

	input *fileInput;

	minInput *minFileInput;

	fileInput = inputHead;

	minFileInput = minInputHead;

	//opening input file in binary read mode

	fp = fopen(inputFile, "r");

	/*fp = fopen("C:\\Users\\Sh\\Desktop\\testfile1.png", "rb");*/

	//fp = fopen("try.txt", "rb");

	//	fp = fopen("image1.png", "rb");

	if (fp == NULL)

	{

		perror("Error in opening file");

		exit(18);

	}

	else

	{

		fseek(fp, 0, SEEK_END);

		if (ftell(fp) == 0)

		{

			printf("File is empty\n");

			exit(16);

		}

	}

	rewind(fp);

	while (1)

	{

		c = fgetc(fp);

		if (c == EOF) {

			break;

		}



		if (inputHead == NULL)

		{

			fileInput = getinputNode();

			fileInput->letter = c;

			inputHead = inputCurrent = fileInput;

			fileInput = fileInput->next;

			n++;

			//also

			minFileInput = getMinInputNode();

			minFileInput->letter = c;

			minInputHead = minInputCurrent = minFileInput;

			minFileInput = minFileInput->next;

		}

		else

		{

			fileInput = getinputNode();

			fileInput->letter = c;

			inputCurrent->next = fileInput;

			inputCurrent = fileInput;

			fileInput = fileInput->next;

			n++;

			//also

			minFileInput = getMinInputNode();

			minFileInput->letter = c;

			minInputCurrent->next = minFileInput;

			minInputCurrent = minFileInput;

			minFileInput = minFileInput->next;

		}

	}

	fileInput = NULL;

	inputCurrent->next = fileInput;

	inputCurrent = fileInput;

	fileInput = inputHead;

	minFileInput = NULL;

	minInputCurrent->next = minFileInput;

	minFileInput = minInputHead;

	//printf("The input is: ");

	// while (fileInput != NULL)

	// {

	// 	printf("%c", fileInput->letter);

	// 	fileInput = fileInput->next;

	// }

	// printf("\n\n");

	fclose(fp);

}

int intArraySizeCounter(int x[])

{

	int count = 0;

	for (int *cp = x; *cp; ++cp)

	{

		count++;

	}

	return count;

}

int charArraySizeCounter(char x[])

{

	int count = 0;

	for (char *cp = x; *cp; ++cp)

	{

		count++;

	}

	return count;

}

void arrayCopier(char x[], char y[], int size)

{

	for (int i = 0; i < size; i++)

	{

		y[i] = x[i]; // copy elements from x to y

	}

	/*	for (char * cp = x; *cp; ++cp) {

	yp = cp;

	printf("The pointer value is %c\n", *yp);

	//printf("The other pointer value is %c\n", *cp);

	++yp;

	}

	*yp = '\0';

	*/

	//printf("ZA pointer value is %s\n", y);

}

void intArrayPrinter(int input[])

{

	int *ip = input;

	//printf("The Integer Array contains: ");

	while (*ip)

	{

		printf("%d, ", *ip);

		++ip;

	}

	printf("\n");

}

void removeDuplicatesGetFrequency(char uniqueString[], int freq[])

{

	//inputHead = minInputHead;

	minInput *tmp1, *tmp2, *dup;

	tmp1 = minInputHead;

	int i = 0, j = 0, flag = 0;

	int *freqP = freq;

	*freqP = 1;

	//int freq[255] = { 0 };

	while (tmp1 != NULL && tmp1->next != NULL)

	{

		tmp2 = tmp1;

		flag = 0;

		while (tmp2->next != NULL)

		{

			if (tmp1->letter == tmp2->next->letter)

			{

				dup = tmp2->next;

				tmp2->next = tmp2->next->next;

				free(dup);

				//freq[j] += 1;

				++(*freqP);

				flag = 1;

			}

			else

			{

				tmp2 = tmp2->next;

			}

		}

		if (tmp1 != NULL && tmp1->next != NULL)

		{

			++freqP;

			*freqP = 1;

		}

		tmp1 = tmp1->next;

	}

	tmp1 = minInputHead;

	//intArrayPrinter(freq);

	//////////////Filling the unique Array////////////////////

	while (tmp1 != NULL)

	{

		uniqueString[i] = tmp1->letter;

		tmp1 = tmp1->next;

		i++;

	}

	//for (int i = 0; i < counter; i++) { //iterate with one elemnt as index

	//	for (int j = i + 1; j < counter;) { //iterate comparing the index with rest of elements

	//		if (uniqueString[j] == uniqueString[i]) { //if element match

	//			for (int k = j; k < counter; k++) { //assign next elemnt to this index removing the duplicate

	//				uniqueString[k] = uniqueString[k + 1];

	//			}

	//			counter--;

	//		}

	//		else {

	//			j++;

	//		}

	//	}

	//}

}



void sortArrays(char uniqString[], int frequency[], int count)

{

	//sorting the two arrays with respect to their frequency in descending order

	//uniqString and frequency we should sort the frequncy and at the same time make the same changes in uniqString

	int flag = 1;

	int tmpi = 0;

	char tmpc;

	///////////////////////////////////////////////////////////////

	//looping through the frequency array

	for (int i = 0; i < count && flag; i++)

	{

		flag = 0;

		for (int j = 0; j < count - 1; j++)

		{

			if (frequency[j + 1] > frequency[j])

			{

				//Swaping numbers

				tmpi = frequency[j];

				frequency[j] = frequency[j + 1];

				frequency[j + 1] = tmpi;

				//Swaping letters

				tmpc = uniqString[j];

				uniqString[j] = uniqString[j + 1];

				uniqString[j + 1] = tmpc;

				flag = 1;

			}

		}

	}

}

//////////////////////////HUFFMAN ENCODING///////////////////


int countZeroes(char letter) {

	//calling the tree
	huffTree *temp = head;

	//initializing zero counter

	int zeroes = 0;

	//Will start by traversing the tree to find the letter and add 0 to left and 1 to right movments	

	while (temp != NULL)

	{

		//Node right is not empty and not NULL - add 1

		if (temp->rightNode != NULL && temp->rightNode->letter == letter)

		{

			//trying a new approach: will count instead of adding actual code
			//if its a 1, then do not act, as it will added at the end anyway for all nodes except the last one
			//in the decoding part of the program
			break;

		}

		//Node contain letter -> valid only in last letter which is on the left buttom node

		if (temp->letter == letter)

		{

			break;

		}

		//adding 0 as we travel left on the tree
		zeroes++;

		temp = temp->leftNode;

	}

	return zeroes;

}

void huffEncode(int uniqueSize, char *uniqueString, int *frequency)

{

	//calling the empty dictionary
	dictHead = dicCurr = NULL;
	dictionary *dictionaryPtr = getdicNode();
	dictionaryPtr = dictHead;

	for (int i = 0; i < uniqueSize; i++) {

		//creating new dictionary node
		dictionaryPtr = getdicNode();
		dictionaryPtr->next = NULL;

		//printf("Encoding letter %c: ", uniqueString[i]);

		//Will take this into a seperate function to avoid nested loops
		dictionaryPtr->letter = uniqueString[i];
		dictionaryPtr->zeros = countZeroes(uniqueString[i]);

		if (dictHead == NULL)

		{

			dictHead = dicCurr = dictionaryPtr;

		}

		dicCurr->next = dictionaryPtr;

		dicCurr = dictionaryPtr;

		dictionaryPtr = dictionaryPtr->next;

	}

}

void codeGenerator()

{

	huffTree *temp = head;

	input *inTemp = inputHead;

	codeVault *codePtr;
	codePtr = codeHead;

	//int i = 0;

	//codeVaultNew

	while (inTemp != NULL){

		if (temp != NULL && temp->rightNode != NULL){

			if (inTemp->letter == temp->rightNode->letter){
				//Adding 1 into codevault
				//codeVaultNew[i] = 1;
				//codePtr->bit = 1;

				//i++;
				//Case: codeVault is empty - fill the head
				if (codeHead == NULL) {
					codePtr = getcodeNode();
					codePtr->bit = 1;
					codeHead = codeCurr = codePtr;
					codePtr = codePtr->next;
				}				
				else {//Case: codevault is not empty - fill below
					codePtr = getcodeNode();
					codePtr->bit = 1;
					codeCurr->next = codePtr;
					codeCurr = codePtr;
					codePtr = codePtr->next;
				}
				temp = head;
				inTemp = inTemp->next;
			}
			else{
				temp = temp->leftNode;

				if (codeHead == NULL) {
					codePtr = getcodeNode();
					codePtr->bit = 0;
					codeHead = codeCurr = codePtr;
					codePtr = codePtr->next;
				}
				else {//Case: codevault is not empty - fill below
					codePtr = getcodeNode();
					codePtr->bit = 0;
					codeCurr->next = codePtr;
					codeCurr = codePtr;
					codePtr = codePtr->next;
				}
				//codeVaultNew[i] = 0;				

				//i++;

				/*if (temp->rightNode != NULL) {

				if (inTemp->letter == temp->rightNode->letter) {

				codeVaultNew[i] = 0;

				i++;

				}

				}*/

			}

		}

		else{
			//codeVaultNew[i] = 0;
			inTemp = inTemp->next;
			temp = head;
			//i++;
		}
	}
	codePtr = NULL;
	codeCurr->next = codePtr;
	codeCurr = codePtr;
	codePtr = codeHead;
//	codeVaultNew[i + 1] = -1;

	/*printf("\nThe Huffman code is: \t");

	for (int i = 0; codeVaultNew[i] != -1; i++)

	{

	printf("%d", codeVaultNew[i]);

	}

	printf("\n\n\n");*/

}

//void createDictionary(int uniqueSize, char uniqueString[])
//
//{
//
//	dictHead = dicCurr = NULL;
//
//	dictionary *temp = getdicNode();
//
//	temp = dictHead;
//
//	for (int i = 0; i < uniqueSize; i++)
//
//	{
//
//		int j = 0;
//
//		temp = getdicNode();
//
//		temp->next = NULL;
//
//		printf("Letter: %c\t", uniqueString[i]);
//
//		temp->letter = uniqueString[i];
//
//		printf("Code:");
//
//		while (code[i][j] != -1)
//
//		{
//
//			printf("%d", code[i][j]);
//
//			temp->code[j] = code[i][j];
//
//			j++;
//
//		}
//
//		temp->code[j] = -1;
//
//		if (dictHead == NULL)
//
//		{
//
//			dictHead = dicCurr = temp;
//
//		}
//
//		dicCurr->next = temp;
//
//		dicCurr = temp;
//
//		temp = temp->next;
//
//		j++;
//
//		printf("\n");
//
//	}
//
//	printf("\nNOW TRYING THE NEW LIST\n\n");
//
//	temp = dictHead;
//
//	while (temp != NULL)
//
//	{
//
//		printf("The Letter is: %c\tAnd the code is:", temp->letter);
//
//		for (int i = 0; temp->code[i] != -1; i++)
//
//		{
//
//			printf("%d", temp->code[i]);
//
//		}
//
//		temp = temp->next;
//
//		printf("\n");
//
//	}
//
//}






//////////////////////////Binary Output/////////////////////////////

void fileWriter(char encodedFile[])

{

	FILE *fp;

	dictionary *temp;

	temp = dictHead;

	codeVault *codePtr;
	codePtr = codeHead;

	char seperator[] = "--Seperator--";

	fp = fopen(encodedFile, "wb+");

	//Opening the output file

	if (!fp)

	{

		printf("Error opening file\n");

		exit(19);

	}

	//getting codeVault length
	/*codeSize = 0;
	for (int i = 0; codeVaultNew[i] != -1; i++)

	{

		codeSize += 1;

	}*/


	//writing the dictionary....
	while (temp && fwrite(temp, offsetof(dictionary, next), 1, fp) == 1) {

		temp = temp->next;

	}

#ifdef __linux__

	if (0 > fputs("\n", fp))

	{

		printf("Error putting string in file");

		exit(2);

	}

#elif _WIN32

	if (0 > fputs("\r\n", fp))

	{

		printf("Error putting string in file");

		exit(2);

	}

#else

#endif

	if (0 > fputs(seperator, fp))

	{

		printf("Error putting string in file");

		exit(3);

	}


	printf("YAAAAAA RAB\n\n");

	for (codePtr; codePtr != NULL; codePtr = codePtr->next){

		size_t wcheck = fwrite(codePtr, offsetof(codeVault, next), 1, fp);
		
		//printf("%d", codeVaultNew[i]);

		if (wcheck != 1){
			printf("Error Wriritng to file");
			exit(5);
		}
	}

	fclose(fp);

}



/////////////////////File Functions/////////////////////////

int getFileSize(FILE *fp) {



	int size = 0;

	if (0 != fseek(fp, 0, SEEK_END))

	{

		printf("Error Seeking file");

		exit(14);

	}

	size = ftell(fp);

	rewind(fp);



	return size;

}

int getDictSize(FILE *fp) {



	char seperator[] = "--Seperator--";

	char seperatorFinder[sizeof(seperator)] = { 0 };

	int size = 0;



	while (1)

	{

		if (NULL == fgets(seperatorFinder, sizeof(seperator), fp))

		{

			perror("Error: ");

			exit(7);

		}

		else

		{

			if (strcmp(seperatorFinder, seperator) == 0)

			{

				//Getting Dictionary Size:

				size = ftell(fp) /*+ sizeof("\n\r")*/;

				//if (0 != fseek(fp, sizeof("\n\r"), SEEK_END)) { printf("Error Seeking file"); exit(8); }

				//Getting data size:				



				break;

			}

		}

	}

	rewind(fp);

	return size;



}

void saveDictionary(FILE *fp)

{

	dictionary TOP;

	TOP.next = NULL;

	dictionary *temp = &TOP;

	dictionary reader;



	int sizer = sizeof("--seperator--") + sizeof("\n");



	while ((ftell(fp) < (dictSize - sizer)) && (fread(&reader, offsetof(dictionary, next), 1, fp) == 1)) {

		// Fill the next field

		reader.next = NULL;

		// Allocate space and copy

		temp->next = malloc(sizeof *(temp->next));

		assert(temp->next);

		*(temp->next) = reader;

		// Advance to the next

		temp = temp->next;

	}



	rewind(fp);

	dictHead = TOP.next;

}

void saveData(FILE *fp) {
	
	codeVault TOP;
	TOP.next = NULL;
	codeVault *temp = &TOP;
	codeVault reader;

	int sizer = sizeof("--seperator--") + sizeof("\n");
	
	//moving in the file till the data part

	rewind(fp);

	if (0 != fseek(fp, dictSize, SEEK_SET))	{
		printf("Error Seeking file");
		exit(14);
	}

	printf("\n\nSHOOF YA SAMEEEEEEER\n\n");
	int i = 0;
	while (i != dataSize){

		if (1 != fread(&reader, offsetof(codeVault, next), 1, fp)) { break; };
		// Fill the next field
		reader.next = NULL;
		// Allocate space and copy
		temp->next = malloc(sizeof *(temp->next));
		assert(temp->next);
		*(temp->next) = reader;
		// Advance to the next		
		temp = temp->next;					
		i++;
	}	
		
	rewind(fp);
	codeHead = TOP.next;

}


///////////////////Binary input analysis//////////////////////

void fileAnalyzer(char binaryInput[])

{

	FILE *fp;



	//temp = getdicNode();

	fp = fopen(binaryInput, "rb");

	if (!fp)

	{

		printf("Error opening file\n");

		exit(6);

	}



	//Getting file size:
	fileSize = getFileSize(fp);

	//Getting Dictionary size:
	dictSize = getDictSize(fp);

	//Getting Data Size:
	dataSize = fileSize - dictSize;

	//Saving the dictionary part to linkedlist:
	saveDictionary(fp);

	//State check for the dectionary		

	//printList();

	//Saving data to buffer
	saveData(fp);


	fclose(fp);

	//	printf("\n\n\n\n\nchecking data................................\n\n");


	/*
	for (int i = 0; i < dataSize; i++)

	{

	printf("%d", codeVaultNew[i]);

	}*/

}

char *getCodeString(int code)

{

	//char *tempcode = malloc(500);
	//char tempcode[] = "";
	strcpy(tempcode, "");

	//counting the zeroes
	for (int i = 0; i < code; i++) {
		strcat(tempcode, "0");
	}

	strcat(tempcode, "1");

	return tempcode;

}

char checkDict(char tempbuff[])

{

	dictionary *dictPtr = dictHead;

	char tempdictCode[512] = "";

	while (dictPtr->next != NULL)

	{

		strcpy(tempdictCode, getCodeString(dictPtr->zeros));

		if (strcmp(tempbuff, tempdictCode) == 0)

		{

			return dictPtr->letter;

			//break;

		}

		dictPtr = dictPtr->next;

	}

	//last check

	char tmpString[512] = "";

	for (int i = 0; i < dictPtr->zeros; i++) {

		strcat(tmpString, "0");
	}

	if (strcmp(tempbuff, tmpString) == 0) {

		return dictPtr->letter;
	}

	//free(dictPtr);
	return NULL;
}

void decoder(char outputFile[])

{

	FILE *fp;

	codeVault *codePtr = codeHead;

	fp = fopen(outputFile, "w+");

	//printf("\n\n\nThe MOMENT of TRUTHTHTHTHTHTHT\n\n");

	for (codePtr; codePtr != NULL; codePtr = codePtr->next) {

		if (codePtr->bit == 1){
			strcat(tempBuff, "1");
		}
		else{
			strcat(tempBuff, "0");
		}
		if (strlen(tempBuff) >= 1){

			char match = checkDict(tempBuff);
			if (match != NULL){

				strcpy(tempBuff, "");
				if (match != fputc(match, fp)) { printf("Issue writing to output file"); exit(15); }
			}
		}
	}

	fclose(fp);

	/*for (int i = 0; i < dataSize; i++) {

	if (buff[i] == 1 && huffTemp != NULL) {

	if (huffTemp->rightNode != NULL) {

	huffTemp = huffTemp->rightNode;

	if (huffTemp->letter != fputc(huffTemp->letter, fp)) { printf("Issue writing to output file"); exit(15); }

	huffTemp = head;

	}

	}

	else if (buff[i] == 0 && huffTemp != NULL) {

	huffTemp = huffTemp->leftNode;

	if (huffTemp->rightNode == NULL) {

	if (huffTemp->letter != fputc(huffTemp->letter, fp)) { printf("Issue writing to output file"); exit(15); }

	huffTemp = head;

	}

	}

	else {

	huffTemp = head;

	}

	}*/

}

/////////////////MAIN FUNCTIONS///////////////////////////////

void encodeFile(char inputFile[], char encodedFile[])

{

	//reading input file and creating linked lists

	fileReader(inputFile);

	//removing duplicates, getting frequencyy, and uniqSize

	removeDuplicatesGetFrequency(uniqueString, frequency);

	int uniqueSize = charArraySizeCounter(uniqueString);

	//sorting the arrays

	sortArrays(uniqueString, frequency, uniqueSize);

	//intArrayPrinter(frequency);

	//Creating huffman Tree

	huffTree *origTree = createHuffTree(uniqueString, frequency, uniqueSize);

	//treePrinter(origTree);

	//encoding start

	huffEncode(uniqueSize, uniqueString, frequency);

	codeGenerator();

	//printCodeList();
	//printList();

	//createDictionary(uniqueSize, uniqueString);

	//writing encoded binary file

	fileWriter(encodedFile);

}

void decodeFile(char binaryInput[], char outputFile[])

{

	//Analyzing binary encoded file

	fileAnalyzer(binaryInput);

	//decoding
	//printCodeList();
	decoder(outputFile);

}

/////////////////////////////////////////////////////////////

int main(int argc, char *argv[])

{

	//strcpy(codeVaultNew, "");

	//There will be 4 scenarios for input command

	//1) ./huffman.x input.file encoded(output).file -> encoding command with input and encoded output present

	//2) ./huffman.x input.file -> encoding command with input and NO encoded output is present --> create custom encoded.output

	//2) ./huffman.x -d encoded.file output.file -> decoding command with encoded input and outputfile present

	//2) ./huffman.x -d input.file ->  decoding command with encoded input and NO outputfile present --> create custom

	//getting command line:

	//case 0: wrong no of argument

	if (argc < 2)

	{

		perror("Incorrect no of inputs");

		exit(1);

	}

	//case 1, 2

	else

	{

		if (strcmp(argv[1], "-d") != 0)

		{

			//case 1

			if (argc == 3)

			{

				encodeFile(argv[1], argv[2]);

			}

			//case 2

			else

			{

				encodeFile(argv[1], "binaryOutput.bin");

			}

		}

		//case 3, 4

		else

		{

			//case 3

			if (argc = 4)

			{

				decodeFile(argv[2], argv[3]);

			}

			//case 4

			else

			{

				//create a filename string for the output

				//check if the encoded input is present

				//if not then use the default binary file saved

				if (argv[2])

				{ //encoded file given

					decodeFile(argv[2], "output.decoded");

				}

				else

				{ //encoded file not given

					decodeFile("binaryOutput.bin", "output.decoded");

				}

			}

		}

	}

	//Initiating the needed elements

	////////////////STAT CHECK//////////////////////////

	//printf("Unique String:\t%s\n", uniqueString);

	//printf("Unique Size:\t%d\n\n", uniqueSize);

	//printf("The Frequency Array: ");

	//intArrayPrinter(frequency);

	//printf("\n");

	//printf("Frequency Size:\t%d\n\n", intArraySizeCounter(frequency));

	///////////////////////////////////////////////////

	printf("\n\n\n");

	printf("\nEND OF PROGRAM\n");

	return 0;

	//getFrequency(uniqueString, origString, uniqueSize, originalSize, frequency);//Getting the Frequency

}
