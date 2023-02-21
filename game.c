
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <time.h>
#include "linkedlist.h"
#include "gameover.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define WIDTH 1600
#define HEIGHT 1200

const int TEXT_WIDTH = 200;
const int TEXT_HEIGHT = 50;

const SDL_Color TEXT_COLOR = { 255, 255, 255, 255 };
const SDL_Color BACKGROUND_COLOR = { 0, 0, 0, 255 };

#define true 1
#define false 0

int isRunning;

int main(int argc, char** argv) {

    SDL_Init(SDL_INIT_EVERYTHING);
    
    // Initialisation de SDL_ttf
    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL_ttf: %s", TTF_GetError());
        return 1;
    }
    
    SDL_Window* window_menu = SDL_CreateWindow(
        "Menu",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (window_menu == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window_menu, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    if (!renderer) {
        fprintf(stderr, "%s\n", SDL_GetError());
    }
    
    TTF_Font* font = TTF_OpenFont("Ubuntu-R.ttf", 24);
    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load font: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface* surface1 = TTF_RenderText_Solid(font, "'1' - Jouer", TEXT_COLOR);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, "'2' - scores", TEXT_COLOR);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Surface* surface3 = TTF_RenderText_Solid(font, "'3' - exit", TEXT_COLOR);
    SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    

    int startingx = WIDTH / 2;
    int startingy = HEIGHT / 2;

    Snake* snake = initializeSnake();

    insertBeginning(snake, startingx, startingy);
    insertEnd(snake, startingx + 25, startingy);
    insertBeginning(snake, startingx - 25, startingy);
    insertBeginning(snake, startingx - 50, startingy);
    insertBeginning(snake, startingx - 75, startingy);
    
    
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    
    int x1 = (WINDOW_WIDTH - TEXT_WIDTH) / 2;
    int y1 = (WINDOW_HEIGHT - 3 * TEXT_HEIGHT) / 2 - 100;
    int x2 = x1;
    int y2 = y1 + TEXT_HEIGHT + 50;
    int x3 = x1;
    int y3 = y1 + 2 * TEXT_HEIGHT + 100;
    
    SDL_Rect dstrect1 = { x1, y1, TEXT_WIDTH, TEXT_HEIGHT };
    SDL_RenderCopy(renderer, texture1, NULL, &dstrect1);
    SDL_Rect dstrect2 = { x2, y2, TEXT_WIDTH, TEXT_HEIGHT };
    SDL_RenderCopy(renderer, texture2, NULL, &dstrect2);
    SDL_Rect dstrect3 = { x3, y3, TEXT_WIDTH, TEXT_HEIGHT };
    SDL_RenderCopy(renderer, texture3, NULL, &dstrect3);
    SDL_RenderPresent(renderer);
    int score = 0;
    
    SDL_Event event;
    int running = 1;
    while (running)
    {
        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_1:
                            SDL_Log("Choix 1");
                            SDL_DestroyWindow(window_menu);
                            SDL_Window* window = SDL_CreateWindow(
			    "Snake",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WIDTH,
			    HEIGHT,
				0
			    );
			    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
			    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			    SDL_RenderClear(renderer);
			    SDL_RenderPresent(renderer);
			    if (!renderer) {
				fprintf(stderr, "%s\n", SDL_GetError());
			    }
			    insertBeginning(snake, startingx, startingy);
			    insertEnd(snake, startingx + 25, startingy);
			    insertBeginning(snake, startingx - 25, startingy);
			    insertBeginning(snake, startingx - 50, startingy);
			    insertBeginning(snake, startingx - 75, startingy);
			    
			    SDL_Rect food;
			    food.x = 100;
			    food.y = 100;
			    food.w = 25;
			    food.h = 25;
			    
			    SDL_RenderDrawRect(renderer, &food);
			    SDL_RenderFillRect(renderer, &food);
			    SDL_RenderPresent(renderer);

			    srand(time(0));
			    
			    isRunning = 1;
			    int done = false;
			    int isStarting = true;

			    int up = 0; int down = 0; int left = 0; int right = 0;

			    while (isRunning) {
				SDL_Event e;
				
				while (SDL_PollEvent(&e)) {

				    switch (e.type) {
					case SDL_QUIT:
					    isRunning = 0;
					    
					    break;

					case SDL_KEYDOWN:
					// case SDL_KEYUP:
					    switch (e.key.keysym.scancode) {
						case SDL_SCANCODE_UP:
						case SDL_SCANCODE_W:
						    if (down == 1) break;
						    up = 1;
						    down = 0;
						    left = 0;
						    right = 0;
						    isStarting = false;
						    break;
						case SDL_SCANCODE_DOWN:
						case SDL_SCANCODE_S:
						    if (up == 1) break;
						    up = 0;
						    down = 1;
						    left = 0;
						    right = 0;
						    isStarting = false;
						    break;
						case SDL_SCANCODE_LEFT:
						case SDL_SCANCODE_A:
						    if (right == 1 || isStarting == true) break;
						    up = 0;
						    down = 0;
						    left = 1;
						    right = 0;
						    break;
						case SDL_SCANCODE_RIGHT:
						case SDL_SCANCODE_D:
						    if (left == 1) break;
						    up = 0;
						    down = 0;
						    left = 0;
						    right = 1;
						    isStarting = false;
						    break;
						default: break;
					    }
					    break;

					default: break;
				    }
				}

				int headx = snake->tail->body.x;
				int heady = snake->tail->body.y;

				if (up && !down) {
				    insertEnd(snake, headx, heady - 25);

				    if (heady  <= 0) {
					SDL_Delay(1500);
					isRunning = false;
					Gameover(renderer, score);
					break;
				    }

				    deleteBeginning(snake);
				}

				if (!up && down) {
				    insertEnd(snake, headx, heady + 25);

				    if ((heady + 25) >= HEIGHT) {
					SDL_Delay(1500);
					isRunning = false;
					Gameover(renderer, score);
					break;
				    }

				    deleteBeginning(snake);
				}

				if (left && !right) {
				    insertEnd(snake, headx - 25, heady);

				    if (headx <= 0) {
					SDL_Delay(1500);
					isRunning = false;
					Gameover(renderer, score);
					break;
				    }

				    deleteBeginning(snake);
				}

				if (!left && right) {
				    insertEnd(snake, headx + 25, heady);

				    if ((headx + 25) >= WIDTH) {
					SDL_Delay(1500);
					isRunning = false;
					Gameover(renderer, score);
					break;
				    }

				    deleteBeginning(snake);
				}

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				SDL_RenderClear(renderer);

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
				SDL_RenderDrawRect(renderer, &food);
				SDL_RenderFillRect(renderer, &food);

				if (snake->tail->body.x == food.x && snake->tail->body.y == food.y) {
				    int foodx = rand() % 31;
				    int foody = rand() % 23;

				    food.x = foodx * 25;
				    food.y = foody * 25;

				    int tailx = snake->head->body.x;
				    int taily = snake->head->body.y;

				    insertBeginning(snake, tailx, taily);
				    
				    score++;
				}
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

				Node* temp = snake->head;

				while ((void *)temp->next != (void *)snake->tail) {
				    if (snake->tail->body.x == temp->body.x && snake->tail->body.y == temp->body.y) {
				    	done = 1;
					Gameover(renderer, score);
					
					break;
				    }
				    temp = (Node*)temp->next;
				}

				if (done == 1) break;

				temp = (Node*)snake->head;

				while (temp != NULL) {
				    SDL_RenderDrawRect(renderer, &temp->body);
				    SDL_RenderFillRect(renderer, &temp->body);
				    temp = (Node*)temp->next;
				}

				SDL_RenderPresent(renderer);
				
				SDL_Delay(70);
			    }	 
			    printf("score : %d\n", score);

			    SDL_Quit();

			    return 0;
						    break;
                        case SDLK_2:
                            SDL_Log("Choix 2");
                            FILE *fp;
			    char *buffer;
			    long file_size;

			    fp = fopen("scores.txt", "rb"); // open file for reading in binary mode

			    if (fp == NULL) {
				printf("Error opening file.\n");
				exit(1);
			    }

			    fseek(fp, 0, SEEK_END); // move file pointer to the end of the file
			    file_size = ftell(fp); // get the current position of the file pointer, which is the file size
			    fseek(fp, 0, SEEK_SET); // move file pointer back to the beginning of the file

			    buffer = (char *) malloc(file_size); // allocate memory for the buffer

			    if (buffer == NULL) {
				printf("Memory allocation error.\n");
				exit(1);
			    }

			    fread(buffer, file_size, 1, fp); // read the entire file into the buffer
			    fclose(fp); // close the file

			    // Do something with the buffer, e.g. print its contents
			    printf("%s\n", buffer);

			    free(buffer); // free the memory allocated for the buffer
                            return 0;
                        case SDLK_3:
                            SDL_Log("Choix 3");
                            return 0;
                    }
                    break;
                }
            }
        }
        
        
        SDL_DestroyTexture(texture1);
			    SDL_DestroyTexture(texture2);
			    SDL_DestroyTexture(texture3);
			    
			    SDL_DestroyRenderer(renderer);
			    SDL_DestroyWindow(window_menu);
			    TTF_CloseFont(font);
	return 0;
}
