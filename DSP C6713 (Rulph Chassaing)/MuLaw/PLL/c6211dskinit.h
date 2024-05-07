/*Function prototypes for routines in c6211dskinit.c*/
void mcbsp0_init();
void mcbsp0_write(int);  
int mcbsp0_read();   
void TLC320AD535_Init();
void c6211_dsk_init();  
void comm_poll();
void comm_intr();
int input_sample(); 
void output_sample(int);
