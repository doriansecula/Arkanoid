/*
 * disputils.h
 *
 *  Created on: May 19, 2016
 *      Author: autoliv.academy
 */

#ifndef DISPUTILS_H_
#define DISPUTILS_H_

#include "drivers/rit128x96x4.h"

#define DSP_WIDTH 128
#define	DSP_HEIGHT 96
#define CHAR_WIDTH 6
#define	CHAR_HEIGHT 8

extern unsigned char logo[6144];


void dspRst();
int goTo(int,int);
void printDec(int);
void printLnDec(int);
void printLn();

void printString(char *); //HW3
int myStrlen(char *s); //HW

#endif /* DISPUTILS_H_ */
