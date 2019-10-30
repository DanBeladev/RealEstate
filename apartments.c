#define _CRT_SECURE_NO_WARNING 1
#include"apatments.h"
#include<stdio.h>
#include<stdlib.h>
#include "utils.h"


int isEqual(char * a, char * b)
{
	return strcmp(a, b) == 0;
}

char * replaceSubStr(char *origCmd, char *subCmd, char *token)
{
	size_t outsize = strlen(origCmd) + 1;

	char *res = malloc(outsize);

	size_t resoffset = 0;

	char *needle;
	while (needle = strstr(origCmd, subCmd)) {

		memcpy(res + resoffset, origCmd, needle - origCmd);
		resoffset += needle - origCmd;


		origCmd = needle + strlen(subCmd);


		outsize = outsize - strlen(subCmd) + strlen(token);
		res = realloc(res, outsize);


		memcpy(res + resoffset, token, strlen(token));
		resoffset += strlen(token);
	}

	strcpy(res + resoffset, origCmd);

	return res;
}

short handleInternalCommand(char* cmd, short* finished, char** shortHistory, ListCommands* oldCommandsList, short* rear,List* apartments)
{
	if (isEqual(cmd, "exit"))
	{
		*finished = 1;
		writeCommandsToText(oldCommandsList, shortHistory);
		writeApartmentsInBinaryFile(apartments);
		return 1;
	}
	else if (isEqual(cmd, "history"))
	{
		printHistory(shortHistory, oldCommandsList);
		return 1;
	}
	else if (isEqual(cmd, "short_history"))
	{
		printShortHistory(shortHistory, oldCommandsList);
		return 1;
	}
	return 0;
}

short handleRestoreCommand(char** cmd, char** shortHistory, ListCommands* oldCommands, short* rear)
{

	if (isEqual(*cmd, "!!"))
	{
		restoreLastCommand(cmd, shortHistory, rear);
		return 1;
	}
	else if (*cmd[0] == '!')
	{

		restoreByCmdID(cmd, shortHistory, oldCommands);

		return 1;
	}

	return 0;
}

char* getCommandByID(short cmdID, char** shortHistory, ListCommands* oldCommandsList)
{
	ListNodeCommands* temp;
	short id = 0, i;
	temp = oldCommandsList->head;
	if (isEmptyHistoryList(oldCommandsList))
		return shortHistory[cmdID - 1];
	else
	{
		while (temp)
		{
			id++;
			if (id == cmdID)
				return temp->command;
			temp = temp->next;
		}

		for (i = 0; i < N; i++)
		{
			id++;
			if (id == cmdID)
				return shortHistory[i];
		}
	}
}

void restoreByCmdID(char** cmd, char** shortHistory, ListCommands* oldCommandsList)
{
	char* CmdByID, *outstr, *instr;
	char* token, *temp;
	short cmdID;
	token = strtok(*cmd + 1, "^");
	if (token)
	{
		cmdID = getshort(token);
		CmdByID = getCommandByID(cmdID, shortHistory, oldCommandsList);
		outstr = strtok(NULL, "^");
		instr = strtok(NULL, "\n");

		if (outstr && instr)
		{
			temp = replaceSubStr(CmdByID, outstr, instr);
			free(*cmd);
			*cmd = temp;
		}

		else
		{
			cmdID = getshort(*cmd + 1);
			CmdByID = getCommandByID(cmdID, shortHistory, oldCommandsList);
			*cmd = (char*)realloc(*cmd, sizeof(char)*strlen(CmdByID) + 1);
			strcpy(*cmd, CmdByID);
		}
	}
}

