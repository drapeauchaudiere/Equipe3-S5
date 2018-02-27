clear all
close all
clc

% Signal entrant
[signalEntrant, Fs_signalEntrant] = audioread('note_guitare_LAd.wav');
N_signalEntrant = length(signalEntrant);
N = N_signalEntrant;

% Autocorrélation du signal entrant pour avoir sa fondamentale
C = xcorr(signalEntrant(1:N),signalEntrant(1:N));
 
% N = length(signalEntrant);
% k = -(N-1):(N-1);
% y = 0:4;
% 
% figure(1)
% stem(y, C)

% [maximumx, maximumy] = max(C);

signalEntrant_conjug = flipud(signalEntrant);

nfft = 2^nextpow2(2*N-1);

SE = fft(signalEntrant,nfft);
SE_C = fft(signalEntrant_conjug,nfft);

CORREL = SE .* SE_C;

correl = ifft(CORREL);

%% Méthode avec l'équation mathématique (très très long)
% for i = 1:N
%    
%    somme_Ci = 0;
%    for j = 1:i
%         x = signalEntrant(j);
%         y = signalEntrant(N-i+j);
%         somme_Ci = somme_Ci + x * y;
%    end
%    
%    C_code(i) = somme_Ci;   
%       
% end
% 
% for i = N-1:-1:1
%    
%     somme_Ci = 0;
%     for j = 0:i-1
%         x = signalEntrant(N-j);
%         y = signalEntrant(i-j);
%         somme_Ci = somme_Ci + x * y;
%     end
%     
%     C_code(N+(N-i)) = somme_Ci;
% end

