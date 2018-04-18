clear all
close all
clc

Fe = 44100;
%% Filtre passe-bas 700 Hz
clc
close all

Theta_700 = 2*700/Fe;
[A,B,C,D] = butter(2,Theta_700);
[sos700 gain_global700] = ss2sos(A,B,C,D, 'up', 'inf');
[B,A] = sos2tf(sos700, gain_global700);

H2 = tf(B, A);

H16 = H2*H2*H2*H2;

[num, den] = tfdata(H2, 'v');

n =  1:1:1000;
x = cos(2*pi*n*200/Fe);

y =  filter(num, den, x);

%Délai de groupe
[Gd,Wgd] = grpdelay(num, den);

figure
plot(Wgd,Gd)
title('Delai de groupe')
xlabel('Fréquence (rad/samples)')
ylabel('Délai de groupe en samples')
grid

%Find TF filter
G = tf(num,den);
freq_coup = bandwidth(G);

figure
zplane(B, A);
figure
freqz(B, A,10000,Fe);

figure
plot(n, y, 'r')
hold on 
plot (n, x, 'b')

round(sos700(1:3)*2^13)
round(sos700(4:6)*2^13)
round(gain_global700*2^13)

% [A,B,C,D] = butter(4,Theta_700);
% [sos700 gain_global700] = ss2sos(A,B,C,D, 'up', 'inf');
%% Filtre passe-bas 5000 Hz et Filtre passe-haut 1000 Hz
clc
close all

Theta_5000 = 2*5000/Fe;
Theta_1000 = 2*1000/Fe;

[B_5000, A_5000] = butter(2,Theta_5000, 'low');
[B_1000, A_1000] = butter(2,Theta_1000, 'high');

%A_PB = conv(A_5000,A_1000);
%B_PB = conv(B_5000,B_1000);

% [B_PB A_PB] = butter(2,[Theta_1000 Theta_5000]);

%[A_Pbande,B_Pbande,C_Pbande,D_Pbande] = tf2ss(B_PB, A_PB);
[A_Phaut,B_Phaut,C_Phaut,D_Phaut] = tf2ss(B_1000, A_1000);
[A_Pbas,B_Pbas,C_Pbas,D_Pbas] = tf2ss(B_5000, A_5000);

[sos_Phaut gain_global_Phaut] = ss2sos(A_Phaut,B_Phaut,C_Phaut,D_Phaut, 'up', 'inf');
[sos_Pbas gain_global_Pbas] = ss2sos(A_Pbas,B_Pbas,C_Pbas,D_Pbas, 'up', 'inf');
%[B,A] = sos2tf(sos_Pbande, gain_global_Pbande);

% figure
% hold on
% [B,A] = sos2tf(sos_Phaut, gain_global_Phaut);
% zplane(B, A);
% [B,A] = sos2tf(sos_Pbas, gain_global_Pbas);
% zplane(B, A);
% hold off

sos_Phaut(1:3) = sos_Phaut(1:3)/26;
gain_global_Phaut = gain_global_Phaut * 26;

[B,A] = sos2tf(sos_Phaut, gain_global_Phaut);
figure
freqz(B,A,10000,Fe);

[B,A] = sos2tf(sos_Pbas, gain_global_Pbas);
figure
freqz(B,A,10000,Fe);

% B1 = round(sos_Phaut(1:3)*2^13)	% B1
% A1 = round(sos_Phaut(4:6)*2^13)	% A1
% B2 = round(sos_Pbas(1:3)*2^13)	% B2
% A2 = round(sos_Pbas(4:6)*2^13)	% A2
% Gain1 = round(gain_global_Phaut*2^13);
% Gain2 = round(gain_global_Pbas*2^13);

B1 = sos_Phaut(1:3)	% B1
A1 = sos_Phaut(4:6) % A1
B2 = sos_Pbas(1:3)	% B2
A2 = sos_Pbas(4:6)	% A2
Gain1 = gain_global_Phaut;
Gain2 = gain_global_Pbas;

H1 = tf(B1, A1);
H2 = tf(B2, A2);

cascade = H1*H2;
[B, A] = tfdata(Gain1*Gain2*cascade, 'v');

figure
freqz(B,A, 10000, Fe)

%% Filtre passe-haut 7000 Hz
clc
close all

Theta_7000 = 2*7000/Fe;
[A,B,C,D] = butter(2,Theta_7000,'high');
[sos7000 gain_global7000] = ss2sos(A,B,C,D, 'up', 'inf');
[B,A] = sos2tf(sos7000, gain_global7000);

%Délai de groupe
[Gd,Wgd] = grpdelay(sos7000);

figure
plot(Wgd,Gd)
title('Delai de groupe')
xlabel('Fréquence (rad/samples)')
ylabel('Délai de groupe en samples')
grid

figure
zplane(B, A);
figure
freqz(B,A,1000000,Fe);

round(sos7000(1:3)*2^13)
round(sos7000(4:6)*2^13)
round(gain_global7000*2^13)

%% Combinaison des trois filtres
clc
close all

[B,A] = sos2tf(sos700, gain_global700);
H_Pbas = tf(B,A);

[B,A] = sos2tf(sos_Phaut, gain_global_Phaut);
H_Pbande = tf(B,A);
[B,A] = sos2tf(sos_Pbas, gain_global_Pbas);
H_Pbande = H_Pbande*tf(B,A);

[B,A] = sos2tf(sos7000, gain_global7000);
H_Phaut = tf(B,A);

H_tot = H_Pbas - H_Pbande + H_Phaut;

[num,den] = tfdata(H_tot,'v');

x = 1:0.1:2000;
n = cos(2*pi*x*4000/Fe);

[num,den] = tfdata(H_Pbas,'v');
y_bas = filter(num,den,n);

[num,den] = tfdata(H_Pbande,'v');
y_bande = filter(num,den,n);

[num,den] = tfdata(H_Phaut,'v');
y_haut = filter(num,den,n);

y = y_bas + y_bande + y_haut;

figure
plot(x,y)

[B, A] = tfdata(H_tot, 'v');
figure
freqz(B,A,10000,Fe)


%Délai de groupe
[Gd,Wgd] = grpdelay(B, A, 44100);

figure
plot(Wgd,Gd)
title('Delai de groupe')
xlabel('Fréquence (rad/samples)')
ylabel('Délai de groupe en samples')
grid

