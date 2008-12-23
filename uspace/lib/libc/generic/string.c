/*
 * Copyright (c) 2005 Martin Decky
 * Copyright (c) 2008 Jiri Svoboda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup libc
 * @{
 */
/** @file
 */

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <malloc.h>

/** Count the number of characters in the string, not including terminating 0.
 *
 * @param str		String.
 * @return		Number of characters in string.
 */
size_t strlen(const char *str) 
{
	size_t counter = 0;

	while (str[counter] != 0)
		counter++;

	return counter;
}

int strcmp(const char *a, const char *b)
{
	int c = 0;
	
	while (a[c] && b[c] && (!(a[c] - b[c])))
		c++;
	
	return (a[c] - b[c]);
}

int strncmp(const char *a, const char *b, size_t n)
{
	size_t c = 0;

	while (c < n && a[c] && b[c] && (!(a[c] - b[c])))
		c++;
	
	return ( c < n ? a[c] - b[c] : 0);
	
}

int stricmp(const char *a, const char *b)
{
	int c = 0;
	
	while (a[c] && b[c] && (!(tolower(a[c]) - tolower(b[c]))))
		c++;
	
	return (tolower(a[c]) - tolower(b[c]));
}

/** Return pointer to the first occurence of character c in string.
 *
 * @param str		Scanned string.
 * @param c		Searched character (taken as one byte).
 * @return		Pointer to the matched character or NULL if it is not
 * 			found in given string.
 */
char *strchr(const char *str, int c)
{
	while (*str != '\0') {
		if (*str == (char) c)
			return (char *) str;
		str++;
	}

	return NULL;
}

/** Return pointer to the last occurence of character c in string.
 *
 * @param str		Scanned string.
 * @param c		Searched character (taken as one byte).
 * @return		Pointer to the matched character or NULL if it is not
 * 			found in given string.
 */
char *strrchr(const char *str, int c)
{
	char *retval = NULL;

	while (*str != '\0') {
		if (*str == (char) c)
			retval = (char *) str;
		str++;
	}

	return (char *) retval;
}

/** Convert string to a number. 
 * Core of strtol and strtoul functions.
 *
 * @param nptr		Pointer to string.
 * @param endptr	If not NULL, function stores here pointer to the first
 * 			invalid character.
 * @param base		Zero or number between 2 and 36 inclusive.
 * @param sgn		It's set to 1 if minus found.
 * @return		Result of conversion.
 */
static unsigned long
_strtoul(const char *nptr, char **endptr, int base, char *sgn)
{
	unsigned char c;
	unsigned long result = 0;
	unsigned long a, b;
	const char *str = nptr;
	const char *tmpptr;
	
	while (isspace(*str))
		str++;
	
	if (*str == '-') {
		*sgn = 1;
		++str;
	} else if (*str == '+')
		++str;
	
	if (base) {
		if ((base == 1) || (base > 36)) {
			/* FIXME: set errno to EINVAL */
			return 0;
		}
		if ((base == 16) && (*str == '0') && ((str[1] == 'x') ||
		    (str[1] == 'X'))) {
			str += 2;
		}
	} else {
		base = 10;
		
		if (*str == '0') {
			base = 8;
			if ((str[1] == 'X') || (str[1] == 'x'))  {
				base = 16;
				str += 2;
			}
		} 
	}
	
	tmpptr = str;

	while (*str) {
		c = *str;
		c = (c >= 'a' ? c - 'a' + 10 : (c >= 'A' ? c - 'A' + 10 :
		    (c <= '9' ? c - '0' : 0xff)));
		if (c > base) {
			break;
		}
		
		a = (result & 0xff) * base + c;
		b = (result >> 8) * base + (a >> 8);
		
		if (b > (ULONG_MAX >> 8)) {
			/* overflow */
			/* FIXME: errno = ERANGE*/
			return ULONG_MAX;
		}
	
		result = (b << 8) + (a & 0xff);
		++str;
	}
	
	if (str == tmpptr) {
		/*
		 * No number was found => first invalid character is the first
		 * character of the string.
		 */
		/* FIXME: set errno to EINVAL */
		str = nptr;
		result = 0;
	}
	
	if (endptr)
		*endptr = (char *) str;

	if (nptr == str) { 
		/*FIXME: errno = EINVAL*/
		return 0;
	}

	return result;
}

/** Convert initial part of string to long int according to given base.
 * The number may begin with an arbitrary number of whitespaces followed by
 * optional sign (`+' or `-'). If the base is 0 or 16, the prefix `0x' may be
 * inserted and the number will be taken as hexadecimal one. If the base is 0
 * and the number begin with a zero, number will be taken as octal one (as with
 * base 8). Otherwise the base 0 is taken as decimal.
 *
 * @param nptr		Pointer to string.
 * @param endptr	If not NULL, function stores here pointer to the first
 * 			invalid character.
 * @param base		Zero or number between 2 and 36 inclusive.
 * @return		Result of conversion.
 */
long int strtol(const char *nptr, char **endptr, int base)
{
	char sgn = 0;
	unsigned long number = 0;
	
	number = _strtoul(nptr, endptr, base, &sgn);

	if (number > LONG_MAX) {
		if ((sgn) && (number == (unsigned long) (LONG_MAX) + 1)) {
			/* FIXME: set 0 to errno */
			return number;		
		}
		/* FIXME: set ERANGE to errno */
		return (sgn ? LONG_MIN : LONG_MAX);	
	}
	
	return (sgn ? -number : number);
}


/** Convert initial part of string to unsigned long according to given base.
 * The number may begin with an arbitrary number of whitespaces followed by
 * optional sign (`+' or `-'). If the base is 0 or 16, the prefix `0x' may be
 * inserted and the number will be taken as hexadecimal one. If the base is 0
 * and the number begin with a zero, number will be taken as octal one (as with
 * base 8). Otherwise the base 0 is taken as decimal.
 *
 * @param nptr		Pointer to string.
 * @param endptr	If not NULL, function stores here pointer to the first
 * 			invalid character
 * @param base		Zero or number between 2 and 36 inclusive.
 * @return		Result of conversion.
 */
unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	char sgn = 0;
	unsigned long number = 0;
	
	number = _strtoul(nptr, endptr, base, &sgn);

	return (sgn ? -number : number);
}

char *strcpy(char *dest, const char *src)
{
	char *orig = dest;
	
	while ((*(dest++) = *(src++)))
		;
	return orig;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	char *orig = dest;
	
	while ((*(dest++) = *(src++)) && --n)
		;
	return orig;
}

char *strcat(char *dest, const char *src)
{
	char *orig = dest;
	while (*dest++)
		;
	--dest;
	while ((*dest++ = *src++))
		;
	return orig;
}

char * strdup(const char *s1)
{
	size_t len = strlen(s1) + 1;
	void *ret = malloc(len);

	if (ret == NULL)
		return (char *) NULL;

	return (char *) memcpy(ret, s1, len);
}

char *strtok(char *s, const char *delim)
{
	static char *next;

	return strtok_r(s, delim, &next);
}

char *strtok_r(char *s, const char *delim, char **next)
{
	char *start, *end;

	if (s == NULL)
		s = *next;

	/* Skip over leading delimiters. */
	while (*s && (strchr(delim, *s) != NULL)) ++s;
	start = s;

	/* Skip over token characters. */
	while (*s && (strchr(delim, *s) == NULL)) ++s;
	end = s;
	*next = (*s ? s + 1 : s);

	if (start == end) {
		return NULL;	/* No more tokens. */
	}

	/* Overwrite delimiter with NULL terminator. */
	*end = '\0';
	return start;
}

/** @}
 */