void handleGetAnApt(List* apartments, const char * cmd) {
	short maxNumRooms, minNumRooms, days;
	int maxPrice;
	short sortFlag, reverseFlag, enterFlag;
	Date freeUntilDate;

	freeUntilDate.day = freeUntilDate.month = freeUntilDate.year = -1;

	maxNumRooms = minNumRooms = maxPrice = days = -1;
	sortFlag = reverseFlag = enterFlag = 0;

	char * token;
	token = strtok(NULL, " -");
	while (token != NULL)
	{
		if (isEqual(token, "MaximumNumRooms"))
		{
			maxNumRooms = getshort(strtok(NULL, " "));
		}
		else if (isEqual(token, "MinimumNumRooms"))
		{
			minNumRooms = getshort(strtok(NULL, " "));
		}
		else if (isEqual(token, "MaximumPrice"))
		{
			maxPrice = strToint(strtok(NULL, " "));
		}
		else if (isEqual(token, "Date"))
		{
			freeUntilDate = getDateFromStr(strtok(NULL, " "));
		}
		else if (isEqual(token, "s"))
		{
			sortFlag = 1;
		}
		else if (isEqual(token, "sr"))
		{
			sortFlag = reverseFlag = 1;
		}

		else if (isEqual(token, "Enter"))
		{
			days = getshort((strtok(NULL, " ")));
			enterFlag = 1;
			handleEnterFunc(apartments, days);

		}
		token = strtok(NULL, " -");
	}


	ListToAptArr(apartments, minNumRooms, maxNumRooms, maxPrice, freeUntilDate, sortFlag, reverseFlag, enterFlag);
}

void handleCommand(List* apartments, char * cmd, int* aptID)
{
	char* temp = (char*)malloc(sizeof(char)*(strlen(cmd) + 1));
	char* typeCmd;
	strcpy(temp, cmd);
	typeCmd = strtok(temp, " ");
	if (typeCmd != NULL)
	{
		if (isEqual(typeCmd, "add-an-apt"))
		{
			addAnApartment(apartments, typeCmd, aptID);
		}
		else if (isEqual(typeCmd, "buy-an-apt"))
		{
			buyAnApartment(apartments, getshort(strtok(NULL, " ")));
		}
		else if (isEqual(typeCmd, "get-an-apt"))
		{
			handleGetAnApt(apartments, cmd);
		}
		else if (isEqual(typeCmd, "delete-an-apt"))
		{
			typeCmd = strtok(NULL, " ");
			handleDeleteByDays(apartments, getshort(strtok(NULL, " ")));
		}
	}
	free(temp);
}

void handleDeleteByDays(List* apartments, short numOfDays)
{
	Date d = getCurrentTime();
	CurrentDateMinusNumOfDays(&d, numOfDays);
	int aptIDtoDelete = 0;
	short flagPrevMoved;
	ListNode * temp, *nextListNode, *prev;
	temp = prev = apartments->head;
	while (temp)
	{
		flagPrevMoved = 0;
		nextListNode = temp->next;
		if (temp->AptData.createdDate.year >= d.year)
			if (temp->AptData.createdDate.month >= d.month)
				if (temp->AptData.createdDate.day >= d.day)
				{
					if (temp == apartments->head)
						apartments->head = nextListNode;
					if (temp == apartments->tail)
						apartments->tail = nextListNode;

					prev->next = nextListNode;
					free(temp);
					temp = nextListNode;
				}
		prev = temp;
		temp = nextListNode;

	}
}

void buyAnApartment(List* ls, int ID)
{
	ListNode* curr, *temp;
	curr = temp = ls->head;

	if (temp->AptData.aptID == ID)
	{
		if (ls->tail == temp)
		{
			ls->head = ls->tail = temp->next;
			free(temp);
			return;
		}

		else if ((ls->head == temp) && (ls->tail != temp))
		{
			ls->head = temp->next;
			free(temp);
			return;
		}
	}


	while (curr)
	{
		if (curr->AptData.aptID == ID)
		{

			temp->next = curr->next;

			if (curr == ls->tail)
				ls->tail = temp;

			free(curr);
			return;
		}
		temp = curr;
		curr = curr->next;
	}
}

