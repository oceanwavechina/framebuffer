# include "common.h"

int clean_screen(void)
{
	memset(fb.fb_mem, 0, fb.w * fb.h * 4);

	return 0;
}

int effect_from_top(info_t new_jpeg, u32_t *buf32) 
{
	int i;
	int j;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;

	for(j = j_start; j < new_jpeg.h + j_start; j++){
		for(i = i_start; i < new_jpeg.w + i_start; i++){
			fb_pixel(i, j, buf32[(j - j_start)*new_jpeg.w + (i - i_start)]);
		}
		usleep(1);
	}

	return 0;
}
#if 1
int effect_random_line(info_t new_jpeg, u32_t *buf32)
{
	int j = 0, i, n;
	unsigned int tmp;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;
	
	while(j < 2300){
		srand(tmp);
		n = rand() % (new_jpeg.h);
		if(n < j_start || n > j_start + new_jpeg.h){
			j--;
			tmp++;
		}else{
			for(i = i_start; i < new_jpeg.w; i++){
				fb_pixel(i, n, buf32[(n - j_start)*new_jpeg.w + (i - i_start)]);
			}
			usleep(1000);
		}
		j++;
		tmp++;
	}	
	for(j = j_start; j < new_jpeg.h + j_start; j++){
		for(i = i_start; i < new_jpeg.w + i_start; i++){
			fb_pixel(i, j, buf32[(j - j_start) * new_jpeg.w + (i - i_start)]);
		}
	}
	return 0;
}
#endif
int effect_from_left(info_t new_jpeg, u32_t *buf32)
{
	int i;
	int j;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;


	for(i = i_start; i < new_jpeg.w + i_start; i++){
		for(j = j_start; j < new_jpeg.h + j_start; j++){
			fb_pixel(i, j, buf32[(j - j_start)*new_jpeg.w + (i - i_start)]);
		}
		usleep(1);
	}

	return 0;
}

int effect_from_middle(info_t new_jpeg, u32_t *buf32)
{
	int i;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;
	int j1 = new_jpeg.h/2 + j_start;
	int j2 = new_jpeg.h/2 + j_start;


	while(j1 >j_start && j2 < new_jpeg.h + j_start){
		
		for(i = i_start; i < new_jpeg.w + i_start; i++){
			fb_pixel(i, j1, buf32[(j1 - j_start) * new_jpeg.w + (i - i_start)]);
		}
		j1--;
		usleep(1);

		for(i = i_start; i < new_jpeg.w + i_start; i++){
			fb_pixel(i, j2, buf32[(j2 - j_start) * new_jpeg.w + (i - i_start)]);
		}
		j2++;
		usleep(1);
	}

	return 0;
}

int effect_scroll(info_t new_jpeg, u32_t *buf32)
{
	int i, j, k;
	int j_small;
	int len = 30;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;


	for(j = j_start; j < new_jpeg.h + j_start; j++){
		for(i = i_start; i < new_jpeg.w + i_start; i++){
			fb_pixel(i, j, buf32[(j - j_start) * new_jpeg.w + (i - i_start)]);
		}
		if(j + len+10 < new_jpeg.h){
			k = len;
			for(j_small = j + 1; j_small < j + len && j + len < new_jpeg.h; j_small++){
				for(i = i_start; i < new_jpeg.w + i_start; i++){
					fb_pixel(i, j_small, buf32[(j+1+k)*new_jpeg.w + (i + i_start)]);
				}
				k--;
			}
		}
		usleep(1);
	}

	return 0;
}

int effect_parts(info_t new_jpeg, u32_t *buf32)
{
	int i, j, k;
	int len = 50;
	int i_start = (fb.w - new_jpeg.w) / 2;
	int j_start = (fb.h - new_jpeg.h) / 2;


	for(k = 0; k < len; k++){
		for(j = k + j_start; j < new_jpeg.h + j_start; j += len){
			for(i = i_start; i < new_jpeg.w + i_start; i++){
				fb_pixel(i, j, buf32[(j - j_start) * new_jpeg.w + (i - i_start)]);
			}
		}
		usleep(1);
	}

	return 0;
}

#if 0
int effect_random_square(info_t new_jpeg, u32_t *buf32)
{
	int h_square = fb.h / 100;
	int w_square = fb.w / 100;
	
	int j, i;

	while(n < 6000){
		srand(n);
		j_square = rand() % fb.h

		for(j = j_square; j < h_square; j++){
			for(i = i_square; i < w_square; i++){
				fb_pixel(i, j, buf32[j * new_jpeg.w + i]);
			}
		}

		n++;
	}

	return 0;
}
#endif
