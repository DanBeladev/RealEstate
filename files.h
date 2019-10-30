#include "apatments.h"

typedef unsigned char BYTE;


//insert back values to node from binary file
void readDetailsToNodeFromFile(ListNode* node, FILE* f);

//calculate the size of file in bytes
long int fileSize(FILE* file);

//create new node to list
ListNode* createNode();

//updating by the lats node ID in apartments list
void updateAptIDIndex(List* apartments, int* aptID);

//main function that manage the loading details from binary file to apartments list
void loadApartmentsfromBinaryFile(List* apartments,FILE* f,int* aptID);

//write to binary file the details from list
void writeAptDetailsBin(Apartment AptData, FILE* f);

//main function to manage the writing the apartments list to bin file
void writeApartmentsInBinaryFile(List* apartments);

//writes all commands into txt. file
void writeCommandsToText(ListCommands* cmd, char** history);

//write from old commands list to txt. file.
void writeListHistoryToText(FILE* fp, ListCommands* cmd);

//write from commands array list to txt. file.
void writeArrayHistoryToText(FILE* fp, char** history);

//reads the commands from the text and adds them to the datas.
void readCommandsFromText(FILE *fp, char** history, ListCommands* cmd, short* index);

//counts how many commands are in the text file.
int getHowManyCommandsFromText(FILE *fp);

//inserts command to head of list.
void insertCmdToHeadListByTextFile(ListCommands* ls, ListNodeCommands* node);

//insert commands from text to commands arr by given index.
void insertCmdToArrayHistoryByIndex(int fromIndex, FILE *fp, char** history);

//updates index after getting data from commands array.
void updateIndexArray(char** historyArr, short* index);

//reads and create new string from command from txt. file
char* getCommandFromText(FILE* f);

//all the commands that did not go into the array will be in the list.
void insertCmdToListByDiffSize(int diffSize, ListCommands* cmd, FILE* fp);