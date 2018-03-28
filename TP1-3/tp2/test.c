#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NBLED 2
#define NBBP 1
char led[NBLED];
char bp[NBBP];
 
int main()
{
   int i;
   int fd = open("/dev/ledbpJL", O_RDWR);
   if (fd < 0) {
      fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
      exit(1);
   }
   for( i = 0; i < NBLED; i ++) {
      led[i] = '1';
   }
   do { 
      led[0] = (led[0] == '0') ? '1' : '0';

      write( fd, led, 2);
      sleep( 1);
      read( fd, bp, 1);
   }    //while (1);
   while (bp[0] == '1');
   return 0;
}
