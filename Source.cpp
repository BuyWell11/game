#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <string.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

typedef struct {
	int x, y;
	int hp;
	int power;
	int dice;
	char name[20];
	char class[20];
	SDL_Texture* icon;
}player;

player heroes[4];

void quit();
void doRender(SDL_Renderer* renderer, player* heroes);

int main(int argc, char* argv[]) {
	bool run = true;
	SDL_Event event;
	SDL_Surface* surface = NULL;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		printf("error init img\n");
	}

	for (int i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			strcpy(heroes[0].class, "warrior");
			break;
		case 1:
			strcpy(heroes[1].class, "wizard");
			break;
		case 2:
			strcpy(heroes[2].class, "archer");
			break;
		case 3:
			strcpy(heroes[3].class, "dwarf");
			break;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (strcmp(heroes[i].class, "warrior") == 0) {
			surface = IMG_Load("warrior.png");
			heroes[i].icon = SDL_CreateTextureFromSurface(renderer, surface);
			if (heroes[i].icon == 0) {
				printf("error create texture\n");
			}
			SDL_FreeSurface(surface);
		}
		if (strcmp(heroes[i].class, "wizard") == 0) {
			surface = IMG_Load("wizard.png");
			heroes[i].icon = SDL_CreateTextureFromSurface(renderer, surface);
			if (heroes[i].icon == 0) {
				printf("error create texture\n");
			}
			SDL_FreeSurface(surface);
		}
		if (strcmp(heroes[i].class, "archer") == 0) {
			surface = IMG_Load("archer.png");
			heroes[i].icon = SDL_CreateTextureFromSurface(renderer, surface);
			if (heroes[i].icon == 0) {
				printf("error create texture\n");
			}
			SDL_FreeSurface(surface);
		}
		if (strcmp(heroes[i].class, "dwarf") == 0) {
			surface = IMG_Load("dwarf.png");
			heroes[i].icon = SDL_CreateTextureFromSurface(renderer, surface);
			if (heroes[i].icon == 0) {
				printf("error create texture\n");
			}
			SDL_FreeSurface(surface);
		}
	}

	while (run) {
		doRender(renderer, heroes);
		SDL_Delay(10);
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}
	}

	quit();

	return 0;
}

void quit() {
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(heroes[i].icon);
	}

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_Quit();
}

void doRender(SDL_Renderer* renderer, player *heroes) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);
	SDL_Rect war_rect = { 40,40,50,60 };
	SDL_RenderCopy(renderer, heroes[0].icon, NULL, &war_rect);
	SDL_RenderPresent(renderer);
}

