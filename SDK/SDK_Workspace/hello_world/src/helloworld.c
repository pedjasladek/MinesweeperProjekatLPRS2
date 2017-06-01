/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 *
 *
 *.
 *.
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "xil_exception.h"
#include "vga_periph_mem.h"
#include "minesweeper_sprites.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#define SIZE 9
#define UP 0b01000000
#define DOWN 0b00000100
#define LEFT 0b00100000
#define RIGHT 0b00001000
#define CENTER 0b00010000
#define SW0 0b00000001
#define SW1 0b00000010
#define BOMB '*'
#define NUM1 '1'
#define NUM2 '2'
#define NUM3 '3'
#define NUM4 '4'
#define BLANK '0'
#define FLAG '#'
#define NUMOFMINES 9
//BEG---unpened field
#define BEG '@'
#define FOUR 4
#define MAX 6

int brojac = 0;
int brojac1 = 0;		//X pozicija gde ce se nacrtati znak u levim poljima
int brojac2 = 0;		//Y pozicija gde ce se nacrtati znak u levim poljima
int brojac3 = 0;		//Brojac za leva 24 polja
int brojac4 = 0;
int finish_itterator = 0;
int Crvene_temp = 0;

int endOfGame;
int inc1;
int inc2;
int i, x, y, ii, oi, R, G, B, RGB, kolona, red, RGBgray;
int m = 0;
int numOfFlags;
int flagTrue;
int randomCounter = 50;
int numOfMines;
int firstTimeCenter;
int k = 0;
int j = 0;

int size = 4;


int S=0,T=16,P=32,H=48,K=64,Z=80;


int Skocko = 0;
int Tref = 16;
int Pik = 32;
int Herc = 48;
int Karo = 64;
int Zvezda = 80;

int Skocko_count = 0;
int Tref_count  = 0;
int Pik_count  = 0;
int Herc_count  = 0;
int Karo_count = 0;
int Zvezda_count  = 0;

int Skocko_count_temp = 0;
int Pik_count_temp = 0;
int Tref_count_temp = 0;
int Herc_count_temp = 0;
int Karo_count_temp = 0;
int Zvezda_count_temp = 0;

char temp_array[4];
int resenje_niz_temp[4];
int z = 0;

int Zute = 0;
int Crvene = 0;
int Zute_temp = 0;
int draw_crvene = 0;
int draw_zute = 0;
int p=0;
int resenje_niz[4];
int colors_itteratorX = 5;
int colors_itteratorY = 0;


//map that is hidden from the user-it contains the solution
char solvedMap[9][9];
//map that has all of player's moves
char blankMap[9][9];
//map used for opening the blank fields that surround blank field selected
char indicationMap[9][9];

//end of game
void printOutEndOfGame(char blankTable[SIZE][SIZE], char solvedMap[SIZE][SIZE]) {

	int i, j, ii, jj;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			ii = (i * 16) + 80;
			jj = (j * 16) + 80;
			if (blankTable[i][j] == FLAG) {
				if (solvedMap[i][j] != BOMB) {
					drawMap(16, 0, ii, jj, 16, 16);
				}
			} else if (blankTable[i][j] != FLAG && solvedMap[i][j] == BOMB) {
				drawMap(0, 0, ii, jj, 16, 16);
			}
		}
	}
}

//when the blank field is pressed, open all blank fields around it

//function for opening selected field

void openField(int x, int y, char map[9][9]) {

	int i, j;
	int x1, y1;
	x1 = (x - 80) / 16;
	y1 = (y - 80) / 16;

	switch (map[x1][y1]) {
	/*case NUM1:
	 drawMap(16, 0, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = NUM1;
	 break;

	 case NUM2:
	 drawMap(32, 0, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = NUM2;
	 break;

	 case NUM3:
	 drawMap(48, 0, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = NUM3;
	 break;

	 case BLANK:
	 drawMap(0, 0, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = BLANK;
	 for (i = 0; i < 9; i++) {
	 for (j = 0; j < 9; j++) {
	 xil_printf("%c", indicationMap[i][j]);
	 }
	 xil_printf("\n");
	 }
	 break;

	 case NUM4:
	 drawMap(64, 0, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = NUM4;
	 break;

	 case BOMB:
	 if (map != blankMap)
	 blankMap[x1][y1] = BOMB;
	 endOfGame = 1;
	 printOutEndOfGame(blankMap, solvedMap);
	 drawMap(32, 16, x - 1, y - 1, 16, 16);
	 drawMap(77, 54, 120, 54, 27, 26);
	 break;

	 case '@':
	 if(brojac==0){
	 drawMap(brojac, 0, x-1, y-1, 16, 16);
	 brojac+=16;
	 }
	 if (brojac==96){
	 drawMap(brojac, 0, x-1, y-1, 16, 16);
	 brojac-=16;
	 }
	 drawMap(brojac, 0, x-1, y-1, 16, 16);

	 break;

	 case '#':
	 drawMap(64, 16, x - 1, y - 1, 16, 16);
	 if (map != blankMap)
	 blankMap[x1][y1] = FLAG;
	 break;*/
	}
}

