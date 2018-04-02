clear all
close all
clc


Fe = 44100;
%% Filtre passe-bas 700 Hz
Theta_700 = 2*pi*700/Fe;
[A,B,C,D] = butter(2,Theta_700);
[sos700 gain_global700] = ss2sos(A,B,C,D, 'up', 'inf');
[B,A] = sos2tf(sos700, gain_global700);

H2 =  tf(B, A);

H16 = H2*H2*H2*H2;

[num, den] = tfdata(H16, 'v');

n =  1:1:1000;
x = cos(2*pi*n*2000/Fe);

y =  filter(num, den, x);

figure
zplane(num, den);
figure
freqz(num, den);

figure
plot(n, y, 'r')
hold on 
plot (n, x, 'b')

% [A,B,C,D] = butter(4,Theta_700);
% [sos700 gain_global700] = ss2sos(A,B,C,D, 'up', 'inf');
%% Filtre passe-bas 5000 Hz et Filtre passe-haut 1000 Hz
Theta_5000 = 2*pi*5000/Fe;
Theta_1000 = 2*pi*1000/Fe;

[B_5000, A_5000] = butter(2,Theta_5000, 'low');
[B_1000, A_1000] = butter(2,Theta_1000, 'high');

A_PB = conv(A_5000,A_1000);
B_PB = conv(B_5000,B_1000);

[A_Pbande,B_Pbande,C_Pbande,D_Pbande] = tf2ss(B_PB, A_PB);

[sos_Pbande gain_global_Pbande] = ss2sos(A_Pbande,B_Pbande,C_Pbande,D_Pbande, 'up', 'inf');
[B,A] = sos2tf(sos_Pbande, gain_global_Pbande);
% figure
% zplane(B, A);
% figure
% freqz(B,A);

%% Filtre passe-bas 7000 Hz
Theta_7000 = 2*pi*7000/Fe;
[A,B,C,D] = butter(2,Theta_7000);
[sos7000 gain_global7000] = ss2sos(A,B,C,D, 'up', 'inf');
[B,A] = sos2tf(sos7000, gain_global7000);
% figure
% zplane(B, A);
% figure
% freqz(B,A);