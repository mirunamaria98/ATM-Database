#include <stdio.h>
#include "lista.h"
#include <stdlib.h>
#include<string.h>
TLC AlocLC()
{
	TLC aux = (TLC) calloc(1,sizeof(LC));
	if (!aux)
		return NULL;
	aux->info = NULL;
	aux->urm = NULL;
	return aux;
}

int InsInc(ALC L)
{
	TLC aux = AlocLC();
	if (!aux)
		return 0;
	aux->urm = *L;
	*L = aux;
	return 1;
}

TLSC AlocCard(unsigned long long CARD_NUMBER, char PIN[7], int CVV,
	      char *EXPIRY_DATE, int BALANCE, char *STATUS)
{
	TLSC aux = (TLSC) calloc(1,sizeof(LSC));
	if (!aux)
		return NULL;
	aux->info1 = (TCARD) calloc(1,sizeof(CARD));
	if (!aux->info1) {
		free(aux);
		return NULL;
	}
	aux->info1->CARD_NUMBER = CARD_NUMBER;
	strcpy(aux->info1->PIN, PIN);
	strcpy(aux->info1->EXPIRY_DATE, EXPIRY_DATE);
	aux->info1->CVV = CVV;
	aux->info1->BALANCE = BALANCE;
	strcpy(aux->info1->STATUS, STATUS);
	aux->info1->urm2 = NULL;
	aux->urm1 = NULL;
	return aux;

}

Thistory AlocHistory(char HISTORY[1000])
{
	Thistory aux = (Thistory) malloc(sizeof(history));
	if (!aux) {
		free(aux);
		return NULL;
	}
	strcpy(aux->HISTORY_INFO, HISTORY);
	aux->history_urm = NULL;
	return aux;
}

int inserare_history(Thistory * L, char h[100])
{
	Thistory aux = AlocHistory(h);
	if (!aux)
		return 0;
	aux->history_urm = *L;
	*L = aux;
	return 1;
}

int cardul_exista(TLC lista, unsigned long long CARD_NUMBER, FILE * out)
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {

		for (m = p->info; m != NULL; m = m->urm1) {

			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				fprintf(out, "The card already exists\n");
				return 1;
			}
		}
	}
	return 0;
}

int Adaugare_card(ALSC lista, unsigned long long CARD_NUMBER, char PIN[7],
		  int CVV, char EXPIRY_DATE[6], int BALANCE, char STATUS[20])
{
	TLSC aux =
	    AlocCard(CARD_NUMBER, PIN, CVV, EXPIRY_DATE, BALANCE, STATUS);
	if (!aux)
		return 0;
	aux->urm1 = *lista;
	*lista = aux;
	return 1;
}

int sumacifre(unsigned long long nr_card)
{
	int suma = 0;
	while (nr_card != 0) {
		suma = suma + nr_card % 10;
		nr_card = nr_card / 10;
	}
	return suma;
}

int pozitie(unsigned long long nr_card, int nr_max_carduri)
{
	int poz;
	int suma = sumacifre(nr_card);
	poz = suma % (nr_max_carduri);
	return poz;
}

void show_card(TLC lista, unsigned long long CARD_NUMBER, FILE * out)
{
	TLC p;
	TLSC m;
	Thistory h;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				fprintf(out, "(card number: %016llu, ",
					m->info1->CARD_NUMBER);
				fprintf(out, "PIN: %s, ", m->info1->PIN);
				fprintf(out, "expiry date: %s, ",
					m->info1->EXPIRY_DATE);
				fprintf(out, "CVV: %d, ", m->info1->CVV);
				fprintf(out, "balance: %d, ",
					m->info1->BALANCE);
				fprintf(out, "status: %s, history: [",
					m->info1->STATUS);
				if (m->info1->HISTORY->HISTORY_INFO == NULL)
					fprintf(out, "])\n");
				else {
					for (h = m->info1->HISTORY; h != NULL;
					     h = h->history_urm) {
						fprintf(out, "(%s)",
							h->HISTORY_INFO);
						if (h->
						    history_urm->HISTORY_INFO ==
						    NULL) {
							fprintf(out, "])\n");
						} else {
							fprintf(out, ", ");
						}
					}
				}

			}
		}
	}
}