//function that generates random game map

void makeTable(char temp[9][9]) {
	int numOfMines = NUMOFMINES, row, column, i, j, m, surroundingMines = 0;
	char table[9][9];



	//popunjava matricu nulama
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			table[i][j] = BLANK;
		}
	}

	/*	//postavlja random mine
	 while (numOfMines > 0) {
	 row = rand() % 9;
	 column = rand() % 9;
	 if (table[row][column] == BLANK) {
	 table[row][column] = BOMB;
	 numOfMines--;
	 }

	 }

	 //proverava poziciju mina i ispisuje brojeve na odg mesta
	 for (i = 0; i < 9; i++) {
	 for (j = 0; j < 9; j++) {
	 surroundingMines = 0;
	 if (table[i][j] != BOMB) {
	 if (i > 0 && j > 0) {
	 if (table[i - 1][j - 1] == BOMB)
	 surroundingMines++;
	 }
	 if (j > 0) {
	 if (table[i][j - 1] == BOMB)
	 surroundingMines++;
	 }
	 if (i < 9 - 1 && j > 0) {
	 if (table[i + 1][j - 1] == BOMB)
	 surroundingMines++;
	 }
	 if (i > 0) {
	 if (table[i - 1][j] == BOMB)
	 surroundingMines++;
	 }
	 if (i < 9 - 1) {
	 if (table[i + 1][j] == BOMB)
	 surroundingMines++;
	 }
	 if (i > 0 && j < 9 - 1) {
	 if (table[i - 1][j + 1] == BOMB)
	 surroundingMines++;
	 }
	 if (j < 9 - 1) {
	 if (table[i][j + 1] == BOMB)
	 surroundingMines++;
	 }
	 if (i < 9 - 1 && j < 9 - 1) {
	 if (table[i + 1][j + 1] == BOMB)
	 surroundingMines++;
	 }
	 table[i][j] = surroundingMines + '0';
	 }
	 }

	 }

	 //for testing

	 for (i = 0; i < 9; i++) {
	 for (j = 0; j < 9; j++) {
	 xil_printf("%c", table[i][j]);
	 }
	 xil_printf("\n");
	 }

	 for (i = 0; i < 9; i++) {
	 for (j = 0; j < 9; j++) {
	 temp[i][j] = table[j][i];

	 }
	 }

	 xil_printf("\n");

	 for (i = 0; i < 9; i++) {
	 for (j = 0; j < 9; j++) {
	 xil_printf("%c", temp[j][i]);
	 }
	 xil_printf("\n");
	 }
	 */
}

//extracting pixel data from a picture for printing out on the display

/*void drawMap(int in_x, int in_y, int out_x, int out_y, int width, int height) {
 int ox, oy, oi, iy, ix, ii;
 for (y = 0; y < height; y++) {
 for (x = 0; x < width; x++) {
 ox = out_x + x;
 oy = out_y + y;
 oi = oy * 320 + ox;
 ix = in_x + x;
 iy = in_y + y;
 ii = iy * minesweeper_sprites.width + ix;
 R = minesweeper_sprites.pixel_data[ii
 * minesweeper_sprites.bytes_per_pixel] >> 5;
 G = minesweeper_sprites.pixel_data[ii
 * minesweeper_sprites.bytes_per_pixel + 1] >> 5;
 B = minesweeper_sprites.pixel_data[ii
 * minesweeper_sprites.bytes_per_pixel + 2] >> 5;
 R <<= 6;
 G <<= 3;
 RGB = R | G | B;

 VGA_PERIPH_MEM_mWriteMemory(
 XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
 + oi * 4, RGB);
 }
 }

 }
 */
void drawMap(int in_x, int in_y, int out_x, int out_y, int width, int height) {
	int ox, oy, oi, iy, ix, ii;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ox = out_x + x;
			oy = out_y + y;
			oi = oy * 320 + ox;
			ix = in_x + x;
			iy = in_y + y;
			ii = iy * gimp_image.width + ix;
			R = gimp_image.pixel_data[ii * gimp_image.bytes_per_pixel] >> 5;
			G = gimp_image.pixel_data[ii * gimp_image.bytes_per_pixel + 1] >> 5;
			B = gimp_image.pixel_data[ii * gimp_image.bytes_per_pixel + 2] >> 5;
			R <<= 6;
			G <<= 3;
			RGB = R | G | B;

			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + oi * 4,
					RGB);
		}
	}

}

