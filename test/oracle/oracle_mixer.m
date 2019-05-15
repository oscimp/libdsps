clear all;

addpath("functions/");

fc = 10e6;
fc_nco = 20e6;
fs = 250e6;
N = 2048;
n = 10 * N;

# Double version
sig = 2.5 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
nco_cos = 2.5 * cos((2.0 * pi * fc_nco * (1/fs)) * [1:n]);
nco_sin = 2.5 * sin((2.0 * pi * fc_nco * (1/fs)) * [1:n]);

out_i = sig .* nco_cos;
out_q = sig .* nco_sin;

write_binary("oracle_mixer_input_signal_double.bin", sig);
write_binary("oracle_mixer_input_nco_cos_double.bin", nco_cos);
write_binary("oracle_mixer_input_nco_sin_double.bin", nco_sin);
write_binary("oracle_mixer_output_i_double.bin", out_i);
write_binary("oracle_mixer_output_q_double.bin", out_q);

