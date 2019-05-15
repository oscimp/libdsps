% Ecriture dans un fichier binaire de nombres complexes
%
% file_name 		Nom du fichier
% val				Valeurs à écrire dans le fichier
% form				Format des nombres (double, float, int32...)

function write_complex_binary(file_name, val, form = "double")
	f = fopen(file_name, "w");
	for it = (1:length(val))
		fwrite(f, real(val(it)), form);
		fwrite(f, imag(val(it)), form);
	endfor
	fclose(f);
endfunction

