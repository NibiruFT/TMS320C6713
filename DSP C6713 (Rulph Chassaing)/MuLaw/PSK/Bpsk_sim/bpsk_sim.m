%BPSK_sim.m  BPSK modulation and demodulation of a random bit stream
clear; close all
SNR = 0.5;                      %Signal to noise ratio
E_s = 1;                        %Signal Power
No = E_s^2/SNR;                 %Noise Power
sig_n = sqrt(No/2);             %Noise standard deviation
w=2*pi*5;                       %Frequency
t=0.01:0.01:0.2;                %Time Vector
N=200;                         %Number of Bits
%Create Signal Vectors
for ii=1:2
    s(ii,:) = E_s*cos(w*t-2*pi*ii/2);
end
%Modulate a random bit stream
signal = ceil(2*rand(1,N));
m_sig = [];
for ii=1:N
    m_sig = [m_sig s(signal(ii),:)];
end
plot(m_sig(1:100),':')              %plot the transmitted signal
n = sig_n*randn(1,length(m_sig));   %Create a noise vector
r = m_sig + n;                  %Received Signal = s + n
hold on; plot(r(1:100),'r')     %Plot the received signal
title('Transmitted and Received signals')
legend('Transmitted Waveform', 'Received Waveform')
sigma1 = E_s*cos(w*t);          %Create basis functions
sigma2 = E_s*sin(w*t);
index = 1:length(t);
for ii=0:N-1                    %detect the signal
    X(ii+1) = sum(sigma1.*r(ii*length(t) + index));    
    Y(ii+1) = sum(sigma2.*r(ii*length(t) + index));
end         
currFig = figure;                   %double buffer so window
set(currFig,'DoubleBuffer','on');   %does not flash
title('Detections and decision regions')  %Plot the detections
hold on;
axlim = 30;
axis([-axlim axlim -axlim, axlim])
plot([0 0],[-axlim axlim], 'k')

for ii=1:N  %Make decisions on received bits
    if(X(ii) < 0)
        received_sig(ii) = 1;
    else
        received_sig(ii) = 2;
    end
    figure(currFig)
    if and(received_sig(ii)-signal(ii),1)
        plot(X(ii),Y(ii),'ro')
    else
        plot(X(ii),Y(ii),'bx')
    end
    hold on
    pause(0.1)
end
x=received_sig - signal;
n_errors = sum(and(x,1))            
p_error = n_errors / N              %Calculate the percentage error
wavwrite(m_sig,5500,16,'bpsk.wav')   %write the wav files
wavwrite(r,5500,16,'bpsk_n.wav')      