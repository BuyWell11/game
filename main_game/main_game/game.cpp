#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

#define SCREEN_WIDTH 1980
#define SCREEN_HEIGHT 1080
#define GRAVITY 0.65f

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

typedef struct
{
	float x;
	float y;
	float dx, dy;
	short life;
	char* name;
	int onLedge;

	int animFrame, facingLeft, slowingDown;
}Player;

typedef struct
{
	int x;
	int y;
}Fon;

typedef struct
{
	int x, y, w, h;
}Ledge;

typedef struct
{
	Player player;

	Fon fons;

	Ledge ledges[100];

	//images
	SDL_Texture* playerFrames[5];
	SDL_Texture* floor;
	SDL_Texture* label;
	SDL_Texture* ustup;
	SDL_Texture* fon;
	SDL_Texture* platform;
	SDL_Texture* box;
	int labelW, labelH;

	int time;
	int statusState;

	SDL_Renderer* renderer;

	TTF_Font* font;
}GameState;

void Init_status_lives(GameState* game)
{
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface* tmp = TTF_RenderText_Blended(game->font, "TheBestGame", white);
	game->labelW = tmp->w;
	game->labelH = tmp->h;
	game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);
}

void Draw_status_lives(GameState* game)
{
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

	SDL_RenderClear(game->renderer);

	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

	SDL_Rect textRect = { 815, 540-game->labelH, game->labelW, game->labelH };
	SDL_RenderCopy(game->renderer, game->label, NULL, &textRect);
}

void Shutdown_status_lives(GameState* game)
{
	SDL_DestroyTexture(game->label);
	game->label = NULL;
}

