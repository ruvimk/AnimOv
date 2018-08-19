<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
	// something (); 
	function RequestParam (name){ 
		var a; 
		var b; 
		a= Request.Form(name) + ""; 
		b= Request.QueryString(name) + ""; 
		if (a == "undefined") return b; 
		else return a; 
	} 
	// file_put_contents (Server.MapPath ("/rt1a.txt"), "Stopping write session... "); 
	Session.Abandon (); 
	%><!--#include file="VirtualDOM.asp"--><% 
	%><!--#include virtual="/script.asp"--><% 
	%><!--#include file="RenderCommands.asp"--><% 
	if (!FourDigit){ 
		var FourDigit= function (n){ 
			var txt= n + ""; 
			var i; 
			var l; 
			l= 4 - txt.length; 
			for (i= 0; i < l; i++) txt= "0" + txt; 
			return txt; 
		}; 
	} 
	var id= parseInt (RequestParam ("id")); 
	var can; 
	var con; 
	var main_w= parseInt (RequestParam ("w")) || 640; 
	var main_h= parseInt (RequestParam ("h")) || 480; 
	var EnableToon= parseInt (RequestParam ("EnableToon")); 
	var EnableMesh= parseInt (RequestParam ("EnableMesh")); 
	var buffer= new Array (); 
	var text= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".xml")); 
	if (text){ 
	var text2; 
	var i= (parseInt (RequestParam ("f")) - 1) || 0; 
	var j; 
	var k; 
	var l; 
	var c; 
	var e; 
	var m= new ToonModule (); 
	m.SetDimensions (main_w, main_h); 
	m.LoadXML (text); 
	delete text; 
	//text2= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".aoc")); 
	// RenderCommands (text2, m.GetPack (), null, function (numerator, denominator){ 
		// file_put_contents (Server.MapPath ("/AnimOv/data/progress" + id + ".txt"), numerator + "/" + denominator + ":c;msg:Compiling commands... "); 
	// }); 
	var text= ""; 
	var frame; 
	var objColor= [255, 255, 255]; 
	var objOutline= [0, 0, 0]; 
	var objLine= 1; 
	var objVector= ""; 
	var sAt= [0, 0, 0]; 
	var sAbout= [0, 0, 0]; 
	var sAxes= [0, 0, 0]; 
	var color; 
	var outline; 
	var toons; 
	var toon; 
	var obj; 
	var pt; 
	var arr; 
	var cam; 
	objColor= [255, 255, 255]; 
		objOutline= [0, 0, 0]; 
		objLine= 1; 
		objVector= ""; 
		sAt= [0, 0, 0]; 
		sAbout= [0, 0, 0]; 
		sAxes= [0, 0, 0]; 
		text= ""; 
		if (EnableToon){ 
			for (j= 0; j < m.pack.pack.objects.length; j++){ 
				obj= m.pack.pack.objects[j]; 
				toons= m.pack.pack.objects[j].tns; 
				for (k= 0; k < toons.length; k++){ 
					toon= toons[k]; 
					color= ParseColor (toon.color || obj.color); 
					outline= ParseColor (toon.outline || obj.outline); 
					if (color[0] != objColor[0] || color[1] != objColor[1] || color[2] != objColor[2]){ 
						objColor[0]= color[0]; 
						objColor[1]= color[1]; 
						objColor[2]= color[2]; 
						text += "color " + color.join (",") + " \r\n"; 
					} 
					if (outline[0] != objOutline[0] || outline[1] != objOutline[1] || outline[2] != objOutline[2]){ 
						objOutline[0]= outline[0]; 
						objOutline[1]= outline[1]; 
						objOutline[2]= outline[2]; 
						text += "outline " + outline.join (",") + " \r\n"; 
					} 
					if (((toon.lineWidth || obj.lineWidth) || (toon.lineWidth || obj.lineWidth) === 0) && (toon.lineWidth || obj.lineWidth) != objLine){ 
						objLine= toon.lineWidth || obj.lineWidth || 0; 
						text += "line " + objLine + " \r\n"; 
					} 
					if (!toon.type){ 
						if (toon.figure != objVector){ 
							objVector= toon.figure; 
							arr= objVector.split ("."); 
							if (arr[0] == "figures"){ 
								arr.shift (); 
								objVector= arr.join ("."); 
							} 
							text += "vector " + objVector + " \r\n"; 
						} 
						if (toon.x != sAt[0] || toon.y != sAt[1] || toon.z != sAt[2]){ 
							sAt[0]= toon.x || 0; 
							sAt[1]= toon.y || 0; 
							sAt[2]= toon.z || 0; 
							text += "at " + sAt.join (", ") + " \r\n"; 
						} 
						if (toon.aboutX != sAbout[0] || toon.aboutY != sAbout[1] || toon.aboutZ != sAbout[2]){ 
							sAbout[0]= toon.aboutX || 0; 
							sAbout[1]= toon.aboutY || 0; 
							sAbout[2]= toon.aboutZ || 0; 
							text += "about " + sAbout.join (", ") + " \r\n"; 
						} 
						if (toon.rx != sAxes[0] || toon.ry != sAxes[1] || toon.rz != sAxes[2]){ 
							sAxes[0]= toon.rx || 0; 
							sAxes[1]= toon.ry || 0; 
							sAxes[2]= toon.rz || 0; 
							text += "axes " + sAxes.join (", ") + " \r\n"; 
						} 
						text += "ellipsoid \r\n"; 
					} else if (toon.type == "curve"){ 
						if (toon.fillCurve){ 
							text += "path \r\n"; 
							for (l= 0; l < toon.fillCurve.length; l++){ 
								pt= toon.fillCurve[l]; 
								if ((pt.vct || toon.figure) != objVector){ 
									objVector= pt.vct || toon.figure; 
									arr= objVector.split ("."); 
									if (arr[0] == "figures"){ 
										arr.shift (); 
										objVector= arr.join ("."); 
									} 
									text += "vector " + objVector + " \r\n"; 
								} 
								text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
							} 
							text += "fill \r\n"; 
						} 
						if (toon.curve && toon.curve != toon.fillCurve){ 
							text += "path \r\n"; 
							for (l= 0; l < toon.curve.length; l++){ 
								pt= toon.curve[l]; 
								if ((pt.vct || toon.figure) != objVector){ 
									objVector= pt.vct || toon.figure; 
									arr= objVector.split ("."); 
									if (arr[0] == "figures"){ 
										arr.shift (); 
										objVector= arr.join ("."); 
									} 
									text += "vector " + objVector + " \r\n"; 
								} 
								text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
							} 
							text += "stroke \r\n"; 
						} 
					} 
				} 
			} 
		} 
		if (EnableMesh){ 
			for (j= 0; j < m.pack.pack.objects.length; j++){ 
				obj= m.pack.pack.objects[j]; 
				for (k= 0; k < obj.fcs.length; k++){ 
					color= ParseColor (obj.fcs[k].color); 
					if (color[0] != objColor[0] || color[1] != objColor[1] || color[2] != objColor[2]){ 
						objColor[0]= color[0]; 
						objColor[1]= color[1]; 
						objColor[2]= color[2]; 
						text += "color " + color.join (",") + " \r\n"; 
					} 
					text += "path \r\n"; 
					for (l= 0; l < obj.fcs[k].points.length; l++){ 
						pt= obj.pts[obj.fcs[k].points[l] - 1]; 
						if (pt.vct != objVector){ 
							objVector= pt.vct; 
							arr= objVector.split ("."); 
							if (arr[0] == "figures"){ 
								arr.shift (); 
								objVector= arr.join ("."); 
							} 
							text += "vector " + objVector + " \r\n"; 
						} 
						text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
						text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
						if (l && l + 1 < obj.fcs[k].points.length) text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
					} 
					text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
					pt= obj.pts[obj.fcs[k].points[0] - 1]; 
					text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
					text += "pt " + pt.x + "," + pt.y + "," + pt.z + " \r\n"; 
					text += "fill \r\n"; 
				} 
			} 
		} 
		file_put_contents (Server.MapPath ("/AnimOv/data/llc" + id + ".llc"), text); 
	file_put_contents (Server.MapPath ("/AnimOv/data/id" + id + ".txt"), main_w + "\r\n" + main_h + "\r\n" + m.GetPack ().fps); 
	} else { 
		text= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".llc")); 
		file_put_contents (Server.MapPath ("/AnimOv/data/llc" + id + ".llc"), text); 
	} 
	Response.Write ("Done. "); 
%>