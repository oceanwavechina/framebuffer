# include "common.h"

int display_jpeg(const char *filename, int effect)
{
	/* decode the picture , save to "buf24"*/
	info_t jpeg;
	u8_t *buf24 = decode_jpeg(filename, &jpeg);
	
	/* zoom the  decoded picture, & switch it to 32 bits*/
	info_t new_jpeg;
	if(jpeg.w > fb.w || jpeg.h > fb.h){
		new_jpeg = fb;
	}else{
		new_jpeg = jpeg;
	}
	u8_t *scalebuf = scale24(buf24, new_jpeg, jpeg);
	u32_t *buf32 = rgb24to32(scalebuf, new_jpeg);

	/* array of pointers to effect functions */
	int (*foo_effect[20])(info_t, u32_t*);
	foo_effect[0] = effect_from_top;
	foo_effect[1] = effect_from_left;
	foo_effect[2] = effect_from_middle;
	foo_effect[3] = effect_scroll;
	foo_effect[4] = effect_parts;
	foo_effect[5] = effect_random_line;

	/* display the picture using the effect that chosen*/
	int n;
	srand(time(NULL));
	if(-1 != effect){
		switch(effect % MAX_EFFECT){
			case 0:	(*foo_effect[0])(new_jpeg, buf32);
					break;
			case 1:	(*foo_effect[1])(new_jpeg, buf32);
					break;
			case 2: (*foo_effect[2])(new_jpeg, buf32);
					break;
			case 3: (*foo_effect[3])(new_jpeg, buf32);
					break;
			case 4:	(*foo_effect[4])(new_jpeg, buf32);
					break;
			case 5:	(*foo_effect[5])(new_jpeg, buf32);
					break;
			default : clean_screen();
					break;
	
		}
	}else{
		n = rand() % MAX_EFFECT;
		(*foo_effect[n])(new_jpeg, buf32);	
	}

	free(buf24); buf24 = NULL;
	free(scalebuf); scalebuf = NULL;
	free(buf32); buf32 = NULL;

	return 0;
}
