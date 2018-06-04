#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "inc/lm3s8962.h"
#include "utils/ustdlib.c"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "alvutils.h"
#include "dsputils.h"
#include <stdlib.h>
#include "inc/hw_ints.h"

#define DISP_HEIGHT 96
#define DISP_WIDTH 128

#define BRICK_HEIGHT 3
#define BRICK_WIDTH 10

#define BALL_SIZE 1
#define SPACEBAR_WIDTH 22

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

typedef struct Caramida {
	int x;
	int y;
} Caramida_t;

#ifdef DEBUG /* called error routine if needed */
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

void afisare_scor(void);
void miscare_bara_stanga(void);
void miscare_bara_dreapta(void);
void stop_bila_game_over(void); // daca nu mai sunt vieti va opri jocul
void terminare_joc(void); // incerca sa opraesca jocul
void stop_bila(void);
void draw_bila(int draw_bila_x, int draw_bila_y);
void clear_bila(int draw_bila_x, int draw_bila_y);
void draw_bara(int draw_bara_x, int draw_bara_y);
void clear_bara(int draw_bara_x, int draw_bara_y);
void PrintCaramizi(Caramida_t t[], int n);
void levelUpgrade(void);
void ballMove(void);
int deleteCaramida(Caramida_t t[], int n, Caramida_t c);
Caramida_t caramidaLovita(Caramida_t t[], int n, int xGlont, int yGlont);

void enablePortF() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); /* enable port */
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1); /* set pins as input */
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU); /* set slectrical specs */
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_FALLING_EDGE); /* set interrupt mode */
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_1); /* set interrupt pins */
	IntEnable(INT_GPIOF); /* enable interrupts */
}

volatile int Xbar, Ybar;
volatile int Xball, Yball;
int Pauza = 1;
int schimbat = 1; // pentru a printa doar cand se va sparge o caramida din nou caramizile
unsigned char imag[6144] = { 0 };
int blockINT = 0;

void portFInterruptHandler(void) {
	static int n = 1;
	if (blockINT != 1) { //blocare Intrerupere dupa GameOver
		if (n % 2 != 0) {
			memset(imag, 0, 6144);
			RIT128x96x4StringDraw("     ", 48, 52, 0);
			RIT128x96x4ImageDraw(imag, 45, 62, 90, 68);
			Pauza = 1;
		}
		if (n % 2 == 0) {
			RIT128x96x4StringDraw("PAUSE", 48, 52, 15);
			RIT128x96x4ImageDraw(imag, 45, 52, 90, 68);
			Pauza = 0;
		}
		n++;
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_1); /* clear interrupt so the next one may come */
	}
}

unsigned long data_e;
volatile int scor; //var afisare scor
volatile int level; //var afisare level
int block_from_ball_length = 4; // distanta
int flag_scor = 1; // flag afisare scor
int neApasat = 1; // flag pentru a nu trece mai mult de un tact la o apasare de button
int count = 0; // contor nr.pasi pentr bara pentru a nu depasi cadrul
int stop_bila_flag = 1;
unsigned afisare_level = 1; // ajuta la afisarea corespunzatoare a levelului
int HitBack = 0; // '1' - se intoarce bila
int GameOverFlag = 0;
int CentrareDispX = 2; // Centrarea DispX
int deltaXBall = 0;
int deltaYBall = 1;
int HighSpeedLeft = 0;
int HighSpeedRight = 0;

Caramida_t vCaramiziLv1[] = { { 17, 12 }, { 30, 18 }, { 43, 23 }, { 56, 18 }, {
		56, 28 }, { 69, 23 }, { 82, 18 }, { 95, 12 } };

Caramida_t vCaramiziLv2[] = { { 17, 28 }, { 30, 18 }, { 43, 23 }, { 56, 18 }, {
		56, 28 }, { 69, 23 }, { 82, 18 }, { 95, 28 } };

Caramida_t vCaramiziLv3[] = { { 17, 12 }, { 30, 23 }, { 43, 12 }, { 56, 23 }, {
		69, 12 }, { 82, 23 }, { 95, 12 } };

Caramida_t vCaramiziLv4[] = { { 17, 12 }, { 17, 28 }, { 30, 12 }, { 30, 28 }, {
		56, 12 }, { 56, 28 }, { 82, 12 }, { 82, 28 }, { 95, 12 }, { 95, 28 } };

Caramida_t vCaramiziLv5[] = { { 17, 12 }, { 17, 23 }, { 30, 18 }, { 56, 18 }, {
		82, 18 }, { 95, 12 }, { 95, 23 } }; // L5

