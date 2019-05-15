clear all;

addpath("functions/");

fs = 250e6;
f1 = 2e6;
f2 = 10e6;
ts = 1/fs;
N = 2048;
Nfir = 128;

# Decim by 10
D = 10;
n = D * N * 5 + Nfir;
t = ts * (1:n);
f = fs / 10 * (0:N/2-1) / N;

s1 = 2.5 * cos(2 * pi * f1 * t);
s2 = 2.5 * cos(2 * pi * f2 * t);

sig = s1 + s2;

b = load("kaiser128_40");
sigF = filter(b, 1, sig)(Nfir:D:end);

write_binary("oracle_fir_input_decim10.bin", sig);
write_binary("oracle_fir_output_decim10.bin", sigF);

# Decim by 100
D = 100;
n = D * N * 5 + Nfir;
t = ts * (1:n);
f = fs / 10 * (0:N/2-1) / N;

s1 = 2.5 * cos(2 * pi * f1 * t + pi/8);
s2 = 2.5 * cos(2 * pi * f2 * t + pi/11);

sig = s1 + s2;

b = load("kaiser128_40");
sigF = filter(b, 1, sig)(Nfir:D:end);

write_binary("oracle_fir_input_decim100.bin", sig);
write_binary("oracle_fir_output_decim100.bin", sigF);


