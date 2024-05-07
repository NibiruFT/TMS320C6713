%RTDX_matlabFFT.m MATLAB-DSK interface with loop. Calls CCS,
%loads .out file. Data from DSK to MATLAB for FFT and plotting 

ccsboardinfo					    %board info
cc=ccsdsp('boardnum',0);			%setup CCS object 
reset(cc);                          %reset board
visible(cc,1);					    %for CCS window
enable(cc.rtdx);					%enable RTDX
if ~isenabled(cc.rtdx);
    error('RTDX is not enabled')
end
cc.rtdx.set('timeout', 20);			%set 20sec timeout for RTDX
open(cc,'rtdx_matlabFFT.pjt');      %open project
load(cc,'./debug/rtdx_matlabFFT.out'); %load executable file
run(cc);						    %run program
configure(cc.rtdx,1024,1);			%configure one RTDX channel
open(cc.rtdx,'ochan','r');			%open output channel
pause(3)						    %wait for RTDX channel to open
 
fs=16e3;						    %set sample rate in MATLAB
fftlen=256;						    %FFT length
fp=[0:fs/fftlen:fs/2-1/fftlen];		%for plotting within MATLAB

enable(cc.rtdx,'ochan');			%enable channel from DSK
isenabled(cc.rtdx,'ochan');

for i=1:2048					    %obtain 2048 buffers then stop
  outdata=readmsg(cc.rtdx,'ochan','int16'); %read 16-bit data from DSK
  outdata=double(outdata);   			    %32-bit data for FFT 
  FFTMag=abs(fftshift(fft(outdata)));	    %FFT using MATLAB
  plot(fp,FFTMag(129:256))
  title('FFT Magnitude of data from DSK');
  xlabel('Frequency');
  ylabel('Amplitude');
  drawnow;
end

halt(cc); 						            %halt processor 
close(cc.rtdx,'ochan'); 			        %close channel       
clear cc						            %clear object
