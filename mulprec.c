#include "mulprec.h"
#include <limits.h>
#include <stdio.h>
// ゼロクリア
void clearByZero(struct NUMBER *a) {
  for (int i = 0; i < KETA; i++) {
    a->n[i] = 0;
  }
  a->sign = 1;
}

// 多倍長変数の表示(0省略)
void dispNumberZeroSuppress(const struct NUMBER *a) {
  if (a->sign == 1) {
    printf("+");
  } else if (a->sign == -1) {
    printf("-");
  }

  int isZero = 1;
  for (int i = KETA - 1; i >= 0; i--) {
    if (isZero && a->n[i] == 0) {
      continue;
    }
    isZero = 0;

    if (i == BASE) {
      printf("%lld", a->n[i]);
    } else {
      printf("%09lld", a->n[i]);
    }
  }

  if (isZero) {
    printf("0");
  }
}

// 多倍長変数のコピー
void copyNumber(const struct NUMBER *a, struct NUMBER *b) { *b = *a; }

// ゼロか判定
bool isZero(const struct NUMBER *a) {
  for (int i = 0; i < KETA; i++) {
    if (a->n[i] != 0) {
      return false;
    }
  }
  return true;
}

// long long int型の値を多倍長変数に設定
int setInt(struct NUMBER *a, long long int x) {
  long long int r = 0;
  clearByZero(a);
  for (int i = 0; x != 0; i++) {
    r = x % RADIX;
    x -= r;
    x /= RADIX;

    if (r < 0) {
      r *= -1;
      a->sign = -1;
    }

    if (a->n[i] == 0) {
      a->n[i] = r;
    } else {
      return -1;
    }
  }

  return 0;
}

// 多倍長変数の比較
// 0  ... a = b
// 1  ... a > b
// -1 ... a < b
int numComp(const struct NUMBER *a, const struct NUMBER *b) {
  if (a->sign > b->sign) {
    return 1;
  }
  if (a->sign < b->sign) {
    return -1;
  }

  int result = 0;
  for (int i = KETA - 1; i >= 0; i--) {
    if (a->n[i] > b->n[i]) {
      result = 1;
      break;
    } else if (a->n[i] < b->n[i]) {
      result = -1;
      break;
    }
  }

  if (a->sign == -1) {
    result = -result;
  }

  return result;
}

// c <- a + b
int add(struct NUMBER *a, struct NUMBER *b, struct NUMBER *c) {
  long long int d, e;
  e = 0;

  // a >= 0, b >= 0
  if (a->sign == 1 && b->sign == 1) {
    for (int i = 0; i < KETA; i++) {
      if (a->n[i] == 0 && b->n[i] == 0 && e == 0) {
        c->n[i] = 0;
        continue;
      }

      d = a->n[i] + b->n[i] + e;
      c->n[i] = d % RADIX;
      e = d / RADIX;
    }

    if (e != 0) {
      clearByZero(c);
      return -1;
    }

    return 0;
  }

  // a >= 0, b < 0
  if (a->sign == 1 && b->sign == -1) {
    struct NUMBER tmp;
    clearByZero(&tmp);
    copyNumber(b, &tmp);
    tmp.sign = 1;
    sub(a, &tmp, c);
    return 0;
  }

  // a < 0, b >= 0
  if (a->sign == -1 && b->sign == 1) {
    struct NUMBER tmp;
    clearByZero(&tmp);
    copyNumber(a, &tmp);
    tmp.sign = 1;
    sub(b, &tmp, c);
    return 0;
  }

  // a < 0, b < 0
  if (a->sign == -1 && b->sign == -1) {
    struct NUMBER tmpa, tmpb;
    clearByZero(&tmpa);
    copyNumber(a, &tmpa);
    tmpa.sign = 1;

    clearByZero(&tmpb);
    copyNumber(b, &tmpb);
    tmpb.sign = 1;

    return add(&tmpa, &tmpb, c);
  }

  return 0;
}

