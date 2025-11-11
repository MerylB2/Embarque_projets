#include "exo.h"

#define BASE_LEN 16

const char base[] = "0123456789abcdef";

unsigned int ft_strlen(const char *s) {
  unsigned int len = 0;
  while (s[len]) {
    len++;
  }
  return len;
}


int	is_printable(unsigned char c) {
	return c >= 32 && c <= 126;
}

int	ft_strncmp(const char *s1, const char *s2, unsigned int n) {
	unsigned int	i;

	i = 0;
	if (n == i)
		return (0);
	while (s1[i] == s2[i] && s1[i] && i < n - 1)
		i++;
	return (unsigned char)s1[i] - (unsigned char)s2[i];
}


int ft_strcmp(const char *s1, const char *s2) {
  unsigned int i = 0;
  while (s1[i] == s2[i] && s1[i] != '\0') {
    i++;
  }
  return s2[i] - s1[i];
}


char is_in(char c, const char *str) {
  while (*str && *str != c) {
    ++str;
  }
  return *str;
}

uint32_t pos_in_base(char c, const char *base) {
  uint32_t pos = 0;
  while (c != base[pos] && base[pos]) {
    pos++;
  }
  return pos;
}


uint32_t atoi_hexa(const char *str) {
  uint32_t result = 0;
  while (is_in(*str, base)) {
    result = BASE_LEN * result + pos_in_base(*str, base);
    ++str;
  }
  return result;
}


void	putnbr_hexa(uint32_t nb) {
	if (nb / BASE_LEN > 0) {
	  putnbr_hexa(nb / BASE_LEN);
  }
  uart_tx(base[nb % BASE_LEN]);
}