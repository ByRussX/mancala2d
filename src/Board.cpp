#include "Board.hpp"

Board::Board(SDL_Renderer *renderer) // Constructor
{
	this->renderer = renderer;
	
	// Cargar texturas desde los archivos fuente
	this->cellTexture = loadTexture("res/cell.png");
	this->mancalaTexture = loadTexture("res/mancala.png");
	this->cellTextureSelected = loadTexture("res/cell_selected.png");
	this->mancalaTextureSelected = loadTexture("res/mancala_selected.png");
	this->seedTexture = loadTexture("res/seed.png");
	
	this->turn = 0;
	this->win = false;
	
	initializeCells();
	
}

bool Board::checkWin() // Getter para win
{
	return this->win;
}

SDL_Texture *Board::loadTexture(const char *filePath) // Mapear una textura desde un archivo fuente
{
	SDL_Surface *tmpSurface = IMG_Load(filePath);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	
	return texture;
}

int Board::cellIndexRetriever(char letter) // Devuelve el indice de una celda en el contenedor dependiendo de su letra
{
	for (int i = 0;(unsigned) i < this->cells.size(); i++) // Iterar por todo el contenedor
	{
		if (this->cells.at(i).getLetter() == letter) // Cuando se encuentra una coincidencia devolver el indice
		{
			return i;
		}
	}
	
	return -1;
}

void Board::initializeCells() // Inicializar las celdas
{
	char letters[] = "ABCDEFGHIJKL";
	int counter = 0;
	
	// Inicializar las celdas
	for (int i = 1; i >= 0; i--) // Bucle for para el eje y; primero la hilera de abajo y luego la de arriba
	{
		for (int j = (i == 1 ? 0 : 5); (i == 1 ? j <= 5 : j >= 0); j += (i == 1 ? 1 : -1)) 
		/* 
		Bucle for avanzado anidado usando condicionales ternarios para el eje X
		Cuando la i del bucle padre vale 1 (la fila inferior), el bucle itera de derecha a izquierda, pero cuando vale 0 (la fila superior), itera de izquierda a derecha 
		El objectivo con esto es que las celdas se creen en sentido antihorario empezando en la inferior derecha para asignarles su letra correcta
		*/
		{
			SDL_Rect position;
			position.x = OFFSET * 2 + CELL_SIZE + (CELL_SIZE + OFFSET) * j; // Calcular la posicion de la X del extermo derecho superior de la celda en la ventana
			position.y = OFFSET + (CELL_SIZE + OFFSET) * i; // Calcular la posicion de la Y del extremo derecho superior de la celda en la ventana
			position.w = CELL_SIZE; // Anchura de la textura de la celda
			position.h = CELL_SIZE; // Altura de la textura de la celda
			
			Cell cell(position, letters[counter], i == 0); // Crear un objecto cell con esta informacion
			this->cells.push_back(cell); // Añadir el objecto al contenedor
			
			counter++;
		}
	}
	
	// Lo mismo que antes pero para los mancalas
	for (int i = 0; i < 2; i++)
	{
		SDL_Rect position;
		position.x = OFFSET + (CELL_SIZE + OFFSET) * 7 * i;
		position.y = OFFSET;
		position.w = CELL_SIZE;
		position.h = CELL_SIZE * 2 + OFFSET;
		
		Cell mancala(position, '\0', i == 0);
		this->mancalas.push_back(mancala);
	}
}