void show(TLC L, FILE * out, int numar_max_carduri, int max)
{

	TLC p;
	TLSC m;
	Thistory h;
	int contor = 0, contor1 = 0;
	for (p = L; p != NULL; p = p->urm) {
		if (p->info != NULL) {
			fprintf(out, "pos%d: [\n", contor);
			for (m = p->info; m != NULL; m = m->urm1) {

				fprintf(out, "(card number: %016llu, ",
					m->info1->CARD_NUMBER);
				fprintf(out, "PIN: %s, ", m->info1->PIN);
				fprintf(out, "expiry date: %s, ",
					m->info1->EXPIRY_DATE);
				fprintf(out, "CVV: %d, ", m->info1->CVV);
				fprintf(out, "balance: %d, ",
					m->info1->BALANCE);
				fprintf(out, "status: %s, ", m->info1->STATUS);
				fprintf(out, "history: [");
				if (m->info1->HISTORY->HISTORY_INFO == NULL)
					fprintf(out, "])\n");
				else {
					for (h = m->info1->HISTORY; h != NULL;
					     h = h->history_urm) {
						contor++;
						contor1++;
						fprintf(out, "(%s)",
							h->HISTORY_INFO);
						if (contor == 0) {
							fprintf(out, "])\n");
						}
						if (h->
						    history_urm->HISTORY_INFO ==
						    NULL) {
							fprintf(out, "])\n");
						} else {
							fprintf(out, ", ");
						}
					}
					contor = 0;
				}

			}
			fprintf(out, "]\n");
		} else if (contor1 == 0) {
			if (contor <= max)
				fprintf(out, "pos%d: []\n", contor);
			contor1 = 0;
		}

		contor++;
	}

}

int delete_card(TLSC * lista, unsigned long long CARD_NUMBER)
{
	TLSC ant, p;
	for (p = *lista, ant = NULL; p != NULL; ant = p, p = p->urm1) {
		if (p->info1->CARD_NUMBER == CARD_NUMBER) {
			break;
		}
	}
	if (p == NULL)
		return 0;

	if (ant == NULL)
		*lista = p->urm1;
	else
		ant->urm1 = p->urm1;
	free(p);

	return 1;
}

int insert_card(TLC lista, unsigned long long CARD_NUMBER, char PIN[7],
		FILE * out, int contor)
{
	TLC p;
	TLSC m;
	char HISTORY_INFO[100];
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				if (strcmp(m->info1->STATUS, "LOCKED") == 0) {
					sprintf(HISTORY_INFO,
						"FAIL, insert_card %llu %s",
						CARD_NUMBER, PIN);
					inserare_history(&m->info1->HISTORY,
							 HISTORY_INFO);
					return 0;
				} else {
					if (strcmp(m->info1->PIN, PIN) != 0) {
						fprintf(out, "Invalid PIN\n");
						sprintf(HISTORY_INFO,
							"FAIL, insert_card %llu %s",
							CARD_NUMBER, PIN);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
						return 1;
					} else
					    if (strcmp
						(m->info1->STATUS,
						 "ACTIVE") != 0) {
						fprintf(out,
							"You must change your PIN.\n");
						sprintf(HISTORY_INFO,
							"SUCCESS, insert_card %llu %s",
							CARD_NUMBER, PIN);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
					} else {
						sprintf(HISTORY_INFO,
							"SUCCESS, insert_card %llu %s",
							CARD_NUMBER, PIN);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);

					}
				}
			}
		}
	}
	return 0;

}

void cancel(TLC lista, unsigned long long CARD_NUMBER)
{
	char HISTORY_INFO[100];
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				sprintf(HISTORY_INFO, "SUCCESS, cancel %llu",
					CARD_NUMBER);
				inserare_history(&m->info1->HISTORY,
						 HISTORY_INFO);
			}
		}
	}
}

void card_status(TLC lista, unsigned long long CARD_NUMBER, char STATUS[20])
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER)
				strcpy(m->info1->STATUS, "LOCKED");
		}
	}
}

void card_status1(TLC lista, unsigned long long CARD_NUMBER, char STATUS[20])
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER)
				strcpy(m->info1->STATUS, "ACTIVE");
		}
	}
}

void unblock_card(TLC lista, unsigned long long CARD_NUMBER)
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER)
				strcpy(m->info1->STATUS, "ACTIVE");
		}
	}
}

int string(char PIN[7])
{
	int i, flag = 0;
	for (i = 0; i < strlen(PIN); i++) {
		if ((PIN[i] >= 'a' && PIN[i] <= 'z')
		    || (PIN[i] >= 'A' && PIN[i] <= 'Z')) {
			flag = flag + 1;
		}
	}
	if (flag == 0)
		return 1;
	else
		return 0;
}

