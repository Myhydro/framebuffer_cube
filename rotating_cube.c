#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define CNONE "\x1B[0m"
#define CYELL "\x1B[33m"
#define CCYAN "\x1B[36m"


struct framebuffer_object{
    int fd;
    uint8_t * fbp;
    struct fb_fix_screeninfo fsi;
    struct fb_var_screeninfo vsi;
};


static void print_fix_si(struct fb_fix_screeninfo * si){
    fprintf(stderr, CYELL"====> FIXED SCREENINFO\n"CNONE);
    fprintf(stderr, "id:        %s\n",  si->id);
    fprintf(stderr, "smem_s:    %lx\n", (unsigned long) si->smem_start);
    fprintf(stderr, "smem_len:  %lx\n", (unsigned long) si->smem_len);
    fprintf(stderr, "type:      %lu\n", (unsigned long) si->type);
    fprintf(stderr, "type_aux:  %lu\n", (unsigned long) si->type_aux);
    fprintf(stderr, "visual:    %lu\n", (unsigned long) si->visual);
    fprintf(stderr, "xpanstep:  %hu\n", (unsigned short) si->xpanstep);
    fprintf(stderr, "ypanstep:  %hu\n", (unsigned short) si->ypanstep);
    fprintf(stderr, "ywrapstep: %hu\n", (unsigned short) si->ywrapstep);
    fprintf(stderr, "linelen:   %lu\n", (unsigned long) si->line_length);
    fprintf(stderr, "mmio_len:  %lu\n", (unsigned long) si->mmio_start);
    fprintf(stderr, "accel:     %lu\n", (unsigned long) si->mmio_len);
    fprintf(stderr, "capabil:   %hu\n", (unsigned short) si->accel);
}

static void print_var_si(struct fb_var_screeninfo * si){
    fprintf(stderr, CYELL"====> VARIABLE SCREENINFO\n"CNONE);
    fprintf(stderr, "xres:      %lu\n", (unsigned long) si->xres);
    fprintf(stderr, "yres:      %lu\n", (unsigned long) si->yres);
    fprintf(stderr, "xres_virt: %lu\n", (unsigned long) si->xres_virtual);
    fprintf(stderr, "yres_virt: %lu\n", (unsigned long) si->yres_virtual);
    fprintf(stderr, "xoff:      %lu\n", (unsigned long) si->xoffset);
    fprintf(stderr, "yoff:      %lu\n", (unsigned long) si->yoffset);
    fprintf(stderr, "bperpix:   %lu\n", (unsigned long) si->bits_per_pixel);
    fprintf(stderr, "grayscale: %lu\n", (unsigned long) si->grayscale);
    fprintf(stderr, "nonstd:    %lu\n", (unsigned long) si->nonstd);
    fprintf(stderr, "activate:  %lu\n", (unsigned long) si->activate);
    fprintf(stderr, "height:    %lu\n", (unsigned long) si->height);
    fprintf(stderr, "width:     %lu\n", (unsigned long) si->width);
    fprintf(stderr, "accelflag: %lu\n", (unsigned long) si->accel_flags);
}


static uint32_t * get_pixel(struct framebuffer_object * fbo, int x, int y){
    long location = (x * fbo->vsi.bits_per_pixel) + (y * fbo->fsi.line_length);
    return (uint32_t *) (fbo->fbp + location);
}

static int init_framebuffer(struct framebuffer_object * fbo){
    fbo->fd = open("/dev/fb0", O_RDWR);

    if(fbo->fd == -1){
        fprintf(stderr, "Failed to open /dev/fb0\n");
        return -1;
    }

    if(ioctl(fbo->fd, FBIOGET_FSCREENINFO, &fbo->fsi) == -1){
        fprintf(stderr, "Failed to get FSCREENINFO\n");
        return -1;
    }
    
    if(ioctl(fbo->fd, FBIOGET_VSCREENINFO, &fbo->vsi) == -1){
        fprintf(stderr, "Failed to get VSCREENINFO\n");
        return -1;
    }

    //Set pointer to frame buffer dma
    fbo->fbp = mmap( NULL, 
                    fbo->fsi.smem_len, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED,
                    fbo->fd,
                    0);
    
    if(fbo->fbp == MAP_FAILED)
        return -1;
    
    return 0;
}



int main(){
    struct framebuffer_object fbo;
    
    if(init_framebuffer(&fbo) == -1){
        fprintf(stderr, "Failed to create framebuffer object.\n");
        exit(-1);
    }

    //start off by just printing the important frame buffer structs.

    print_fix_si(&fbo.fsi);
    print_var_si(&fbo.vsi);
    
    puts(CCYAN"[PRESS ENTER TO CONTINUE]"CNONE);
    getchar();

}



