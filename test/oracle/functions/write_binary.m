% Ecriture dans un fichier binaire
%
% file_name 		Nom du fichier
% val				Valeurs à écrire dans le fichier
% form				Format des nombres (double, float, int32...)

function write_binary(file_name, val, form = "double")
	f = fopen(file_name, "w");
	fwrite(f, val, form);
	fclose(f);
endfunction

