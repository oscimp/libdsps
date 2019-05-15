% Lecture dans un fichier binaire de nombre réels
%
% file_name 		Nom du fichier
% form				Format des nombres (double, float, int32...)
%
% val				Valeurs contenu dans le fichier

function val = load_binary(file_name, form = "double")
	f = fopen(file_name, "r");
	if (f == -1)
		error("The file '%s' is not found", file_name);
	endif

	val = fread(f, Inf, form);
	fclose(f);
endfunction

