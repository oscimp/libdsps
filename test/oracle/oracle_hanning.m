clear all;

addpath("functions/");

fs = 250e6;
fc = 10e6;
N = 2048;

% chargement du bruit + convertion en phase
xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin");
phi_th = 2*pi*fc*xtt';

phi_w = [];
for it = 0:9
	phi_w = [phi_w (hanning(N) .* phi_th'(it*N+1:it*N+N))'];
endfor

write_binary("oracle_hanning_input.bin", phi_th);
write_binary("oracle_hanning_output.bin", phi_w);

