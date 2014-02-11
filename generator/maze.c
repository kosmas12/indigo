#include "maze.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int mod(int x, int d)
{
	if(x < 0) return d + x;
	return x % d;
}

int maze_cell_isfree(Maze *maze, unsigned int x, unsigned int y)
{
	Cell *cell = maze_get_cell(maze, x, y);
	return !cell->up && !cell->down && !cell->left && !cell->right;
}

void maze_generate_cell(Maze *maze, Cell *cell)
{
	enum Direction paths[4];
	int directions;
	
	while(1)
	{
		directions = 0;
		if(maze->wrap_y || cell->y != 0) if(maze_cell_isfree(maze, cell->x, mod(cell->y-1, maze->height))) paths[directions++] = UP;
		if(maze->wrap_y || cell->y != maze->height-1) if(maze_cell_isfree(maze, cell->x, mod(cell->y+1, maze->height))) paths[directions++] = DOWN;
		if(maze->wrap_x || cell->x != 0) if(maze_cell_isfree(maze, mod(cell->x-1, maze->width), cell->y)) paths[directions++] = LEFT;
		if(maze->wrap_x || cell->x != maze->width-1) if(maze_cell_isfree(maze, mod(cell->x+1, maze->width), cell->y)) paths[directions++] = RIGHT;
		
		if(!directions) break;
		
		int index = rand() % directions;
		Cell *new_cell = NULL;
		switch(paths[index])
		{
			case UP:
			cell->up = 1;
			new_cell = maze_get_cell(maze, cell->x, mod(cell->y-1, maze->height));
			new_cell->down = 1;
			break;
			
			case DOWN:
			cell->down = 1;
			new_cell = maze_get_cell(maze, cell->x, mod(cell->y+1, maze->height));
			new_cell->up = 1;
			break;
			
			case LEFT:
			cell->left = 1;
			new_cell = maze_get_cell(maze, mod(cell->x-1, maze->width), cell->y);
			new_cell->right = 1;
			break;
			
			case RIGHT:
			cell->right = 1;
			new_cell = maze_get_cell(maze, mod(cell->x+1, maze->width), cell->y);
			new_cell->left = 1;
			break;
		}
		maze_generate_cell(maze, new_cell);
	}
}

Maze* maze_generate(unsigned int width, unsigned int height, int wrap_x, int wrap_y)
{
	Maze *maze = malloc(sizeof(Maze));
	maze->width = width;
	maze->height = height;
	maze->wrap_x = wrap_x;
	maze->wrap_y = wrap_y;
	
	maze->cells = calloc(width*height, sizeof(Cell));
	//Coordinate System:
	// + x->
	// y
	// |
	// \/
	//Write Positions
	int x, y;
	for(x=0; x<width; x++) for(y=0; y<height; y++)
	{
		Cell *cell = maze_get_cell(maze, x, y);
		cell->x = x;
		cell->y = y;
	}
	
	Cell *cell = maze_get_cell(maze, rand() % width, rand() % height);
	maze_generate_cell(maze, cell);
	
	return maze;
}

Cell* maze_get_cell(Maze *maze, unsigned int x, unsigned int y)
{
	return &maze->cells[y*maze->width+x];
}

void maze_free(Maze *maze)
{
	free(maze->cells);
	free(maze);
}

void maze_print(Maze *maze)
{
	char buf[(maze->width+1)*2];
	int x, y;
	
	strcpy(buf, "+");
	for(x=0; x<maze->width; x++)
	{
		strcat(buf, "-+");
	}
	puts(buf);
	
	for(y=0; y<maze->height; y++)
	{
		strcpy(buf, "|");
		for(x=0; x<maze->width; x++)
		{
			Cell *cell = maze_get_cell(maze, x, y);
			strcat(buf, " ");
			if(cell->right) strcat(buf, " ");
			else strcat(buf, "|");
		}
		puts(buf);
		
		strcpy(buf, "+");
		for(x=0; x<maze->width; x++)
		{
			Cell *cell = maze_get_cell(maze, x, y);
			if(cell->down) strcat(buf, " ");
			else strcat(buf, "-");
			strcat(buf, "+");
		}
		puts(buf);
	}
}
