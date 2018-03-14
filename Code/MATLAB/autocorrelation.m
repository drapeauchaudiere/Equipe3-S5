clear all
close all
clc

datafile = '../../Documentation/Tests/data_autocorr_matlab.mat';

% Signal entrant
% [signalEntrant, Fs_signalEntrant] = audioread('note_guitare_LAd.wav');
% N_signalEntrant = length(signalEntrant);
signalEntrant1 = [0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9];
signalEntrant2 = [0 -1 -2 -3 -4 -5 -6 -7 -8 -9 0 -1 -2 -3 -4 -5 -6 -7 -8 -9 0 -1 -2 -3 -4 -5 -6 -7 -8 -9];
signalEntrant3 = zeros(1,30);
N = 30;

%% Autocorrélation avec xcorr
C1 = xcorr(signalEntrant1(1:N),signalEntrant1(1:N));
C2 = xcorr(signalEntrant2(1:N),signalEntrant2(1:N));
C3 = xcorr(signalEntrant3(1:N),signalEntrant3(1:N));
 

%% Autocorrelation avec FFT 
% signalEntrant_conjug = flipud(signalEntrant);
% 
% nfft = 2^nextpow2(2*N-1);
% 
% SE = fft(signalEntrant,nfft);
% SE_C = fft(signalEntrant_conjug,nfft);
% 
% CORREL = SE .* SE_C;
% 
% correl = ifft(CORREL);

%% Méthode avec l'équation mathématique

C1_code = zeros(1,N);
C2_code = zeros(1,N);
C3_code = zeros(1,N);
% Signal 1
for i = 1:N
   
   somme_Ci = 0;
   for j = 1:i
        x = signalEntrant1(j);
        y = signalEntrant1(N-i+j);
        somme_Ci = somme_Ci + x * y;
   end
   
   C1_code(i) = somme_Ci;   
      
end

for i = N-1:-1:1
   
    somme_Ci = 0;
    for j = 0:i-1
        x = signalEntrant1(N-j);
        y = signalEntrant1(i-j);
        somme_Ci = somme_Ci + x * y;
    end
    
    C1_code(N+(N-i)) = somme_Ci;
end

% Signal 2
for i = 1:N
   
   somme_Ci = 0;
   for j = 1:i
        x = signalEntrant2(j);
        y = signalEntrant2(N-i+j);
        somme_Ci = somme_Ci + x * y;
   end
   
   C2_code(i) = somme_Ci;   
      
end

for i = N-1:-1:1
   
    somme_Ci = 0;
    for j = 0:i-1
        x = signalEntrant2(N-j);
        y = signalEntrant2(i-j);
        somme_Ci = somme_Ci + x * y;
    end
    
    C2_code(N+(N-i)) = somme_Ci;
end

% Signal 3
for i = 1:N
   
   somme_Ci = 0;
   for j = 1:i
        x = signalEntrant3(j);
        y = signalEntrant3(N-i+j);
        somme_Ci = somme_Ci + x * y;
   end
   
   C3_code(i) = somme_Ci;   
      
end

for i = N-1:-1:1
   
    somme_Ci = 0;
    for j = 0:i-1
        x = signalEntrant3(N-j);
        y = signalEntrant3(i-j);
        somme_Ci = somme_Ci + x * y;
    end
    
    C3_code(N+(N-i)) = somme_Ci;
end

save(datafile);