void handleEnterFunc(List* apartments, int days)
{
	Date d = getCurrentTime();
	CurrentDateMinusNumOfDays(&d, days);

	ListNode* temp = apartments->head;
	while (temp)
	{
		if (temp->AptData.createdDate.year >= d.year)
			if (temp->AptData.createdDate.month >= d.month)
				if (temp->AptData.createdDate.day >= d.day)
					printf("%d\n", temp->AptData.aptID);

		temp = temp->next;
	}
}

char* getCommand()

{
	char ch;
	int logSize = 0, phiSize = 2;
	char* cmd;
	cmd = (char*)malloc(sizeof(char)*phiSize);

	scanf("%c", &ch);
	while (ch != '\n')
	{
		if (logSize == phiSize)
		{
			phiSize *= 2;
			cmd = (char*)realloc(cmd, sizeof(char)*phiSize);
		}
		cmd[logSize] = ch;
		logSize++;
		scanf("%c", &ch);
	}

	cmd[logSize] = '\0';



	if (logSize < phiSize)
		cmd = (char*)realloc(cmd, sizeof(char)*(logSize + 1));


	return cmd;
}

void makeEmptyList(List* ls)
{
	ls->head = ls->tail = NULL;

}

short isEmptyList(List* ls)
{
	return (ls->head == NULL && ls->tail == NULL);
}

ListNode* createNewListNode(char* cmd, int* aptID)
{

	ListNode* newApt = (ListNode*)malloc(sizeof(ListNode));

	SetaptDetails(&newApt->AptData, cmd, aptID);

	newApt->next = NULL;

	return newApt;
}

void insertToEndList(List* aptlist, ListNode* aptNode)
{
	if (isEmptyList(aptlist))
		aptlist->head = aptlist->tail = aptNode;
	else
	{
		aptlist->tail->next = aptNode;
		aptlist->tail = aptNode;
	}
}

void insertToStartList(List* aptlist, ListNode* aptNode)
{
	if (isEmptyList(aptlist))
		aptlist->head = aptlist->tail = aptNode;
	else
	{
		aptNode->next = aptlist->head;
		aptlist->head = aptNode;
	}
}

void printApartment(Apartment apt)
{
	printf("Apt details:\n");
	printf("Code: %d\n", apt.aptID);
	printf("Address: %s\n", apt.address);
	printf("Number of rooms: %d\n", apt.numOfRooms);
	printf("Price: %d\n", apt.price);
	printf("Entry date: %d.%d.%d\n", apt.enterDate.day, apt.enterDate.month, apt.enterDate.year);
	printf("Database entry date:  %d.%d.%d\n", apt.createdDate.day, apt.createdDate.month, apt.createdDate.year);
}

void printMaximunRooms(List* apartments, short maxRooms)
{
	ListNode* temp = apartments->head;
	while (temp)
	{
		if (temp->AptData.numOfRooms <= maxRooms)
			printApartment(temp->AptData);

		temp = temp->next;
	}
}

void printMinimumRooms(List* apartments, short minRooms)
{
	ListNode* temp = apartments->head;
	while (temp)
	{
		if (temp->AptData.numOfRooms >= minRooms)
			printApartment(temp->AptData);

		temp = temp->next;
	}
}

void printByDate(List* apartments, Date entryDate)
{
	ListNode* temp = apartments->head;
	while (temp)
	{
		if (temp->AptData.enterDate.year <= entryDate.year)
			if (temp->AptData.enterDate.year < entryDate.year)
				printApartment(temp->AptData);
			else
			{
				if (temp->AptData.enterDate.month <= entryDate.month)
					if (temp->AptData.enterDate.day <= entryDate.day)
						printApartment(temp->AptData);
			}

		temp = temp->next;
	}
}

printRevArr(Apartment* aptArr, int size)
{
	int i;
	for (int i = size - 1; i >= 0; i--)
	{
		printApartment(aptArr[i]);
	}
}

printArr(Apartment* aptArr, int size)
{
	int i;
	for (int i = 0; i < size; i++)
	{
		printApartment(aptArr[i]);
	}
}

