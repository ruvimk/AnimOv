<% 
function RenderCommands (text, pack, cmnd, clbk){ 
	var arr; 
	var pk; 
	var a; 
	var b; 
	var c; 
	var d; 
	var f; 
	var i; 
	var j; 
	var n; 
	var cc= ""; 
	var cmd; 
	var type; 
	var ctype= ""; 
	var one2; 
	var one; 
	var two; 
	var at; 
	var last_cmd= 0; 
	c= 0; 
	if (cmnd){ 
		a= cmnd.split ("<stop>"); 
		if (a.length > 1){ 
			a= a[0].split ("\t"); 
			if (a.length > 1) a[0]= a[1]; 
			c= a[0].split (" "); 
			cc= a[0]; 
			ctype= a[1]; 
		} 
	} 
	a= text.split ("<end of message>"); 
	if (a.length == 1) return false; 
	arr= a[0].split ("\r\n"); 
	for (i= 0; i < arr.length; i++){ 
		b= arr[i].split ("<stop>"); 
		if (b.length == 1) continue; 
		if (clbk){ 
			clbk (i, arr.length); 
		} 
		a= b[0].split ("\t"); 
		last_cmd= parseInt (a[0]); 
		if (a.length == 2) a[0]= a[1]; 
		b= a[0]; 
		a= b.split (" "); 
		cmd= a[0]; 
		type= a[1]; 
		one= a[2]; 
		two= a[3]; 
		at= parseInt (b.split ("@")[1]); 
		if (cmd == "set"){ 
			if (!at) continue; 
			if (type == "figure"){ 
				if (!one) continue; 
				one= FigurePath (one); 
				if (pack){ 
					two= parseFloat (two); 
					if (isNaN (two)) continue; 
					pk= new PackModule (); 
					pk.pack= pack; 
					f= pk.GetKeyFrame (at); 
					pack.frames[at - 1]= f; 
					f= RetrieveParent (f.figures, one); 
					f[next_path_node]= two; 
				} 
			} else if (type == "camera"){ 
				if (!one) continue; 
				if (pack){ 
					two= parseFloat (two); 
					if (isNaN (two)) continue; 
					pk= new PackModule (); 
					pk.pack= pack; 
					f= pk.GetKeyFrame (at); 
					pack.frames[at - 1]= f; 
					f= RetrieveParent (f.cameras, one); 
					f[next_path_node]= two; 
				} 
			} else if (type == "background"){ 
				if (pack){ 
					if (!one) one= ""; 
					pk= new PackModule (); 
					pk.pack= pack; 
					f= pk.GetKeyFrame (at); 
					pack.frames[at - 1]= f; 
					if (one.charAt (0) == "#"){ 
						f.properties.bgcolor= one; 
						f.properties.bgimage= ""; 
					} else if (!one.split ("RGB")[0]){ 
						f.properties.bgcolor= one; 
						f.properties.bgimage= ""; 
					} else { 
						f.properties.bgcolor= ""; 
						f.properties.bgimage= one; 
					} 
				} 
			} 
		} else if (cmd == "ren"){ 
			if (type == "figure"){ 
				if (!one) continue; 
				if (!two) continue; 
				if (ctype == "figure" && c){ 
					one= FigurePath (one); 
					one2= FigurePath (c[2]); 
					if (cc == "set"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} else if (cc == "ren"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} else if (cc == "del"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} 
				} 
				if (pack){ 
					one= FigurePath (one); 
					for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= RetrieveParent (f.figures, one); 
						f[two]= f[next_path_node]; 
						if (next_path_node != two) delete f[next_path_node]; 
						f[two].name= two; 
						f[two].path= NewName (f[two].path, two); 
						ReplacePaths (f[two], one, two); 
					} 
				} 
			} else if (type == "camera"){ 
				if (ctype == "camera" && c){ 
					one2= c[2]; 
					if (cc == "set"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} else if (cc == "ren"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} else if (cc == "del"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName (one, two)); 
						} 
					} 
				} 
				if (pack){ 
					for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= f.cameras; 
						if (!f[one]) continue; 
						f[two]= f[one]; 
						if (one != two) delete f[one]; 
						f[two].name= two; 
					} 
				} 
			} 
		} else if (cmd == "new"){ 
			if (!one) continue; 
			if (type == "figure"){ 
				if (pack){ 
					one= FigurePath (one); 
					for (j= 0; j < pack.frames.length; j++){ 
						if (pack.frames[j]) EnsureFigure (pack.frames[j].figures, one); 
					} 
				} 
			} else if (type == "camera"){ 
				if (pack){ 
					for (j= 0; j < pack.frames.length; j++){ 
						if (pack.frames[j]) EnsureCamera (pack.frames[j].cameras, one); 
					} 
				} 
			} else if (type == "frame"){ 
				if (pack){ 
					n= parseInt (one); 
					if (n){ 
						pk= new PackModule (); 
						pk.pack= pack; 
						f= pk.GetKeyFrame (n); 
						pack.frames[n - 1]= f; 
					} 
				} 
			} 
		} else if (cmd == "del"){ 
			if (!one) continue; 
			if (type == "figure"){ 
				if (pack){ 
					one= FigurePath (one); 
					for (j= 0; j < pack.frames.length; j++){ 
						if (f= pack.frames[j]){ 
							f= RetrieveParent (f.figures, one); 
							if (f) delete f[next_path_node]; 
						} 
					} 
				} 
			} else if (type == "camera"){ 
				if (pack){ 
					for (j= 0; j < pack.frames.length; j++){ 
						if (f= pack.frames[j]){ 
							f= f.cameras; 
							delete f[one]; 
						} 
					} 
				} 
			} else if (type == "frame"){ 
				if (pack){ 
					n= parseInt (one); 
					if (n){ 
						f= pack.frames; 
						delete f[n - 1]; 
					} 
				} 
			} 
		} 
	} 
	return [last_cmd, (c || []).join (" ") + "<stop>"]; 
} 
var next_path_node= ""; 
function FigurePath (text){ 
	var a= text.split ("."); 
	if (a[0] == "figures") a.shift (); 
	return a.join ("."); 
} 
function NewName (path, name){ 
	var arr= path.split ("."); 
	var c= arr.length; 
	if (!c) return name; 
	arr[c - 1]= name; 
	return arr.join ("."); 
} 
function EnsureCamera (cameras, name){ 
	var ref; 
	if (!cameras[name]){ 
		ref= cameras[name]= new Object (); 
		ref.name= name; 
		ref.x= 0; 
		ref.y= -1000; 
		ref.z= 0; 
		ref.aboutX= ref.aboutY= ref.aboutZ= 0; 
		ref.aspectRatio= "4:3"; 
		ref.rh= 0.7853981634; 
		ref.rv= 0.589048622548; 
	} else { 
		ref= cameras[name]; 
		if (!ref.name) ref.name= name; 
		if (!ref.x) ref.x= 0; 
		if (!ref.y && ref.y !== 0) ref.y= -1000; 
		if (!ref.z) ref.z= 0; 
		if (!ref.aboutX) ref.aboutX= 0; 
		if (!ref.aboutY) ref.aboutY= 0; 
		if (!ref.aboutZ) ref.aboutZ= 0; 
		if (!ref.aspectRatio) ref.aspectRatio= "4:3"; 
		if (!ref.rh) ref.rh= 0.7853981634; 
		if (!ref.rv) ref.rv= 0.589048622548; 
	} 
} 
function EnsureFigure (figures, path){ 
	var prev_p= figures; 
	var ref; 
	var a= path.split ("."); 
	var p= figures; 
	var i= 0; 
	var path_so_far= "figures"; 
	while ((i < a.length) && (p= p[a[i]])){ 
		path_so_far= p.path; 
		prev_p= p; 
		i++; 
	} 
	for (p= prev_p; i < a.length; i++){ 
		path_so_far += "." + a[i]; 
		ref= p[a[i]]= new Object (); 
		ref.name= a[i]; 
		ref.path= path_so_far; 
		ref.x= ref.y= ref.z= 0; 
		ref.aboutX= ref.aboutY= ref.aboutZ= 0; 
		ref.net= []; 
		ref.opc= 1; 
		p= p[a[i]]; 
	} 
} 
function ReplacePaths (obj, from, to){ 
	var a; 
	var i; 
	for (i in obj){ 
		if (!obj[i].path) continue; 
		a= obj[i].path.split (from); 
		if ((!a[0] || a[0] == "figures.") && (!a[1] || a[1].charAt (0) == ".")){ 
			obj[i].path= a.join (to); 
		} 
		ReplacePaths (obj[i], from, to); 
	} 
} 
function RetrieveParent (start_object, path){ 
	var prev_obj= start_object; 
	var arr= path.split ("."); 
	var obj= start_object; 
	var i= 0; 
	while ((i + 1 < arr.length) && (obj= obj[arr[i]])){ 
		prev_obj= obj; 
		i++; 
	} 
	next_path_node= arr[i]; 
	return prev_obj; 
} 
%>