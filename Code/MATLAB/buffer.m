
N = 2048;
n_ech = 0:N-1;
bufferVals = 10*sin(2*pi*n_ech*440/44100);

% Écrire le fichier .dat
fileID = fopen('buffer.dat','w');
fprintf(fileID, '#define SAMPLES %d\n', N);
fprintf(fileID, 'const float buffer[SAMPLES] = {\n');
for i=1:N-1
    fprintf(fileID, '    %d,\n', bufferVals(i));
end
fprintf(fileID, '    %d\n', bufferVals(N));
fprintf(fileID, '};\n');
fclose(fileID);