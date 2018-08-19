<% 
	var lastCon; 
	var navigator= new Object (); 
	navigator.appName= "Microsoft Internet Explorer"; 
	navigator.appVersion= "MSIE Microsoft Internet Explorer"; 
	var document= new Object (); 
	document.body= new Object (); 
	document.body.childNodes= []; 
	document.body.appendChild= function (){}; 
	document.body.removeChild= function (){}; 
	document.createElement= function (tag_name){ 
		var obj; 
		var con; 
		if (tag_name == "canvas"){ 
			obj= new Object (); 
			obj.c2d= new Object (); 
			obj.getContext= function (){ 
				var obj= this; 
				return obj.c2d; 
			}; 
			con= obj.c2d; 
				con.script= ""; 
				con.gradCount= 0; 
				con.canvas= obj; 
				con.fillStyle= "#FFFFFF"; 
				con.strokeStyle= "#000000"; 
				con.lineWidth= 1; 
				con.beginPath= function (){ 
					var con= this; 
					con.script += "con.beginPath ();"; 
				}; 
				con.closePath= function (){ 
					var con= this; 
					con.script += "con.closePath ();"; 
				}; 
				con.moveTo= function (x, y){ 
					var con= this; 
					con.script += "con.moveTo (s*" + x + ",s*" + y + ");"; 
				}; 
				con.lineTo= function (x, y){ 
					var con= this; 
					con.script += "con.lineTo (s*" + x + ",s*" + y + ");"; 
				}; 
				con.quadraticCurveTo= function (x1, y1, x2, y2){ 
					var con= this; 
					con.script += "con.quadraticCurveTo (s*" + x1 + ",s*" + y1 + ",s*" + x2 + ",s*" + y2 + ");"; 
				}; 
				con.bezierCurveTo= function (x1, y1, x2, y2, x3, y3){ 
					var con= this; 
					con.script += "con.bezierCurveTo (s*" + x1 + ",s*" + y1 + ",s*" + x2 + ",s*" + y2 + ",s*" + x3 + ",s*" + y3 + ");"; 
				}; 
				con.arc= function (a, b, c, d, e, f){ 
					var con= this; 
					con.script += "con.arc (s*" + a + ",s*" + b + ",s*" + c + ",s*" + d + "," + e + "," + f + ");"; 
				}; 
				con.fillRect= function (a, b, c, d){ 
					var con= this; 
					if (con.fillStyle + "" === con.fillStyle) con.script += "con.fillStyle= \"" + con.fillStyle + "\"; "; 
					else con.script += "con.fillStyle= " + con.fillStyle.name + "; "; 
					con.script += "con.fillRect (s*" + a + ",s*" + b + ",s*" + c + ",s*" + d + ");"; 
				}; 
				con.strokeRect= function (a, b, c, d){ 
					var con= this; 
					if (con.strokeStyle + "" === con.strokeStyle) con.script += "con.strokeStyle= \"" + con.strokeStyle + "\"; "; 
					else con.script += "con.strokeStyle= " + con.strokeStyle.name + "; "; 
					con.script += "con.lineWidth= " + con.lineWidth + ";"; 
					con.script += "con.strokeRect (s*" + a + ",s*" + b + ",s*" + c + ",s*" + d + ");"; 
				}; 
				con.fill= function (){ 
					var con= this; 
					if (con.fillStyle + "" === con.fillStyle) con.script += "con.fillStyle= \"" + con.fillStyle + "\"; "; 
					else con.script += "con.fillStyle= " + con.fillStyle.name + "; "; 
					con.script += "con.fill ();"; 
					//file_put_contents (Server.MapPath ("/rt01.txt"), con.script); 
					lastCon= con; 
				}; 
				con.stroke= function (){ 
					var con= this; 
					if (con.strokeStyle + "" === con.strokeStyle) con.script += "con.strokeStyle= \"" + con.strokeStyle + "\"; "; 
					else con.script += "con.strokeStyle= " + con.strokeStyle.name + "; "; 
					con.script += "con.lineWidth= " + con.lineWidth + ";"; 
					con.script += "con.stroke ();"; 
				}; 
				con.createLinearGradient= function (a, b, c, d){ 
					var con= this; 
					con.gradCount++; 
					con.script += "gradient" + con.gradCount + "= con.createLinearGradient (s*" + a + ",s*" + b + ",s*" + c + ",s*" + d + ");"; 
					return {name: "gradient" + con.gradCount, addColorStop: con.addColorStopFxn}; 
				}; 
				con.createRadialGradient= function (a, b, c, d, e, f){ 
					var con= this; 
					con.gradCount++; 
					con.script += "gradient" + con.gradCount + "= con.createRadialGradient (s*" + a + ",s*" + b + ",s*" + c + ",s*" + d + ",s*" + e + ",s*" + f + ");"; 
					return {name: "gradient" + con.gradCount, addColorStop: con.addColorStopFxn, context: con}; 
				}; 
				con.addColorStopFxn= function (d, b){ 
					this.context.script += this.name + ".addColorStop (" + d + ",\"" + b + "\"); "; 
				}; 
			return obj; 
		} else { 
			return document.body; 
		} 
	}; 
	document.createTextNode= function (){}; 
%>