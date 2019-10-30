#include "apatments.h"
#include <string.h>
#include "utils.h"
#include"files.h"

void main()
{
	char *short_term_history[N];
	List apartments;
	ListCommands oldCommands;

	makeEmptyList(&apartments);
	makeNullArray(short_term_history);
	makeEmptyHistoryList(&oldCommands);

	short  index = -1;
	int aptID = 0;

	FILE* file, *binFile;
	file = fopen("commands.txt", "rt");
	if (file)
	{
		readCommandsFromText(file, short_term_history, &oldCommands, &index);
		fclose(file);
	}

	binFile = fopen("Apartments.bin", "rb");
	if (binFile)
	{
	loadApartmentsfromBinaryFile(&apartments,binFile,&aptID);
		fclose(binFile);
	}
	char* cmd;
	short finished = 0;

	printStartMessageToUser();

	while (!finished)
	{
		cmd = getCommand();

		if (handleInternalCommand(cmd, &finished, short_term_history, &oldCommands, &index, &apartments))
		{
			if (finished)
			{
				freeAll(&apartments, short_term_history, &oldCommands);
				printf("Good Bye!\n");
			}
		}
		else if (handleRestoreCommand(&cmd, short_term_history, &oldCommands, &index))
		{
			insertCommandToHistory(short_term_history, &oldCommands, &index, cmd);
			handleCommand(&apartments, cmd, &aptID);
		}
		else
		{
			insertCommandToHistory(short_term_history, &oldCommands, &index, cmd);
			handleCommand(&apartments, cmd, &aptID);
		}

	}


}






