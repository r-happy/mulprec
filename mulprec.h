#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RADIX 1000000000
#define RADIX_KETA 9

// /* 1000
#define KETA 256
#define BASE 125
// */

struct NUMBER {
  long long int n[KETA];
  int sign;
};

// prototype
void clearByZero(struct NUMBER *);
void dispNumberZeroSuppress(const struct NUMBER *);
void copyNumber(const struct NUMBER *, struct NUMBER *);
bool isZero(const struct NUMBER *);
int numComp(const struct NUMBER *, const struct NUMBER *);
int setInt(struct NUMBER *, long long int);
int add(struct NUMBER *, struct NUMBER *, struct NUMBER *);
int sub(struct NUMBER *, struct NUMBER *, struct NUMBER *);
int increment(struct NUMBER *, struct NUMBER *);
int decrement(struct NUMBER *, struct NUMBER *);
int multiple(struct NUMBER *, struct NUMBER *, struct NUMBER *);
int divide_w_inverse(struct NUMBER *, struct NUMBER *, struct NUMBER *);
int one_divide(struct NUMBER *, struct NUMBER *, struct NUMBER *);
int getKeta(const struct NUMBER *);
int newton_no_sqrt(struct NUMBER *);
int mulBy10N(const struct NUMBER *, struct NUMBER *, int);
int divBy10N(const struct NUMBER *, struct NUMBER *, int);
int inverse(struct NUMBER *, struct NUMBER *, int);
int checkPi(struct NUMBER *a);
