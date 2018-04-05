clc
clear
close all

[y1,fe] = audioread('Cena.wav');	% fichier original
[y2,fe] = audioread('Cena.wav');	% fichier filtré

Y1 = fft(y1);
Y2 = fft(y2);

Y1 = Y1/length(y1);
Y2 = Y2/length(y2);

figure

subplot(2,1,1)
plot(abs(Y1(1:end/2)))
title('Fichier audio original')
xlim([0 3000])
xlabel('Fréquences (Hz)')
ylabel('Amplitude')

subplot(2,1,2)
plot(abs(Y2(1:end/2)))
title('Fichier audio filtré')
xlim([0 6000])
xlabel('Fréquences (Hz)')
ylabel('Amplitude')

