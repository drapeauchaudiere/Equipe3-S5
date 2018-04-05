
N = 512;
bufferVals = [ones(1,128),zeros(1,128),ones(1,128),zeros(1,128)];

% Écrire le fichier .dat
fileID = fopen('buffer.dat','w');
fprintf(fileID, '#define SAMPLES %d\n', N);
fprintf(fileID, 'const short buffer[SAMPLES] = {\n');
for i=1:N-1
    fprintf(fileID, '    %d,\n', bufferVals(i));
end
fprintf(fileID, '    %d\n', bufferVals(N));
fprintf(fileID, '};\n');
fclose(fileID);