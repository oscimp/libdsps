clear all;

addpath("functions/");

# Pretty Fs/Fc
fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 2.5 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_pretty_output_cos.bin", sig);
sig = 2.5 * sin((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_pretty_output_sin.bin", sig);

# Ugly Fs/Fc
fs = 125e6;
fc = 10e6 + 5e3;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 1.25 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_ugly_output_cos.bin", sig);
sig = 1.25 * sin((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_ugly_output_sin.bin", sig);

# Very ugly Fs/Fc
fs = 578.38e6;
fc = 87.3974e4 + 3.5e2;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 0.525 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_very_ugly_output_cos.bin", sig);
sig = 0.525 * sin((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_nco_very_ugly_output_sin.bin", sig);

