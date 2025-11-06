/* compie with this "gcc main.c -fsanitize=address,undefined -g" to debug nicely */

#include<stdio.h> // printf
#include <sys/ioctl.h>
#include <net/if.h> // struct ifreq, IFNAMSIZ
#include <stdlib.h> // exit
#include <fcntl.h> // open
#include <linux/if_tun.h> // IFF_TUN
#include <string.h> //memset, strncpy
#include <unistd.h> // close

/*
 * Using TAP/TUN devices for simulating packest/farmes transmission
 * */

int tun_alloc(char *dev)
{
 struct ifreq ifr;
 int fd, err;
 /* char *dev = NULL; is an uninitialize pointer with some garbage value, char *dev; is an null pointer it points to nothing*/

 if ((fd = open("/dev/net/tun", O_RDWR)) < 0){
   printf("Cannot open TUN/TAP dev\n");
   exit(1);
 }

 memset(&ifr, 0, sizeof(ifr));

 /*
  * Flags:
  *     IFF_TUN - TUN device (no Ethernet headers)
  *     IFF_TAP - TAP device
  *
  *     IFF_NO_PI - Do not provide packet information
  * */

 /*
  *
  * PI packet information Without this flag, the kernel prepends an extra 4-byte header before every packet to tell the user space program about the protocol type and flags.
  * */

 ifr.ifr_flags = IFF_TUN | IFF_NO_PI; // setting network device for socket to use

 /* checks if first byte is non-zero for dev */

 if (*dev){
   strncpy(ifr.ifr_name, dev, IFNAMSIZ); // copy device name from struct to dev
 }

 /* TUNSETIFF fullform tun set inter face flags
  * temp tun interface with TUNSETIFF, premanent interface with TUNSETPERSIST
  * ioctl(open file desc, necessary code, a struct pointer casts as void it needs that)
  * */

 if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0){
   printf("ioctl tun set failed! %d", err);
   return err;
 }


 strcpy(dev, ifr.ifr_name);

 return fd;

}


int main(){
  char dev[IFNAMSIZ] = "tun0"; // %d gets the avaiable integer, if 0 is free it's tun0
  tun_alloc(dev);
}
