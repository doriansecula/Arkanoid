/*
 * alvutils.c
 *
 *  Created on: May 19, 2016
 *      Author: autoliv.academy
 */

#include "utils/ustdlib.h"
#include "drivers/rit128x96x4.h"
#include "math.h"
#include "dsputils.h"

#define DISP_HEIGHT 96
#define DISP_WIDTH 128


void printMatrix(int m[][10], int lin, int col) {
	int i, j;
	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			printDec(m[i][j]);
		}
		printLn();
	}

}
/*
 int deleteElem(int t[], int n, int x) {
 int x;
 deleteElemByPoz(t, *n, x);
 return n;
 }
 */

void deleteElemByPoz(int t[], int *n, int k) {
	int i;
	for (i = k; i < *n - 1; i++) {
		t[i] = t[i + 1];
	}
	*n = *n - 1;
}

int getMaxArray(int t[], int n) {

	int max = 0, i = 0;
	max = t[0];
	for (i = 1; i < n; i++) {
		if (t[i] > max) {
			max = t[i];
		}
	}
	return max;
}

void getMaxCount(int t[], int n, unsigned long *max, unsigned long *ctMax) {
	int i = 0;
	*ctMax = 0;
	*max = getMaxArray(t, n);
	for (i = 1; i < n; i++) {
		if (t[i] == *max) {
			*ctMax = *ctMax + 1; //verifica de cate ori apare *max
		}
	}
}

int eMonoton(int t[], int n) {
	int trend = t[0] - t[1];
	int ok = 1;
	int i;
	for (i = 1; i < n - 1; i++) {
		if ((t[i] - t[i + 1]) * trend < 0) {
			ok = 0;
		}
	}

	return ok;
}

int eSimetric(int t[], int n) {
	int i = 0, ok = 1;
	for (i = 0; (i <= n / 2) && (ok); i++) { // &&(ok) se opreste cand ok==0 ; iese din fct;
		if (t[i] != t[n - 1 - i]) {
			ok = 0;
		}
	}
	return ok;
}

void printtab(int t[], unsigned len) {
	int i;
	for (i = 0; i < len; ++i) {
		printDec(t[i]);
	}
}

void printUl(unsigned long x, unsigned pxx, unsigned pxy) {
	char buff[10];
	usprintf(buff, "%u", x);
	RIT128x96x4StringDraw(buff, pxx, pxy, 15);
}

unsigned setBit(unsigned x, int k) {
	return (x | (1 << k));
}

unsigned resetBit(unsigned x, int k) {
	return (x & (~(1 << k)));
}

void swap(unsigned *a, unsigned *b) {

	unsigned aux;

	if (*a > *b) {
		aux = *a;
		*a = *b;
		*b = aux;
	}
}

void sort3(unsigned *a, unsigned *b, unsigned *c) {

	//a<=b<=c;
	if (*a > *b) {
		swap(a, b);
	}
	if (*a > *c) {
		swap(a, c);
	}
	if (*b > *c) {
		swap(b, c);
	}

	printUl(*a, 10, 10);
	printUl(*b, 10, 20);
	printUl(*c, 10, 30);

}

void printHex(unsigned long x, unsigned pxx, unsigned pxy) {
	char buff[15];
	usprintf(buff, "0x%x", x);
	RIT128x96x4StringDraw(buff, pxx, pxy, 15);
}

void printBinar(unsigned char x, int px, int py) {
	int k, bit;

	for (k = 7; k >= 0; k--) {
		bit = (x & (1 << k)) == 0 ? 0 : 1;
		printUl(bit, px, py);
		px = px + 6;
	}

}

int ePrim(unsigned x) {
	int i;
	for (i = 2; i <= x / 2; i++) {
		if (x % i == 0) {
			return 0;
		}
	}
	return 1;

}

int getPrimaCifra(int n) {
	while (n > 9) {
		n = n / 10;
	}
	return n;
}

int nCif(int n) {
	int ct = 0;
	while (n != 0) {
		n = n / 10;
		ct = ct + 1;
	}
	return ct;
}

int nrFaraPrima(int n) {
	n = n % ((int) pow(10, nCif(n) - 1)); /* eroare din cauza funciei pow  */
	return n;
}

int shl10(int n) {
	int p = getPrimaCifra(n);
	int y = getPrimaCifra(n);
	//int y = nrFaraPrima(n);
	return y * 10 + p;

}

int eExtraPrim(int x) {
	int cpX = x;
	int eExtra = 0;
	do {
		x = shl10(x);
		if (ePrim(x))
			//return 0;
			eExtra = 1;
	} while (x != cpX);
	//return 1;
	return eExtra;
}

