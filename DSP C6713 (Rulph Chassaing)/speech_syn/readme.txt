1] Copy the .wav file to be synthesized into the same directory as the input_read.m.
2] Insert the appropriate wav file to be used in the 1st line of input_read.m
3] Save and run the Matlab program, "input_read.m". header files "input.h" and "hamming.h" are generated.
4] Note the value of Def_total displayed in the Matlab command window.
5] Copy  the header files-"input.h" and "hamming.h" into the ".pjt" folder.
6] Change the Def_total in "speech.c" to the corresponding value displayed in the matlab command window 

The values for various synthesised speech are as follows:
"Go Away!"        Def_total:7440  used in program
"Hello Professor" Def_total:7360                  
"Vacation"        Def_total:6880          
"Good Evening"    Def_total:7120          
7] After modifying the above mentioned variables, build the code.
8] Run the .out file generated.
9] Listen to the synthesized voice.  