// c <- a - b
int sub(struct NUMBER *a, struct NUMBER *b, struct NUMBER *c) {
  if (a->sign == 1 && b->sign == 1) {
    if (numComp(a, b) == -1) {
      sub(b, a, c);
      c->sign = -1;
      return 0;
    }

    int h;
    int ai, bi;

    h = 0;

    for (int i = 0; i < KETA; i++) {
      ai = a->n[i];
      bi = b->n[i];
      ai -= h;

      if (ai >= bi) {
        c->n[i] = ai - bi;
        h = 0;
      }

      if (ai < bi) {
        c->n[i] = RADIX + ai - bi;
        h = 1;
      }
    }

    if (h != 0) {
      clearByZero(c);
      return -1;
    }
  }

  if (a->sign == 1 && b->sign == -1) {
    struct NUMBER tmp;
    clearByZero(&tmp);
    copyNumber(b, &tmp);
    tmp.sign = 1;
    return add(a, &tmp, c);
  }

  if (a->sign == -1 && b->sign == 1) {
    int r;
    struct NUMBER tmp;
    clearByZero(&tmp);
    copyNumber(a, &tmp);
    tmp.sign = 1;
    r = add(&tmp, b, c);
    c->sign = -1;
    return r;
  }

  if (a->sign == -1 && b->sign == -1) {
    struct NUMBER tmpa, tmpb;
    clearByZero(&tmpa);
    copyNumber(a, &tmpa);
    tmpa.sign = 1;

    clearByZero(&tmpb);
    copyNumber(b, &tmpb);
    tmpb.sign = 1;

    return sub(&tmpb, &tmpa, c);
  }

  return 0;
}

// b <- a + 1
int increment(struct NUMBER *a, struct NUMBER *b) {
  struct NUMBER one;
  int r;

  setInt(&one, 1);
  r = add(a, &one, b);

  return r;
};

// b <- a -1
int decrement(struct NUMBER *a, struct NUMBER *b) {
  struct NUMBER one;
  int r;

  setInt(&one, 1);
  r = sub(a, &one, b);

  return r;
}

// c <- a * b
int multiple(struct NUMBER *a, struct NUMBER *b, struct NUMBER *c) {
  clearByZero(c);
  int a_keta, b_keta;

  a_keta = getKeta(a);
  b_keta = getKeta(b);

  for (int i = 0; i <= b_keta; i++) {
    if (b->n[i] == 0) {
      continue;
    }

    for (int j = 0; j <= a_keta; j++) {
      if (j + i >= a_keta + b_keta) {
        break;
      }

      if (a->n[j] == 0) {
        continue;
      }

      c->n[i + j] += a->n[j] * b->n[i];

      if (c->n[i + j] >= RADIX) {
        c->n[i + j + 1] += c->n[i + j] / RADIX;
        c->n[i + j] = c->n[i + j] % RADIX;
      }
    }
  }

  return 0;
}

// 現在の桁数を返す
int getKeta(const struct NUMBER *a) {
  int keta = 0;
  for (int i = KETA - 1; i >= 0; i--) {
    if (a->n[i] != 0) {
      keta = i + 1;
      break;
    }
  }
  return keta;
}

// c <- a / bの商
int one_divide(struct NUMBER *a, struct NUMBER *b, struct NUMBER *c) {
  long long int t, h;
  clearByZero(c);
  if (isZero(b)) {
    return -1;
  }
  if (getKeta(b) > 1) {
    return divide_w_inverse(a, b, c);
  }
  t = 0;
  h = 0;
  for (int i = getKeta(a) - 1; i >= 0; i--) {
    t = RADIX * h + a->n[i];
    h = t % b->n[0];
    c->n[i] = (t - h) / b->n[0];
  }
  return 0;
}

// c <- a / bの商
int divide_w_inverse(struct NUMBER *a, struct NUMBER *b, struct NUMBER *c) {
  struct NUMBER i_b;
  struct NUMBER tmp1, tmp2;
  int tmp_comp;

  clearByZero(c);
  clearByZero(&i_b);
  clearByZero(&tmp1);
  clearByZero(&tmp2);

  int N = getKeta(a) + 1;

  if (isZero(b)) {
    return -1;
  }

  inverse(b, &i_b, N);
  multiple(a, &i_b, &tmp1);
  divBy10N(&tmp1, &tmp2, N);

  // 誤差
  increment(&tmp2, &tmp1);
  multiple(b, &tmp1, &tmp2);
  tmp_comp = numComp(&tmp2, a);
  if (tmp_comp == 1) {
    decrement(&tmp1, c);
  } else {
    copyNumber(&tmp1, c);
  }

  return 0;
}