int pin_change(TLC lista, unsigned long long CARD_NUMBER, char PIN[7],
	       FILE * out)
{
	TLC p;
	TLSC m;
	char HISTORY_INFO[100];
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				if (strlen(PIN) != 4) {

					sprintf(HISTORY_INFO,
						"FAIL, pin_change %llu %s",
						CARD_NUMBER, PIN);
					inserare_history(&m->info1->HISTORY,
							 HISTORY_INFO);
					fprintf(out, "Invalid PIN\n");
				}

				else {
					if (string(PIN) == 1) {
						sprintf(HISTORY_INFO,
							"SUCCESS, pin_change %llu %s",
							CARD_NUMBER, PIN);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
						strcpy(m->info1->PIN, PIN);
						return 1;
					} else {
						sprintf(HISTORY_INFO,
							"FAIL, pin_change %llu %s",
							CARD_NUMBER, PIN);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
						fprintf(out, "Invalid PIN\n");
					}

				}
			}

		}
	}
	return 0;
}

void balance_inquiry(TLC lista, unsigned long long CARD_NUMBER, FILE * out)
{
	TLC p;
	TLSC m;
	char HISTORY_INFO[100];
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				fprintf(out, "%d\n", m->info1->BALANCE);
				sprintf(HISTORY_INFO,
					"SUCCESS, balance_inquiry %llu",
					CARD_NUMBER);
				inserare_history(&m->info1->HISTORY,
						 HISTORY_INFO);
			}
		}
	}

}

void balance_card(TLC lista, unsigned long long CARD_NUMBER, int BALANCE,
		  int numerar)
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				m->info1->BALANCE = m->info1->BALANCE + numerar;
			}
		}
	}
}

int recharge(TLC lista, unsigned long long CARD_NUMBER, int numerar, FILE * out)
{
	TLC p;
	TLSC m;
	char HISTORY_INFO[100];
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				if ((numerar % 10) != 0) {
					fprintf(out,
						"The added amount must be multiple of 10\n");
					sprintf(HISTORY_INFO,
						"FAIL, recharge %llu %d",
						CARD_NUMBER, numerar);
					inserare_history(&m->info1->HISTORY,
							 HISTORY_INFO);
					return 0;
				}

				else {
					sprintf(HISTORY_INFO,
						"SUCCESS, recharge %llu %d",
						CARD_NUMBER, numerar);
					inserare_history(&m->info1->HISTORY,
							 HISTORY_INFO);
					numerar = numerar + m->info1->BALANCE;
					fprintf(out, "%d\n", numerar);
					return 1;
				}
			}
		}
	}
	return 0;
}

int cash_withdrawal(TLC lista, unsigned long long CARD_NUMBER, int numerar,
		    FILE * out)
{
	TLC p;
	TLSC m;
	char HISTORY_INFO[100];
	printf("%d\n", numerar);
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER) {
				if ((numerar % 10) != 0) {
					fprintf(out,
						"The requested amount must be multiple of 10\n");
					sprintf(HISTORY_INFO,
						"FAIL, cash_withdrawal %llu %d",
						CARD_NUMBER, numerar);
					inserare_history(&m->info1->HISTORY,
							 HISTORY_INFO);
					return 0;
				}

				else {

					if (m->info1->BALANCE < numerar) {
						fprintf(out,
							"Insufficient funds\n");
						sprintf(HISTORY_INFO,
							"FAIL, cash_withdrawal %llu %d",
							CARD_NUMBER, numerar);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
						return 0;

					} else {
						sprintf(HISTORY_INFO,
							"SUCCESS, cash_withdrawal %llu %d",
							CARD_NUMBER, numerar);
						inserare_history(&m->
								 info1->HISTORY,
								 HISTORY_INFO);
						numerar =
						    m->info1->BALANCE - numerar;
						fprintf(out, "%d\n", numerar);
						return 1;
					}
				}
			}
		}
	}
	return 1;
}

void cash_balance(TLC lista, unsigned long long CARD_NUMBER, int BALANCE,
		  int numerar)
{
	TLC p;
	TLSC m;
	for (p = lista; p != NULL; p = p->urm) {
		for (m = p->info; m != NULL; m = m->urm1) {
			if (m->info1->CARD_NUMBER == CARD_NUMBER)
				m->info1->BALANCE = m->info1->BALANCE - numerar;
		}
	}
}

