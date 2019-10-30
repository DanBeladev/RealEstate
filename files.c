#include "files.h"

void writeArrayHistoryToText(FILE* fp, char** history)
{
	int i, size = 0;

	for (i = N - 1; i >= 0; i--)
	{
		if (history[i])
		{

			fprintf(fp, "%s\n", history[i]);
		}
	}
}

void writeListHistoryToText(FILE* fp, ListCommands* cmd)
{
	int i, size;
	ListNodeCommands* temp;
	temp = cmd->tail;
	while (temp)
	{
		fprintf(fp, "%s\n", temp->command);

		temp = temp->prev;
	}
}

void writeCommandsToText(ListCommands* cmd, char** history)
{
	FILE *fp;
	fp = fopen("commands.txt", "wt");

	writeArrayHistoryToText(fp, history);
	writeListHistoryToText(fp, cmd);

	fclose(fp);
}

void readCommandsFromText(FILE *fp, char** history, ListCommands* cmd, short* index)
{
	int countCmd = getHowManyCommandsFromText(fp);
	fseek(fp, 0, SEEK_SET);

	int diff = countCmd - N;

	char* newLine;

	if (diff > 0)
	{
		insertCmdToArrayHistoryByIndex(N - 1, fp, history);

		insertCmdToListByDiffSize(diff, cmd, fp);
	}
	else if (diff == 0)
	{
		insertCmdToArrayHistoryByIndex(N - 1, fp, history);
	}
	else
	{
		insertCmdToArrayHistoryByIndex(countCmd - 1, fp, history);
	}

	updateIndexArray(history, index);
}

void insertCmdToListByDiffSize(int diffSize, ListCommands* cmd, FILE* fp)
{
	int i;
	char* newLine;
	ListNodeCommands* node;

	for (i = 0; i < diffSize; i++)
	{
		newLine = getCommandFromText(fp);
		node = createNewListCmdNode(newLine);
		insertCmdToHeadListByTextFile(cmd, node);
	}
}

void insertCmdToArrayHistoryByIndex(int fromIndex, FILE *fp, char** history)
{
	char* cmd;
	int i;
	for (i = fromIndex; i >= 0; i--)
	{
		cmd = getCommandFromText(fp);

		history[i] = (char*)malloc(sizeof(char)*strlen(cmd) + 1);
		strcpy(history[i], cmd);
	}
}

void updateIndexArray(char** historyArr, short* index)
{
	int i;

	for (i = N - 1; i >= 0; i--)
	{
		if (historyArr[i])
		{
			*index = i;
			return;
		}

	}
}

int getHowManyCommandsFromText(FILE *fp)
{
	int count = 0;
	char ch;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			count++;
		}
	}
	return count;

}

void insertCmdToHeadListByTextFile(ListCommands* ls, ListNodeCommands* node)
{

	if (isEmptyHistoryList(ls))
	{

		ls->head = ls->tail = node;
		node->prev = NULL;
	}
	else
	{
		node->next = ls->head;
		ls->head->prev = node;
		ls->head = node;
	}

}

char* getCommandFromText(FILE* f)

{
	char ch;
	int logSize = 0, phiSize = 2;
	char* cmd;
	cmd = (char*)malloc(sizeof(char)*phiSize);
	fread(&ch, sizeof(char), 1, f);
	while (ch != '\n')
	{
		if (logSize == phiSize)
		{
			phiSize *= 2;
			cmd = (char*)realloc(cmd, sizeof(char)*phiSize);
		}
		cmd[logSize] = ch;
		logSize++;
		fread(&ch, sizeof(char), 1, f);
	}

	cmd[logSize] = '\0';



	if (logSize < phiSize)
		cmd = (char*)realloc(cmd, sizeof(char)*(logSize + 1));


	return cmd;
}

