#include <string.h>
#include <stdio.h>

#define BOARD_NAME_MAX_SIZE 5
#define REDUNDANT 13

void getboard(char * filename, char * boardname)
{
        int i;
	char *tmp;

        if(boardname == NULL || filename == NULL \
		|| strlen(filename) < BOARD_NAME_MAX_SIZE)
                return;

	tmp = boardname;
        for(i = 0; *(filename + REDUNDANT + i) != '.' \
			&& i < BOARD_NAME_MAX_SIZE; i++ ){
                *tmp++ = *(filename + REDUNDANT + i);
        }
}

void getboard1(char * filename, char * boardname)
{
        int i;
	char *tmp1, *tmp2;

        if(boardname == NULL || filename == NULL || strlen(filename) < 13)
                return;

	tmp1 = boardname;
	tmp2 = filename + REDUNDANT;

	while(*tmp2 != '.') {
		*tmp1++ = *tmp2++;
	}

        printf("board 1 name = %s.\n", boardname);
}

int main()
{
	char *filename = "ux400-module-10gibc.tar.gz";
	char board[BOARD_NAME_MAX_SIZE]= "";

        memset(board, 0, BOARD_NAME_MAX_SIZE);
	getboard(filename, board);

        printf("board 2 name = %s.\n", board);

}
