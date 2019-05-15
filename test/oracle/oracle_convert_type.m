clear all;

addpath("functions/");

fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 10* N;
t = ts * (1:n);

% Generate the signal
sig = 2.5 * cos(2 * pi * fc * t);
write_binary("oracle_convert_type_real.bin", sig);

% Create the output for 8 16 24 32 bits
nob = [8 16 24 32];
for it = nob
	sig_uni = sig ./ max(abs(sig));
	sig_int = round(sig_uni .* (power(2, it-1) - 1));
	filename = sprintf("oracle_convert_type_int%02u.bin", it);
	write_binary(filename, sig_int, "int64");
	filename = sprintf("oracle_convert_type_real%02u.bin", it);
	write_binary(filename, sig_int);
endfor

