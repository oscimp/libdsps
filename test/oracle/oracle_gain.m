clear all;

addpath("functions/");

fs = 150e6;
fc = 10e6;
N = 2048;
n = 10 * N;

# real
in = cos(2 * pi * fc * (1 / fs) * [1:n]);
out_pos = 2.1337 .* in;
out_neg = 0.421337 .* in;

write_binary("oracle_gain_input_real.bin", in);
write_binary("oracle_gain_output_positive_real.bin", out_pos);
write_binary("oracle_gain_output_negative_real.bin", out_neg);

# complex
in = cos(2 * pi * fc * (1 / fs) * [1:n]) + i * sin(2 * pi * fc * (1 / fs) * [1:n]);
out_pos = 2.1337 .* in;
out_neg = 0.421337 .* in;

write_complex_binary("oracle_gain_input_complex.bin", in);
write_complex_binary("oracle_gain_output_positive_complex.bin", out_pos);
write_complex_binary("oracle_gain_output_negative_complex.bin", out_neg);

