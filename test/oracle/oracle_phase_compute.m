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
xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin");
phi_th = 2*pi*fc*xtt';

% Power spectrum
S=fft(phi_th);
P=S.*conj(S);
PS_ref =2*abs(P(1:N/2))/(N*fs);

% Géneration du signal
s = 2.5 * cos(2 * pi * fc * t + phi_th(1:n));

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(s, fc, phi_offset, t);

% Chargement des coeff du FIR
b = load('kaiser128_40');

% Etage 1 sans décimation
% Filtrage des I,Q
ifir = filter(b, 1, Ibf)(Nfir+1:n);
qfir = filter(b, 1, Qbf)(Nfir+1:n);

phi = unwrap(atan2(qfir, ifir)(1:N));

write_binary("oracle_phase_compute_ifir.bin", ifir);
write_binary("oracle_phase_compute_qfir.bin", qfir);
write_binary("oracle_phase_compute_output.bin", phi);

