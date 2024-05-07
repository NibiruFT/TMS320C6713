%Spectrogram_RTDX.m For spectrogram plot using RTDX with MATLAB
clc;
ccsboardinfo					%board info
cc=ccsdsp('boardnum',0);			%set up CCS object
reset(cc);						%reset board
visible(cc,1);					%for CCS window
enable(cc.rtdx);					%enable RTDX
if ~isenabled(cc.rtdx);	
	error('RTDX is not enabled')
end
cc.rtdx.set('timeout',50);			%set 50sec timeout for RTDX
open(cc,'spectrogram_rtdx_mtl.pjt');			%open CCS project
load(cc,'./debug/spectrogram_rtdx_mtl.out');		%load executable file
run(cc);						%run program
configure(cc.rtdx,2048,1);			%configure one RTDX channel
open(cc.rtdx,'ochan','r');		%open output channel
pause(3)						%wait for RTDX channel to open

enable(cc.rtdx,'ochan');			%enable channel from DSK
isenabled(cc.rtdx,'ochan');

M = 256;       					%window size
N = round(M/2);
B = 128;    					%No. of blocks (128)
fs = 8000;      					%sampling rate
t=(1:B)*(M/fs);					%spectrogram axes generation
f=((0:(M-1)/2)/(M-1))*fs;

set(gcf,'DoubleBuffer','on');
y = ones(N,B);
column = 1;

set(gca,'NextPlot','add');
axes_handle = get(gcf,'CurrentAxes');
set(get(axes_handle,'XLabel'),'String','Time (s)');
set(get(axes_handle,'YLabel'),'String','Frequency (Hz)');
set(get(axes_handle,'Title'),'String','\fontname{times}\bf Real-Time Spectrogram');

set(gca,'XLim', [0 4.096]);
set(gca,'YLim', [0 4000]);
set(gca,'XLimMode','manual');
set(gca,'YLimMode','manual');

for i = 1:32768    
  w=readmsg(cc.rtdx,'ochan','single');  	%read FFT data from DSK            
  w=double(w(1:N));    
  y(:, column) = w';    
  imagesc(t,f,dB(y));				%plot spectrogram    
  column = mod(column, B) + 1;
end

halt(cc);						%halt processor
close(cc.rtdx,'ochan');				%close channel
clear cc						%clear object