int ListSize(List* lst)
{
	int count = 0;
	ListNode* curr = lst->head;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return count;
}

int ListCommandsSize(ListCommands* lst)
{
	int count = 0;
	ListNodeCommands * curr = lst->head;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return count;
}

int checkFilters(Apartment * apt, ListNode* n, short minNumRooms, short maxNumRooms, int maxPrice, Date freeUntilDate)
{
	if ((minNumRooms > 0 && n->AptData.numOfRooms < minNumRooms))
		return 0;

	if ((maxNumRooms > 0 && n->AptData.numOfRooms > maxNumRooms))
		return 0;

	if ((maxPrice > 0 && n->AptData.price > maxPrice))
		return 0;

	if (freeUntilDate.year > 0 && n->AptData.enterDate.year > freeUntilDate.year)
		return 0;
	else if ((freeUntilDate.year > 0 && n->AptData.enterDate.year == freeUntilDate.year) &&
		(freeUntilDate.month > 0 && n->AptData.enterDate.month > freeUntilDate.month))
		return 0;
	else if ((freeUntilDate.year > 0 && n->AptData.enterDate.year == freeUntilDate.year) &&
		(freeUntilDate.month > 0 && n->AptData.enterDate.month == freeUntilDate.month) &&
		(freeUntilDate.day > 0 && n->AptData.enterDate.day > freeUntilDate.month))
		return 0;

	return 1;
}

void ListToAptArr(List* lst, short minNumRoom, short maxNumRooms, int maxPrice, Date freeUntilDate, short sortFlag, short reverseFlag, short enterFlag)
{
	if (enterFlag)
		return;

	Apartment* aptArr;

	ListNode* curr = lst->head;
	int size = 0;
	int sizeOfList = ListSize(lst);
	aptArr = (Apartment*)malloc((sizeof(Apartment)) * (sizeOfList));
	while (curr)
	{
		if (checkFilters(aptArr + size, curr, minNumRoom, maxNumRooms, maxPrice, freeUntilDate))
		{
			copyAptData(aptArr + size, curr);
			size++;
		}

		curr = curr->next;
	}

	if (sortFlag || reverseFlag)
		mergeSortApartmentsArray(aptArr, size);

	if (reverseFlag)
		printRevArr(aptArr, size);
	else
	{
		printArr(aptArr, size);
	}
	free(aptArr);
}

void updateDaysByMonth(Date* d, int month)
{
	if (month == 2)
		d->day = 28;
	if (month == 6 || month == 9 || month == 11 || month == 4)
		d->day = 30;
	else
		d->day = 31;
}

void CurrentDateMinusNumOfDays(Date* d, int numOfDays)
{
	int i;
	for (i = 0; i < numOfDays; i++)
	{
		d->day--;
		if (d->day == 0)
		{
			d->month--;
			updateDaysByMonth(d, d->month);
		}
		if (d->month == 0)
			d->year--;
	}
}

void copyAptData(Apartment* apt, ListNode* node)
{
	apt->numOfRooms = node->AptData.numOfRooms;
	apt->address = node->AptData.address;
	apt->aptID = node->AptData.aptID;
	apt->price = node->AptData.price;
	apt->createdDate = node->AptData.createdDate;
	apt->enterDate = node->AptData.enterDate;
	return;
}

void mergeApartmentsArray(Apartment* arr1, int size1, Apartment* arr2, int size2, Apartment* res)
{
	int i1 = 0, i2 = 0, write = 0;

	while (i1 < size1 && i2 < size2)
	{
		if (arr1[i1].price < arr2[i2].price)
		{
			res[write] = arr1[i1];
			i1++;
			write++;
		}
		else
		{
			res[write] = arr2[i2];
			i2++;
			write++;
		}
	}

	while (i1 < size1)
	{
		res[write] = arr1[i1];
		i1++;
		write++;
	}

	while (i2 < size2)
	{
		res[write] = arr2[i2];
		i2++;
		write++;
	}
}

