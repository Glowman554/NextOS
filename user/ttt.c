#include <syslib.h>

char field[3][3] = {
	{' ',' ',' '},
	{' ',' ',' '},
	{' ',' ',' '}
};

void draw(char f[3][3]){
	clrscr();
	kprintf("%c|%c|%c\n", f[0][0], f[0][1], f[0][2]);
	kprintf("-----\n");
	kprintf("%c|%c|%c\n", f[1][0], f[1][1], f[1][2]);
	kprintf("-----\n");
	kprintf("%c|%c|%c\n", f[2][0], f[2][1], f[2][2]);
}

void computer_move(){
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			if(field[i][j] == ' ') break;
		}
		if(field[i][j] == ' ') break;
	}
	
	if(i * j == 9){
		kprintf("draw\n");
		exec("/terminal.bin");
		task_exit(0);
	} else {
		field[i][j] = 'X';
	}
}

void check(){
	for(int i; i < 3; i++){
		if(field[i][0] == 'O' && field[i][1] == 'O' && field[i][2] == 'O'){
			draw(field);
			kprintf("you win\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][i] == 'O' && field[1][i] == 'O' && field[2][i] == 'O'){
			draw(field);
			kprintf("you win\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[i][0] == 'X' && field[i][1] == 'X' && field[i][2] == 'X'){
			draw(field);
			kprintf("computer wins\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][i] == 'X' && field[1][i] == 'X' && field[2][i] == 'X'){
			draw(field);
			kprintf("computer wins\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][0] == 'O' && field[1][1] == 'O' && field[2][2] == 'O'){
			draw(field);
			kprintf("you win\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][0] == 'X' && field[1][1] == 'X' && field[2][2] == 'X'){
			draw(field);
			kprintf("computer wins\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][2] == 'O' && field[1][1] == 'O' && field[2][0] == 'O'){
			draw(field);
			kprintf("you win\n");
			exec("/terminal.bin");
			task_exit(0);
		}
		if(field[0][2] == 'X' && field[1][1] == 'X' && field[2][0] == 'X'){
			draw(field);
			kprintf("computer wins\n");
			exec("/terminal.bin");
			task_exit(0);
		}
	}
}

void input(){
	int x, y;
	kprintf("its your turn\n");
	kprintf("first type in your row\n");
	x = getchar() -48;
	reset_timer_tick();
	while(get_timer_tick() < 10);
	kprintf("and now your collum\n");
	y = getchar() -48;
	reset_timer_tick();
	while(get_timer_tick() < 10);
	x--;
	y--;
	//kprintf("x %d, y %d\n", x, y);
	if(field[x][y] == 'X' || field[x][y] == 'O'){
		
		kprintf("already taken\n");
		input();
	} else {
		field[x][y] = 'O';
		check();
		computer_move();
		check();
		draw(field);
		input();
	}
}

void _start(){
	draw(field);
	input();
}