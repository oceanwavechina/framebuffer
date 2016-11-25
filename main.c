#include "common.h"

/* globle vars */
info_t fb;
lnode *head = NULL;	
page_t page;
int flag_keyboard = 0;
int flag_font = -1;
int flag_kill_program = 0;

static void keyboard(void);
void sys_time(void);

int main(void)
{
	/* init framebuffer && freetype */							
 	init_fb("/dev/fb0");
	
	/* init globle var */
	h_zoom = fb.h/7;
	w_zoom = fb.w/7;
	h_space = h_zoom/7;
	w_space = w_zoom/7;

#if 1
	/* save jpg files to linklist */				
	head = search_jpeg_to_list("./pic/");
	page.save_top = -1;
	page.save_node[++page.save_top] = head;
	
	/* show the main page */
	clean_screen();
	windows_main(page.save_node[page.save_top]);

	/* create a thread for the mouse */
	pthread_t thrd_mouse;				
	pthread_t thrd_keyboard;
	pthread_t thrd_time;
	Pthread_create(&thrd_mouse, NULL, (void *)mouse, NULL);		
	Pthread_create(&thrd_keyboard, NULL, (void *)keyboard, NULL);
	Pthread_create(&thrd_time, NULL, (void *)sys_time, NULL);

	/* kill the program */
	while(1){
		if(flag_kill_program == 1){
			pthread_kill(thrd_mouse, SIGKILL);
			pthread_kill(thrd_keyboard, SIGKILL);
			pthread_kill(thrd_time, SIGKILL);
//			return 0;
		}
	}

	pthread_join(thrd_keyboard, NULL);
	pthread_join(thrd_mouse, NULL);
	pthread_join(thrd_time, NULL);
#endif

	return 0;
}

void sys_time(void)
{
	time_t ptime;
	char buf[50];

	while(1){
		if(flag_font != 3){
			init_ft("cour.ttf", 17);
			flag_font = 3;
		}
		if(page.flag_page == 1){
			time(&ptime);
			strcpy(buf, ctime(&ptime) + 11);
			buf[8] = '\0';
			display_string("■■■■■■■■", 5 *(w_zoom + w_space) + 3 * w_space, 35, 0x000000);
			display_string(buf, 5 *(w_zoom + w_space) + 3 * w_space, 35, 0xbfe111);
		}
		sleep(1);
	}
}

void keyboard(void)
{
	while(1){
		if('\n' == getchar()){
			flag_keyboard = 1;
		}
	}

	return ;
}
