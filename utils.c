#include "utils.h"
#include <time.h>

Date getCurrentTime()
{
	Date currentTime;

	time_t    now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	currentTime.year = tstruct.tm_year + 1900;
	currentTime.month = tstruct.tm_mon + 1;
	currentTime.day = tstruct.tm_mday;

	return currentTime;
}

int strToint(char* str)
{
	int newNum = 0;
	int size = strlen(str);
	for (int i = 0; i < size; i++)
	{
		newNum = newNum * 10 + charToInt(str[i]);
	}
	return newNum;
}

int charToInt(char ch)
{
	return ch - '0';
}

short charToShort(char ch)
{
	return ch - '0';
}

short strToShort(char* str)
{
	short newNum = 0;
	int size = strlen(str);
	for (int i = 0; i < size; i++)
	{
		newNum = newNum * 10 + charToInt(str[i]);
	}
	return newNum;
}

short getshort(char* p)
{
	return strToint(p);
}

int getPrice(char* priceStr)
{
	return strToint(priceStr);
}

short getYear(char* y)
{
	int i;
	short res = 20;
	int size = strlen(y);
	for (i = 0; i < size; i++)
	{
		res = res * 10 + charToInt(y[i]);
	}
	return res;
}

Date getDateFromStr(char* str)
{
	Date d;
	int i;
	short day = 0, month = 0, year = 0;

	for (i = 0; i < 2; i++)
	{
		day = day * 10 + (str[i] - '0');
	}
	for (i = 2; i < 4; i++)
	{
		month = month * 10 + (str[i] - '0');
	}
	for (i = 4; i < 8; i++)
	{
		year = (year * 10) + (str[i] - '0');
	}
	d.day = day;
	d.month = month;
	d.year = year;
	return d;
}