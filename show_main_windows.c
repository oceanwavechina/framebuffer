# include "common.h"

/* the size of zoom-pic && the space between the zoom-pic */
int w_zoom;
int h_zoom;
int w_space;
int h_space;

static int draw_background(void);
static int draw_button(void);
static int display_zoom_pic(lnode *start);

int windows_main(lnode *start)
{
	/* jpeg info */

	draw_background();

	draw_button();

	display_zoom_pic(start);

	page.flag_page = 1;

	return 0;
}


static int draw_background(void)
{
	int i = w_space, j = h_zoom / 2;
	int tmp;

//	memset(fb.fb_mem, 0x000000, fb.w * fb.h * 4);

	/* the line */
	fb_row(i, j, fb.w - (2 * w_space + w_zoom), 0xcccccc);
	fb_row(i, j + 1, fb.w - (2 * w_space + w_zoom), 0xcccccc);

	/* the grid around the string */
	i = 5 * (w_space + w_zoom) + 2 * w_space + w_space / 3;
	j = (2 * h_space + h_zoom) + h_space / 3;

	fb_row(i - 12, j - 12, 150 + 24, 0x181821);
	fb_row(i - 12, j + 12 + 150, 150 + 24, 0x181821);
	fb_collum(i - 12, j - 12, 150 + 24, 0x181821);
	fb_collum(i + 12 + 150, j - 12, 150 + 24, 0x181821);

	for(tmp = j; tmp - j < 150; tmp++){
		fb_row(i, tmp, 150, 0x181821);
	}
	
	/* the title */
	if(flag_font != 0){
		init_ft("simhei.ttf", 40);
		flag_font = 0;
	
		display_string("图片", 5 *(w_zoom + w_space) + (w_zoom + w_space) / 2, w_zoom + w_space, 0xbfe111);
		display_string("浏览", 5 *(w_zoom + w_space) + (w_zoom + w_space) / 2, w_zoom + w_space	 + 55, 0xbfe111);
	}
	return 0;
}
static int draw_button(void)
{
	/* 
	 * the space between button is [ w_space / 2 ]; 
	 * the button's width is fb.w / 18 , length is fb.h / 18; 
	 *
	 */

	info_t jpeg, button_jpeg = {fb.w / 18, fb.h / 18, fb.bpp, fb.fb_mem};
	u8_t *buf24 = NULL, *scalebuf = NULL;  u32_t *buf32 = NULL;

	int i_fb, j_fb, i_pic, j_pic;
	int i = 5 * (w_space + w_zoom) + 2 * w_space;
	int j = 4 * (h_zoom+h_space) + 2 * h_space;
	int i_ioc;

//	i += fb.w / 18 + w_space / 2;
	for(i_ioc = 0; i_ioc < 4; i_ioc++){	
		if(i_ioc == 0){
			buf24 = decode_jpeg("4ioc.jpg", &jpeg);
		}
		if(i_ioc == 1){
			buf24 = decode_jpeg("1ioc.jpg", &jpeg);
		}
		if(i_ioc == 2){
			buf24 = decode_jpeg("2ioc.jpg", &jpeg);
		}	
		if(i_ioc == 3){
			buf24 = decode_jpeg("3ioc.jpg", &jpeg);
		}

		/* sacle the button and display */
		scalebuf = scale24(buf24, button_jpeg, jpeg);
		buf32 = rgb24to32(scalebuf, button_jpeg);
		for(j_fb = j, j_pic = 0; j_pic < fb.h / 18; j_fb++, j_pic++){
			for(i_fb = i, i_pic = 0; i_pic < fb.w / 18; i_fb++, i_pic++){
				fb_pixel(i_fb, j_fb, buf32[j_pic * button_jpeg.w + i_pic]);
			}
		}

		/* move to next position */
		if(i_ioc == 0){
			i += fb.w / 18 + w_space / 2;
			j -= h_space + h_zoom;
		}
		if(i_ioc == 1){
			i -= fb.w / 18 + w_space / 2;
			j -= 2 * h_space;
		}
		if(i_ioc == 2){
			i += fb.w / 18 + w_space / 2;
			j -= h_space + h_zoom;	
		}
		j += h_space + h_zoom;

		/* destroy the used memory space */
		free(buf24); buf24 = NULL;
		free(scalebuf); scalebuf = NULL;
		free(buf32); buf32 = NULL;
	}

	return 0;
}

static int display_zoom_pic(lnode *start)
{

	info_t jpeg, zoom_jpeg = {w_zoom, h_zoom, fb.bpp, fb.fb_mem};
	u8_t *buf24 = NULL, *scalebuf = NULL;  u32_t *buf32 = NULL;
	int j_fb, j_pic, i_tmp = 0;
//	int i_fb, i_pic;	
	int i = w_space, j = h_zoom;
	lnode *tmp = start;

//	while(tmp -> next != head && tmp -> next != NULL){
	do{
		/* draw the grid */
		fb_row(i - 2, j - 2, w_zoom + 4, 0xeeeeee);
		fb_row(i - 2, j + h_zoom + 2, w_zoom + 4, 0xeeeeee);
		fb_collum(i - 2, j - 2, h_zoom + 4, 0xeeeeee);
		fb_collum(i + w_zoom + 2, j - 2, h_zoom + 4, 0xeeeeee); 

		/* decode the pictures */
		buf24 = decode_jpeg(tmp -> filename, &jpeg);
		scalebuf = scale24(buf24, zoom_jpeg, jpeg);
		buf32 = rgb24to32(scalebuf, zoom_jpeg);

		/* display the zoom_picture */
#if 0
		for(j_fb = j, j_pic = 0; j_pic < h_zoom; j_fb++, j_pic++){
			for(i_fb = i, i_pic = 0; i_pic < w_zoom; i_fb++, i_pic++){
				fb_pixel(i_fb, j_fb, buf32[j_pic * zoom_jpeg.w + i_pic]);
			}
		}	
#endif		
#if 1		
		for(j_fb = j, j_pic = 0; j_pic < h_zoom; j_fb++, j_pic++){
			memcpy((color_t *)fb.fb_mem + fb.w *j_fb + i, &buf32[j_pic * zoom_jpeg.w], w_zoom * 4);
		}
#endif
		/* save the zoom_picture's info */
		page.pic_show[i_tmp] = tmp->filename;
		i_tmp++;

		/* mov to next zoom_picture's position */
#if 0		
		i = i_fb + w_space;
#endif
		tmp = tmp -> next;
		i += w_zoom + w_space;
		if(i + 2 * w_zoom >= fb.w){
			j = j_fb + h_space;
			i = w_space;
		}
		if(j + h_zoom >= fb.h){
			page.save_node[++page.save_top] = tmp;
			break;
		}

		/* destroy the used memory space */
		free(buf24); buf24 = NULL;
		free(scalebuf); scalebuf = NULL;
		free(buf32); buf32 = NULL;
	}while(tmp != head);

	/* save the windows_main's info */
	page.n_pic_show = i_tmp;

	return 0;
}