void LoadGame(GameState* game)
{
	SDL_Surface* surface = NULL;

	surface = IMG_Load("box.png");
	if (surface == NULL)
	{
		printf("Cannot find box.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->box = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("platform.png");
	if (surface == NULL)
	{
		printf("Cannot find platform.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->platform = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("fon.png");
	if (surface == NULL)
	{
		printf("Cannot find fon.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->fon = SDL_CreateTextureFromSurface(game->renderer, surface);
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

	surface = IMG_Load("ustup.png");
	game->ustup = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//load fonts
	game->font = TTF_OpenFont("game_font.ttf", 48);
	if (!game->font)
	{
		printf("Cannot find out font file!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->label = NULL;

	game->player.x = 20;
	game->player.y = 1000;
	game->player.dy = 0;
	game->player.dx = 0;
	game->player.onLedge = 0;
	game->player.animFrame = 0;
	game->player.facingLeft = 1;
	game->player.slowingDown = 0;
	game->statusState = STATUS_STATE_LIVES;

	game->fons.x = 0;
	game->fons.y = 0;

	Init_status_lives(game);

	game->time = 0;

	//init ledges
	for (int i = 0; i < 100; i++)
	{
		game->ledges[i].w = 256;
		game->ledges[i].h = 64;
		game->ledges[i].x = i * 256;
		game->ledges[i].y = 1080-64;
	}

	//left wall
	game->ledges[11].w = 1;
	game->ledges[11].h = 1080;
	game->ledges[11].x = 10;
	game->ledges[11].y = 0;

	//right wall
	game->ledges[12].w = 1;
	game->ledges[12].h = 1080;
	game->ledges[12].x = 1950;
	game->ledges[12].y = 0;
	
	//ustup
	game->ledges[15].w = 120;
	game->ledges[15].h = 180;
	game->ledges[15].x = 200;
	game->ledges[15].y = 845;

	game->ledges[16].w = 120;
	game->ledges[16].h = 180;
	game->ledges[16].x = 1660;
	game->ledges[16].y = 845;

	//platforms
	game->ledges[17].w = 140;
	game->ledges[17].h = 50;
	game->ledges[17].x = 430;
	game->ledges[17].y = 720;

	game->ledges[18].w = 140;
	game->ledges[18].h = 50;
	game->ledges[18].x = 230;
	game->ledges[18].y = 520;

	game->ledges[19].w = 140;
	game->ledges[19].h = 50;
	game->ledges[19].x = 660;
	game->ledges[19].y = 440;

	game->ledges[20].w = 140;
	game->ledges[20].h = 50;
	game->ledges[20].x = 1360;
	game->ledges[20].y = 720;

	game->ledges[21].w = 140;
	game->ledges[21].h = 50;
	game->ledges[21].x = 1160;
	game->ledges[21].y = 470;

	game->ledges[22].w = 140;
	game->ledges[22].h = 50;
	game->ledges[22].x = 1160;
	game->ledges[22].y = 470;

	game->ledges[22].w = 140;
	game->ledges[22].h = 50;
	game->ledges[22].x = 1680;
	game->ledges[22].y = 580;

	game->ledges[23].w = 140;
	game->ledges[23].h = 50;
	game->ledges[23].x = 925;
	game->ledges[23].y = 250;

	//boxes
	game->ledges[24].w = 55;
	game->ledges[24].h = 55;
	game->ledges[24].x = 665;
	game->ledges[24].y = 963;

	game->ledges[25].w = 55;
	game->ledges[25].h = 55;
	game->ledges[25].x = 720;
	game->ledges[25].y = 963;

	game->ledges[26].w = 55;
	game->ledges[26].h = 55;
	game->ledges[26].x = 692;
	game->ledges[26].y = 909;

	game->ledges[27].w = 55;
	game->ledges[27].h = 55;
	game->ledges[27].x = 1145;
	game->ledges[27].y = 963;

	game->ledges[28].w = 55;
	game->ledges[28].h = 55;
	game->ledges[28].x = 1200;
	game->ledges[28].y = 963;

	game->ledges[29].w = 55;
	game->ledges[29].h = 55;
	game->ledges[29].x = 1145;
	game->ledges[29].y = 908;

}

//useful utility function to see if two rectangles are colliding at all
int collided(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2+wt2) || (x2 > (x1 + wt1)) || (y1 + ht2)) || (y2 > (y1 + ht1))));
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

	//more jumping
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP])
	{
		game->player.dy -= 0.2f;
	}

	//walking
	if (state[SDL_SCANCODE_LEFT])
	{
		game->player.dx -= 3;
		if (game->player.dx < -10)
		{
			game->player.dx = -10;
		}
		game->player.facingLeft = 1;
		game->player.slowingDown = 0;
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		game->player.dx += 3;
		if (game->player.dx > 10)
		{
			game->player.dx = 10;
		}
		game->player.facingLeft = 0;
		game->player.slowingDown = 0;
	}
	else
	{
		game->player.animFrame = 0;
		game->player.dx *= 0.8f;
		game->player.slowingDown = 1;
		if (fabsf(game->player.dx) < 0.1f)
		{
			game->player.dx = 0;
		}
	}

	return done;
}

void DoRender(SDL_Renderer* renderer, GameState* game)
{
	if (game->statusState == STATUS_STATE_LIVES)
	{
		Draw_status_lives(game);
	}
	else if (game->statusState == STATUS_STATE_GAME)
	{
		//set the drawing color to blue
		SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);

		//clear the screen (to blue)
		SDL_RenderClear(renderer);

		//set the drawing color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_Rect fonRect = { game->fons.x, game->fons.y, 1960, 1080 };
		SDL_RenderCopy(renderer, game->fon, NULL, &fonRect);

		for (int i = 0; i < 15; i++)
		{
			SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopy(renderer, game->floor, NULL, &ledgeRect);
		}

		for (int i = 15; i < 17; i++)
		{
			SDL_Rect ustupRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopy(renderer, game->ustup, NULL, &ustupRect);
		}

		for (int i = 17; i < 24; i++)
		{
			SDL_Rect platformRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopy(renderer, game->platform, NULL, &platformRect);
		}

		for (int i = 24; i < 100; i++)
		{
			SDL_Rect boxRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopy(renderer, game->box, NULL, &boxRect);
		}

		//draw a rectangle at player's position
		SDL_Rect rect = { game->player.x, game->player.y, 100, 100 };
		SDL_RenderCopyEx(renderer, game->playerFrames[game->player.animFrame], NULL, &rect, 0, NULL, game->player.facingLeft);
	}

	//We are done draing, 'present' or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}

void process(GameState* game)
{
	//add time
	game->time++;

	if (game->time > 120)
	{
		Shutdown_status_lives(game);
		game->statusState = STATUS_STATE_GAME;
	}

	if (game->statusState == STATUS_STATE_GAME)
	{
		//player movement
		Player* player = &game->player;
		player->x += player->dx;
		player->y += player->dy;

		//if (player->dx != 0 && player->onLedge && !player->slowingDown)
		//{
		//	if (game->time % 2 == 0)
		//	{
		//		if (player->animFrame == 0)
		//		{
		//			player->animFrame = 1;
		//		}
		//		else if (player->animFrame == 1)
		//		{
		//			player->animFrame = 2;
		//		}
		//		else if (player->animFrame == 2)
		//		{
		//			player->animFrame == 3;
		//		}
		//		else if (player->animFrame == 3)
		//		{
		//			player->animFrame == 4;
		//		}
		//		else
		//		{
		//			player->animFrame == 0;
		//		}
		//	}
		//}

		player->dy += GRAVITY;
	}
	
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

		if (px + pw / 2 > fx && px + pw / 2 < fx + fw)
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
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("TheBestGame",
								SDL_WINDOWPOS_UNDEFINED, // x
								SDL_WINDOWPOS_UNDEFINED, // y
								SCREEN_WIDTH, //������
								SCREEN_HEIGHT, //������
								SDL_WINDOW_SHOWN); //����� ����

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	game.renderer = renderer;

	//Initialize font system
	TTF_Init();

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

	/*ProcessEvents(window, renderer, done);*/

	//shutdown game and unload all memory
	SDL_DestroyTexture(game.playerFrames[0]);
	SDL_DestroyTexture(game.playerFrames[1]);
	SDL_DestroyTexture(game.playerFrames[2]);
	SDL_DestroyTexture(game.playerFrames[3]);
	SDL_DestroyTexture(game.playerFrames[4]);
	SDL_DestroyTexture(game.floor);
	SDL_DestroyTexture(game.ustup);
	SDL_DestroyTexture(game.fon);
	SDL_DestroyTexture(game.platform);
	SDL_DestroyTexture(game.box);

	if (game.label != NULL)
	{
		SDL_DestroyTexture(game.label);
	}
	TTF_CloseFont(game.font);

	//close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	TTF_Quit();

	//Clean up
	SDL_Quit();

	return 0;
}