void Board::renderNumber(int number, SDL_Rect position, SDL_Color color) // Renderizar el numero de semillas en un punto
{
	// Valores de la fuente
	int width = 72;
	int height = 67;
	TTF_Font *font = TTF_OpenFont("res/font.ttf", 72);
	
	// Se debe proveer la posicion central del texto a renderizar, a partir del cual se calcula el punto superior derecho en funcion de la anchura y altura del texto
	position.x -= width / 2;
	position.y -= height / 2;
	position.w = width;
	position.h = height;
	
	std::string numberText = std::to_string(number); // Convertir texto a numero
	numberText = (number / 10 >= 1) ? numberText : "0" + numberText; // Siempre debe tener dos digitos
	
	SDL_Surface *surfaceText = TTF_RenderText_Solid(font, numberText.c_str(), color);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(this->renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	
	SDL_RenderCopy(this->renderer, textTexture, NULL, &position); // Añadir al buffer
}

void Board::renderText(std::string text, SDL_Rect position, SDL_Color color) // Renderizar texto en un punto
{
	int width, height;
	TTF_Font *font = TTF_OpenFont("res/font.ttf", 72);
	TTF_SizeText(font, text.c_str(), &width, &height); // Obtener anchura y altura de la fuente
	
	position.x -= width / 2;
	position.y -= height / 2;
	position.w = width;
	position.h = height;
	
	SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(this->renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	
	SDL_RenderCopy(this->renderer, textTexture, NULL, &position);
}

void Board::renderCells() // Renderizar celdas
{
	// Todas las texturas a renderizar van a ser del mismo tamaño
	SDL_Rect origin;
	origin.x = 0;
	origin.y = 0;
	origin.w = CELL_SIZE;
	origin.h = CELL_SIZE;
	
	// Renderizar las celdas normales
	
	for (Cell cell : this->cells)
	{
		SDL_Texture *texture = (cell.getOwner() == this->turn % 2 && !this->win) ? this->cellTextureSelected : this->cellTexture; // Ver si la textura esta seleccionada (el turno actual es el del dueño) o no
		SDL_Rect position = cell.getPosition(); // Obtener la posicion destino en la ventana
		SDL_RenderCopy(this->renderer, texture, &origin, &position);
	}
	
	// Renderizar los mancalas
	
	origin.h = CELL_SIZE * 2 + OFFSET;
	
	for (Cell mancala : this->mancalas)
	{
		SDL_Texture *texture = (mancala.getOwner() == this->turn % 2 && !this->win) ? this->mancalaTextureSelected : this->mancalaTexture; // Ver si la textura esta seleccionada (el turno actual es el del dueño) o no
		SDL_Rect position = mancala.getPosition(); // Obtener la posicion destino en la ventana
		SDL_RenderCopy(this->renderer, texture, &origin, &position);
	}
	
	renderSeeds();
}

void Board::renderSeeds() // Renderizar semillas
{
	SDL_Rect origin;
	SDL_Rect position;
	
	// Todas las texturas a renderizar van a ser del mismo tamaño
	origin.x = 0;
	origin.y = 0;
	origin.w = BALL_SIZE;
	origin.h = BALL_SIZE;
	
	// Render balls in normal cells
	
	if (!this->win) // Si no se ha ganado
	{
		for (Cell cell : this->cells) // Iterar por todas las celdas
		{
			// Establecer el color en funcion del jugador
			if (cell.getOwner() == 0)
				SDL_SetTextureColorMod(this->seedTexture, 0, 128, 255); // Azul
			else
				SDL_SetTextureColorMod(this->seedTexture, 255, 128, 0); // Naranja
		
			switch (cell.getSeedAmount()) 
			/* 
			Usamos el switch para ejecutar un codigo u otro en funcion del numero de semillas que haya en la celda
			Todos los casos hacen lo mismo: usar de forma ingeniosa un numero de bucles for para renderizar todas las semillas
			dentro de la celda calculando su posicion en funcion de cuantas haya
			*/
			{
				case 0:
					break;
				
				case 1: // Centrarla
					position = cell.getPosition();
					position.w = BALL_SIZE;
					position.h = BALL_SIZE;
					
					position.x += CELL_SIZE / 2 - BALL_SIZE / 2;
					position.y += CELL_SIZE / 2 - BALL_SIZE / 2;
					
					SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
					
					break;
				
				case 2: // Colocarlas horizontalmente a la misma distancia del centro con simetria en el eje x
					
					for (int i = 0; i < 2; i++)
					{
						position = cell.getPosition();
						position.w = BALL_SIZE;
						position.h = BALL_SIZE;
						
						position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * i;
						position.y += CELL_SIZE / 2 - BALL_SIZE / 2;
						
						SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						
					}
					
					break;
				
				case 3: // Usar transformaciones lineales para colocar a las tres con un angulo de 120º entre ellas
					
					for (int i = 0; i < 3; i++)
					{
						position = cell.getPosition();
						position.w = BALL_SIZE;
						position.h = BALL_SIZE;
						
						// Esto ha sido matador calcularlo con trigonometria
						position.x += CELL_SIZE / 4 - (CELL_SIZE / 4 * cos((90 + 120 * i) * PI / 180) - BALL_SIZE);
						position.y += CELL_SIZE / 4 - (CELL_SIZE / 4 * sin((90 + 120 * i) * PI / 180) - BALL_SIZE);
						
						SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						
					}
					
					break;
				
				case 4: // Colocarlas horizontalmente y verticalmente a la misma distancia del centro con simetria en ambos ejes
					
					for (int i = 0; i < 2; i++)
					{
						for (int j = 0; j < 2; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
						
					}
					
					break;
				
				case 5: // Como con 4 pero añadiendo una en el centro
					
					for (int i = 0; i < 2; i++)
					{
						for (int j = 0; j < 2; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
					}
					
					position = cell.getPosition();
					position.w = BALL_SIZE;
					position.h = BALL_SIZE;
					
					position.x += CELL_SIZE / 2 - BALL_SIZE / 2;
					position.y += CELL_SIZE / 2 - BALL_SIZE / 2;
					
					SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
					
					break;
				
				case 6: // Como con 4 pero añadiendo dos semillas a la misma altura que las de las hileras ya calculadas
					
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 2; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 4 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
					}
					
					break;
				
				case 7: // Como con 6 pero añadiendo una en el centro
					
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 2; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 4 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
					}
					
					position = cell.getPosition();
					position.w = BALL_SIZE;
					position.h = BALL_SIZE;
					
					position.x += CELL_SIZE / 2 - BALL_SIZE / 2;
					position.y += CELL_SIZE / 2 - BALL_SIZE / 2;
					
					SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
					
					break;
				
				case 8: // Como con 6 pero añadiendo dos alineadas en el eje Y con las ultimas dos de cada hilera
					
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 2; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 4 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
					}
					
					for (int i = 0; i < 2; i++)
					{
						position = cell.getPosition();
						position.w = BALL_SIZE;
						position.h = BALL_SIZE;
						
						position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 2 * i;
						position.y += CELL_SIZE / 2 - BALL_SIZE / 2;
						
						SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						
					}
					
					break;
				
				case 9: // Como con 6 pero añadiendo otra hilera en el medio, de tal manera que sea un bloque de 3x3 semillas centrado y simetrico en ambos ejes
					
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							position = cell.getPosition();
							position.w = BALL_SIZE;
							position.h = BALL_SIZE;
							
							position.x += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 4 * i;
							position.y += CELL_SIZE / 4 - BALL_SIZE / 2 + CELL_SIZE / 4 * j;
							
							SDL_RenderCopy(this->renderer, this->seedTexture, &origin, &position);
						}
					}
					
					break;
				
				default: // En caso de que haya mas, renderizaremos el numero de semillas, dado que no caben mas en la celda
					position = cell.getPosition();
			
					position.x += CELL_SIZE/ 2;
					position.y += CELL_SIZE/ 2;
			
					SDL_Color blue = {255, 128, 0};
					SDL_Color orange = {0, 128, 255};
			
					SDL_Color color = (cell.getOwner() == 0) ? orange : blue; // Calcular el color
			
					renderNumber(cell.getSeedAmount(), position, color);
					
			}
		}
	}
	
	for (Cell mancala : this->mancalas) // Iterar por todos los mancalas
	{
		position = mancala.getPosition();
		
		position.x += CELL_SIZE / 2;
		position.y += EXTREME_CELL_LENGTH / 2;
		
		SDL_Color blue = {255, 128, 0};
		SDL_Color orange = {0, 128, 255};
		
		SDL_Color color = (mancala.getOwner() == 0) ? orange : blue;
		
		renderNumber(mancala.getSeedAmount(), position, color); // Aqui solo necesitamos renderizar el numero de semillas
	}
	
}

