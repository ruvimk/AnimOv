var pack_new_anim= true; 
function RenderCommands (text, pack, cmnd){ 
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
	var time; 
	var last_cmd= 0; 
	var b01; 
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
		a= b[0].split ("\t"); 
		if (parseInt (a[0])) last_cmd= parseInt (a[0]); 
		if (a.length == 2) a[0]= a[1]; 
		b= a[0]; 
		a= b.split (" "); 
		cmd= a[0]; 
		type= a[1]; 
		one= a[2]; 
		two= a[3]; 
		at= parseInt (b.split ("@")[1]); 
		time= parseFloat (b.split ("$")[1]); 
		if (!time && time !== 0) time= ((at || 1) - 1) / pack.fps; 
		// document.body.appendChild (document.createTextNode ("command { " + a.join (" ") + " } at " + time + "; ")); 
		if (cmd == "cam"){ 
			EnsurePathBin (pack.vctbin, "CAM", a[1], time, false); 
		} else if (cmd == "fps"){ 
			pack.fps= parseFloat (a[1]); 
		} else if (cmd == "len"){ 
			pack.len= parseFloat (a[1]); 
		} else if (cmd == "set"){ 
			if (!at) continue; 
			if (type == "figure"){ 
				if (!one) continue; 
				one= FigurePath (one); 
				if (pack){ 
					two= parseFloat (two); 
					if (isNaN (two)) continue; 
					EnsurePathBin (pack.vctbin, one, two, time, false); 
				} 
			} else if (type == "camera"){ 
				if (!one) continue; 
				if (pack){ 
					two= parseFloat (two); 
					if (isNaN (two)) continue; 
					EnsurePathBin (pack.vctbin, "[CAM]" + one, two, time, false); 
				} 
			} else if (type == "background"){ 
				if (pack){ 
					if (!one) one= ""; 
					/*pk= new PackModule (); 
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
					} */
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
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= RetrieveParent (f.figures, one); 
						f[two]= f[next_path_node]; 
						if (next_path_node != two) delete f[next_path_node]; 
						f[two].name= two; 
						f[two].path= NewName (f[two].path, two); 
						ReplacePaths (f[two], one, two); 
					} */
					for (j= 0; pack.vctbin[j]; j += 2){ 
						if (pack.vctbin[j] == one){ 
							pack.vctbin[j]= NewName (one, two); 
						} else if (!pack.vctbin[j].split (one + ".")[0]){ 
							pack.vctbin[j]= pack.vctbin[j].split (one).join (NewName (one, two)); 
						} 
					} 
					for (j= 0; pack.figures[j]; j++){ 
						if (pack.figures[j] == one){ 
							pack.figures[j]= NewName (one, two); 
						} else if (!pack.figures[j].split (one + ".")[0]){ 
							pack.figures[j]= pack.figures[j].split (one).join (NewName (one, two)); 
						} 
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
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= f.cameras; 
						if (!f[one]) continue; 
						f[two]= f[one]; 
						if (one != two) delete f[one]; 
						f[two].name= two; 
					} */
					one= "[CAM]" + one; 
					for (j= 0; pack.vctbin[j]; j += 2){ 
						if (pack.vctbin[j] == one){ 
							pack.vctbin[j]= NewName (one, two); 
						} else if (!pack.vctbin[j].split (one + ".")[0]){ 
							pack.vctbin[j]= pack.vctbin[j].split (one).join (NewName (one, two)); 
						} 
					} 
					for (j= 0; pack.caml[j]; j++){ 
						if (pack.caml[j] == one){ 
							pack.caml[j]= NewName (one, two); 
						} else if (!pack.caml[j].split (one + ".")[0]){ 
							pack.caml[j]= pack.caml[j].split (one).join (NewName (one, two)); 
						} 
					} 
				} 
			} 
		} else if (cmd == "rna"){ 
			if (type == "figure"){ 
				if (!one) continue; 
				if (!two) continue; 
				if (ctype == "figure" && c){ 
					one= FigurePath (one); 
					one2= FigurePath (c[2]); 
					if (cc == "set"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName2 (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName2 (one, two)); 
						} 
					} else if (cc == "ren"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName2 (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName2 (one, two)); 
						} 
					} else if (cc == "del"){ 
						d= one2.split (one + "."); 
						if (!d[0]){ 
							one2= c[2]= d.join (NewName2 (one, two) + "."); 
						} 
						d= one2.split (one); 
						if (!d[0] && !d[1]){ 
							one2= c[2]= d.join (NewName2 (one, two)); 
						} 
					} 
				} 
				if (pack){ 
					one= FigurePath (one); 
					two= FigurePath (two); 
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= RetrieveParent (f.figures, one); 
						f[two]= f[next_path_node]; 
						if (next_path_node != two) delete f[next_path_node]; 
						f[two].name= two; 
						f[two].path= NewName (f[two].path, two); 
						ReplacePaths (f[two], one, two); 
					} */
					for (j= 0; pack.vctbin[j]; j += 2){ 
						if (pack.vctbin[j] == one){ 
							pack.vctbin[j]= NewName2 (one, two); 
						} else if (!pack.vctbin[j].split (one + ".")[0]){ 
							pack.vctbin[j]= pack.vctbin[j].split (one).join (NewName2 (one, two)); 
						} 
					} 
					for (j= 0; pack.figures[j]; j++){ 
						if (pack.figures[j] == one){ 
							pack.figures[j]= NewName2 (one, two); 
						} else if (!pack.figures[j].split (one + ".")[0]){ 
							pack.figures[j]= pack.figures[j].split (one).join (NewName2 (one, two)); 
						} 
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
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (!(f= pack.frames[j])) continue; 
						f= f.cameras; 
						if (!f[one]) continue; 
						f[two]= f[one]; 
						if (one != two) delete f[one]; 
						f[two].name= two; 
					} */
					one= "[CAM]" + one; 
					for (j= 0; pack.vctbin[j]; j += 2){ 
						if (pack.vctbin[j] == one){ 
							pack.vctbin[j]= NewName (one, two); 
						} else if (!pack.vctbin[j].split (one + ".")[0]){ 
							pack.vctbin[j]= pack.vctbin[j].split (one).join (NewName (one, two)); 
						} 
					} 
					for (j= 0; pack.caml[j]; j++){ 
						if (pack.caml[j] == one){ 
							pack.caml[j]= NewName (one, two); 
						} else if (!pack.caml[j].split (one + ".")[0]){ 
							pack.caml[j]= pack.caml[j].split (one).join (NewName (one, two)); 
						} 
					} 
				} 
			} 
		} else if (cmd == "new"){ 
			if (!one) continue; 
			if (type == "figure"){ 
				if (pack){ 
					one= FigurePath (one); 
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (pack.frames[j]) EnsureFigure (pack.frames[j].figures, one); 
					} */
					b01= false; 
					for (j= 0; pack.figures[j]; j++) if (FigurePath (pack.figures[j]) == one) b01= true; 
					if (!b01) pack.figures.push (one); 
				} 
			} else if (type == "camera"){ 
				if (pack){ 
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (pack.frames[j]) EnsureCamera (pack.frames[j].cameras, one); 
					} */
					b01= false; 
					for (j= 0; pack.caml[j]; j++) if (pack.caml[j] == one) b01= true; 
					if (!b01) pack.caml.push (one); 
				} 
			} else if (type == "frame"){ 
				if (pack){ 
					/*n= parseInt (one); 
					if (n){ 
						pk= new PackModule (); 
						pk.pack= pack; 
						f= pk.GetKeyFrame (n); 
						pack.frames[n - 1]= f; 
					} */
				} 
			} 
		} else if (cmd == "del"){ 
			if (!one) continue; 
			if (type == "figure"){ 
				if (pack){ 
					one= FigurePath (one); 
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (f= pack.frames[j]){ 
							f= RetrieveParent (f.figures, one); 
							if (f) delete f[next_path_node]; 
						} 
					} */
					for (j= 0; pack.figures[j]; j++){ 
						if (pack.figures[j] == one || !pack.figures[j].split (one + ".")[0]){ 
							pack.figures.splice (j, 1); 
							j--; 
						} 
					} 
					DeletePropertiesBin (pack.vctbin, one); 
				} 
			} else if (type == "camera"){ 
				if (pack){ 
					/*for (j= 0; j < pack.frames.length; j++){ 
						if (f= pack.frames[j]){ 
							f= f.cameras; 
							delete f[one]; 
						} 
					} */
					for (j= 0; pack.caml[j]; j++){ 
						if (pack.caml[j] == one){ 
							pack.caml.splice (j, 1); 
							j--; 
						} 
					} 
					DeletePropertiesBin (pack.vctbin, "[CAM]" + one); 
				} 
			} else if (type == "frame"){ 
				if (pack){ 
					n= parseInt (one); 
					if (n){ 
						f= pack.frames; 
						delete f[n - 1]; 
					} 
				} 
			} else if (type == "key"){ 
				if (pack){ 
					one= FigurePath (one); 
					EnsurePathBin (pack.vctbin, one, 0, time, true); 
				} 
			} 
		} 
	} 
	return [last_cmd, (c || []).join (" ") + "<stop>"]; 
} 
function EnsurePathBin (buff, path, value, at, del_key){ 
	var buf; 
	var i; 
	var j; 
	var rAt= Math.floor (at * 100); 
	for (i= 0; buff[i]; i += 2){ 
		if (buff[i] != path) continue; 
		if (!(buf= buff[i+1])) buf= buff[i+1]= []; 
		for (j= 0; buf[j] || buf[j] === 0; j += 2){ 
			if (Math.floor (buf[j] * 100) != rAt) continue; 
			if (value || value === 0 || value === ""){ 
				buf[j+1]= value; 
			} else if (del_key){ 
				buf.splice (j, 2); 
			} 
			return buff; 
		} 
		if (value || value === 0 || value === ""){ 
			buf.push (at, value); 
		} 
		return buff; 
	} 
	if (value || value === 0 || value === ""){ 
		buff.push (path, [at, value]); 
	} 
	return buff; 
} 
function DeletePropertiesBin (buff, path){ 
	var i; 
	for (i= 0; buff[i]; i += 2){ 
		if (buff[i] == path || !buff[i].split (path + ".")[0]){ 
			buff.splice (i, 2); 
			i -= 2; 
		} 
	} 
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
function NewName2 (path, name){ 
	return name; 
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
	while ((i + 1 < arr.length) && (obj= (obj[arr[i]] || new Object ()))){ 
		prev_obj= obj; 
		if (!obj.x) obj.x= 0; if (!obj.y) obj.y= 0; if (!obj.z) obj.z= 0; 
		if (!obj.aboutX) obj.aboutX= 0; if (!obj.aboutY) obj.aboutY= 0; if (!obj.aboutZ) obj.aboutZ= 0; 
		if (!obj.net) obj.net= []; 
		if (!obj.opc && obj.opc !== 0) obj.opc= 1; 
		i++; 
	} 
	next_path_node= arr[i]; 
	return prev_obj; 
} 