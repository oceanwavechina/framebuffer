# ifndef __COMMON_H__
# define __COMMON_H__

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <time.h>
# include <string.h>
# include <errno.h>
# include <dirent.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <sys/time.h>
# include <sys/mman.h>
# include <linux/fb.h>
# include <signal.h>

# define MAX_EFFECT 6

/* micro of mouse */
# define T 0xeeeeee
# define B 0xffffff
# define X 0x000000
# define M_ROW 16
# define M_COLLUM 11

/* define my type  */
typedef unsigned int u32_t;
typedef unsigned int color_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef void *(func)(void *);

/* the framebuffer info */
typedef struct {
    int w;
    int h;
    int bpp;				  /* bits per pixel */
    u8_t *fb_mem;
}info_t;

/* node that save the pic-name */
typedef struct node{
	char filename[50];
	struct node * next;
}lnode;

/* page info */
typedef struct page_info{
	int flag_page;
	int n_pic_show;
	char *pic_show[50];
	lnode *save_node[50];	/* the page's first pic */
	int save_top;
}page_t;

/* location info */
typedef struct location_info{
	int x;
	int y;
}loc_info;


/* the globle var */
/* defined in main.c */
extern info_t fb;
extern lnode *head;	
extern page_t page;
extern int flag_keyboard;
extern int flag_font;
extern int flag_kill_program;

/* defined in show_main_windows.c */
extern int w_zoom;
extern int h_zoom;
extern int w_space;
extern int h_space;

/* defined in mouse_action.c */
extern int no_played;


/* ******************** fb.c ******************** */
extern int init_fb(const char *device_file);
extern int fb_pixel(int x, int y, u32_t color);
extern int fb_row(int x, int y, int len, color_t color);
extern int fb_collum(int x, int y, int len, color_t color);

/* ******************** jpeg.c ******************** */
extern u8_t *decode_jpeg (const char *filename, info_t *jpeg_inf);
extern u8_t * scale24(u8_t *buf24, info_t new_inf, info_t jpeg_inf);
extern u32_t * rgb24to32(u8_t *buf24, info_t jpeg_inf);
extern u16_t * rgb24to16(u8_t *buf24, info_t jpeg_inf);

/* ******************** display.c ******************** */
extern int display_jpeg(const char *filename, int effect);

/* ******************* effect.c *********************** */
extern int effect_from_top(info_t new_jpeg, u32_t *buf32);
extern int effect_from_left(info_t new_jpeg, u32_t *buf3);
extern int effect_from_middle(info_t new_jpeg, u32_t *buf32);
extern int effect_scroll(info_t new_jpeg, u32_t *buf32);
extern int effect_parts(info_t new_jpeg, u32_t *buf32);
extern int effect_random_line(info_t new_jpeg, u32_t *buf32);
extern int clean_screen(void);

/* ****************** search_jpeg.c ******************** */
extern lnode* search_jpeg_to_list(char *dir_name);

/* ************************ mouse.c *******************  */
extern int mouse(void);
extern int draw_mouse(int x, int y, color_t tmp[M_ROW][M_COLLUM]);

/* ***************** wrap_linux_api ************* */
extern int Open(const char *pathname, int flags);
extern int Read(int fd, char *buf, int size);
extern int Pthread_create(pthread_t *id, pthread_attr_t *attr, func *f, void *arg);

/* **************** show_main_windows.c *************** */
extern int windows_main(lnode *start);

/* ************** mouse_action.c ***************** */
extern int mouse_move_action(int x, int y);
extern int right_key_action(int x, int y);
extern int left_key_action(int x, int y);
extern int parse_main_page_pos(int x, int y);
extern int parse_menu_pos(int x, int y);

/* ************** font.c ****************** */
extern int init_ft (const char *file, int size);
extern int display_string (const char * buf, int x, int y, color_t color);

# endif
