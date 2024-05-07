%% designsteps.m   Software Linear PLL MATLAB design and simulation program
%% Upendra Ummethala and Mike Mellor
%% December 19, 2000   EE539 Real Time DSP Final Project
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% PLL parameters design first
%% See Chapters 2 and 5 in Phase-Locked Loops : Design, Simulation and
%% Applications by Roland E Best; Fourth Edition, McGraw Hill
%% for design procedure and parameter selection
%% A continuous time implementation is first made which is discretized
%% Proper scaling is then applied to facilitate a fixed point implementation.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;						%% Start with a clean slate	
close all;		

w0=100*2*pi; 					%% Target DCO center frequency = 100Hz
zeta=0.7; 						%% Target loop damping ratio
delta_w=50*2*pi; 				%% Target lock range = +/-50Hz

w2_min=w0-1.5*delta_w;		%% Target min. frequency = 100-50=50Hz
w2_max=w0+1.5*delta_w;		%% Target max. frequency = 100+50=150Hz

uf_max=2^15-1;					%% Loop filter output upper clipping limit
uf_min=-2^15;					%% Loop filter output upper clipping limit


% Ko here is <1 so in the C code for the DSK, this is scaled up by 2^6
Ko=(w2_max-w2_min)/(uf_max-uf_min);

% Using an active filter, i.e. the DC gain is greater than unity.
Ka=6;

Kd=31000*2/pi;							%% Phase detector gain 
wn=delta_w/(2*zeta);					%% Loop natural frequency 
tau2=(zeta*2/wn)-(1/(Ko*Kd*Ka)); %% Filter zero time constant
tau1=Ko*Kd*Ka/wn^2;					%% Filter pole time constant

pole_freq=1/tau1				%% Single pole frequency	
zero_freq=1/tau2				%% Singe zero frequency

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Filter coefficient design
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
zerofreq=zero_freq*2*pi; 	%% Zero frequency in radians
polefreq=pole_freq*2*pi;  	%% Pole frequency in radians
gain=Ka;							%% Loop filter DC gain

sys=tf(gain*[1/zerofreq 1],[1/polefreq 1]); %Create continuous time equivalent

Ts=1/8000; 						%% DSK sample time fixed at 8kHz
sysd=c2d(sys,Ts,'matched');%% Obtain identical discrete time frequency
									%% response to continuous time system

bode(sys,'r*',sysd,'b',{1e2,1e5}); 	%% Compare discrete vs. continuous
title('Continuous (red star) and discrete (blue) freq responses')

[num,den]=tfdata(sysd,'v') %% Obtain the numerator and denominator polynomial
									%%	coefficients for the discrete filter 
coeffscale=2^12;				%% Scale factor for the coefficients

num=round(num*coeffscale); %% Scale the numerator and denomiantor polynomial
den=round(den*coeffscale);	%% coefficients and round them to integers

b0=num(1)						%% Separate out the coefficients from the arrays
b1=num(2)
a0=den(1)
a1=den(2)

% Overlay original and rounded off filter freq responses for comparison
sysdround=tf(num,den,Ts);
figure
bode(sysd,'r*',sysdround,'b',{1e2,1e5});
title('Not rounded (red star) and rounded (blue) freq responses')


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%% We have completed the design now we will implement the PLL
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Initialize variables:
u2_n=0;							%% Initial DCO output level					
phi2_n=0;						%% Initial DCO output phase	
ud_n_1=0;						%% Initial phase detector output
uf_n_1=0;						%% Initial loop filter output

Ts=1/8000;						%% Sample rate for the DSK fixed at 8kHz

t=[1:(9000)]*Ts; 				%% time vector for Matlab simulation

%% compute the closed loop pole transfer unction and pole locations:
CLTF=tf((Ko*Kd/(tau1))*[tau2 1],[1 (1+Ko*Kd*tau2)/(tau1) Ko*Kd/(tau1)]);
[z,p,k]=zpkdata(CLTF,'v')
figure
bode(CLTF,{1e1,1e5});