void Board::handleClick() // Manejar clicks
{
	int x, y;
	SDL_GetMouseState(&x, &y); // Obtener la posicion del click
	
	for (Cell cell : this->cells) // Iterar por todas las celdas
	{
		// Si se ha hecho click dentro de una celda, manejar el click
		if ((x >= cell.getPosition().x && x <= cell.getPosition().x + cell.getPosition().w) && (y >= cell.getPosition().y && y <= cell.getPosition().y + cell.getPosition().h) && cell.getOwner() == turn % 2)
			cellClicked(cell.getLetter());
	}
}

void Board::cellClicked(char letter) // Manejar el click de una celda
{
	// Nos referiremos a la celda en la que se ha hecho click como alfa
	
	std::vector<Cell *> cellsToModify; // Contenedor para guardar las celdas que vamos a modificar y poder iterar sobre ellas despues
	int cellIndex = cellIndexRetriever(letter); // Indice de alfa en el contenedor de celdas
	int startingBalls = this->cells.at(cellIndex).getSeedAmount(); // Numero de semillas en alfa
	bool anyFilled = false;
	bool repeat = false;
	
	if (startingBalls != 0) // Si el numero de semillas de alfa es mayor que cero
	{
		int counter = 1;
		for (int i = 0; i < startingBalls; i++) // Iterar tantas veces como semillas haya en alfa sobre las casillas siguientes
		{
			if ((cellIndex + counter) % 12 == 6 && this->turn % 2 == 0) // La casilla es la anterior al mancala derecho (evitamos un overflow)
			{
				cellsToModify.push_back(&this->mancalas.at(1)); // Añadir el mancala derecho a las celdas a modificar
				i++; // Es importante incrementar i porque el mancala no esta guardado en el contenedor de celdas, pero queremos que cuente como celda valida para guardar semillas
			}
			
			if ((cellIndex + counter) % 12 == 0 && this->turn % 2 == 1) // La casilla es la anterior al mancala izquierdo (evitamos un overflow)
			{
				cellsToModify.push_back(&this->mancalas.at(0)); // Añadir el mancala izquierdo a las celdas a modificar
				i++; // Es importante incrementar i porque el mancala no esta guardado en el contenedor de celdas, pero queremos que cuente como celda valida para guardar semillas
			}
			
			if (i < startingBalls) // Chequeamos por si acaso otra vez que no hemos superado el número de semillas de alfa (debido a nuestro incremento sorpresa en el manejo de los mancalas anterior)
			{
				cellsToModify.push_back(&this->cells.at((cellIndex + counter) % 12)); // Añadir la celda actual a las celdas a modificar
				
				/* Si queremos poner el numero de semillas maximo en 9
				if (this->cells.at((cellIndex + counter) % 12).getSeedAmount() >= 9) // Si al menos una casilla tiene 9 semillas
				anyFilled = true;
				*/
				
				counter++;
			}
		}
		
		if (!anyFilled) // No hay ninguna casilla con mas de 9 (solo sirve si queremos poner el numero de semillas maximo en 9)
		{
			// Llamaremos la celda sobre la que estemos iterando beta
			
			for (Cell * cell : cellsToModify) // Por cada celda a modificar
			{
				cell->increaseSeedAmount(); // Aumentar beta en una semillas
				this->cells.at(cellIndexRetriever(letter)).decreaseSeedAmount(); // Decrementar alfa en una semillas

				 // Si beta es la ultima celda del contenedor de celdas a modificar, debemos comprobar si no tenia ninguna semilla para la logica de captura
				if (cell == cellsToModify.back())
				{
					
					if (cell->getLetter() == '\0') // Si beta es un mancala, sumar un turno
					{
						repeat = true;
					}
					
					else if (cell->getSeedAmount() == 1 && cell->getOwner() == this->turn % 2) // Si la beta estaba vacia y nos pertenece
					{
						int newIndex = cellIndexRetriever(cell->getLetter()); // Obtener el indice de beta
						
						int oppositeCellIndex = 11 - newIndex; // Obtener el indice de la celda opuesta a beta (delta)
			
						// Si la celda opuesta no es nuestra y tiene alguna semilla
						if (this->cells.at(oppositeCellIndex).getOwner() != this->turn % 2 && this->cells.at(oppositeCellIndex).getSeedAmount() != 0)
						{
							this->mancalas.at(this->turn % 2 == 0).increaseSeedAmount(cells.at(oppositeCellIndex).getSeedAmount()); // Añadir a nuestro mancala el numero de semillas de delta
							this->cells.at(oppositeCellIndex).resetSeedAmout(); // Establecer en 0 el numero de semillas en delta
							
							this->mancalas.at(this->turn % 2 == 0).increaseSeedAmount(); // Añadir a nuestro mancala nuestra propia semilla en beta
							cell->resetSeedAmout(); // Establecer en 0 el numero de semillas en beta
						}
					}
				}
			}
			
		winCondition(); // Comprobar si se ha ganado
		
		
		if (!repeat)
		{
			this->turn += 1; // Incrementar el turno
		}
		
		}
	}
}

