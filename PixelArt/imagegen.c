#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Convert RGB to xterm Index

int a(int i, int c,int x){
	x=abs(c-=i>215?8+(i-216)*10:x*40+!!x*55);
	return x;
}
int rgb_to_xterm(int r, int g, int b){
	int l,m,t,i;
	for(i=l=240;~i--;t=a(i,r,i/36)+a(i,g,i/6%6)+a(i,b,i%6),t<l?l=t,m=i:1);
	i=m+16;
	return i;
}

int main(int argc, char* argv[]){

	if(argc < 2){
		printf("\033[0;31m"); 
		printf("ERROR | No filename given!\n");
		printf("\033[0m"); 
		exit(0);
	}
	//ncurses and color init
	initscr();
	cbreak();
	noecho();
	start_color();
	for (int i = 0; i < 256; i++) {
	    init_pair(i, i, i); 
	}

	int width, height, bpp;
	uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &bpp, 3);
	/* printf("w: %d, h: %d, bbp: %d\n",width,height,bpp); */
	int as_xterm[width*height];
	int vals = width * height * 3;
	/* printf("vals: %d\n",vals); */
	int idx = 0;
	for(int i=0; i<vals; i++){
		if((i+1)%3 == 0){
			as_xterm[idx] = rgb_to_xterm(*(rgb_image+i-2),*(rgb_image+i-1),*(rgb_image+i));
			idx++;
		}
	}

	
	int res = height * width;
	int cord = 0;
	for(int i=0; i<res; i++){
		int rest = i % width;
		int div = i / width; 
		if(rest == 0 && div % 2 == 1){
			i = i + width;
			cord++;
			continue;
		}
		int y = cord / width;	
		int x = i % width;	
		attron(COLOR_PAIR(as_xterm[i]));
		mvaddch(y, x,'#');
		attroff(COLOR_PAIR(as_xterm[i]));
		cord++;
	}	
	mvaddch(height-1, width, '\n');
	refresh();
	getch();
	endwin();
	return 0;
}