void mergeSortApartmentsArray(Apartment* arr, int size)
{
	Apartment* temp;
	int i;

	if (size <= 1)
	{
		return;
	}
	else
	{
		mergeSortApartmentsArray(arr, size / 2);
		mergeSortApartmentsArray(arr + (size / 2), size - (size / 2));
		temp = (Apartment*)malloc(sizeof(Apartment)*size);
		mergeApartmentsArray(arr, size / 2, arr + (size / 2), size - (size / 2), temp);

		for (i = 0; i < size; i++)
		{
			arr[i] = temp[i];
		}

		free(temp);
	}
}

void restoreLastCommand(char** cmd, char**  shortHistory, short* rear)
{
	int newStrSize = strlen(shortHistory[*rear]);
	*cmd = (char*)realloc(*cmd, sizeof(char)*(newStrSize + 1));
	strcpy(*cmd, shortHistory[*rear]);
}

void SetaptDetails(Apartment* apt, char* str, int* aptID)
{
	short roomsNum, day, month, year;
	int strSize = strlen(str);

	char* temp;
	int price;
	char* del = " ";
	temp = strtok(NULL, "\"");
	char* address = (char*)malloc(sizeof(char)*(strlen(temp) + 1));
	strcpy(address, temp);

	price = getPrice(strtok(NULL, del));
	roomsNum = getshort(strtok(NULL, del));
	day = getshort(strtok(NULL, del));
	month = getshort(strtok(NULL, del));
	year = getYear(strtok(NULL, del));


	*aptID += 1;

	apt->aptID = *aptID;
	apt->address = address;
	apt->price = price;
	apt->numOfRooms = roomsNum;
	apt->enterDate.day = day;
	apt->enterDate.month = month;
	apt->enterDate.year = year;
	apt->createdDate = getCurrentTime();
}

void addAnApartment(List* apartments, char* cmd, int* aptID)
{

	ListNode* newApt = createNewListNode(cmd, aptID);


	insertToEndList(apartments, newApt);

}

