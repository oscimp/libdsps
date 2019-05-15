clear all;

addpath("functions/");

fs = 250e6;
ts = 1/fs;
N = 2048;
t = ts * (1:N);
f = fs * (0:N/2-1) / N;
fc = 10e6;

xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin");
phi_th = 2*pi*fc*xtt';

% Power spectrum
P = fft(phi_th(1:N));
PS = 4 * abs((P .* conj(P)) / (N * N));
DBC = 10*log10(PS) - 10*log10(fs/N);
DBC = DBC(50:N/2-50);

s = std(DBC);
m = mean(DBC);

DBC = DBC;

write_binary("oracle_noise_output.bin", [s m]);
