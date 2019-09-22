#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP *new_wp() {
	if (free_ == NULL) {
		printf("No more free watchpoints!\n");
		assert(0);
	}

	WP *p = free_;
  free_ = free_->next;
	p->next = head;
	p->EXPR = '\0';
	p->value = 0;
	head = p;
	return p;
}

void free_wp(WP *wp) {
	if (wp == NULL || head == NULL)
		assert(0);
	if (head->NO == wp->NO) {
		WP *p = head;
		head = head->next;
		p->EXPR = '\0';
		p->value = 0;
		p->next = free_;
		free_ = p;
		return;
	}
	WP *p = head;
	for (p = head; p->next != NULL; p = p->next) {
		printf("%d\n", p->next->NO);
		if (p->next->NO == wp->NO) {
			WP *tmp = p->next;
			p->next = p->next->next;
			tmp->EXPR = '\0';
			tmp->value = 0;
			tmp->next = free_;
			free_ = tmp;
			return;
		}
	}

	if (p->next == NULL)
		assert(0);
}

