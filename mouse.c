# include "common.h"

static void get_pos_handle_boundry(int *x, int *y, char buf[3]);
static void save_screen(int x, int y, color_t tmp[M_ROW][M_COLLUM]);
static void write_back_screen(int x, int y, color_t tmp[M_ROW][M_COLLUM]);

unsigned int mouse_pixel[M_ROW][M_COLLUM] = 
{
	{B,T,T,T,T,T,T,T,T,T,T},
	{B,B,T,T,T,T,T,T,T,T,T},
	{B,X,B,T,T,T,T,T,T,T,T},
	{B,X,X,B,T,T,T,T,T,T,T},
	{B,X,X,X,B,T,T,T,T,T,T},
	{B,X,X,X,X,B,T,T,T,T,T},
	{B,X,X,X,X,X,B,T,T,T,T},
	{B,X,X,X,X,X,X,B,T,T,T},
	{B,X,X,X,X,X,X,X,B,T,T},
	{B,X,X,X,X,X,X,X,X,B,T},
	{B,X,X,X,B,B,B,B,B,B,B},
	{B,X,X,B,T,T,T,T,T,T,T},
	{B,X,B,T,T,T,T,T,T,T,T},
	{B,B,T,T,T,T,T,T,T,T,T},
	{B,T,T,T,T,T,T,T,T,T,T},
	{T,T,T,T,T,T,T,T,T,T,T},
};

int mouse(void)
{
	int fd;
	char buf[3];
	int x = 500, y = 365;
	int m, n;
	color_t tmp[M_ROW][M_COLLUM];

	fd = Open("/dev/input/mice", O_RDONLY);
	draw_mouse(x, y, tmp);

	while(1){
		Read(fd, buf, 3);
		write_back_screen(x, y, tmp);
		get_pos_handle_boundry(&x, &y, buf);

//		mouse_move_action(x, y);

		if(1 == (buf[0] & 1)){
			m = x;
			n = y;
			Read(fd, buf, 3);
			if(0 == (buf[0] & 1)){
				left_key_action(m, n);
			}
		}
		if(2 == (buf[0] & 2)){
			m = x;
			n = y;
			Read(fd, buf, 3);
			if(2 == (buf[0] & 2)){
				right_key_action(m, n);
			}
		}
		draw_mouse(x, y, tmp);
	}

	close(fd);
	return 0;
}


int draw_mouse(int x, int y, unsigned int tmp[M_ROW][M_COLLUM])
{
	int i, j;
	int row = M_ROW, collum = M_COLLUM;
	
	save_screen(x, y, tmp);

	if(y + M_ROW > fb.h){
		row = row - (y + M_ROW - fb.h);
	}
	if(x + M_COLLUM > fb.w){
		collum = collum - (x + M_COLLUM - fb.w);
	}

	for(j = 0; j < row; j++){
		for(i = 0; i < collum; i++){
			if(mouse_pixel[j][i] != T){
				*((color_t *)fb.fb_mem + (y+j)*fb.w + x +i) = mouse_pixel[j][i];
			}
		}
	}

	return 0;
}

static void save_screen(int x, int y, color_t tmp[M_ROW][M_COLLUM])
{
	int i, j;
	int row = M_ROW, collum = M_COLLUM;
	color_t tmp_color;

	if(y + M_ROW > fb.h){
		row = row - (y + M_ROW - fb.h);
	}
	if(x + M_COLLUM > fb.w){
		collum = collum - (x + M_COLLUM - fb.w);
	}

	for(j = 0; j < row; j++){
		for(i = 0; i < collum; i++){
			tmp_color = *((color_t *)fb.fb_mem + (y+j)*fb.w + x +i);
			tmp[j][i] = tmp_color;
		}
	}
}


static void write_back_screen(int x, int y, color_t tmp[M_ROW][M_COLLUM])
{
	int i, j;
	int row = M_ROW, collum = M_COLLUM;
	color_t tmp_color;

	if(y + M_ROW > fb.h){
		row = row - (y + M_ROW - fb.h);
	}
	if(x + M_COLLUM > fb.w){
		collum = collum - (x + M_COLLUM - fb.w);
	}

	for(j = 0; j < row; j++){
		for(i = 0; i < collum; i++){
			tmp_color = tmp[j][i];
			*((color_t *)fb.fb_mem + (y+j)*fb.w + x +i) = tmp_color;
		}
	}
}

static void get_pos_handle_boundry(int *x, int *y, char buf[3])
{

	*x += buf[1];
	*y += -buf[2];
	if(*x < 0){
		*x = 0;
	}
	if(*x >= fb.w - 1){
		*x = fb.w - 1;
	}
	if(*y < 0){
		*y = 0;
	}
	if(*y > fb.h - 1){
		*y = fb.h - 1;
	}
//	printf("x = %d, y = %d\n", *x, *y);
}

