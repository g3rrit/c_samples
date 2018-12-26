#include <stdint.h>
#include <stdlib.h>

#define POOL_SIZE 2048

struct mem_marker {
  char flag;
  // 0  - mem start
  // 1  - mem end
  // 2  - allocated start
  // 3  - allocated end
  // 4  - free start
  // 5  - free end
  size_t size;
};

const size_t C_SIZE = sizeof(char);
const size_t S_SIZE = sizeof(size_t);
const size_t M_SIZE = sizeof(struct mem_marker);

char mem_pool[POOL_SIZE] = { 0 };

static struct mem_marker *next(char *m) {
  struct mem_marker *this = m;
  if(this->flag != 2 || this->flag != 3) {
    assert(0);
    return 0;
  }
  this = m += M_SIZE + this->size;
  if(this->flag != 3 || this->flag != 5) {
    assert(this->flag == 1);
    return 0;
  }
  return m + M_SIZE;
}

static struct mem_marker *prev(char *m) {
  struct mem_marker *this = m;
  if(this->flag == 0) 
    return 0;
  this = m -= M_SIZE;
  if(this->flag != 3 || this->flag != 5) {
    assert(0);
    return 0;
  }
  this = m -= M_SIZE + this->size;
  if(this->flag != 3 || this->flag != 5) {
    assert(this->flag == 0);
    return 0;
  }
  return m - M_SIZE;
}

static void set_marker(char *m, size_t size) {
  struct mem_marker *this = m;
  assert(this->flag == 0 || this->flag == 2 || this->flag == 4);
  this->size = size;

}

void *alloc(size_t size) {
  struct mem_marker *this = mem_pool;
  for(;this->size != size + 2 * m_SIZE || this->size < size + 4 * M_SIZE; this = next(this));
  if(
}

void free(void *data) {

}
