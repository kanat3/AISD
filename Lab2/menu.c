#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "function.h"
#include "deque.h"

int dq_quantity (void) {
	int quantity, ch;
	puts("Please use unsigned integer and not '0'. ");
	puts("Enter max quantity patients in the queue: \n");
	do {
		ch = scanf("%d", &quantity);
		getchar(); //abort aborting ache vsmisli
	} while (ch <= 0 || quantity <= 0);
	printf("\n");
	return quantity;
}

int menu(const char *msgs[], const int n) {
	int choice;
	choice = 0;;
	for (int i = 0; i < n; ++i) {
		puts(msgs[i]);
	}
	puts("\nMake your choice: \n");
	int k;
	k = scanf("%d", &choice);
	getchar();
	if (k <= 0) {
		puts("\nYou're wrong. Try again!\n");
		while (getchar() != '\n') {}
		return 0;
	}
	if ((choice < 1) || (choice > 3)) {
		puts("\nYou're wrong. Try again!\n");
		return -1;
	}
	return choice;
}

int dialog (void) {
	const char *MSGS[] = {"1. Exit", "2. Display the status of the queue (not working) ", "3. Add a patient to the queue"};
	const int MSGS_SIZE = 3;
	int choice;
	do {
		choice = menu(MSGS, MSGS_SIZE);
		if (choice == 1) {
			exit (0);
		}
		if (choice == 2) {
			puts("\n> Display the status of the queue <\n");
		}
		if (choice == 3) {
			#ifdef DQ_LIST
			puts("\n> Add a patient to the queue <\n");
			get_str();
			create_dq();
			#else
			int q;
			q = dq_quantity();
			puts("\n> Add a patient to the queue <\n");
			get_str();
			create_dq(q);
			#endif
		}
	} while (choice != 1);
	return 0;
}