//drawing cursor for indicating position
void drawingCursor(int startX, int startY, int endX, int endY) {

	for (x = startX; x < endX; x++) {
		for (y = startY; y < startY + 1; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + i * 4,
					0x000000);
		}
	}

	for (x = startX; x < endX; x++) {
		for (y = endY - 1; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + i * 4,
					0x000000);
		}
	}

	for (x = startX; x < startX + 1; x++) {
		for (y = startY; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + i * 4,
					0x000000);
		}
	}

	for (x = endX - 1; x < endX; x++) {
		for (y = startY; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + i * 4,
					0x000000);
		}
	}

}


int generateRandom(int l, int u)
{

	int r = rand() % ((u - l) + 1);
	r = l + r;

	return r * 16;
}



//	Function that controls movement of cursor

void move() {


	typedef enum {
		IDLE, LEFT_PRESSED, RIGHT_PRESSED, CENTER_PRESSED
	} state_t;

	int startX = 105, startY = 61, endX = 119, endY = 75;
	int oldStartX, oldEndX;
	int X = 80, Y = 80, XX, YY, i;

	for (i = 0; i < FOUR; i++)
	{
		resenje_niz[i] = generateRandom(0, 5);
		drawMap(resenje_niz[i], 0, 60 + m, 40, 16, 16);
		m += 16;
	}

	state_t state = IDLE;
	state_t btn_old_state = IDLE;

	drawingCursor(startX, startY, endX, endY);

	makeTable(solvedMap);

	while (endOfGame != 1) {

		if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & RIGHT) == 0) {
			state = RIGHT_PRESSED;
		} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & LEFT) == 0) {
			state = LEFT_PRESSED;
		} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & CENTER) == 0) {
			state = CENTER_PRESSED;
		} else {
			state = IDLE;
		}

		if (state != btn_old_state) {

			switch (state) {

			case IDLE:
				break;

			case LEFT_PRESSED:

				if (startX > 117) {

					oldStartX = startX;
					startX -= 16;
					endX -= 16;
					brojac -= 16;

					drawingCursor(startX, startY, endX, endY);
					drawMap(oldStartX - 105, 0, oldStartX - 1, startY - 1, 16,
							16);
				}

				break;

			case RIGHT_PRESSED:

				if (endX < 187) {
					oldStartX = startX;
					oldEndX = endX;
					startX += 16;
					endX += 16;
					brojac += 16;

					drawingCursor(startX, startY, endX, endY);
					drawMap(oldStartX - 105, 0, oldStartX - 1, startY - 1, 16,
							16);

				}
				break;

			case CENTER_PRESSED:


				if(brojac1 < 64){
					drawMap(brojac, 0, 80 + brojac1 * 16, 80 + brojac2 * 16, 16, 16);
					temp_array[k] = brojac;
					k++;
					brojac3++;

					if (brojac == 0)
						temp_array[k] = '0';
					else if (brojac == 16)
						temp_array[k] = '1';
					else if (brojac == 32)
						temp_array[k] = '2';
					else if (brojac == 48)
						temp_array[k] = '3';
					else if (brojac == 64)
						temp_array[k] = '4';
					else if (brojac == 80)
						temp_array[k] = '5';


					/*
					if ((brojac == resenje_niz[0] && brojac1 == 0)|| (brojac == resenje_niz[1] && brojac1 == 1)|| (brojac == resenje_niz[2] && brojac1 == 2)|| (brojac == resenje_niz[3] && brojac1 == 3)) {

						if (brojac == 0 ) {
							Crvene++;
						} else if (brojac == 16) {
							Crvene++;
						} else if (brojac == 32) {
							Crvene++;
						} else if (brojac == 48) {
							Crvene++;
						} else if (brojac == 64) {
							Crvene++;
						} else if (brojac == 80) {
							Crvene++;
						}
						brojac4++;
					}
					*/




					/*
					else if((brojac == resenje_niz[0] && brojac1 != 0)|| (brojac == resenje_niz[1] && brojac1 != 1)|| (brojac == resenje_niz[2] && brojac1 != 2)|| (brojac == resenje_niz[3] && brojac1 != 3)) {
						if(brojac == 0 && Skocko_count != 0){
							Skocko_count--;
							Zute++;
						}
						else if(brojac == 16 && Tref_count != 0){
							Tref_count--;
							Zute++;
						}
						else if(brojac == 32 && Pik_count != 0){
							Pik_count--;
							Zute++;
						}
						else if(brojac == 48 && Herc_count != 0){
							Herc_count--;
							Zute++;
						}
						else if(brojac == 64 && Karo_count != 0){
							Karo_count--;
							Zute++;
						}
						else if(brojac == 80 && Zvezda_count != 0){
							Zvezda_count--;
							Zute++;
						}


					}
		*/

					brojac1++;

					if (brojac1 == 4) {

						srand(getpid());

						Crvene = 0;
						Zute = 0;
						int it, pt, xt;
						int skip[FOUR] = { 0 };
						int whiteSkip[FOUR] = { 0 };

						// Red Loop, sets indexes to skip
						for (it = 0; it < FOUR; it++) {
							if (resenje_niz[it] == temp_array[it]) {
								Crvene += 1;
								skip[it] = 1;
								whiteSkip[it] = 1;
							}
						}

						for (pt = 0; pt < FOUR; pt++) {
							if (skip[pt]) // Skipping those marked as red
							{
								continue;
							}

							for (xt = 0; xt < FOUR; xt++) {
								if (whiteSkip[xt]) // Skiping those marked as white
								{
									continue;
								} else if (temp_array[pt] == resenje_niz[xt]) // Finds whites
										{
									//White found
									Zute += 1;
									whiteSkip[xt] = 1;
									skip[pt] = 1;
									break;
								}
							}
						}

					/*	for(k=0; k < 4; k++){
							for(j = 0; j < size; j++){
								if(temp_array[k] == resenje_niz_temp[j]){
									Zute_temp++;
									for(i = 0; i < size; i++){
										if(temp_array[k] == resenje_niz_temp[i]){
											resenje_niz_temp[i] = 100;
										}
									}

								}
							}
						}

						Zute = Zute_temp - Crvene; */
						Crvene_temp = Crvene;
						draw_crvene = Crvene + 5;

						while(Crvene > 0){



							for(colors_itteratorX; colors_itteratorX < draw_crvene; colors_itteratorX++){
								drawMap(96, 0, 80 + colors_itteratorX * 16, 80 + colors_itteratorY * 16, 16, 16);
								Crvene--;
							}

						}

						draw_zute = Zute + draw_crvene;


						while(Zute > 0){

							for(colors_itteratorX; colors_itteratorX < draw_zute; colors_itteratorX++){
								drawMap(112, 0, 80 + colors_itteratorX * 16, 80 + colors_itteratorY * 16, 16, 16);
								Zute--;
							}

						}

						if (brojac3 == 24 || Crvene_temp == 4) {
							endOfGame = 1;
							//printOutEndOfGame(blankMap, solvedMap);
							for (kolona = 7; kolona < 8; kolona++) {
								for (red = 0; red < 4;
										red++, finish_itterator++) {
									drawMap(resenje_niz[finish_itterator], 0,
											80 + red * 16, 80 + kolona * 16, 16,
											16);
								}
							}
						}

						k = 0;
						i = 0;
						Crvene = 0;
						Crvene_temp = 0;
						Zute = 0;
						Zute_temp = 0;
						brojac4 = 0;
						brojac1 = 0;
						brojac2++;
						colors_itteratorX = 5;
						colors_itteratorY++;
					}

				}

				break;
			}
		}
		btn_old_state = state;
	}
}



