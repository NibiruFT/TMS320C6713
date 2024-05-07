%Input.m Reads .wav file and create/store samples of .wav->input.h
%Hamming coefficients stored in file->hamming.h

[input,Fs]=wavread('goaway.wav'); 	%insert .wav file here

div=round(Fs/8000);			%in same directory as .m file
input=resample(input,1,div);		%resample for 8 kHz
start=1;
frame_length=80;
given=length(input);          	%specifies length of speech segment         
input =round(input*2^15);
zeros_to_add=frame_length-(mod((given-start+1),frame_length)); %zero-padding
input(given+1:given+zeros_to_add)=0;

fid=fopen('input.h','w');		%printing samples as array->input.h
disp('The value of Def_total in Speech.c is');
disp(given-start+1+zeros_to_add);
fprintf(fid,'short input[%d]={',given-start+1+zeros_to_add);
for index=start:given+zeros_to_add
	if index==given+zeros_to_add
    	  fprintf(fid,'%i\n',input(index)); 
      else
        fprintf(fid,'%i,\n',input(index));
      end 
end
fprintf(fid,'};\n');
fclose(fid);

ham=hamming(frame_length); 		%for hamming window coefficients
ham= round(ham*2^3);
fid=fopen('hamming.h','w');
fprintf(fid,'int ham[%d]={',frame_length); %coefficients->hamming.h
for index=1:frame_length
    if index==frame_length
       fprintf(fid,'%i\n',ham(index)); 
    else
       fprintf(fid,'%i,\n',ham(index));
    end     
end
fprintf(fid,'};\n');
fclose(fid);