Caramida_t vCaramiziLv6[] = { { 17, 23 }, { 30, 18 }, { 30, 28 }, { 43, 12 }, {
		56, 12 }, { 56, 28 }, { 69, 12 }, { 82, 18 }, { 82, 28 }, { 95, 23 } };

Caramida_t vCaramiziLv7[] = { { 17, 23 }, { 30, 18 }, { 43, 12 }, { 43, 23 }, {
		56, 12 }, { 56, 23 }, { 69, 12 }, { 69, 23 }, { 82, 18 }, { 95, 23 } };

Caramida_t *pLevel = vCaramiziLv1;

int nc = ARRAY_SIZE(vCaramiziLv1);
int scor_nc = ARRAY_SIZE(vCaramiziLv1);

int main(void) {
	SysCtlClockSet(
			SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN
					| SYSCTL_XTAL_8MHZ); /* clock set */

	RIT128x96x4Init(1000000); /* screen initialize */

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIO_PORTE_DIR_R = 0x00u;
	GPIO_PORTE_DEN_R = 0xFFu;

	GPIO_PORTF_DIR_R = 0x01u;
	GPIO_PORTF_DEN_R = 0x01u;

	GPIOPadConfigSet(GPIO_PORTE_BASE,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

	dspRst();
	RIT128x96x4ImageDraw(logo, 0, 0, 128, 96);
	SysCtlDelay(SysCtlClockGet());
	dspRst();

	scor = 0;
	level = 1;

	Xbar = (DISP_WIDTH / 2) - SPACEBAR_WIDTH / 2 - CentrareDispX;
	Ybar = DISP_HEIGHT - 2;

	Xball = ((DISP_WIDTH / 2) - BALL_SIZE) - CentrareDispX; //Aliniere Centrala
	Yball = (((DISP_HEIGHT / 2) - BALL_SIZE) - block_from_ball_length);

	draw_bara(Xbar, Ybar);

	while (1) {

		unsigned long data_e = GPIO_PORTE_DATA_R;
		unsigned long data_f = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);

		afisare_scor();

		if (schimbat) {
			PrintCaramizi(pLevel, nc);
			schimbat = 0; //pentru a updata doar 1 nu la fiecare ciclu
		}

		if (Pauza) {
			ballMove();
		}

		Caramida_t c = caramidaLovita(pLevel, nc, Xball, Yball);
		if (c.x != -1) {
			nc = deleteCaramida(pLevel, nc, c);
			schimbat = 1;
		}

		enablePortF();

		if (((data_f & (1 << 1)) == 0) && (neApasat)) { // select
		}
		if ((data_f & (1 << 1)) == 1) {
			neApasat = 1;
		}

		if (((data_e & (1 << 2)) == 0) && (neApasat)) { // left
			miscare_bara_stanga();
		}
		if ((data_e & (1 << 2)) == 1) {
			neApasat = 1;
		}

		if (((data_e & (1 << 3)) == 0) && (neApasat)) { // right
			miscare_bara_dreapta();
		}
		if ((data_e & (1 << 3)) == 1) {
			neApasat = 1;
		}

	}
}

void afisare_scor(void) {
	if (scor >= 0) {
		RIT128x96x4StringDraw("BRICKS:", 0, 4, 15);
		RIT128x96x4StringDraw("LEVEL:", 62, 4, 15);
		if (GameOverFlag == 0) {
			RIT128x96x4StringDraw("|||", 108, 4, 15);
		}
		RIT128x96x4ImageDraw(imag, 0, 0, 0, 10);
		printUl(scor, 42, 4);
		printUl(level, 98, 4);
	}
}

void miscare_bara_stanga(void) {
	if ((count < 6) && (count > -8)) { // max 10 pasi stanga-dreapta
		count++;
		clear_bara(Xbar, Ybar);
		Xbar -= 8;
		draw_bara(Xbar, Ybar);
	}
}

void miscare_bara_dreapta(void) {
	if ((count < 7) && (count > -7)) {
		count--;
		clear_bara(Xbar, Ybar);
		Xbar += 8;
		draw_bara(Xbar, Ybar);
	}
}

