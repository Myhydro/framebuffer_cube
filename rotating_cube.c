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

static void print_fix_si(struct fb_fix_screeninfo * si){
    fprintf(stderr, CYELL"====> FIXED SCREENINFO\n"CNONE);
    fprintf(stderr, "id:        %s\n",  si->id);
    fprintf(stderr, "smem_s:    %lu\n", (unsigned long) si->smem_start);
    fprintf(stderr, "smem_len:  %lu\n", (unsigned long) si->smem_len);
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

int main(){
    
    //start off by just printing the important frame buffer structs.
    struct fb_fix_screeninfo fsi;
    struct fb_var_screeninfo vsi;
    int fd = open("/dev/fb0", O_RDWR);

    if(fd == -1){
        fprintf(stderr, "Failed to open /dev/fb0\n");
        exit(1);
    }

    if(ioctl(fd, FBIOGET_FSCREENINFO, &fsi) == -1){
        fprintf(stderr, "Failed to get FSCREENINFO\n");
        exit(1);
    }
    
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vsi) == -1){
        fprintf(stderr, "Failed to get VSCREENINFO\n");
        exit(1);
    }

    print_fix_si(&fsi);
    print_var_si(&vsi);
    
    puts(CCYAN"[PRESS KEY TO CONTINUE]"CNONE);
    getchar();
    

    /*
     * Drawing to screen
     */

    //Get a pointer to frame buffer dma
    uint8_t * fbp = mmap(   NULL, 
                            fsi.smem_len, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED,
                            fd,
                            0                       );
    

}



