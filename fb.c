#include "common.h"

static void sys_api_error_handler(char *str);

int init_fb(const char *device_file)
{
    /* step 1: open device file */
    int fd;
   	fd = Open(device_file, O_RDWR);

    /* step 2: map -> user space */
	int ret;
    struct fb_var_screeninfo fb_var;
    ret = ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
	if(ret < 0){
	   sys_api_error_handler("ioctl");
    }
    fb.w = fb_var.xres;
    fb.h = fb_var.yres;
    fb.bpp = fb_var.bits_per_pixel;
    printf("w:%d\th:%d\tbpp:%d\n", fb.w, fb.h, fb.bpp) ;    

    fb.fb_mem = mmap(NULL, fb.w * fb.h * fb.bpp / 8,
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(fb.fb_mem == MAP_FAILED){
		sys_api_error_handler("mmap");
    }
    
    /* step 3: close file */
    close(fd);
    return 0;
}

void sys_api_error_handler(char *str)
{
	fprintf(stderr, "Error %s: <%s>\n", str, strerror(errno));
	exit(1);
}

int fb_pixel(int x, int y, u32_t color)
{
    *((u32_t*)(fb.fb_mem) + fb.w * y + x) = color;

    return 0;
}

int fb_row(int x, int y, int len, color_t color)
{
	int i;

	for(i = x; i < len + x; i++){
		fb_pixel(i, y, color);
	}

	return 0;
}
int fb_collum(int x, int y, int len, color_t color)
{
	int j;

	for(j = y; j < len + y; j++){
		fb_pixel(x, j, color);
	}

	return 0;
}