/*****************************************HW**********************************************/

int AdunareNrMari(int a[], int x, int b[], int y, int S[], int nElem) {

	//int nElem=sizeof(S)/sizeof(int);
	int ns = (x > y) ? x : y;
	int nElemRez = ns;
	ns--;
	x--;
	y--;

	int carry = 0;
	while ((x >= 0) && (y >= 0)) {
		S[ns] = (carry + a[x] + b[y]) % 10;
		carry = (carry + a[x] + b[y]) / 10;
		x--;
		y--;
		ns--;

	}
	if (x >= 0) {
		while (x > 0) {
			S[ns] = (a[x] + carry) % 10;
			carry = (a[x] + carry) / 10;
			ns--;
			x--;
		}
	}
	if (y >= 0) {
		S[ns] = (b[y] + carry) % 10;
		carry = (b[y] + carry) / 10;
		ns--;
		y--;
	}

	return nElemRez;

}

int elimElementeMultiple(int t[], int n, int j, int k) {
	int i = 0;
	int aux = k;
	int count = j;

	for (i = j; i < k; i++) {
		if (aux < n) {
			t[i] = t[aux]; //shifteaza elementele spre stanga eliminand elem. de pe pozitiile (j-k);
			count++; // numarul curent de elemente al sirului;
			aux++;
		} else {
			break;
		}
	}
	for (i = k; i < n; i++) {
		t[i] = t[i + 1]; //shifteaza elementele spre stanga elem ramase in dreapta lui 'k';
		count++;
	}

	return count; //nr. de elem. ramase

}

int verifOrdElem(int t[], int n) {
	int i;
	int ok = 1; //crescatoare
	int aux = t[0] - t[1]; // -1 daca pare a fi cresc
	for (i = 1; i < n - 1; i++) {
		if ((t[i] - t[i + 1]) * aux < 0) { //daca se intra pe if e descresc
			ok = 0;
		}
	}
	return ok;
}

void IntToArray(int n, int t[], int nElm) {
	int j = 0;
	int ncif = nCif(n);
	for (j = 0; j < ncif; j++) {
		t[j] = getPrimaCifra(n);
		n = nrFaraPrima(n);
	}
}

int IntArray(int n, int t[], int nElmMax){ //varianta a 2-a
	int ncif=nCif(n);
	if(ncif>nElmMax){
		return -1;
	}
	int j=ncif-1;
	while(n){
		t[j]=n%10;
		j--;
		n=n/10;
	}
	return ncif;
}

int swapMatrix(int a[][10], int lin, int col, int l1, int l2) {
	int aux = 0;
	int i;
	if ((l1 < 0) || (l1 >= lin) || (l2 < 0) || (l2 >= lin)) {
		return 0;
	}

	for (i = 0; i < col; i++) {
		aux = a[l1][i];
		a[l1][i] = a[l2][i];
		a[l2][i] = aux;
	}

	return 1;
}


void tabsort(int a[], int x){
	int i,j,aux1;
	for (i = 0; i < x - 1; i++) {
				for (j = i + 1; j < x; j++) {
					if (a[i] > a[j]) {
						aux1 = a[i];
						a[i] = a[j];
						a[j] = aux1;
					}
				}
			}

}

int Anagrame(int a[], int x, int b[], int y) {
	int i,ok;
	if (x == y) {
		tabsort(a,x);
		tabsort(b,y);
		for (i = 0; i < x; i++) {
				if (a[i] != b[i]) {
					ok = 0;
				}
		}
	}
	else ok=0;
	return ok;
}




/***************************************extra**************************************************/


unsigned char image[6144] = { 0 };


int getPixel(unsigned char disp[], int x, int y) {

	if ( (*(disp + (y * 64 + x / 2)) & 0xF0) || (*(disp + (y * 64 + x / 2)) & 0x0F) ) {
		return 1;
	}
	else{
		return 0;
	}
}


void resetPixel(unsigned char disp[], int x, int y) {

	if (x % 2 == 0) {
		*(disp + (y * 64 + x / 2)) &= 0x0F;

	} else {
		*(disp + (y * 64 + x / 2)) &= 0xF0;

	}

}

void setPixel(unsigned char disp[], int x, int y) {

	if (x % 2 == 0) {
		*(disp + (y * 64 + x / 2)) |= 0xF0;

	} else {
		*(disp + (y * 64 + x / 2)) |= 0x0F;

	}

}
void resetLineV(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = y; i < len; i++){
		resetPixel(disp, x, i);
	}
}