int transfer_funds(TLC lista, unsigned long long CARD_NUMBER1,
		   unsigned long long CARD_NUMBER2, int numerar, FILE * out)
{
	TLC p;
	TLSC m1, m2;
	char HISTORY_INFO[100];
	int verificare = 0;
	for (p = lista; p != NULL; p = p->urm) {
		for (m1 = p->info; m1 != NULL; m1 = m1->urm1) {
			if (m1->info1->CARD_NUMBER == CARD_NUMBER1) {
				if ((numerar % 10) != 0) {
					fprintf(out,
						"The transferred amount must be multiple of 10\n");
					sprintf(HISTORY_INFO,
						"FAIL, transfer_funds %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					inserare_history(&m1->info1->HISTORY,
							 HISTORY_INFO);
					return 0;
				}
				if (m1->info1->BALANCE < numerar) {
					fprintf(out, "Insufficient funds\n");
					sprintf(HISTORY_INFO,
						"FAIL, transfer_funds %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					inserare_history(&m1->info1->HISTORY,
							 HISTORY_INFO);
					return 0;
				} else {
					verificare++;
					sprintf(HISTORY_INFO,
						"SUCCESS, transfer_funds %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					inserare_history(&m1->info1->HISTORY,
							 HISTORY_INFO);
					fprintf(out, "%d\n",
						(m1->info1->BALANCE) - numerar);

				}

			}
		}
	}

	for (p = lista; p != NULL; p = p->urm) {
		for (m2 = p->info; m2 != NULL; m2 = m2->urm1) {
			if (m2->info1->CARD_NUMBER == CARD_NUMBER2) {
				if (verificare != 0) {
					sprintf(HISTORY_INFO,
						"SUCCESS, transfer_funds %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					inserare_history(&m2->info1->HISTORY,
							 HISTORY_INFO);
					numerar = m2->info1->BALANCE - numerar;
				}

			}
		}
	}

	return 1;

}

int eliminare_history(Thistory * lista, char HISTORY_INFO[1000])
{
	Thistory ant, p;
	for (p = *lista, ant = NULL; p != NULL; ant = p, p = p->history_urm) {
		if (strcmp(p->HISTORY_INFO, HISTORY_INFO) == 0)
			break;
	}
	if (p == NULL)
		return 0;
	if (ant == NULL)
		*lista = p->history_urm;
	else
		ant->history_urm = p->history_urm;
	free(p);
	return 1;
}

