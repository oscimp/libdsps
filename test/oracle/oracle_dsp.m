clear all;

addpath("functions/");

fs = 250e6;
ts = 1/fs;
N = 2048;
Nfir = 128;
D = 10;
n = N * D + Nfir;
t = ts * (1:n);
f = fs * (0:N/2-1) / N;
fc = 10e6;

% chargement du bruit + convertion en phase
xtt_dut = load_binary("oracle--160-sr250-fc10-20608.xtt.bin");

% Channel 1
xtt_ch1 = load_binary("oracle--140-sr250-fc10-20608-ch1.xtt.bin");
phi_th = 2*pi*fc*(xtt_ch1' + xtt_dut');

% Géneration du signal
s = 2.5 * cos(2 * pi * fc * t + phi_th(1:n));

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(s, fc, phi_offset, t);

% Chargement des coeff du FIR
b = load('kaiser128_40');

% Etage 1
% Filtrage des I,Q
ifir = filter(b, 1, Ibf)(Nfir:D:n);
qfir = filter(b, 1, Qbf)(Nfir:D:n);

phi = unwrap(atan2(qfir, ifir)(1:N));

phi_w = hanning(N)' .* phi;

PHI1 = fft(phi_w)(1:N/2);

% Channel 2
xtt_ch2 = load_binary("oracle--140-sr250-fc10-20608-ch2.xtt.bin");
phi_th = 2*pi*fc*(xtt_ch2' + xtt_dut');

% Géneration du signal
s = 2.5 * cos(2 * pi * fc * t + phi_th(1:n));

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(s, fc, phi_offset, t);

% Chargement des coeff du FIR
b = load('kaiser128_40');

% Etage 2
% Filtrage des I,Q
ifir = filter(b, 1, Ibf)(Nfir:D:n);
qfir = filter(b, 1, Qbf)(Nfir:D:n);

phi = unwrap(atan2(qfir, ifir)(1:N));

phi_w = hanning(N)' .* phi;

PHI2 = fft(phi_w)(1:N/2);

% Cross-spectrum
a = real(PHI1);
b = imag(PHI1);
c = real(PHI2);
d = imag(PHI2);
CROSS = a .* c + b .* d;

PS = 8 * abs(CROSS / (N*N));
PSD = [10*log10(PS) - 10*log10(1.5) - 10 * log10(fs / D / N)];

write_binary("oracle_dsp_output.bin", [mean(PSD) std(PSD)]);
