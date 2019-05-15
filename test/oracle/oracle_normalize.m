clear all;

addpath("functions/");

fs = 250e6;
ts = 1/fs;
N = 2048;
Nfir = 128;
n = N + Nfir;
t = ts * (1:n);
f = fs * (0:N/2-1) / N;
fc = 10e6;
D = 10;

% chargement du bruit + convertion en phase
xtt_dut = load_binary("oracle--160-sr250-fc10-20480.xtt.bin");
phi_th = 2*pi*fc*xtt_dut';

AUTO = [];
PS_total = [];
for it = 0:9
	phi_w = hanning(N)' .* phi_th(it*N+1:(it+1)*N);
	PHI = fft(phi_w)(1:N/2);

	% Auto-spectrum
	a = real(PHI);
	b = imag(PHI);
	c = real(PHI);
	d = imag(PHI);
	PS_total = [PS_total (a .* c + b .* d) 0*(1:N/2)];
	PS = 8 * abs((a .* c + b .* d) / (N*N));
	AUTO = [AUTO (10*log10(PS) - 10*log10(1.5) - 10*log10(fs/N)) 0*(1:N/2)];
endfor

write_binary("oracle_normalize_input.bin", PS_total);
write_binary("oracle_normalize_output.bin", AUTO);

