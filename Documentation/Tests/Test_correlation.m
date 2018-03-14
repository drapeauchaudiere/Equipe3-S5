clc
clear
close all

% Noms des fichiers de données
matlab_datafile = 'data_autocorr_matlab.mat';
C1_datafile = 'Correlation_0a9.csv';
C2_datafile = 'Correlation_0a-9.csv';
C3_datafile = 'Correlation_zero.csv';

% Chargement des données des corrélations Matlab
load(matlab_datafile);

C_xcorr = [C1;C2;C3];
C_matlab = [C1_code;C2_code;C3_code];
ref = C_xcorr;

diff_matlab = zeros(3,59);
for loop1 = 1:3
	for loop2 = 1:59
		diff_matlab(loop1,loop2) = (C_matlab(loop1,loop2) - ref(loop1,loop2))/ref(loop1,loop2)*100;
	end
end

% Chargement des données des corrélations C et asm
C1 = csvread(C1_datafile);
C2 = csvread(C2_datafile);
C3 = csvread(C3_datafile);

C_codeC = [C1(1,1:59);C2(1,1:59);C3(1,1:59)];
C_asm = [C1(2,1:59);C2(2,1:59);C3(2,1:59)];
ref = C_matlab;

diff_codeC = zeros(3,59);
diff_asm = zeros(3,59);
for loop1 = 1:3
	for loop2 = 1:59
		diff_codeC(loop1,loop2) = (C_codeC(loop1,loop2) - C_matlab(loop1,loop2))/C_matlab(loop1,loop2)*100;
		diff_asm(loop1,loop2) = (C_asm(loop1,loop2) - C_codeC(loop1,loop2))/C_codeC(loop1,loop2)*100;
	end
end

mean(diff_matlab(1,2:end-1))
mean(diff_matlab(2,2:end-1))
mean(diff_matlab(3,2:end-1))
mean(diff_codeC(1,2:end-1))
mean(diff_codeC(2,2:end-1))
mean(diff_codeC(3,2:end-1))
mean(diff_asm(1,2:end-1))
mean(diff_asm(2,2:end-1))
mean(diff_asm(3,2:end-1))

