/*
 * string.h
 *
 *  Created on: Jun 10, 2016
 *      Author: autoliv.academy
 */

#ifndef STRING_H_
#define STRING_H_


int myStrlen(char *s);
int mystrcpy(char *dest,const char *src);
int mystrncpy(char *dest,const char *src, char size);
int mystrcat(char *string1, const char *string2);
int mystrcmp (const char *s1, const char *s2);
char *mystrchr(const char *s, char c);
char *mystrstr(char *s1, char *s2);

#endif /* STRING_H_ */