void stop_bila_game_over(void) {
	GameOverFlag += 1;
	Xball = ((DISP_WIDTH / 2) - BALL_SIZE - CentrareDispX);
	Yball = (((DISP_HEIGHT / 2) - BALL_SIZE) - block_from_ball_length);

	switch (GameOverFlag) {
	case 1:
		RIT128x96x4StringDraw("|| ", 108, 4, 15);
		draw_bila(Xball, Yball);
		RIT128x96x4StringDraw("READY", 48, 64, 15);
		SysCtlDelay(SysCtlClockGet() / 2);
		RIT128x96x4StringDraw("     ", 48, 64, 0);
		break;
	case 2:
		RIT128x96x4StringDraw("|  ", 108, 4, 15);
		draw_bila(Xball, Yball);
		RIT128x96x4StringDraw("READY", 48, 64, 15);
		SysCtlDelay(SysCtlClockGet() / 2);
		RIT128x96x4StringDraw("     ", 48, 64, 0);

		break;
	default:
		blockINT = 1;
		while (1) {
			RIT128x96x4StringDraw("   ", 108, 4, 15);
			RIT128x96x4StringDraw("GAME OVER", 36, 50, 15);
			RIT128x96x4StringDraw("SCOR:", 43, 60, 15);
			printUl((((level - 1) * 10) + scor), 74, 60);
			SysCtlDelay(SysCtlClockGet());
		}
	}
}

void terminare_joc(void) {

	if (((Yball > DISP_HEIGHT - 5) && (Xball < Xbar - 2)) //4 reprezinta y2
	|| ((Yball > DISP_HEIGHT - 5) && (Xball > Xbar + 22))) { //terminare joc daca bila trece pe langa bara
		stop_bila_game_over();
	}

}

void stop_bila(void) {
	if (stop_bila_flag == 1) {
		drawFullRect(imag, Xball, Yball - 4, Xball - 4, Yball - 6);
		RIT128x96x4ImageDraw(imag, Xball, Yball - 4, Xball - 4, Yball - 6);
		stop_bila_flag = 0;
	}
}

void printCaramida(Caramida_t c, unsigned char disp[]) {
	drawFullRect(disp, c.x, c.y, c.x + BRICK_WIDTH, c.y + BRICK_HEIGHT);
}

void PrintCaramizi(Caramida_t t[], int n) {
	memset(imag, 0, 6144); //clear imag
	int i;
	for (i = 0; i < n; i++) {
		printCaramida(t[i], imag);
	}
	RIT128x96x4ImageDraw(imag, 0, 11, 128, 40);

}

int deleteCaramida(Caramida_t t[], int n, Caramida_t c) {
	int i;
	for (i = 0; i < n; i++) {
		if ((t[i].x == c.x) && (t[i].y == c.y)) { // compara x,y al fiecarei caramizi cu c.x,c.y adica coord. caramizii de spart
			t[i] = t[n - 1];
			n = n - 1; // nr elementelor vectorului scade cu 1

		}
	}

	return n; //nr de elemente ramase
}

Caramida_t caramidaLovita(Caramida_t t[], int n, int xGlont, int yGlont) {
	Caramida_t result;
	result.x = -1; //niciuna nu e lovita
	int i;
	for (i = 0; i < n; i++) {

		if ((xGlont >= t[i].x)
				&& (xGlont - BRICK_WIDTH + 4 <= t[i].x + BRICK_WIDTH)
				&& (yGlont >= t[i].y)
				&& (yGlont - BRICK_HEIGHT - 4 <= t[i].y + BRICK_HEIGHT)) {
			result = t[i];
			scor++;
			schimbat = 1; // update Afisare Caramizi
			HitBack = 1; // bila se intoarce dupa ce loveste o caramida
		}
	}
	return result;
}

void draw_bila(int draw_bila_x, int draw_bila_y) {
	unsigned char bila_vector[4] = { 0 };
	memset(bila_vector, 255, 4);
	RIT128x96x4ImageDraw(bila_vector, draw_bila_x, draw_bila_y, 2, 2);
}

void clear_bila(int draw_bila_x, int draw_bila_y) {
	unsigned char bila_vector[4] = { 0 };
	memset(bila_vector, 0, 4);
	RIT128x96x4ImageDraw(bila_vector, draw_bila_x, draw_bila_y, 2, 2);
}

void draw_bara(int draw_bara_x, int draw_bara_y) {
	unsigned char bara_vector[24] = { 0 };
	memset(bara_vector, 255, 24);
	RIT128x96x4ImageDraw(bara_vector, draw_bara_x, draw_bara_y, 22, 2);
}

void clear_bara(int draw_bara_x, int draw_bara_y) {
	unsigned char bara_vector[24] = { 0 };
	memset(bara_vector, 0, 24);
	RIT128x96x4ImageDraw(bara_vector, draw_bara_x, draw_bara_y, 22, 2);
}

