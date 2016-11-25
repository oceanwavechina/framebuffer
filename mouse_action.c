#include "common.h"

/* the pic number in the pic-frame (in page 1)*/
char pic_played[50];

/* the just played pic pos in pic-frame  */
int no_played = 0;

/* the menu area (in page 2)*/
char *save_top = NULL;				
char *save_bottom = NULL;			

/* top menu or bottom menu (in page 2)*/
int flag_menu = 0;				

/* the width of the menu (in page 2)*/
int width_menu;

static int draw_rightkey_menu(void);
static int left_key_action_page1(int x, int y);
static int left_key_action_page2(int x, int y);

int left_key_action(int x, int y)
{
	if(page.flag_page == 1){
		left_key_action_page1(x, y);
	}

	if(page.flag_page == 2)	{
		left_key_action_page2(x, y);
	}
#if 0	
	if(page.flag_page == 3){
		flag_keyboard = 1;
	}
#endif
	return 0;
}

int right_key_action(int x, int y)
{
	if(page.flag_page == 2){
		if(flag_menu == 1){
			return 0;	
		}
		draw_rightkey_menu();
		flag_menu = 1;
	}
	return 0;
}

static int draw_rightkey_menu(void)
{
	int i_menu, j_top, j_bottom;
	width_menu = fb.h / 20 + 10;

	/* save the area */
	save_top = malloc(fb.w * width_menu * 4);
	save_bottom = malloc(fb.w * width_menu * 4);
	memcpy(save_top, fb.fb_mem, fb.w * width_menu * 4);
	memcpy(save_bottom, (char *)((u32_t *)fb.fb_mem + fb.w * (fb.h - width_menu)), fb.w * width_menu * 4);

	/* menu background */
	for(j_top = 0, j_bottom = fb.h; j_top < width_menu; j_top++, j_bottom--){
		for(i_menu = 0; i_menu < fb.w; i_menu++){
			*((color_t *)fb.fb_mem + j_top * fb.w + i_menu)  |= 0x00000022;
		}		
		for(i_menu = 0; i_menu < fb.w; i_menu++){
			*((color_t *)fb.fb_mem + j_bottom * fb.w + i_menu)  |= 0x00000022;
		}	
		usleep(10000);
	}

	/* menu button  
	 * the space between button is [ w_space / 2 ]; 
	 * the button's width is [ fb.w / 20 ] , [ height is fb.h / 20 ]; 
	 */

	info_t jpeg, button_jpeg = {fb.h / 19, fb.h / 19, fb.bpp, fb.fb_mem};
	u8_t *buf24 = NULL, *scalebuf = NULL;  u32_t *buf32 = NULL;

	int i_fb, j_fb, i_pic, j_pic;
	int i = 6 * fb.w / 20 + w_space / 2;
	int j = 0;
	int i_ioc;

	for(i_ioc = 0; i_ioc < 6; i_ioc++){
		switch(i_ioc){
			case 0:	buf24 = decode_jpeg("21ioc.jpg", &jpeg);
					break;
			case 1:	buf24 = decode_jpeg("22ioc.jpg", &jpeg);
					break;
			case 2:	buf24 = decode_jpeg("23ioc.jpg", &jpeg);
					break;
			case 3:	buf24 = decode_jpeg("24ioc.jpg", &jpeg);
					break;
			case 4:	buf24 = decode_jpeg("31ioc.jpg", &jpeg);
					break;
			case 5:	buf24 = decode_jpeg("32ioc.jpg", &jpeg);
					break;;
			default:	
					break;
		}

		/* sacle the button and display */
		scalebuf = scale24(buf24, button_jpeg, jpeg);
		buf32 = rgb24to32(scalebuf, button_jpeg);
		for(j_fb = j, j_pic = 0; j_pic < fb.h / 20; j_fb++, j_pic++){
			for(i_fb = i, i_pic = 0; i_pic < fb.h / 20; i_fb++, i_pic++){
				fb_pixel(i_fb, j_fb, buf32[j_pic * button_jpeg.w + i_pic]);
			}
		}
#if 0
		/* save the menu button's location */
		loc_menu[menu_top++].x = i;
		loc_menu[menu_top++].y = j;
#endif

		/* move to next position */
		i += 2 * fb.w / 20;
		if(i_ioc == 3){
			i = 8 * fb.w / 20 + w_space / 2;
			j = fb.h - fb.h / 20;
		}

		/* destroy the used memory space */
		free(buf24); buf24 = NULL;
		free(scalebuf); scalebuf = NULL;
		free(buf32); buf32 = NULL;
	}

	return 0;
}

