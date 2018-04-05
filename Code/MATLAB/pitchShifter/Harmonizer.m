clear all
close all
clc

[x,Fs] = audioread('test.wav');
x = x(:,1);
% n = 1:50*1024;
% x = 0.1*cos(2*pi*440/Fs*n); 
y = pitchShift(x,1024,256/2,-8); % modifier le dernier paramètre pour changer le pitch du son. negatif = son plus bas. positif = son plus haut 

sound (y',Fs);

