%DTMF.m Partial MATLAB file to generate DTMF signals

clear all
t=1:8000;
t=t/8000;
num_1=zeros(8000,1);
num_2=zeros(8000,1);
...
num_pound=zeros(8000,1);

for n=1:8000
	num_1(n)=sin(2*pi*697*t(n))+sin(2*pi*1209*t(n));
	num_2(n)=sin(2*pi*697*t(n))+sin(2*pi*1336*t(n));
	...
	num_pound(n)=sin(2*pi*941*t(n))+sin(2*pi*1477*t(n));
end

for i=1:100000000
    	soundsc(num_1);
    	pause(1.5);
    	soundsc(num_2);
    	pause(1.5);
    	...   
    	soundsc(num_pound);
    	pause(1.5);
end

