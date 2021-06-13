#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include <SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRAVITY 0.35f

typedef struct
{
	float x;
	float y;
	float dx, dy;
	short life;
	char* name;
	int onLedge;
}Player;

typedef struct
{
	int x;
	int y;
}Bear;

typedef struct
{
	int x, y, w, h;
}Ledge;

typedef struct
{
	Player player;

	Bear bears[100];

	Ledge ledges[100];

	SDL_Texture* bear;
	SDL_Texture* playerFrames[5];
	SDL_Texture* floor;

	SDL_Renderer* renderer;
}GameState;

void LoadGame(GameState *game)
{
	SDL_Surface* surface = NULL;

	//load images and create rendering textures from them
	surface = IMG_Load("player.png");
	if (surface == NULL)
	{
		printf("Cannot find player.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->bear = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("idle.png");
	if (surface == NULL)
	{
		printf("Cannot find idle.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->playerFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("going1.png");
	if (surface == NULL)
	{
		printf("Cannot find going1.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->playerFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("going2.png");
	if (surface == NULL)
	{
		printf("Cannot find going2.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->playerFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("going3.png");
	if (surface == NULL)
	{
		printf("Cannot find going3.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->playerFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("going4.png");
	if (surface == NULL)
	{
		printf("Cannot find going4.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->playerFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("floor.png");
	game->floor = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	game->player.x = 220;
	game->player.y = 140;
	game->player.dy = 0;
	game->player.dx = 0;
	game->player.onLedge = 0;

	for (int i = 0; i < 100; ++i)
	{
		game->bears[i].x = i * 64;
		game->bears[i].y = i * 32;
	}

	//init ledges
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = 256;
		game->ledges[i].h = 64;
		game->ledges[i].x = i*256;
		game->ledges[i].y = 400;
	}
	game->ledges[99].x = 350;
	game->ledges[99].y = 200;
}

int ProcessEvents(SDL_Window* window, GameState *game)
{
	SDL_Event event;
	int done = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			case SDLK_UP:
				if (game->player.onLedge)
				{
					game->player.dy = -15;
					game->player.onLedge = 0;
				}
				break;
			}
		}
		break;
		case SDL_QUIT:
			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		game->player.dx -= 3;
		if (game->player.dx < -10)
		{
			game->player.dx = -10;
		}
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		game->player.dx += 3;
		if (game->player.dx > 10)
		{
			game->player.dx = 10;
		}
	}
	else
	{
		game->player.dx *= 0.8f;
		if (fabsf(game->player.dx) < 0.1f)
		{
			game->player.dx = 0;
		}
	}

	return done;
}

void DoRender(SDL_Renderer *renderer, GameState* game)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	//clear the screen (to blue)
	SDL_RenderClear(renderer);

	//set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 100; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
		SDL_RenderCopy(renderer, game->floor, NULL, &ledgeRect);
	}

	SDL_Rect rect = { game->player.x, game->player.y, 100, 100 };
	SDL_RenderCopy(renderer, game->playerFrames[0], NULL, &rect, 0, NULL, 0);
	/*SDL_RenderFillRect(renderer, &rect);*/

	//for (int i = 0; i < 100; i++)
	//{
	//	SDL_Rect bearRect = { game->bears[i].x, game->bears[i].y, 64, 64 };
	//	SDL_RenderCopy(renderer, game->bear, NULL, &bearRect);
	//}



	//We are done draing, 'present' or wshow to the screen what we've drawn
	SDL_RenderPresent(renderer);

}

void process(GameState* game)
{
	Player *player = &game->player;
	player->x += player->dx;
	player->y += player->dy;

	player->dy += GRAVITY;
}

void CollisionDetect(GameState* game)
{
	//Check for collision with any ledges
	for (int i = 0; i < 100; i++)
	{
		float pw = 80;
		float ph = 90;
		float px = game->player.x;
		float py = game->player.y;
		float fx = game->ledges[i].x;
		float fy = game->ledges[i].y;
		float fw = game->ledges[i].w;
		float fh = game->ledges[i].h;

		if (px + pw / 2 > fx && px + pw / 2 < px + pw)
		{
			//are we bumpibg our head?
			if (py < fy + fh && py > fy && game->player.dy < 0)
			{
				//correct y
				game->player.y = fy + fh;
				py = fy + ph;

				//bumped our head, stop any jump velocity
				game->player.dy = 0;
				game->player.onLedge = 1;
			}
		}

		if (px + pw > fx && px < fx + fw)
		{
			//we are landing on the ledge
			if (py + ph > fy && py < fy && game->player.dy > 0)
			{
				//correct y
				game->player.y = fy - ph;
				py = fy - ph;

				//landed on this ledge, stop any jump velocity
				game->player.dy = 0;
				game->player.onLedge = 1;
			}
		}

		if (py + ph > fy && py < fy + fh)
		{
			//rubbing agaisnt right edge
			if (px < fx + fw && px + pw > fx + fw && game->player.dx < 0)
			{
				//correct x
				game->player.x = fx + fw;
				px = fx + fw;

				game->player.dx = 0;
			}
			//rubbing against left edge
			else if (px + pw > fx && px < fx && game->player.dx > 0)
			{
				//correct x
				game->player.x = fx - pw;
				px = fx - pw;

				game->player.dx = 0;
			}
		}
	}
}

#undef main
int main(int argc, char* args[])
{
	GameState game;
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("TheBestGame",
								SDL_WINDOWPOS_UNDEFINED, // x
								SDL_WINDOWPOS_UNDEFINED, // y
								SCREEN_WIDTH, //ширина
								SCREEN_HEIGHT, //высота
								SDL_WINDOW_SHOWN); //Задаём окно

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	game.renderer = renderer;

	LoadGame(&game);

	int done = 0;

	while (!done)
	{
		//Check for events
		done = ProcessEvents(window, &game);

		process(&game);
		CollisionDetect(&game);

		//render display
		DoRender(renderer, &game);

		/*SDL_Delay(10);*/
	}

	ProcessEvents(window, renderer, done);

	SDL_DestroyTexture(game.bear);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return 0;
}