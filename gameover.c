#include "gameover.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

void Gameover(SDL_Renderer* renderer, int score) {
    // Load the game over texture
    const SDL_Color TEXT_COLOR = { 255, 0, 0, 0 };
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    TTF_Font* font = TTF_OpenFont("Ubuntu-R.ttf", 34);
    SDL_Surface* gameover_surface = SDL_LoadBMP("gameover.bmp");
    if (!gameover_surface) {
        printf("Could not load game over image. SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    SDL_Texture* gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_surface);
    SDL_FreeSurface(gameover_surface);

    if (!gameover_texture) {
        printf("Could not create game over texture. SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Set the game over texture position
    SDL_Rect gameover_rect;
    gameover_rect.x = (1600 - gameover_surface->w) / 2;
    gameover_rect.y = (1200 - gameover_surface->h) / 2;
    gameover_rect.w = gameover_surface->w;
    gameover_rect.h = gameover_surface->h;
    
    // Render the score text
    char score_str[10];
    snprintf(score_str, 10, "score : %d", score);
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_str, TEXT_COLOR);
    SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    SDL_FreeSurface(score_surface);
    if (!score_texture) {
        printf("Could not create score texture. SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    // Set the score texture position
    SDL_Rect score_rect;
    score_rect.x = gameover_rect.x + (gameover_rect.w - score_surface->w) / 2;
    score_rect.y = gameover_rect.y + 400;
    score_rect.w = score_surface->w;
    score_rect.h = score_surface->h;
    
    // Render the game over texture and the score texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover_rect);
    SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
    SDL_RenderPresent(renderer);

    // Wait for user to close the window
    SDL_Event event;
	int quit = 0;
	while (!quit) {
	    // Poll for events
	    while (SDL_PollEvent(&event)) {
		switch (event.type) {
		    case SDL_QUIT:
		        quit = 1;
		        break;
		    case SDL_KEYDOWN:
		        if (event.key.keysym.sym == SDLK_SPACE) {
		            quit = 1;
		        }
		        break;
		}
	    }
	}
	
	FILE *f_handle;
    	f_handle = fopen("scores.txt", "a+");
    	if (f_handle == NULL) {
        	printf("Erreur d'ouverture du fichier !");
        	exit(-1);
    	}
    fprintf(f_handle, "%d\n",score );
    fclose(f_handle);
	
    SDL_DestroyTexture(gameover_texture);
    SDL_DestroyTexture(score_texture);
}