int reverse_transaction(TLC lista, unsigned long long CARD_NUMBER1,
			unsigned long long CARD_NUMBER2, int numerar,
			FILE * out)
{
	TLC p;
	TLSC m1, m2;
	char HISTORY_INFO[100];
	int verificare = 0;
	for (p = lista; p != NULL; p = p->urm) {
		for (m2 = p->info; m2 != NULL; m2 = m2->urm1) {
			if (m2->info1->CARD_NUMBER == CARD_NUMBER2) {
				if (numerar <= m2->info1->BALANCE) {
					verificare++;
					sprintf(HISTORY_INFO,
						"SUCCESS, transfer_funds %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					eliminare_history(&m2->info1->HISTORY,
							  HISTORY_INFO);
				}
			}
		}

	}
	for (p = lista; p != NULL; p = p->urm) {
		for (m1 = p->info; m1 != NULL; m1 = m1->urm1) {
			if (m1->info1->CARD_NUMBER == CARD_NUMBER1) {
				if (verificare != 0) {
					sprintf(HISTORY_INFO,
						"SUCCESS, reverse_transaction %llu %llu %d",
						CARD_NUMBER1, CARD_NUMBER2,
						numerar);
					inserare_history(&m1->info1->HISTORY,
							 HISTORY_INFO);
					return 1;


				} else {
					fprintf(out,
						"The transaction cannot be reversed\n");

					return 0;
				}
			}
		}

	}
	return 1;
}
/*void dezaloca_history(Thistory *lista){
	Thistory m;
	while( *lista){
		m=*lista;
		lista=m->history_urm;
		free(m);
	}
}*/
void dezalocare_LSC(TLSC *lista){
	TLSC m;
		while(*lista){
			m=*lista;
			*lista= m->urm1;
			//dezaloca_history(&m->info1->HISTORY);
			free(m->info1);
			free(m);
		}
}
void dezalocare_LC( TLC *lista){
	TLC m;
	while(*lista){
		m=*lista;
		*lista=m->urm;
		dezalocare_LSC(&m->info);
		free(m);
	}

}
int main()
{
	char comanda[100];
	int numar_max_carduri = 0, copie = 0, CVV, BALANCE, blocat = 0, numerar;
	char PIN[7], EXPIRY_DATE[6], STATUS[20];
	unsigned long long CARD_NUMBER, CARD_NUMBER1, CARD_NUMBER2;
	TLC lista = NULL;
	TLC aux = NULL;
	FILE *in;
	in = fopen("input.in", "r");
	if (in == NULL) {
		fprintf(stderr, "ERROR: Can't open file %s\n", "input.in");
		return -1;
	}
	fscanf(in, "%d", &numar_max_carduri);

	char output_filename[] = "output.out";
	FILE *out;
	out = fopen(output_filename, "w");
	copie = numar_max_carduri;
	while (copie != 0) {
		InsInc(&lista);
		copie--;
	}
	int poz = 0;
	copie = numar_max_carduri;
	int contor = 0;
	aux = lista;
	int max = -1;
	while ((fscanf(in, "%s", comanda) != EOF)) {
		if (strcmp(comanda, "add_card") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			fscanf(in, "%s", PIN);
			fscanf(in, "%s", EXPIRY_DATE);
			fscanf(in, "%d", &CVV);
			BALANCE = 0;
			strcpy(STATUS, "NEW");
			poz = pozitie(CARD_NUMBER, numar_max_carduri);
			contor = 0;
			if (max < poz)
				max = poz;
			for (aux = lista; aux != NULL; aux = aux->urm) {
				if (poz == contor) {

					if (cardul_exista
					    (lista, CARD_NUMBER, out) == 0) {
						Adaugare_card(&aux->info,
							      CARD_NUMBER, PIN,
							      CVV, EXPIRY_DATE,
							      BALANCE, STATUS);
					}
				}
				contor++;
			}
		}

		if (strcmp(comanda, "show") == 0) {
			if (fscanf(in, "%llu", &CARD_NUMBER) == 1)
				show_card(lista, CARD_NUMBER, out);
			else
				show(lista, out, numar_max_carduri, max);

		}
		if (strcmp(comanda, "delete_card") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			for (aux = lista; aux != NULL; aux = aux->urm) {
				delete_card(&aux->info, CARD_NUMBER);
			}
			blocat = 0;
		}
		if (strcmp(comanda, "insert_card") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			fscanf(in, "%s", PIN);
			if (insert_card(lista, CARD_NUMBER, PIN, out, contor) ==
			    1)
				blocat++;
			if (blocat == 3) {
				card_status(lista, CARD_NUMBER, STATUS);
				fprintf(out,
					"The card is blocked. Please contact the administrator.\n");
			}
		}
		if (strcmp(comanda, "cancel") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			cancel(lista, CARD_NUMBER);
			blocat--;
		}
		if (strcmp(comanda, "unblock_card") == 0) {
			blocat = 0;
			fscanf(in, "%llu", &CARD_NUMBER);
			unblock_card(lista, CARD_NUMBER);
		}
		if (strcmp(comanda, "pin_change") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			fscanf(in, "%s", PIN);
			if (pin_change(lista, CARD_NUMBER, PIN, out) == 1)
				card_status1(lista, CARD_NUMBER, STATUS);

		}
		if (strcmp(comanda, "balance_inquiry") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			balance_inquiry(lista, CARD_NUMBER, out);
		}
		if (strcmp(comanda, "recharge") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			fscanf(in, "%d", &numerar);
			if (recharge(lista, CARD_NUMBER, numerar, out) == 1) {
				balance_card(lista, CARD_NUMBER, BALANCE,
					     numerar);
			}
		}
		if (strcmp(comanda, "cash_withdrawal") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER);
			fscanf(in, "%d", &numerar);
			if (cash_withdrawal(lista, CARD_NUMBER, numerar, out) ==
			    1)
				cash_balance(lista, CARD_NUMBER, BALANCE,
					     numerar);
		}
		if (strcmp(comanda, "transfer_funds") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER1);
			fscanf(in, "%llu", &CARD_NUMBER2);
			fscanf(in, "%d", &numerar);
			//printf("%llu \n%llu\n",CARD_NUMBER1,CARD_NUMBER2 );
			if (transfer_funds
			    (lista, CARD_NUMBER1, CARD_NUMBER2, numerar,
			     out) == 1) {
				balance_card(lista, CARD_NUMBER2, BALANCE,
					     numerar);
				cash_balance(lista, CARD_NUMBER1, BALANCE,
					     numerar);
			}

		}
		if (strcmp(comanda, "reverse_transaction") == 0) {
			fscanf(in, "%llu", &CARD_NUMBER1);
			fscanf(in, "%llu", &CARD_NUMBER2);
			fscanf(in, "%d", &numerar);
			if (reverse_transaction
			    (lista, CARD_NUMBER1, CARD_NUMBER2, numerar,
			     out) == 1) {
				balance_card(lista, CARD_NUMBER1, BALANCE,
					     numerar);
				cash_balance(lista, CARD_NUMBER2, BALANCE,
					     numerar);
			}
		}
	}
	dezalocare_LC(&lista);
	fclose(in);
	fclose(out);
	return 0;
}
