' Remember to place the order of the filter in the 1st line of the file after
' exporting the coefficients.
cof = round(Num*2^15);
fid = fopen('Rec_HP_2000.cof','w');
fprintf(fid,'%i\n',cof);
fclose(fid);

' you should place the order of the FIR filter as the 1st line of the newly created file.