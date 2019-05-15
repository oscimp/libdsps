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

% Channel 1
xtt_ch1 = load_binary("oracle--140-sr250-fc10-20480-ch1.xtt.bin");
phi_th = 2*pi*fc*(xtt_ch1' + xtt_dut');

% Géneration du signal
s = 2.5 * cos(2 * pi * fc * t + phi_th(1:n));

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(s, fc, phi_offset, t);

% Chargement des coeff du FIR
b = load('kaiser128_40');

% Etage 1 sans décimation
% Filtrage des I,Q
ifir = filter(b, 1, Ibf)(Nfir:n);
qfir = filter(b, 1, Qbf)(Nfir:n);

phi = unwrap(atan2(qfir, ifir)(1:N));

phi_w = hanning(N)' .* phi;

PHI1 = fft(phi_w)(1:N);

% Channel 2
xtt_ch2 = load_binary("oracle--140-sr250-fc10-20480-ch2.xtt.bin");
phi_th = 2*pi*fc*(xtt_ch2' - xtt_dut');

% Géneration du signal
s = 2.5 * cos(2 * pi * fc * t + phi_th(1:n));

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(s, fc, phi_offset, t);

% Chargement des coeff du FIR
b = load('kaiser128_40');

% Etage 1 sans décimation
% Filtrage des I,Q
ifir = filter(b, 1, Ibf)(Nfir:n);
qfir = filter(b, 1, Qbf)(Nfir:n);

phi = unwrap(atan2(qfir, ifir)(1:N));

phi_w = hanning(N)' .* phi;

PHI2 = fft(phi_w)(1:N);

% Cross-spectrum
a = real(PHI1)(1:N/2);
b = imag(PHI1)(1:N/2);
c = real(PHI2)(1:N/2);
d = imag(PHI2)(1:N/2);
CROSS = [(a .* c + b .* d) 0*(1:N/2)];

% Auto-spectrum
a = real(PHI1)(1:N/2);
b = imag(PHI1)(1:N/2);
c = real(PHI1)(1:N/2);
d = imag(PHI1)(1:N/2);
AUTO = [(a .* c + b .* d) 0*(1:N/2)];

write_complex_binary("oracle_cross_spectrum_ch1.bin", PHI1);
write_complex_binary("oracle_cross_spectrum_ch2.bin", PHI2);
write_binary("oracle_cross_spectrum_output_cross.bin", CROSS);
write_binary("oracle_cross_spectrum_output_auto.bin", AUTO);

