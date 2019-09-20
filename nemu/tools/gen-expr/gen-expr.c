#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
static int point = 0;

uint32_t choose(uint32_t n) {
	return rand() % n;
}

static inline void gen_num() {
	uint32_t iter = choose(3);
	for (int i = 0; i <= iter; i++) {
		uint32_t num;
		if (i == 0)
			num = choose(9) + 1;
		else
			num = choose(10);
		buf[point] = num + '0';
		point++;
		buf[point] = '\0';
	}
}

static inline void gen(char ch) {
	buf[point] = ch;
	point++;
	buf[point] = '\0';
}

static inline void gen_rand_op() {
	char op;
	switch (choose(4)) {
		case 0: op = '+'; break;
		case 1: op = '-'; break;
		case 2: op = '*'; break;
		default: op = '/'; break;
	}
	buf[point] = op;
	point++;
	buf[point] = '\0';
}

static inline void gen_rand_space() {
	if (choose(2)) {
		buf[point] = ' ';
		point++;
		buf[point] = '\0';
	}
}

static inline void gen_rand_expr() {
	switch (choose(3)) {
		case 0: gen_num(); break;
		case 1: gen('('); gen_rand_space(); gen_rand_expr(); gen_rand_space(); gen(')'); break;
		default: gen_rand_expr(); gen_rand_space(); gen_rand_op(); gen_rand_space(); gen_rand_expr(); break;
	}
	if (point >= 65536)
		assert(0);
}

static char code_buf[66536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[0] = '\0';
		point = 0;
		gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result = -1;
    fscanf(fp, "%d", &result);
    pclose(fp);
		
		if (result >= 0)
    	printf("%u %s\n", result, buf);
  }
  return 0;
}