int left_key_action_page1(int x, int y)
{
	int pic_click;
	lnode *tmp = head;

	pic_click = parse_main_page_pos(x, y);
	if(pic_click == -1){
		return -1;
	}

	/* the picture frame */
	if(pic_click < page.n_pic_show){
		page.flag_page = 2;
		clean_screen();
		strcpy(pic_played, page.pic_show[pic_click]);
		no_played = pic_click;
		display_jpeg(page.pic_show[pic_click], -1);
	}else if(pic_click < 100){
		return -1;
	}

	/* the button frame */
	if(pic_click >= 100){
		switch(pic_click){

		/* page down button */
		case 104:	if(page.n_pic_show < 25){
						return -1;
					}
					clean_screen();
					windows_main(page.save_node[page.save_top]);
					break;

		/* page up button */
		case 103:	clean_screen();
					windows_main(page.save_node[--page.save_top]);
					break;

		/* play button */
		case 102:	page.flag_page = 3;
					clean_screen();
				//	while(tmp -> next != head){
					while(1){
						display_jpeg(tmp -> filename, -1);
						tmp = tmp -> next;
						sleep(1);	
						if(flag_keyboard == 1){
							flag_keyboard = 0;
							break;
						}
					}

					clean_screen();
					windows_main(page.save_node[--page.save_top]);
					page.flag_page = 1;
					break;

		/* exit button */
		case 101:	flag_kill_program = 1;
					break;

		default :	break;
		}
	}

	return 0;
}

