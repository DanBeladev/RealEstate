#ifndef UTILS_H
#define UTILS_H

typedef struct date {
	short day;
	short month;
	short year;
} Date;

//convert functions.
int strToint(char* str);
int charToInt(char ch);
short charToShort(char ch);
short strToShort(char* str);
short getshort(char* p);

//updates price value.
int getPrice(char* priceStr);

//updates year value.
short getYear(char* y);

//updates time value.
Date getCurrentTime();

//updates date value.
Date getDateFromStr(char* str);
#endif