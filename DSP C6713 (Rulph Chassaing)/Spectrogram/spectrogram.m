%Spectrogram.m Reads .wav file,plots spectrogram using STFT with MATLAB

[x,fs,bits] = wavread('chirp.wav');	%read .wav file
N = length(x);
t=(0:N-1)/fs;
set(0,'DefaultAxesColorOrder',[0 0 0],...
      'DefaultAxesLineStyleOrder','-|-.|--|:');
figure(1); plot(t,x);			%plots time-domain signal
xlabel('Time (sec)'); ylabel('Amplitude'); title('Waveform of signal');

M=256;  B=floor(N/M);			%divide signal->blocks of M samples
x_mat=reshape(x(1:M*B),[M B]); 	%reshape vector into MxB matrix
win=hamming(M); 				%Hamming window before FFT
win_mat=repmat(win,[1 B]); 
x_fft=fft(x_mat.*win_mat);		%perform FFT
y=abs(x_fft(1:M/2,:)); 			%want positive freq and mag info
t=(1:B)*(M/fs);				%values for time and freq axes
f=((0:M-1)/(M-1))*(fs/2);

figure(2);
imagesc(t,f,dB(y));			%plot spectrogram
colormap(jet);   colorbar;   set(gca,'ydir','normal');
xlabel('Time (sec)');  ylabel('Frequency (Hz)');  title('Spectrogram');







