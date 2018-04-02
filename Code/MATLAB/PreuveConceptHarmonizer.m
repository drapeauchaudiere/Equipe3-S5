clear all
close all
clc

[y,Fs] = audioread('note_guitare_LAd.wav');
x = y(10000:10000+1023); 
n = 1:length(x);
%Fs possible: ADC-DAC
% 96    - 96
% 88.2  - 88.2
% 48    - 48
% 44.1  - 44.1
% 32    - 32
% 8.021 - 8.021
% 8     - 8
Fs = 44100; %16 ou 32 bit 
F = 440;
%x=0.1*cos(2*pi*n*F/Fs);
N = length(x);

figure();
plot(y);

%% Premier bloc est une FFT
Xfft = fft(x);
figure();
plot(abs(Xfft));
hold on

%% Second Bloc est une filtre moyenne mobile pour adoucir la FFT
for k = 1 : N
   B = 1/3*ones(3,1);
   out = filter(B,1,Xfft);
end
plot(abs(out));





