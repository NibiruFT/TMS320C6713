  %RTDX_MATLAB_sim.m MATLAB-DSK interface using RTDX. Calls CCS
%loads .out file.  Data transfer from MATLAB->DSK,then DSK->MATLAB

indata(1:10) = [1:10];				%data to send to DSK

ccsboardinfo					    %board info
cc = ccsdsp('boardnum',0);			%set up CCS object
reset(cc)                           %reset board
visible(cc,1);					    %for CCS window
enable(cc.rtdx);					%enable RTDX
if ~isenabled(cc.rtdx)
    error('RTDX is not enabled')
end
cc.rtdx.set('timeout', 20);			%set 20sec time out for RTDX
open(cc,'rtdx_matlab_sim.pjt');    %open project
load(cc,'./debug/rtdx_matlab_sim.out');	%load executable file
run(cc);						    %run

configure(cc.rtdx,1024,4);			%configure two RTDX channels
open(cc.rtdx,'ichan','w');			%open input channel
open(cc.rtdx,'ochan','r');			%open output channel
pause(3)						    %wait for RTDX channel to open

enable(cc.rtdx,'ichan');			%enable channel TO DSK
if isenabled(cc.rtdx,'ichan')
    writemsg(cc.rtdx,'ichan', int16(indata)) %send 16-bit data to DSK  
    pause(3)
else
    error('Channel ''ichan'' is not enabled')
end
enable(cc.rtdx,'ochan');			%enable channel FROM DSK
if isenabled(cc.rtdx,'ochan')
    outdata = readmsg(cc.rtdx,'ochan','int16') %read 16-bit data from DSK
    pause(3)
else
    error('Channel ''ochan'' is not enabled')
end

if isrunning(cc), halt(cc);    		%if DSP running halt processor       		
end
disable(cc.rtdx);					%disable RTDX
close(cc.rtdx,'ichan');       		%close input channel
close(cc.rtdx,'ochan');       		%close output channel

