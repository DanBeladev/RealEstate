#ifndef APRTMENTS_H
#define APRTMENTS_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include<string.h>


#define N 7


typedef struct apartment {
	int aptID;
	char* address;
	int price;
	short numOfRooms;
	Date enterDate;
	Date createdDate;
} Apartment;

typedef struct listNode {
	Apartment AptData;
	struct listNode* next;
} ListNode;

typedef struct list {
	ListNode* head;
	ListNode* tail;
} List;

typedef struct listNodeCommands {
	char* command;
	struct listNodeCommands* next;
	struct listNodeCommands* prev;
} ListNodeCommands;

typedef struct listCommands {
	ListNodeCommands* head;
	ListNodeCommands* tail;
} ListCommands;

//Function that compares two strings//
int isEqual(char * a, char * b);

// Function that recieves a string(command) and a paremeter, checks if the command is internal, and executes it if needed// 
short handleInternalCommand(char * cmd, short* finished, char** shortHistory, ListCommands* oldCommandsList, short* rear, List* apartments);

// Function that recieves a string(command) and a paremeter, checks if the command is a restore type, and executes it if needed// 
short handleRestoreCommand(char** cmd, char** shortHistory, ListCommands* oldCommands, short* rear);

//Function that handle the "get an apt" command type//
void handleGetAnApt(List* apartments, const char * cmd);

//Function that deletes all the apartments entered in the last -x- days//
void handleDeleteByDays(List* apartments, short numOfDays);

//Function that check if the command is related to the apartment data. executes it if needed//
void handleCommand(List* apartments, char * cmd, int* aptID);

//recieves the command by allocating dynamic string//
char* getCommand();

void makeEmptyList(List* ls);

//creates a new node in the list.
ListNode* createNewListNode(char* cmd, int* aptID);

short isEmptyList(List* ls);

int ListSize(List* lst);
int ListCommandsSize(ListCommands* lst);

void insertToStartList(List* aptlist, ListNode* aptNode);

void insertToEndList(List* aptlist, ListNode* aptNode);

//inserts command to the end of list.
void ToEndHistoryList(ListCommands* oldCommands, ListNodeCommands* cmdNode);

//deletes an apartment by given ID
void buyAnApartment(List* ls, int ID);

//creates a new apartment node and adds it to the list
void addAnApartment(List* apartments, char* cmd, int* aptID);

//prints the apartments by maximum rooms number.
void printMaximunRooms(List* apartments, short maxRooms);

//prints the apartments by maximum rooms number.
void printMinimumRooms(List* apartments, short minRooms);

//prints apartment details
void printApartment(Apartment apt);

//prints apartments by entry date.
void printByDate(List* apartments, Date entryDate);

//prints apartments in reverse.
printRevArr(Apartment* aptArr, int size);

//prints apartments array.
printArr(Apartment* aptArr, int size);

//initializing apartment data into the node.
void copyAptData(Apartment* apt, ListNode* node);

//calculates and updates exact day in year, based on "num of days" paremeter entered
void CurrentDateMinusNumOfDays(Date* d, int numOfDays);

//updates month when days reach day "0".
void updateDaysByMonth(Date* d, int month);

//prints ID of apartments entered in last -x- days
void handleEnterFunc(List* apartments, int days);

//merge the apartments array by price
void mergeApartmentsArray(Apartment* arr1, int size1, Apartment* arr2, int size2, Apartment* res);
void mergeSortApartmentsArray(Apartment* arr, int size);


//checks which command where entered, and executes wished commands, so we will know which apartments to print by command(and send to list).
int checkFilters(Apartment * apt, ListNode* n, short minNumRooms, short maxNumRooms, int maxPrice, Date freeUntilDate);

//insert the apartments from the list to the arr by the commands restrictions.
void ListToAptArr(List* lst, short minNumRoom, short maxNumRooms, int maxPrice, Date freeUntilDate, short sortFlag, short reverseFlag, short enterFlag);

//checks if commands array is empty.
short isEmptyQueue(short* rear);

//checks if commands array is full.
short isFullQueue(short* rear);

//insert command to array
void enQueue(char** shortHistoryArr, char*cmd, short* rear);

//releases command from array
void deQueue(char** shortHistoryArr, char*cmd, short* rear);

//makes all the strings in the array to shift back one spot
void shiftBack(char** shortHistoryArr);

//puts NULL in head and tail of history commands list
void makeEmptyHistoryList(ListCommands* oldCommands);

//checks if commands list is empty
short isEmptyHistoryList(ListCommands* oldCommands);

//insert oldest command in array to history command list
void insertCommandTohistoryList(ListCommands* oldCommands, ListNodeCommands* cmdNode);
void insertCommandToHistory(char** short_term_history, List* oldCommands, short* rear, char* command);

//finds command by ID.
char* getCommandByID(short cmdID, char** shortHistory, ListCommands* oldCommandsList);

//creates node for new command(to list)
ListNodeCommands* createNewListCmdNode(char* cmd);

void makeNullArray(char** historyArray);

//prints all the commands.
void printHistory(char ** shortHistory, ListCommands* oldCommandsList);

//prints all commands in array.
void printShortHistory(char ** shortHistory, ListCommands* oldCommandsList);

//prints commands in list.
void printListCommands(ListCommands* oldCommands, int* sizeList);

//counts how many commands as entered.
int sizeOfCommands(ListCommands* lst, char ** newCommands);

//restores and executes last command entered.
void restoreLastCommand(char** cmd, char**  shortHistory, short* rear);

//restores command by gives apartment code
void restoreByCmdID(char** cmd, char** shortHistory, ListCommands* oldCommandsList);

//creates and allocates string for array.
char* createCmdToArray(char* cmd);

//recieves original string, and replace wished substring in it, for a given substring.
char * replaceSubStr(char *origCmd, char *subCmd, char *token);

//extarcts apartment details from command.
void SetaptDetails(Apartment* apt, char* str, int* aptId);

//free all memory data allocated during runtime.
void freeAll(List* apartments, char** historyArray, ListCommands* historyList);
void freeApartmentsList(List* apartments);
void freeCommandsList(ListCommands* historyList);
void freeCommandsArray(char** historyArray);

void printStartMessageToUser();

#endif // APRTMENTS_H