ListNodeCommands* createNewListCmdNode(char* cmd)
{
	ListNodeCommands* newNode = (ListNodeCommands*)malloc(sizeof(ListNodeCommands));
	newNode->command = (char*)malloc(sizeof(char)*(strlen(cmd) + 1));

	newNode->command = cmd;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

void makeEmptyHistoryList(ListCommands* oldCommands)
{
	oldCommands->head = oldCommands->tail = NULL;

}

short isEmptyHistoryList(ListCommands* oldCommands)
{
	return (oldCommands->head == NULL && oldCommands->tail == NULL);
}

void ToEndHistoryList(ListCommands* oldCommands, ListNodeCommands* cmdNode)
{
	if (isEmptyHistoryList(oldCommands))
		oldCommands->head = oldCommands->tail = cmdNode;
	else
	{
		oldCommands->tail->next = cmdNode;
		oldCommands->tail = cmdNode;
	}
}

void insertCommandTohistoryList(ListCommands* oldCommands, ListNodeCommands* cmdNode)
{

	if (isEmptyHistoryList(oldCommands))
		oldCommands->head = oldCommands->tail = cmdNode;
	else
	{
		oldCommands->tail->next = cmdNode;
		cmdNode->prev = oldCommands->tail;
		oldCommands->tail = cmdNode;
	}


}

void printListCommands(ListCommands* oldCommands, int* sizeList)
{
	int count = 0;
	ListNodeCommands* temp;
	temp = oldCommands->head;
	while (temp)
	{
		count++;
		printf("%d: %s\n", count, temp->command);
		temp = temp->next;
	}

	*sizeList = count;
}

void printHistory(char ** shortHistory, ListCommands* oldCommandsList)
{
	int i;
	int sizeofList;
	printListCommands(oldCommandsList, &sizeofList);

	for (i = 0; shortHistory[i] != NULL && i < N; i++)
	{
		printf("%d: %s\n", (i + 1) + sizeofList, shortHistory[i]);
	}

}

void printShortHistory(char ** shortHistory, ListCommands* oldCommandsList)
{
	int i;
	int sizeOfList = ListCommandsSize(oldCommandsList);

	for (i = 0; shortHistory[i] != NULL && i < N; i++)
	{
		printf("%d: %s\n", (i + 1) + sizeOfList, shortHistory[i]);
	}
}

void makeNullArray(char** historyArray)
{
	for (int i = 0; i < N; i++)
	{
		historyArray[i] = NULL;
	}
}

char* createCmdToArray(char* cmd)
{
	char* command = (char*)malloc(sizeof(char)*(strlen(cmd) + 1));
	strcpy(command, cmd);
	return command;
}

short isEmptyQueue(short* rear) {
	return  *(rear) == -1;
}

short isFullQueue(short* rear) {
	return (*rear == N - 1);
}

void enQueue(char** shortHistoryArr, char*cmd, short* rear) {
	if (isFullQueue(rear)) {
		return;
	}
	if (isEmptyQueue(rear))
		*rear = 0;
	else
		*rear = (*rear + 1);

	shortHistoryArr[*rear] = cmd;
}

void deQueue(char** shortHistoryArr, char*cmd, short* rear) {
	if (isEmptyQueue(rear)) {
		return;
	}
	else
		*rear = *rear - 1;

}

void shiftBack(char** shortHistoryArr)
{
	int i;
	for (i = 1; shortHistoryArr[i] != NULL&& i < N; i++)
	{
		shortHistoryArr[i - 1] = shortHistoryArr[i];
	}
	shortHistoryArr[i - 1] = NULL;
}

void insertCommandToHistory(char** short_term_history, List* oldCommands, short* rear, char* command)
{

	char* str;
	ListNodeCommands* cmdToList;
	if (isFullQueue(rear))
	{
		str = short_term_history[0];
		deQueue(short_term_history, str, rear);
		shiftBack(short_term_history);
		cmdToList = createNewListCmdNode(str);
		insertCommandTohistoryList(oldCommands, cmdToList);
		enQueue(short_term_history, command, rear);
	}
	else
	{
		str = createCmdToArray(command);
		enQueue(short_term_history, str, rear);
	}
}

int sizeOfCommands(ListCommands* lst, char ** newCommands)
{
	int i, size = 0;
	if (!isEmptyHistoryList(lst))
		size = ListCommandsSize(lst);
	for (i = 0; i < N; i++)
		if (newCommands[i])
			size++;

	return size;
}

void printStartMessageToUser()
{
	printf("Please enter one of the following commands:\nadd-an-apt, get-an-apt, buy-an-apt or delete-an-apt\nFor reconstruction commands, please enter:\n!!, !num, history, short_history or !num^str1^str2\nTo exit, enter exit.\n");
}

void freeApartmentsList(List* apartments)
{
	int i;
	ListNode* tempApt = apartments->head;
	ListNode* prev;

	while (tempApt)
	{
		free(tempApt->AptData.address);
		prev = tempApt;
		tempApt = tempApt->next;
		free(prev);
	}
}

void freeCommandsList(ListCommands* historyList)
{
	int i;
	ListNodeCommands* tempCmd = historyList->head;
	ListNodeCommands* prevCmd;

	while (tempCmd)
	{
		free(tempCmd->command);
		prevCmd = tempCmd;
		tempCmd = tempCmd->next;
		free(prevCmd);
	}
}

void freeCommandsArray(char** historyArray)
{
	int i;


	for (i = 0; i < N; i++)
	{
		if (historyArray[i])
			free(historyArray[i]);
	}

}

void freeAll(List* apartments, char** historyArray, ListCommands* historyList)
{
	freeApartmentsList(apartments);

	freeCommandsList(historyList);

	freeCommandsArray(historyArray);

}