void levelUpgrade(void) {
	if (scor == scor_nc) {
		draw_bara(Xbar, Ybar);
		RIT128x96x4StringDraw("LEVEL COMPLETE!", 20, 50, 15);
		SysCtlDelay(SysCtlClockGet());
		afisare_level++;
		schimbat = 1;
		Xball = ((DISP_WIDTH / 2) - BALL_SIZE - CentrareDispX);
		Yball = (((DISP_HEIGHT / 2) - BALL_SIZE) - block_from_ball_length);

		switch (afisare_level) {
		case 1:
			pLevel = vCaramiziLv1;
			nc = ARRAY_SIZE(vCaramiziLv1);
			scor_nc = ARRAY_SIZE(vCaramiziLv1);
			break;
		case 2:
			pLevel = vCaramiziLv2;
			nc = ARRAY_SIZE(vCaramiziLv2);
			scor_nc = ARRAY_SIZE(vCaramiziLv2);
			break;
		case 3:
			pLevel = vCaramiziLv3;
			nc = ARRAY_SIZE(vCaramiziLv3);
			scor_nc = ARRAY_SIZE(vCaramiziLv3);
			break;
		case 4:
			pLevel = vCaramiziLv4;
			nc = ARRAY_SIZE(vCaramiziLv4);
			scor_nc = ARRAY_SIZE(vCaramiziLv4);
			break;
		case 5:
			pLevel = vCaramiziLv5;
			nc = ARRAY_SIZE(vCaramiziLv5);
			scor_nc = ARRAY_SIZE(vCaramiziLv5);
			break;
		case 6:
			pLevel = vCaramiziLv6;
			nc = ARRAY_SIZE(vCaramiziLv6);
			scor_nc = ARRAY_SIZE(vCaramiziLv6);
			break;
		case 7:
			pLevel = vCaramiziLv7;
			nc = ARRAY_SIZE(vCaramiziLv7);
			scor_nc = ARRAY_SIZE(vCaramiziLv7);
			break;
		default:
			blockINT = 1; //Pentru a nu da "Pauza" in acest moment
			while (1) {
				RIT128x96x4StringDraw("GAME COMPLETE", 20, 50, 15);
				SysCtlDelay(SysCtlClockGet());
				pLevel = NULL;
			}
		}
		scor = 0;
		RIT128x96x4StringDraw("   ", 42, 4, 15);
		level++;
		printUl(scor, 42, 4);
		RIT128x96x4StringDraw("               ", 20, 50, 15);
		RIT128x96x4StringDraw("          ", 39, 62, 15);
	}
}

void ballMove(void) {

	short centralArea = 3; //zone ale barei
	short sensitiveArea = 2;

	Xball += deltaXBall;
	Yball += deltaYBall;

	terminare_joc();
	levelUpgrade();

	if (Yball > DISP_HEIGHT - 5) {
		deltaYBall = -1;

	}
	if ((Yball < 12) || (HitBack)) {
		deltaYBall = 1;
		HitBack = 0;
	}

	if ((Xball >= Xbar + SPACEBAR_WIDTH / 2 - centralArea)
			&& (Xball <= Xbar + SPACEBAR_WIDTH / 2 + centralArea)
			&& Yball > 90) {
		deltaXBall = deltaXBall;
	}

	if ((Xball >= Xbar + sensitiveArea)
			&& (Xball < Xbar + SPACEBAR_WIDTH / 2 - centralArea) && Yball > 90) // spre stanga NormalSpeed
					{
		deltaXBall = -1;
	}

	if ((Xball >= Xbar) && (Xball < Xbar + sensitiveArea) && Yball > 90) // spre stanga HighSpeed {-1 pentru da impresia de limita maxima stanga}
			{
		HighSpeedLeft = 1;
		deltaXBall = -2;
	}

	if (Xball > DISP_WIDTH - 5) {

		if (HighSpeedRight == 1) {
			deltaXBall = -2;
			HighSpeedRight = 0;
		} else {
			deltaXBall = -1;
		}
	}

	if ((Xball > Xbar + SPACEBAR_WIDTH / 2 + centralArea)
			&& (Xball < Xbar + SPACEBAR_WIDTH - sensitiveArea) && Yball > 90) // spre dreapta NormalSpeeed
					{
		deltaXBall = 1;
	}

	if ((Xball >= Xbar + SPACEBAR_WIDTH / 2 + centralArea + sensitiveArea)
			&& (Xball < Xbar + SPACEBAR_WIDTH) && Yball > 90) // spre dreapta HighSpeeed
					{
		HighSpeedRight = 1;
		deltaXBall = 2;
	}

	if (Xball < 5) {
		if (HighSpeedLeft == 1) {
			deltaXBall = 2;
			HighSpeedLeft = 0;
		} else {
			deltaXBall = 1;
		}

	}

	draw_bila(Xball, Yball);
	SysCtlDelay(45000);
	clear_bila(Xball, Yball);

}
