clear all;

addpath("functions/");

fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * (1:n);

sig = 2.5 * cos(2 * pi * fc * t);

% Démodulation
phi_offset = pi/7;
[Ibf, Qbf] = demodulation(sig, fc, phi_offset, t);
demod = Ibf + Qbf .* j;

write_binary("oracle_splitter_input_real.bin", sig);
write_complex_binary("oracle_splitter_input_complex.bin", demod);

