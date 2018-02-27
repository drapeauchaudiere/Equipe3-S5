clc
clear
close all

% Noms des fichiers de données
matlab_datafile = 'data_autocorr_matlab.mat';


% Chargement des données
load(matlab_datafile);

diff_matlab = (mean(C) - mean(C_code))/mean(C_code) * 100;
