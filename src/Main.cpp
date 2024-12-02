#include "Main.hpp"
#include "Cell.hpp"
#include "Board.hpp"

int main(int argc, char ** argv)
{
	int frameStart, deltaTime = 0;
	bool quit = false;
	
	// Inicializar subsistemas de SDL y TTF
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
	// Crear ventana y renderizador
	SDL_Window *window = SDL_CreateWindow("MancalaDeliverable", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	
	// El objecto board contiene toda la informacion de la partida
	Board board(renderer);
	
	// Bucle principal
	while (!quit)
	{
		frameStart = SDL_GetTicks();
		
		// Manejar eventos
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: // Cerrar la ventana
					quit = true;
					break;
				
				case SDL_MOUSEBUTTONDOWN: // Hacer click
					if (event.button.button == SDL_BUTTON_LEFT && !board.checkWin()) // Si es el boton derecho y no se ha ganado manejar el click
						board.handleClick();
					break;
				
				default:
					break;
			}
		}
		
		
		// Renderizar
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 86, 45, 0, 255); // Renderizar el fondo
		
		board.renderCells(); // Renderizar cada celda y mancala
		
		if (board.checkWin()) // Si un jugador ha ganado
		{
			board.triggerWin();
		}
		
		SDL_RenderPresent(renderer); // Renderizar todo lo anteriormente añadido al buffer (no estabamos renderizando, solo añadiendo al buffer)
		
		deltaTime = SDL_GetTicks() - frameStart; // Calcular el delta time para que el juego no se ejecute muy rapido https://es.wikipedia.org/wiki/Sincronizaci%C3%B3n_delta
		
		if (1000 / 60 > deltaTime) // Si el bucle se esta ejecutando muy rapido
		{
			SDL_Delay(1000 / 60 - deltaTime); // Añadir un pequeño delay
		}
	}
	
	return 0;
}