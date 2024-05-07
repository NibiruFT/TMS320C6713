%BPSK_ReIn.m  Simulation of received waveform -> transmitted bits
function bpskms()
clear; close all
[Y,FS,nbits]=wavread('theforce.wav');  %read the file
Y = Y(length(Y)/2:length(Y));           
sample_rate = 1000;                    %new sample rate 
X = decimate(Y,round(FS/sample_rate)); %resample at sample_rate
X=round(X*128);                        %integers between -127/128
w=2*pi*5;                              %omega
t=0.01:0.01:0.2;                       %time
for ii=1:2 					   %BPSK signals
    s(ii,:) = sin(w*t-2*pi*ii/2); end
m_sig(1:100) =zeros;                   %buffer for plotting
figure                                 %make figure
currFig = get(0,'CurrentFigure');      %double buffer so window
set(currFig,'DoubleBuffer','on');      %does not flash
%m_sig=zeros(1,length(X)*8*length(t)); %preallocate  signal vec
for i=1:length(X) 			   %create/output BPSK waveform
 if X(i)<0  bits=xor(num2bin(-X(i),8),[1 1 1 1 1 1 1 1]); %bits of each sample     
 else       bits=num2bin(X(i),8);  end
 subplot(2,1,1)  			   	   %plot voice signal 
 if(i<50)
  hold off; plot(((-50:49)+i),[zeros(1,51-i) X(1:49+i)']); 
  hold on; plot(i,X(i),'ro') 
 else
  hold off; plot(((-49:50)+i),X((-49:50)+i)'); hold on; plot(i,X(i),'ro')
 end
 title('Received Waveform')  		   %title of decimated voice
 ylabel('Amplitude')         		   %y axis label
 xlabel('Sample')            		   %x axis label
 axis([(-50 + i) (49 + i) min(X) max(X)])    
 for ii=1:8      				   %output BPSK waveform to plot->scope
  m_sig = [m_sig s(bits(ii)+1,:)];
  %m_sig((i-1)*8*length(t) + ii*ti) = s(bits(ii)+1,:);
  subplot(2,1,2)                       %oscilloscope plot
  title('BPSK signal of theforce.wav') %title of plot
  ylabel('Amplitude')                  %y axis label
  xlabel('Sample')                     %x axis label
  legend(['Transmitted Bits ' sprintf('%d', bits)]) 
  plot(length(m_sig)-100:length(m_sig),m_sig(length(m_sig)-100:length(m_sig)));   
  axis([length(m_sig)-100 length(m_sig) -1.2 1.2]) %shift axis
  hold on;  pause(0.01);      %update @0.01s rate(plot last 100 bpsk samples)
 end
end

function s=num2bin(d,n) 	%creates array of bits 
if (nargin<2)			%based on positive input d and length n
  n=1; end;
a=dec2bin(d,n); s=zeros(1,n);
for k=1:n
    s(k)=str2num(a(k));  end;


