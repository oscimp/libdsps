clear all;

addpath("functions/");

fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * (1:n);

% chargement du bruit + convertion en phase
xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin");
phi_th = 2*pi*fc*xtt';

PHI = [];
for it = 0:9
	PHI = [PHI fft(phi_th(it*N+1:it*N+N))(1:N/2) 0*(1:N/2)];
endfor

write_binary("oracle_fft_input.bin", phi_th);
write_complex_binary("oracle_fft_output.bin", PHI);

