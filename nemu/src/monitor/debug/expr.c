#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM = 1000

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
	{"\\-", '-'},         // minus
	{"\\*", '*'},         // multiply
	{"\\/", '/'},         // division
	{"\\(", '('},         // left parentheses
	{"\\)", ')'},         // right parentheses
	{"[0-9]+", TK_NUM},   // decimal number
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
					case TK_NUM: {
						int start = position - substr_len;
						for (int j = start; j < position; j++) {
							if ((j - start) > 32)
								assert(0);
							tokens[nr_token].str[j-start] = e[j];
						}
						if (substr_len < 32)
							tokens[nr_token].str[position] = '\0';
						tokens[nr_token].type = TK_NUM;
						nr_token++;
						break;
					};
					case TK_NOTYPE: break;
					default: 
						tokens[nr_token].type = rules[i].token_type;
					  nr_token++;	
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

// Insert part
bool checkparentheses(int left, int right) {
	if (tokens[left].type != '(')
		return false;
	if (tokens[right].type != ')')
		return false;

	int num_l = 1;
	bool flag = true;
	for (int i = left+1; i <= right-1; i++) {
		if (num_l == 0)
			flag = false;
		if (num_l < 0)
			assert(0);
		if (tokens[i].type == '(')
			num_l++;
		if (tokens[i].type == ')')
			num_l--;
	}
	
	if (num_l > 1)
		assert(0);

	return flag;
}

inline int priority(int type) {
	switch (type) {
		case '+': return 1;
		case '-': return 1;
		case '*': return 2;
		case '/': return 2;
		case TK_NUM: return 3;
		default: assert(0);
	}
}

int find_majority_token_position(int left, int right) {
	int min_priority = 1000;
	int point = left;
	int in_parentheses = 0;
	for (int i = left; i <= right; i++) {
		if (tokens[i].type == '(') 
			in_parentheses++;

		if (in_parentheses == 0) {
			int cur_priority = priority(tokens[i].type);
			if (cur_priority <= min_priority) {
		  	min_priority = cur_priority;
				point = i;
			}
		}
		
		if (tokens[i].type == ')')
			in_parentheses--;
  }
	return point;
}

uint32_t eval(int left, int right) {
	if (left > right) 
		assert(0);

	else if (left == right) {
		uint32_t num = 0;
		int point = 0;
		while(point < 32 && tokens[left].str[point]) {
			num = num * 10 + tokens[left].str[point] - '0';
			point++;
		}
		return num;
	}
	
	else if (checkparentheses(left, right) == true) 
		return eval(left+1, right-1);

	else {
		int op = find_majority_token_position(left, right);
		uint32_t val1 = eval(left, op-1);
		uint32_t val2 = eval(op+1, right);

		switch (tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: assert(0);
		}
	}
}
// End of Insert part

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  return eval(0, nr_token-1);

  return 0;
}
