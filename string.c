/*
 * string.c
 *
 *  Created on: Jun 10, 2016
 *      Author: autoliv.academy
 */

#include "utils/ustdlib.h"
#include "drivers/rit128x96x4.h"
#include "math.h"
#include "dsputils.h"

#define NULL 0

int myStrlen(char *s) { //lungimea string-ului
	int inc = 0;
	while (s[inc] != '\0') {
		inc++;
	}
	return inc;
}

/*
int mystrcpy(char *dest, const char *src) { //Copies up to n characters from the string pointed to, by src to dest.
	int i;
	int d = 0;
	int s = sizeof(src) / sizeof(src[0]);  //afla nr elementelor
	for (i = s; i >= 0; i++) {
		dest[d] = src[i];
	}
	return dest;
}
*/
/*
int mystrncpy(char *dest, const char *src, char size) {   //Copies up to n characters from the string pointed to, by src to dest.
	int i;
	int d = 0;
	for (i = size; i >= 0; i++) {
		dest[d] = src[i];
	}
	return dest;
}
*/
/*
int mystrcat(char *string1, const char *string2) { //Appends the string pointed to, by src to the end of the string pointed to by dest.
	int i;
	int str_one = myStrlen(string1);
	char str_two = myStrlen(string2);
	int const array_len = str_one + str_two;
	char *buff;
	for (i = 0; i < str_one; i++) {
		buff[i] = string1[i];
	}
	for (i = str_one; i < array_len; i++) {
			buff[i] = string2[i];
		}
	return buff; //fair enough

}
*/

int mystrcmp(const char *s1, const char *s2) { //Compares the string pointed to, by str1 to the string pointed to by str2.


	while(*s1 == *s2){
		if((*s1 == '\0') && (*s2 =='\0')){
			return 0; //egale
		}
		s1++;
		s2++;
	}
	if(*s1>*s2){
		return 1; //mai mare
	}
	else{
		return -1; //mai mic..
	}

	/* varianta mai 'simplificata'
	 while ((*s1 == *s2) && (*s1!='\0')){
	 s1++; s2++;
	 }
	 return *s1-*s2; //ascii
	 	 */

}

/*
char *mystrchr( const char *s, char c){  //searches for the first occurrence of the character c
char *ps = NULL;
int i;
for(i=0; s[i]!='\0'; i++){
	if (s[i]==c){
		ps=&s[i];
	}
}


//mystrchr(s,'c')-s;
//if(strchr("aeiou",c)! == NULL)
//	==NULL;
return ps;
}*/

char *mystrstr(char *s1, char *s2){

	int i,j=0,flag=1;
	char *ps = NULL;

	for(i=0; s1[i]!='\0'; i++){
		if ( (flag == 1) && (s2[j]==s1[i]) ){
			ps=&s1[i];
			flag = 0;
		}
		else{}

		if( (flag == 0) && (s2[++j]==s1[i]) ){
			ps=&s1[i];
		}
		else{
			break;
		}
	}
		return ps;
 }



/*
char *mystrstr(char *s1, char *s2){   //prima aparitie a elem sirului s2 in sirul 21  HW!->impementare strstr -> mecanism de a tipari numerele, pe rand 23,74,5,9 (suma chiar)
	("gigel si maria merg", "maria")   //va ret un pointer la aparitia primei litere a cuv cautat adica la 'm'S   (de lucrat cu strcpy)
 }

 *s= '23;74;5;8'
 char *ps
 while ((ps=strchr(s,';'))!=NULL))
 scrie (ps);
 s=ps+1;

 *******************************alta varianta

 while (s[i]!=c) && s[i])
 i++;
 if(s[i]!=NULL
 return &s[i]
 else return NULL
*/