void resetLineH(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = x; i < len; i++) {
		resetPixel(disp, i, y);
	}
}

void drawLineH(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = x; i < len; i++) {
		setPixel(disp, i, y);
	}
}

void drawLineV(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = y; i < len; i++) {
		setPixel(disp, x, i);
	}
}
void drawEmptyLineV(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = y; i < len; i++) {
		resetPixel(disp, x, i);
	}
}
void drawEmptyLineH(unsigned char disp[], int x, int y, int len) {
	int i;
	for (i = x; i < len; i++) {
		resetPixel(disp, i, y);
	}
}

void drawFullRect(unsigned char disp[], int x1, int y1, int x2, int y2) {
	int i, j;
	for (i = x1; i <= x2; i++) {
		for (j = y1; j <= y2; j++) {
			setPixel(disp, i, j);
		}
	}
}

void drawEmptyRect(unsigned char disp[], int x1, int y1, int x2, int y2) {
	int i, j;
	for (i = x1; i <= x2; i++) {
		for (j = y1; j <= y2; j++) {
			resetPixel(disp, i, j);
		}
	}
}

void drawFullSquare(unsigned char disp[], int x, int y, int len) {
	drawFullRect(disp, x, y, x + len, y + len);
}

void drawChess(unsigned char disp[], int x, int y, int len) {

	int ln, col;
	for (ln = 0; ln < 8; ln++) {
		for (col = 0; col < 8; col++) {
			if ((col + ln) % 2 == 0) {
				drawFullRect(image, col * 10, ln * 10, (col + 1) * 10,
						(ln + 1) * 10);
				RIT128x96x4ImageDraw(image, 0, 0, 128, 96);

			}
		}
	}

	drawLineH(image, 0, 80, 80);
	drawLineH(image, 0, 0, 80);
	drawLineV(image, 0, 0, 80);
	drawLineV(image, 80, 0, 81);

}

void drawDiag(unsigned char disp[], int x0, int y0, int x1, int y1) {
	float deltax = x1 - x0;
	float deltay = y1 - y0;
	float error = -1.0;
	float deltaerr = fabs(deltay / deltax); //functie op. modul math.h

	int y = y0;
	int x;
	for (x = x0; x <= x1 - 1; x++) {
		setPixel(disp, x, y);
		//SysCtlDelay(SysCtlClockGet()/20);
		//RIT128x96x4ImageDraw(disp, 0, 0, 128, 96);
		error = error + deltaerr;
		if (error >= 0.0) {
			y = y + 1;
			error = error - 1.0;
		}
	}

}

void drawProiectil(unsigned char disp[], int x0, int y0, int x1, int y1) {
	float deltax = x1 - x0;
	float deltay = y1 - y0;
	float error = -1.0;
	float deltaerr = fabs(deltay / deltax); //functie op. modul math.h

	int y = y0;
	int x;
	for (x = x0; x <= x1 - 1; x++) {
		setPixel(disp, x, y);
		setPixel(disp, x + 1, y + 1);
		//SysCtlDelay(SysCtlClockGet() / 20);
		RIT128x96x4ImageDraw(disp, 0, 0, 128, 96);
		resetPixel(disp, x, y);
		resetPixel(disp, x + 1, y + 1);
		error = error + deltaerr;
		if (error >= 0.0) {
			y = y + 1;
			error = error - 1.0;
		}
	}

}

void bres(unsigned char disp[], int x1, int y1, int x2, int y2) {
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	incx = 1;
	if (x2 < x1)
		incx = -1;
	incy = 1;
	if (y2 < y1)
		incy = -1;
	x = x1;
	y = y1;

	if (dx > dy) {
		setPixel(disp, x, y);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			} else
				e += inc2;
			x += incx;
			setPixel(disp, x, y);
		}
	} else {
		setPixel(disp, x, y);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			} else
				e += inc2;
			y += incy;
			setPixel(disp, x, y);
		}
	}
}


void drawAxes(unsigned char disp[], int x, int y) {
	bres(disp, x, y, DISP_WIDTH - 1, y); //Ox
	bres(disp, x, y, x, 0); //Oy

	bres(disp, DISP_WIDTH - 1, y, DISP_WIDTH - 1 - 5, y - 5); //Ox
	bres(disp, DISP_WIDTH - 1, y, DISP_WIDTH - 1 - 5, y + 5); //Ox

	bres(disp, x, 0, x-5 , 0+5); //Oy
	bres(disp, x, 0, x+5 , 0+5); //Oy

}




/***************************************extra**************************************************/
