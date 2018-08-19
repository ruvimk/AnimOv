<% 
	function file_get_contents (filename){ 
		var a; 
		var fso= Server.CreateObject ("Scripting.FileSystemObject"); 
		var fStream; 
		if (fso.FileExists (a= filename)){ 
			fStream= fso.OpenTextFile (filename, 1, false, -2); 
		} else { 
			try { 
				if (!fso.FileExists (a= Server.MapPath ("/experimental/js/" + filename))) return ""; 
				fStream= fso.OpenTextFile (a, 1, false, -2); 
			} catch (err){ 
				return ""; 
			} 
		} 
		if (!fso.GetFile (a).Size) return ""; 
		var text= fStream.ReadAll (); 
		fStream.Close (); 
		return text; 
	} 
	function file_put_contents (filename, text){ 
		var fso= new ActiveXObject ("Scripting.FileSystemObject"); 
		var file= fso.CreateTextFile (filename); 
		file.Write (text); 
		file.Close (); 
	} 
%>