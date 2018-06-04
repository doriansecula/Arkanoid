/*
 * alvutils.h
 *
 *  Created on: May 19, 2016
 *      Author: autoliv.academy
 */

#ifndef ALVUTILS_H_
#define ALVUTILS_H_



void printtab(int t[], unsigned len);
void printUl(unsigned long, unsigned, unsigned);
unsigned setBit(unsigned x, int k);
unsigned resetBit(unsigned x, int k);
void swap(unsigned *a, unsigned *b);
void sort3(unsigned *a, unsigned *b, unsigned *c);
void printHex(unsigned long x, unsigned pxx, unsigned pxy);
void printBinar(unsigned char x, int px, int py);
int ePrim(unsigned x);
int getPrimaCifra(int n);
int nrFaraPrima(int n);
int nCif(int n);
int shl10(int n);
int eExtraPrim(int x);

int getMaxArray(int t[], int n);
int eSimetric(int t[], int n); // {1,2,6,2,1};
int eMonoton(int t[], int n); // cresc, sau descresc!
void getMaxCount(int t[], int n, int *max, int *ctMax); //HW1 Val max a sirului + Pozitia sa
int deleteElem(int t[], int *n, int x); //HW2  Se va modif nr de elem prin stergere,
void deleteElemByPoz(int t[], int *n, int k);
int AdunareNrMari(int a[], int x, int b[], int y, int S[], int nElem);
int elimElementeMultiple(int t[], int n, int j, int k);
int verifOrdElem(int t[], int n);
void IntToArray(int n, int t[], int nElm);
int IntArray(int n, int t[], int nElmMax);
int swapMatrix(int a[][10], int lin, int col, int l1, int l2);
void tabsort(int a[], int x);
int Anagrame(int a[], int x, int b[], int y);



void printMatrix(int m[][10], int lin, int col);



/***************************************extra**************************************************/


int getPixel(unsigned char disp[], int x, int y);
void resetPixel(unsigned char disp[], int x, int y);
void setPixel(unsigned char disp[], int x, int y);
void resetLineH(unsigned char disp[], int x, int y, int len);
void resetLineV(unsigned char disp[], int x, int y, int len);
void drawLineH(unsigned char disp[], int x, int y, int len);
void drawLineV(unsigned char disp[], int x, int y, int len);
void drawFullRect(unsigned char disp[], int x1, int y1, int x2, int y2);
void drawEmptyRect(unsigned char disp[], int x1, int y1, int x2, int y2);
void drawFullSquare(unsigned char disp[], int x, int y, int len);
void drawChess(unsigned char disp[], int x, int y, int len);
void drawDiag(unsigned char disp[], int x0, int y0, int x1, int y1);
void drawProiectil(unsigned char disp[], int x0, int y0, int x1, int y1);
void bres(unsigned char disp[], int x1, int y1, int x2, int y2);
void drawEmptyRect(unsigned char disp[], int x1, int y1, int x2, int y2);
void drawAxes(unsigned char disp[], int x, int y);
void drawEmptyLineV(unsigned char disp[], int x, int y, int len);
void drawEmptyLineH(unsigned char disp[], int x, int y, int len);


/***************************************extra**************************************************/




#endif /* ALVUTILS_H_ */