input=round(31000*sin((w0+delta_w/2)*t));	% Generate input sinewave

%% Shift frequency and/or phase for the middle third of the simulation
input(round(length(t)/3)+1:round(length(t)*2/3))=round(31000*sin((w0-delta_w/2)*t(round(length(t)/3)+1:round(length(t)*2/3))+pi/2));

%%%%%%%%%%%%%%%%% Simulate the PLL %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i=1:length(t),			
   
   %% Save the intermediate sample values for plotting	
   ud(i)=ud_n_1;				%% Phase detector output
   u2(i)=u2_n;					%% DCO output level
   uf(i)=uf_n_1;				%% Loop filter output
   phi2(i)=phi2_n;			%%	DCO output phase
   
   ud_n=round(input(i)*u2_n);	%% Phase detector output
     
   uf_n=-round(a1*uf_n_1)+round(b0*ud_n)+round(b1*ud_n_1);	%% Loop filter output
   																			%% rounded to an integer	

   lowerlimit=-2^31;		%% Limit checks for internal state saturation. 
   upperlimit=2^31-1;	%% Assume 32 bit signed integers for all variables	
   
   if ( (a1*uf_n_1 >= upperlimit) | (a1*uf_n_1 <= lowerlimit)),
      disp(['WARNING !!!! a1*uf_n_1 out of range']);
   end
   
   if ( (b0*ud_n >= upperlimit) | (b0*ud_n <= lowerlimit)),
      disp(['WARNING !!!! b0*ud_n out of range']);
   end
  	if ( (b1*ud_n_1 >= upperlimit) | (b1*ud_n_1 <= lowerlimit)),
      disp(['WARNING !!!! b1*ud_n_1 out of range']);
   end
  	if ( (uf_n >= upperlimit) | (uf_n <= lowerlimit)),
      disp(['WARNING !!!! a1*uf_n_1 out of range']);
   end
      
   uf_n=round(uf_n/coeffscale);  %% Undo the effect of coefficient scaling 
   										%% Would correspond to bitshifts on the DSP

   phi2_nplus1=phi2_n+round(w0+Ko*uf_n);	%% update the DCO phase
   
   %%%%%%% Check for saturation	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   if ( (Ko*uf_n >= upperlimit) | (Ko*uf_n <= lowerlimit)),
      disp(['WARNING !!!! Ko*uf_n out of range']);
   end
	if ( (phi2_nplus1 >= upperlimit) | (phi2_nplus1 <= lowerlimit)),
      disp(['WARNING !!!! phi2_nplus1 out of range']);
   end
   if ( ((w0+Ko*uf_n) >= upperlimit) | ((w0+Ko*uf_n) <= lowerlimit)),
      disp(['WARNING !!!! (w0+Ko*uf_n) out of range']);
   end
   
   %% Unwrap the phase by 2*pi if the current phase is >pi 
   if phi2_nplus1>round(pi*1/Ts),
      phi2_nplus1=phi2_nplus1-2*round(pi*1/Ts);
   end
   
   %% Output the appropriate Walsh function value(+1, -1) based on the phase
   if phi2_nplus1>=0,
      u2_nplus1=1;
   else
      u2_nplus1=-1;
   end
   
   	%% Update the variable for the next trip through the loop   
      ud_n_1=ud_n;			%% Save the previous phase detector output 					
      uf_n_1=uf_n;			%% Save the previous filter output
      phi2_n=phi2_nplus1;	%% Save the predicted DCO phase output
      u2_n=u2_nplus1;		%% Save the predicted DCO output
      
   end
   
   %% Plot the important simulation results
   figure						%% Plot the DCO output vs. time
   plot(t,input/31000,t,u2);  
   axis([min(t) max(t) min(u2)*1.1 max(u2)*1.1])
   
   figure						%% Plot the DCO output phase vs. time
   plot(t,phi2);				
   
   figure						%% Plot the phase detector output vs. time
   plot(t,ud);
   
   figure						%% Plot the loop filter output vs. time
   plot(t,uf);
   