#include "Cell.hpp"

Cell::Cell(SDL_Rect position, char letter, int owner) // Constructor
{
	this->position = position;
	this->letter = letter;
	this->owner = owner;
	this->seedAmount = (letter == '\0') ? 0 : NUM_SEEDS / NUM_HOLES; // La cantidad de semillas depende del numero total de semillas (NUM_SEEDS), el numero de celdas (NUM_HOLES) y si es una celda o un mancala
}

SDL_Rect Cell::getPosition() // Getter para position
{
	return this->position;
}

int Cell::getOwner() // Getter para owner
{
	return this->owner;
}

int Cell::getLetter() // Getter para letter
{
	return this->letter;
}

int Cell::getSeedAmount() // Getter para seedAmount
{
	return this->seedAmount;
}

void Cell::increaseSeedAmount(int amount) // Incrementar seedAmount
{
	this->seedAmount += amount;
}

void Cell::decreaseSeedAmount() // Decrementar seedAmount
{
	this->seedAmount--;
}

void Cell::resetSeedAmout() // Poner seedAmount en 0
{
	this->seedAmount = 0;
}