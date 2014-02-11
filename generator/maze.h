#ifndef H_MAZE
#define H_MAZE

typedef struct
{
	unsigned up:1;
	unsigned down:1;
	unsigned left:1;
	unsigned right:1;
	unsigned int x;
	unsigned int y;
} Cell;

typedef struct
{
	unsigned int width, height;
	int wrap_x, wrap_y;
	Cell *cells;
} Maze;

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

Maze* maze_generate(unsigned int width, unsigned int height, int wrap_x, int wrap_y);
Cell* maze_get_cell(Maze *maze, unsigned int x, unsigned int y);
void maze_free(Maze *maze);
void maze_print(Maze *maze);

#endif //H_MAZE
