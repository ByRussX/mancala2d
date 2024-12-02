#pragma once
#include "Main.hpp"

class Cell
{
	public:
		Cell(SDL_Rect, char, int); 			// Constructor
		SDL_Rect getPosition();				// Getter para position
		int getOwner();						// Getter para owner
		int getLetter();					// Getter para letter
		int getSeedAmount();				// Getter para seedAmount
		void increaseSeedAmount(int = 1);	// Incrementar seedAmount
		void decreaseSeedAmount();			// Decrementar seedAmount
		void resetSeedAmout();				// Poner seedAmount en 0
	
	private:
		SDL_Rect position;					// Posicion de la celda
		int owner;							// Jugador que posee la celda
		char letter;						// Letra de la celda (A-L, \0 para mancala)
		int seedAmount;						// Cantidad de semillas en la celda
};