void writeAptDetailsBin(Apartment AptData,FILE* f)
{
	BYTE left, middle, right;
	BYTE b1, b2;

	short int sizeOfLetters = strlen(AptData.address);
	short int tempID = (short int)AptData.aptID;
	fwrite(&tempID, sizeof(short int), 1, f);
	fwrite(&sizeOfLetters, sizeof(short int), 1, f);
	fwrite(AptData.address, sizeof(char), sizeOfLetters, f);
	fwrite(&AptData.price, sizeof(int), 1, f);

	left = (AptData.numOfRooms << 4);
	left = left | (AptData.enterDate.day >> 1);
	middle = (AptData.enterDate.day << 7);
	middle = middle | (AptData.enterDate.month << 3);
	middle = middle | ((AptData.enterDate.year-2000) >> 4);
	right = (AptData.enterDate.year << 4);

	fwrite(&left, sizeof(unsigned char), 1, f);
	fwrite(&middle, sizeof(unsigned char), 1, f);
	fwrite(&right, sizeof(unsigned char), 1, f);

	b1 = (AptData.createdDate.day << 3);
	b1 = b1 | (AptData.createdDate.month >> 1);
	b2= (AptData.createdDate.month << 7);
	b2 = b2 | (AptData.createdDate.year - 2000);
	fwrite(&b1, sizeof(unsigned char), 1, f);
	fwrite(&b2, sizeof(unsigned char), 1, f);

}

void writeApartmentsInBinaryFile(List* apartments)
{
	FILE * f;
	ListNode* temp = apartments->head;
	f = fopen("Apartments.bin", "wb");
	while (temp)
	{
		writeAptDetailsBin(temp->AptData,f);
		temp = temp->next;
	}
	fclose(f);
}

void loadApartmentsfromBinaryFile(List* apartments, FILE* f,int* aptID)
{
	int sizeFile = fileSize(f);
	ListNode* node;

	while (ftell(f)<sizeFile)
	{
		node = createNode();
		readDetailsToNodeFromFile(node,f);
		insertToEndList(apartments, node);
	}

	updateAptIDIndex(apartments, aptID);

}

void updateAptIDIndex(List* apartments, int* aptID)
{
	if (isEmptyList(apartments))
		*aptID = 0;
	else
	{
		*aptID = apartments->tail->AptData.aptID;
	}
}

void readDetailsToNodeFromFile(ListNode* node,FILE* f)
{
	short ID, numOfrooms, numOfAddressLetters;
	int price;
	char* address;
	Date entryDate, createdDate;
	BYTE left, middle, right,b1,b2;
	BYTE mask = 0xff;
	
	fread(&ID, sizeof(short int), 1, f);
	fread(&numOfAddressLetters, sizeof(short int), 1, f);
	node->AptData.address = (char*)malloc(sizeof(char)*numOfAddressLetters + 1);
	fread(node->AptData.address, sizeof(char), numOfAddressLetters, f);
	node->AptData.address[numOfAddressLetters] = '\0';
	fread(&price, sizeof(int), 1, f);
	
	fread(&left, sizeof(unsigned char), 1, f);
	fread(&middle, sizeof(unsigned char), 1, f);
	fread(&right,sizeof(unsigned char), 1, f);
	numOfrooms = (left >> 4);
	entryDate.day = ((left & 0xf) << 1) | ((middle >> 7)&1);
	entryDate.month = (middle >> 3 & 0xf);
	entryDate.year = 0;
	entryDate.year = ((middle & 7) << 4) | ((right >> 4) & 0xf);
	entryDate.year += 2000;



	fread(&b1, sizeof(unsigned char), 1, f);
	fread(&b2, sizeof(unsigned char), 1, f);

	createdDate.day = (b1 >> 3);
	createdDate.month = 0;
	createdDate.month = ((b1 & 7) << 1) | ((b2 >> 7) & 1);
	createdDate.year = (b2 << 1);
	createdDate.year = (createdDate.year >> 1);
	createdDate.year += 2000;

	node->AptData.aptID = ID;
	node->AptData.createdDate = createdDate;
	node->AptData.enterDate = entryDate;
	node->AptData.numOfRooms = numOfrooms;
	node->AptData.price = price;

}

ListNode* createNode()
{
	ListNode* node;
	node = (ListNode*)malloc(sizeof(ListNode));
	return node;

}

long int fileSize(FILE* file)
{
	long int res;

	fseek(file, 0, SEEK_END);
	res = ftell(file);
	fseek(file, 0, SEEK_SET);

	return res;
}