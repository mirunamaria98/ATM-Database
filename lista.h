#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct history
{
	char HISTORY_INFO[500];
	struct history *history_urm;
}history,*Thistory,**Ahistory;

typedef struct card
{
	unsigned long long CARD_NUMBER;
	char PIN[7];
	char EXPIRY_DATE[6];
	int CVV;
	int BALANCE;
	char STATUS[20];
	history *HISTORY;
	struct card *urm2;

}CARD,*TCARD,*ACARD;

typedef struct LSC
{
	CARD *info1;
	struct LSC *urm1;

}LSC,*TLSC,**ALSC;


typedef struct LC
{
	LSC * info;
	struct LC *urm;

}LC,*TLC,**ALC;




//LSC AlocCelula(void *ae);