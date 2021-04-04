#include <string.h>

void* memset(void* buf, int c, int n){
	unsigned char* p = buf;

	while (n--) {
		*p++ = c;
	}

	return buf;
}

void* memcpy(void* dest, const void* src, size_t n){
	unsigned char* d = dest;
	const unsigned char* s = src;

	while (n--) {
		*d++ = *s++;
	}

	return dest;
}

int memcmp(const void * _s1, const void* _s2, size_t n) {

    const unsigned char* s1 = _s1;
    const unsigned char* s2 = _s2;

	while(n--) {
		if(*s1 != *s2) {
			return *s1 - *s2;
		}
		++s1;
		++s2;
	}
	return 0;
}



int strcmp(char *str1, char *str2){
	int i = 0;
	int failed = 0;
	while(str1[i] != '\0' && str2[i] != '\0'){
		if(str1[i] != str2[i]){
			failed = 1;
			break;
		}
		i++;
	}
	if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
		failed = 1;
	return failed;
}

char *strcpy(char *dest, const char *src){
	do{
		*dest++ = *src++;
	} while (*src != 0);
	return 0;
}

int strlen(char *src){
	int i = 0;
	while (*src++)
		i++;
	return i;
}

char* strchr(const char* s, int c) {
	while(*s) {
		if(*s == (char) c) {
			return (char*) s;
		}
		s++;
	}
	return NULL;
}

char* strrchr(const char* s, int c) {
	size_t i = 0;
	while (s && s[i++]);
	do {
		if (s && s[--i] == (char) c) {
			return (char*) s + i;
		}
	} while ( i );
	return NULL;
}

size_t strnlen(const char *s, size_t maxlen) {
	size_t i;
	for (i = 0; i < maxlen; ++i)
	if (s[i] == '\0')
		break;
	return i;
}

static int maxExponent = 511;

static double powersOf10[] = {
	10.,
	100.,
	1.0e4,
	1.0e8,
	1.0e16,
	1.0e32,
	1.0e64,
	1.0e128,
	1.0e256
};


double strtod(const char* string, char** endPtr) {
	bool sign, expSign = false;
	double fraction, dblExp, *d;
	register const char *p;
	register int c;
	int exp = 0;
	int fracExp = 0;		
	int mantSize;		
	int decPt;			
	const char *pExp;

	p = string;
	while (isspace(*p)) {
		p += 1;
	}
	if (*p == '-') {
		sign = true;
		p += 1;
	} else {
		if (*p == '+') {
			p += 1;
		}
		sign = false;
	}

	decPt = -1;
	for (mantSize = 0; ; mantSize += 1) {
		c = *p;
		if (!isdigit(c)) {
			if ((c != '.') || (decPt >= 0)) {
				break;
			}
			decPt = mantSize;
		}
		p += 1;
	}
	
	pExp  = p;
	p -= mantSize;
	if (decPt < 0) {
		decPt = mantSize;
	} else {
		mantSize -= 1;
	}
	if (mantSize > 18) {
		fracExp = decPt - 18;
		mantSize = 18;
	} else {
		fracExp = decPt - mantSize;
	}
	if (mantSize == 0) {
		fraction = 0.0;
		p = string;
		goto done;
	} else {
		int frac1, frac2;
		frac1 = 0;
		for ( ; mantSize > 9; mantSize -= 1) {
			c = *p;
			p += 1;
			if (c == '.') {
				c = *p;
				p += 1;
			}
			frac1 = 10*frac1 + (c - '0');
		}
		frac2 = 0;
		for (; mantSize > 0; mantSize -= 1) {
			c = *p;
			p += 1;
			if (c == '.') {
				c = *p;
				p += 1;
			}
			frac2 = 10*frac2 + (c - '0');
		}
		fraction = (1.0e9 * frac1) + frac2;
	}

	p = pExp;
	if ((*p == 'E') || (*p == 'e')) {
		p += 1;
		if (*p == '-') {
			expSign = true;
			p += 1;
		} else {
			if (*p == '+') {
				p += 1;
			}
			expSign = false;
		}
		while (isdigit(*p)) {
			exp = exp * 10 + (*p - '0');
			p += 1;
		}
	}
	if (expSign) {
		exp = fracExp - exp;
	} else {
		exp = fracExp + exp;
	}
	
	if (exp < 0) {
		expSign = true;
		exp = -exp;
	} else {
		expSign = false;
	}
	if (exp > maxExponent) {
		exp = maxExponent;
	}
	dblExp = 1.0;
	for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
		if (exp & 01) {
			dblExp *= *d;
		}
	}
	if (expSign) {
		fraction /= dblExp;
	} else {
		fraction *= dblExp;
	}

done:
	if (endPtr != NULL) {
		*endPtr = (char *) p;
	}

	if (sign) {
		return -fraction;
	}
	return fraction;
}

char* strcat(char *dest, const char *src){
    int i;
    int j;

    for (i = 0; dest[i] != '\0'; i++);
    for (j = 0; src[j] != '\0'; j++) {
        dest[i+j] = src[j];
    }

    dest[i+j] = '\0';

    return dest;
}

char* strncpy(char* _dst, const char* _src, size_t count) {
	size_t len = strlen((char*) _src);
	if(count < len) {
		len = count;
	}
	memcpy(_dst, _src, len);
	for(size_t i = len; i < count; ++i) {
		_dst[i] = 0;
	}
	return _dst;
}

int strncmp(const char* _s1, const char* _s2, size_t count) {
	size_t len1 = strlen((char*) _s1);
	size_t len2 = strlen((char*) _s2);
	size_t len = len1 < len2 ? len1 : len2;

	if(count < len) {
		len = count;
	}
	int ret = memcmp(_s1, _s2, len);
	if(len < len1 && len < len2) {
		return ret;
	}
	if(ret == 0) {
		if(len1 == len2) {
			return 0;
		} else if (len == len1) {
			return -1;
		} else {
			return 1;
		}
	}
	return ret;
}