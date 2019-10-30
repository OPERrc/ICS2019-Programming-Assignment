#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *, bool *);

uint32_t str2num(char *num) {
	// Change str type num into uint32_t type num
	int point = 0;
	uint32_t result = 0;
	while (num[point]) {
		result = result * 10 + num[point] - '0';
		point++;
	}	
	return result;
}

void value_test() {
	// Test whether expr() function works appropriately
	static int total = 0;
	FILE *fp = fopen("tools/gen-expr/input", "r");
	assert(fp != NULL);

	char line[1024];
  char *line_read = line;
	fgets(line_read, 1024, fp);

	for (int i = 1; i <= 100; i++) {
		// Get the result from the file
		char *num = strtok(line_read, " ");
    uint32_t key = str2num(num);

		// Get the EXPR from the file
		char *EXPR = line_read + strlen(num) + 1;
		int point = 0;
		while (EXPR[point] != '\n')
			point++;
		EXPR[point] = '\0';

		bool flag = true;
		bool *success = &flag;
		// Get answer from expr() to test
		uint32_t answer = expr(EXPR, success);
		printf("key = %d, answer = %d", key, answer);
		total++;
		if (key == answer)
			printf(", correct! current line %d\n", total);
		else {
			printf(", wrong! current line %d\n", total);
			assert(0);
		}
		fgets(line_read, 1024, fp);
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
	
	/* Test expr() function */
	// value_test();

  /* Receive commands from user. */
  // is_batch_mode = false;
  ui_mainloop(is_batch_mode);

  return 0;
}