void Board::winCondition() // Determinar si hay victoria o no
{
	bool allEmpty = true;
	
	for (Cell cell : this->cells) // Iterar por cada celda
	{
		if ((cell.getOwner() == this->turn % 2) && cell.getSeedAmount() != 0) // Si hay al menos una celda perteneciente al jugador actual que no tiene 0 semillas no estan todas vacias
			allEmpty = false;
	}
	
	if (allEmpty) // Si estan todas vacias
	{
		for (Cell cell : this->cells) // Iterar por cada celda
		{
			if ((cell.getOwner() != this->turn % 2) && cell.getSeedAmount() != 0) // Si la celda es del otro jugador (el que no se ha quedado sin semillas en sus celdas)
			{
				this->mancalas.at(turn % 2 != 0).increaseSeedAmount(cell.getSeedAmount()); // Añadir al mancala del otro jugador las semillas en la celda
				cell.resetSeedAmout(); // Establecer en 0 el numero de semillas de la celda
			}
		}
		
		this->win = true; // Activar la victoria
	}

}

void Board::triggerWin() // Manejar victoria
{
	// Cantidad de semillas de los jugadores
	int southAmount = mancalas.at(1).getSeedAmount();
	int northAmount = mancalas.at(0).getSeedAmount();
	
	std::string text;
	SDL_Color color;
	
	// Determinar quien ha ganado
	if (southAmount > northAmount)
    {
        text = "Blue Wins!";
        color = {0, 128, 255};
    }
	
    else if (northAmount > southAmount)
    {
        text = "Orange Wins!";
        color = {255, 128, 0};
    }
	
    else // Empate
    {
        text = "It's a draw!";
        color = {255, 255, 255};
    }
	
	SDL_Rect position;
	position.x = SCREEN_WIDTH / 2;
	position.y = SCREEN_HEIGHT / 2;
	
	renderText(text, position, color); // Renderizar el mensaje de final de partida
}