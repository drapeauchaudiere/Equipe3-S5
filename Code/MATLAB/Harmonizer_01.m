clear all
close all
clc

[x,Fs] = audioread('test.wav');
figure(10)
plot(x)
x = x(:,1);
 n = 0:511;
 x = 0.1*sin(2*pi*440/Fs*n); 
%tram1 = [ones(1,128),zeros(1,128),ones(1,128),zeros(1,128)];
y = pitchShift_01(x,256,256/8,12); % modifier le dernier paramètre pour changer le pitch du son. negatif = son plus bas. positif = son plus haut 

figure(1)
plot(y)