// b <- 1 / a
int inverse(struct NUMBER *a, struct NUMBER *b, int N) {
  struct NUMBER x, y, h, one;
  struct NUMBER tmp1, tmp2;
  int keta;

  clearByZero(&x);
  clearByZero(&y);
  clearByZero(&h);
  clearByZero(&tmp1);
  clearByZero(&tmp2);
  keta = getKeta(a);

  setInt(&one, 1);
  mulBy10N(&one, &tmp1, N);
  copyNumber(&tmp1, &one);

  // tmp1 <- 2
  setInt(&tmp1, 2);
  // x
  mulBy10N(&tmp1, &x, N - keta);

  while (1) {
    // y <- x
    copyNumber(&x, &y);
    // tmp1 <- a * y
    multiple(a, &y, &tmp1);
    // h <- one - tmp1
    sub(&one, &tmp1, &h);

    // tmp1 <- h * h
    multiple(&h, &h, &tmp1);
    divBy10N(&tmp1, &tmp2, N);
    // tmp1 <- tmp2 + h
    add(&tmp2, &h, &tmp1);
    // tmp2 <- tmp1 + one
    add(&tmp1, &one, &tmp2);
    // tmp1 <- y * tmp2
    multiple(&y, &tmp2, &tmp1);
    divBy10N(&tmp1, &x, N);

    if ((N - getKeta(&h)) * 3 >= N) {
      break;
    }
  }

  copyNumber(&x, b);

  return 0;
}

// 桁のシフト
int mulBy10N(const struct NUMBER *a, struct NUMBER *b, int N) {
  clearByZero(b);
  for (int i = KETA - 1; i >= 0; i--) {
    if (a->n[i] != 0) {
      if (i + N >= KETA) {
        return -1;
      }
      b->n[i + N] = a->n[i];
    }
  }
  b->n[0] = 0;
  b->sign = a->sign;
  return 0;
}

// 桁のシフト
int divBy10N(const struct NUMBER *a, struct NUMBER *b, int N) {
  clearByZero(b);
  for (int i = 0; i < KETA - N; i++) {
    if (a->n[i + N] != 0) {
      b->n[i] = a->n[i + N];
    }
  }
  b->sign = a->sign;
  return 0;
}

// ニュートン無平方根法
int newton_no_sqrt(struct NUMBER *a) {
  struct NUMBER denominator1, denominator2, numerator;
  struct NUMBER pi, tmp_pi, zero, twoi;
  struct NUMBER tmp1, tmp2;

  clearByZero(&denominator1);
  clearByZero(&denominator2);
  clearByZero(&numerator);
  clearByZero(&pi);
  clearByZero(&twoi);
  clearByZero(&zero);
  clearByZero(&tmp_pi);
  clearByZero(&tmp1);
  clearByZero(&tmp2);
  clearByZero(a);

  tmp_pi.n[BASE] = 1;
  pi.n[BASE] = 1;

  for (int i = 0; 1; i++) {
    if (i % 200 == 0) {
      printf("#");
      fflush(stdout);
    }

    setInt(&twoi, 2 * i + 3);
    setInt(&tmp1, 2 * i + 1);
    multiple(&tmp_pi, &tmp1, &numerator);

    // denominator
    setInt(&denominator1, 8 * (i + 1));
    setInt(&denominator2, 2 * i + 3);

    if (numComp(&numerator, &zero) == 0) {
      printf("\nresult n: %d\n", i);
      break;
    }

    one_divide(&numerator, &denominator1, &tmp1);
    one_divide(&tmp1, &denominator2, &tmp2);

    add(&pi, &tmp2, &pi);

    multiple(&tmp2, &twoi, &tmp_pi);
  }

  setInt(&tmp1, 3);
  multiple(&pi, &tmp1, a);

  return 0;
}

int checkPi(struct NUMBER *a) {
  int cnt = 0;
  int file[KETA * 9];

  FILE *fp;
  fp = fopen("pi.txt", "r");
  if (fp == NULL) {
    return -1;
  }

  int i = 0;
  while (i < KETA * 9) {
    int r = fgetc(fp);
    if (r == EOF) {
      break;
    }
    if (r >= '0' && r <= '9') {
      file[i] = r - '0';
      i++;
    }
  }
  fclose(fp);

  for (int j = 0; j < KETA; j++) {
    long long int tmp = a->n[getKeta(a) - j - 1];
    for (int k = 0; tmp > 0; k++) {
      int tmp1 = file[j * 9 - k];
      int tmp2 = tmp % 10;
      tmp /= 10;

      if (tmp1 != tmp2) {
        break;
      }

      cnt++;
    }
  }

  return cnt;
}