int main() {


	int j, p, r;
	inc1 = 0;
	inc2 = 0;
	firstTimeCenter = 0;

	init_platform();

	//map which contains all the moves of the player
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			blankMap[i][j] = BEG;
		}
	}

	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0); // direct mode   0
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3); // display_mode  1
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x0); // show frame      2
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0xff); // font size       3
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0x000000); // foreground 4
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x0000FF); // background color 5
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000); // frame color      6
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x20, 1);

	//black background+
	for (x = 0; x < 320; x++) {
		for (y = 0; y < 240; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF + i * 4,
					0x0f0f0f);
		}
	}

	//drawing a map (LEFT FIELDS)

	for (kolona = 0; kolona < 6; kolona++) {
		for (red = 0; red < 4; red++) {
			drawMap(128, 0, 80 + red * 16, 80 + kolona * 16, 16, 16);
		}
	}

	//drawing a map (RIGHT FIELDS)

	for (kolona = 0; kolona < 6; kolona++) {
		for (red = 5; red < 9; red++) {
			drawMap(128, 0, 80 + red * 16, 80 + kolona * 16, 16, 16);
		}
	}

	//drawing a map (TOP FIELDS)
	int i = 0;
	for (kolona = 0; kolona < 6; kolona++) {
		drawMap(i, 0, 88 + 16 + kolona * 16, 60, 16, 16);
		i += 16;
	}

	//drawing a map (BLANK FIELDS BETWEEN LEFT FIELDS AND FINAL LINE)

	for (kolona = 7; kolona < 8; kolona++) {
		for (red = 0; red < 4; red++) {
			drawMap(128, 0, 80 + red * 16, 80 + kolona * 16, 16, 16);
		}
	}

	move();

	cleanup_platform();

	return 0;
}
