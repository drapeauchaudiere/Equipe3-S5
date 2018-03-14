clear all
close all
clc


%Frequences de coupure normalisées des filtres  
Fs = 44100;
Wn1 = 2/Fs*700;
Wn2 = 2/Fs*1000;
Wn3 = 2/Fs*5000;
Wn4 = 2/Fs*7000; 




n=1:256;
X1 = 0.1*sin(2*pi*700*n/Fs);
X2 = 0.1*sin(2*pi*3000*n/Fs);
X3 = 0.1*sin(2*pi*5000*n/Fs);
X4 = 0.1*sin(2*pi*7000*n/Fs);


N = length(n);

% Conception de la fenêtre de Hamming de longueur N
ham = hamming(N);     

Xtot = X1+X2+X3;


fftXtot=fft(Xtot.*ham',N);


%figure();
%stem((1:length(fftXtot))/length(fftXtot)*Fs, fftXtot);


%% Conception Filtre passe bas 0-700Hz
[numF1,denF1] = butter(2,Wn1,'low');
figure();
[h,w]  = freqz(numF1,denF1);
figure();
plot(w*Fs/(2*pi),20*log10(abs(h)));
title('filtre passe bas 700Hz');
xlabel('frequence (Hz)');
ylabel('Gain (dB)');
axis([0 1000 -4 0]);

%% Conception filtre passe-bande 1000-5000Hz
[numF2,denF2] = butter(2,Wn2,'high');
[numF3,denF3] = butter(2,Wn3,'low');
numPB = conv(numF2, numF3);
denPB = conv(denF2,denF3);

[h,w]  = freqz(numPB,denPB);
figure();
plot(w*Fs/(2*pi),20*log10(abs(h)));
title('filtre passe bande 1000-5000Hz');
xlabel('frequence (Hz)');
ylabel('Gain (dB)');
axis([300 7000 -22 0]);

%% Conception filtre passe-haut 7000Hz
[numF4,denF4] = butter(2,Wn3,'high');
[h,w]  = freqz(numF4,denF4);
figure();
plot(w*Fs/(2*pi),20*log10(abs(h)));
title('filtre passe-haut 7000 Hz');
xlabel('frequence (Hz)');
ylabel('Gain (dB)');
axis([5000 9000 -3.5 0]);


%% Conception du filtre total 
H1 = filt(numF1,denF1);
H2 = filt(0.7*numPB,denPB);% on fait *0.7 pour améliorer le flatness
H3 = filt(numF4,denF4);

% Position des potentiometres pour le gain des filtres
pot1 = 10^(0/20);
pot2 = 10^(0/20);
pot3 = 10^(0/20);

Htot = pot1*H1 - pot2*H2 + pot3*H3;

[numTot, denTot] = tfdata(Htot,'v');
[H,w] = freqz(numTot, denTot);
figure();
plot(w*Fs/(2*pi),20*log10(abs(H)));
title('Filtre Total');
xlabel('frequence (Hz)');
ylabel('Gain (dB)');
axis([0 10000 -2 2]); 


Xfiltre = filter(numTot,denTot, Xtot);
%sound(Xfiltre./4, Fs);

open_system('SchemaBlocSimulink')
set_param('SchemaBlocSimulink','AlgebraicLoopSolver','LineSearch')
sim('SchemaBlocSimulink')

% figure();
% plot(Yint);
% hold on
% plot(Xtot)
 size(Y1)
figure

% figure();
% stem(abs(fftXtot));
% 
% figure();
% myCorr= xcorr(Xtot, Xtot)
% plot(myCorr)








 

