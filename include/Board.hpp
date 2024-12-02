#pragma once
#include "Main.hpp"
#include "Cell.hpp"

class Board
{
	public:
		Board(SDL_Renderer *);								// Constructor
		bool checkWin();									// Getter para win
		void renderCells();									// Renderizar celdas
		void handleClick();									// Manejar clicks
		void triggerWin();									// Manejar victoria
	
	private:
		SDL_Renderer *renderer;								// Renderizador
		
		SDL_Texture *cellTexture;							// Textura celdas
		SDL_Texture *mancalaTexture;						// Textura mancalas
		SDL_Texture *cellTextureSelected;					// Textura celdas seleccionadas
		SDL_Texture *mancalaTextureSelected;				// Textura mancalas seleccionados
		SDL_Texture *seedTexture;							// Textura semilla
		
		std::vector<Cell> cells;							// Contenedor celdas
		std::vector<Cell> mancalas;							// Contenedor mancalas
		
		int turn;											// Turno actual (0 o 1)
		bool win;											// Victoria? (true o false)
		
		SDL_Texture *loadTexture(const char *);				// Mapear una textura desde un archivo fuente
		int cellIndexRetriever(char);						// Devuelve el indice de una celda en el contenedor dependiendo de su letra
		void initializeCells();								// Inicializar las celdas
		void renderNumber(int, SDL_Rect, SDL_Color);		// Renderizar el numero de semillas en un punto
		void renderText(std::string, SDL_Rect, SDL_Color);	// Renderizar texto en un punto
		void renderSeeds();									// Renderizar semillas
		void cellClicked(char);								// Manejar el click de una celda
		void winCondition();								// Determinar si hay victoria o no
};