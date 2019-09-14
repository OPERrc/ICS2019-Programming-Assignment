#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
// #include ""

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display(void);
uint32_t paddr_read(paddr_t, int);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

// Insert part
static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);
// End of Insert part

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  // Insert part
  { "si", "With an argument [N]. Let the program run N steps then stop. If N is not given it is set to default value '1'", cmd_si },
  { "info", "With an argument 'r' or 'w'. 'r' print the register information, 'w' print the watchpoint information.", cmd_info},
  { "x", "", cmd_x},
  // End of Insert part

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

// Insert part
inline bool is_num(char ch) {
  if (ch >= '0' && ch <= '9')
	return true;
  else
	return false;
}

static int cmd_si(char *args) {
  if (args == NULL)
    cpu_exec(1);
  else {
	uint64_t num = 0, point = 0;

	while (args[point]) {
	  if (is_num(args[point]))
	    num = num * 10 + args[point] - '0';
	  else
	    if (args[point] == ' ') {
		  printf("Too many arguments! Need only 1 non-negative integer.\n");
		  return 0;
		}
		else {
		  printf("Arguments input error! Need 1 non-negative integer.\n");
		  return 0;
		}
	  point = point + 1;
	}

	cpu_exec(num);
  }
  return 0; 
}

static int cmd_info(char *args) {
  if (args == NULL) {
    printf("Lack of arguments! Need an 'r' or 'w' argument.\n");
	return 0;
  }
  else {
    if (strcmp(args, "r") == 0) {
	  isa_reg_display();
	  return 0;
	}	
	if (strcmp(args, "w") == 0) {
	  // Insert watchpoint code here
	  return 0;
	}
	printf("Arguments input error! Need only an 'r' or 'w' argument.\n");
    return 0;
  }
}

static int cmd_x(char *args) {
  // Get the first argument N
  char *N = strtok(args, " ");
  if (N == NULL) {
    printf("Lack of arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }
  
  // Get the second argument EXPR
  args = NULL;
  char *EXPR = strtok(args, " ");
  if (EXPR == NULL) {
    printf("Lack of arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }

  // Get the remaining string
  args = NULL;
  char *EMPTY = strtok(args, " ");
  if (EMPTY != NULL) {
    printf("Too many arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }

  // Get the number from N
  int point = 0, num = 0;
  while (N[point]) {
    if (is_num(N[point])) {
	  num = num * 10 + N[point] - '0';
	  point++;
	}
    else {
	  printf("Arguments input error! The first argument 'N' should be a non-negative integer!\n");
	  return 0;
	}	
  }

  // Get the address from EXPR, the result of EXPR should be a 16bit integer
  // EXPR = function(EXPR); // Here should be the funciton to calculate EXPR
  paddr_t expr = 0;
  point = 2;
  
  inline bool is_a2f(char ch) {
    if (ch >= 'a' && ch <='f')
	  return true;
	return false;
  }

  while (EXPR[point]) {
    if (is_num(EXPR[point])) {
	  expr = expr * 16 + EXPR[point] - '0';
	  point++;
	}
	else
	if (is_a2f(EXPR[point])) {
	  expr = expr * 16 + 10 + EXPR[point] - 'a';
	  point++;
	}
	else {
	  printf("Arguments input error! The second argument should be an expression!\n");
	  return 0;
	}
  }

  for (int i=0; i<num; i++)
    printf("0x%-12x: 0x%x\n", expr+i, paddr_read(expr+i, 3));   
  
  return 0;
}
// End of Insert part

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
