clear all
close all
clc

%[Xguitare,FS1] = audioread('note_guitare_LAd.wav');
Fs = 44100;
Wn1 = 2/Fs*700;
Wn2 = 2/Fs*1000;
Wn3 = 2/Fs*5000;
Wn4 = 2/Fs*7000; 

n=1:1000;

X1 = sin(n*2*pi*300/Fs);
X2 = sin((Wn2+Wn3)/2*n);
X3 = sin ((Wn3+Wn3/2)*n);
Xtot =X1+X2+X3;


[numF1,denF1] = butter(2,Wn1,'low');

%freqz(numF1,denF1);


[numF2,denF2] = butter(2,Wn2,'high');
[numF3,denF3] = butter(2,Wn3,'low');
numPB = conv(numF2, numF3);
denPB = conv(denF2,denF3);


[numF4,denF4] = butter(2,Wn3,'high');


H1 = filt(numF1,denF1);
H2 = filt(numPB,denPB);
H3 = filt(numF4,denF4);


K1=0.7;
Htot = H1-K1*H2+1*H3;
[numTot, denTot] = tfdata(Htot,'v');
figure();
plot(Xtot);
hold on
%sound (Xtot./4,Fs);
Xtot = filter(numTot,denTot,Xtot);
plot(Xtot);
sound(Xtot./3, Fs);
freqz(numTot,denTot);

% K1 = 0.1;
% K2 = 0.1;
% 
% figure();
% for K2 = 1
%     for K1 = 0.7
%         Htot = -H1+K1*H2-K2*H3;
%         [numTot, denTot] = tfdata(Htot,'v');
%         [H,w] = freqz(numTot,denTot);
%         plot(w,mag2db(abs(H)));
%         legend(strcat('K1 = ', num2str(K1)));
%         hold on
%     end
% end

B = round(numPB*10000);
A = round(denPB*10000);
figure();
freqz(B,A);
HPB = filt(B,A);
figure();
freqz(1000*numPB,1000*denPB)


y = filtrePB(X1, length(X1));

figure();
plot(y);
hold on
plot(X2);

