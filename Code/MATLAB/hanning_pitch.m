
N = 256;
coeffs = hann(N*2+1);
coeffs = coeffs(2:2:end);

% Écrire le fichier .dat
fileID = fopen('hanning.dat','w');
fprintf(fileID, '#define WINDOWSIZE %d\n', N);
fprintf(fileID, 'const float hanning[WINDOWSIZE] = {\n');
for i=1:N-1
    fprintf(fileID, '    %d,\n', coeffs(i));
end
fprintf(fileID, '    %d\n', coeffs(N));
fprintf(fileID, '};\n');
fclose(fileID);