int left_key_action_page2(int x, int y)
{
	info_t jpeg;
	info_t new_jpeg = fb;
	u8_t *buf24, *scalebuf;
	u32_t *buf32;
	color_t tmp_pixel;
	int x_s = w_space - 2, y_s = h_zoom - 2;

	int menu_pos = parse_menu_pos(x, y);
	if(page.flag_page == 2 && menu_pos == -1){
		return -1;
	}

	switch(menu_pos){

	/* quit button */			
	case 11:	clean_screen();
				if(page.n_pic_show < 25){
					windows_main(page.save_node[page.save_top]);
				}else{
					windows_main(page.save_node[--page.save_top]);
				}
					
				/* the pic chosen info and the grid */
				if(flag_font != 2){
					init_ft("SIMYOU.TTF", 20);
					flag_font = 2;
				}
				display_string(pic_played, 2 *(w_zoom + w_space), 30, 0xbfe111);
				if(no_played != 0){
					y_s = (no_played / 5) * (h_zoom + h_space) + h_zoom - 2;
					if(no_played % 5 != 0){
						x_s = (no_played % 5) * (w_zoom + w_space) + w_space - 2;
					}
				}
				fb_row(x_s, y_s, w_zoom + 4, 0xff0000);
				fb_row(x_s, y_s + h_zoom + 4, w_zoom + 4, 0xff0000);
				fb_collum(x_s, y_s, h_zoom + 4, 0xff0000);
				fb_collum(x_s + w_zoom + 4, y_s, h_zoom + 4, 0xff0000); 
						
				flag_menu = 0;
				page.flag_page = 1;
				break;
	
	/* remove menu button */
	case 12:	memcpy(fb.fb_mem, save_top, fb.w * width_menu * 4);
				memcpy((char *)((u32_t *)fb.fb_mem + fb.w * (fb.h - (width_menu))), save_bottom, fb.w * width_menu * 4);
				flag_menu = 0;
				free(save_top);
				free(save_bottom);
				break;

	/* zomm++ pic button */
	case 13:	buf24 = decode_jpeg(page.pic_show[no_played], &jpeg);
				scalebuf = scale24(buf24, new_jpeg, jpeg);
				buf32 = rgb24to32(scalebuf, new_jpeg);
				memcpy(fb.fb_mem, buf32, fb.w * fb.h * 4);
				flag_menu = 0;
			//	free(save_top);
			//	free(save_bottom);
				free(buf24);
				free(scalebuf);
				free(buf32);
				break;

	/* reverse pic button */			
	case 14:	memcpy(fb.fb_mem, save_top, fb.w * width_menu * 4);
				memcpy((char *)((u32_t *)fb.fb_mem + fb.w * (fb.h - (width_menu))), save_bottom, fb.w * width_menu * 4);
				free(save_top);
				free(save_bottom);

				flag_menu = 0;				
				int i = 1, j = fb.w * fb.h - 1;

				while(i != j){
					tmp_pixel = *((color_t *)fb.fb_mem + i);
					*((color_t *)fb.fb_mem + i) = *((color_t *)fb.fb_mem + j);
					*((color_t *)fb.fb_mem + j) = tmp_pixel;
					i++;
					j--;
				}
				break;
			

	/* the bottom menu */
	/* pre-pic button */			
	case 21:	flag_menu = 0;
				if(--no_played > -1){
					clean_screen();
					display_jpeg(page.pic_show[no_played], -1);				
				}else{
					clean_screen();
					no_played = 0;
					display_jpeg(page.pic_show[no_played], -1);				
				}
				break;

	/* next-pic button */
	case 22:	flag_menu = 0;
				if(++no_played < page.n_pic_show){
					clean_screen();
					display_jpeg(page.pic_show[no_played], -1);				
				}else{
					clean_screen();
					no_played = page.n_pic_show - 1;
					display_jpeg(page.pic_show[no_played], -1);				
				}
				break;
				
	default :	break;
	}

	return 0;
}


int parse_main_page_pos(int x, int y)
{
	int i, j, i_mod, j_mod;
	int pos;

	i = x / (w_zoom + w_space);
	i_mod = x % (w_zoom + w_space);
	j = (y-h_zoom + h_space) / (h_zoom + h_space);
	j_mod = (y-h_zoom + h_space) % (h_zoom + h_space);

	/* the pic-frame area */
	if(i < 5){
		if(j_mod < h_space || i_mod < w_space){
			return -1;
		}
		pos = j * 5 + i;
		return pos;
	}

	/* the button-frame area */
	if(i == 5){
		if(i_mod < w_space || j <2){
			return -1;
		}else {

			if(j == 3 && i_mod < (w_space + fb.w / 18)){
				pos = 101;
				return pos;
			}else if(j == 3){
				pos = 102;
				return pos;		
			}

			if(j == 4 && i_mod < (w_space + fb.w / 18)){
				pos = 103;
				return pos;
			}else if(j == 4){
				pos = 104;
				return pos;
			}
		}
	}

	return -1;
}

int parse_menu_pos(int x, int y)
{
	/* the top menu */
	if(y <= width_menu && flag_menu == 1){
		if(x > 12 *fb.w / 20 && x < 13 *fb.w / 20){
			return 11;
		}
		if(x > 10 * fb.w / 20 && x < 11 * fb.w / 20){
			return 12;

		}
		if(x > 8 * fb.w / 20 && x < 9 * fb.w / 20){	
			return 13;
		}
		if(x > 6 * fb.w / 20 && x < 8 * fb.w / 20){
			return 14;

		}
	}
				
	/* the bottom menu */
	if(y >= fb.h - fb.h / 20 && flag_menu == 1){
		if(x > 8 * fb.w / 20 && x < 9 *fb.w / 20){
			return 21;
		}	
		if(x > 10 * fb.w / 20 && x < 11 *fb.w / 20){
			return 22;
		}
	}

	return -1;
}

