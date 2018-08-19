<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
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
	var id= parseInt (RequestParam ("id")); 
	var can; 
	var con; 
	var main_w= parseInt (RequestParam ("w")); 
	var main_h= parseInt (RequestParam ("h")); 
	var buffer= new Array (); 
	var text= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".xml")); 
	var text2; 
	var i; 
	var c; 
	var e; 
	var m= new ToonModule (); 
	// file_put_contents (Server.MapPath ("/rt1a.txt"), "id=" + id + ", width=" + main_w + ", height=" + main_h); 
	// Response.Write ("Initializing rendering module... \r\n"); 
	m.SetDimensions (main_w, main_h); 
	m.LoadXML (text); 
	delete text; 
	text2= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".aoc")); 
	RenderCommands (text2, m.GetPack (), null, function (numerator, denominator){ 
		file_put_contents (Server.MapPath ("/AnimOv/data/progress" + id + ".txt"), numerator + "/" + denominator + ":c;msg:Compiling commands... "); 
	}); 
	c= m.GetPack ().frames.length; 
	// file_put_contents (Server.MapPath ("/rt1a.txt"), "c=" + c); 
	// Response.Write ("Preparing to save frames... \r\n"); 
	var fso= new ActiveXObject ("Scripting.FileSystemObject"); 
	var file= fso.CreateTextFile (Server.MapPath ("/AnimOv/data/id" + id + ".aof")); 
	// Response.Write ("Starting the rendering process... \r\n"); 
	for (i= 0; i < c; i++){ 
		if (Response.IsClientConnected == false){ 
			file_put_contents (Server.MapPath ("/AnimOv/data/progress" + id + ".txt"), (i + 1) + "/" + c + ":r;msg:Stopped. "); 
			break; 
		} 
		// file_put_contents (Server.MapPath ("/rt1a.txt"), "Rendering frame " + (i + 1) + "... "); 
		Response.Write ("Rendering frame " + (i + 1) + "... <BR> \r\n"); 
		can= m.RenderFrame (i + 1); 
		con= can.getContext ("2d"); 
		file.Write (lastCon.script + "<frame break>\r\n"); 
		ClearVectorCache (); 
		file_put_contents (Server.MapPath ("/AnimOv/data/progress" + id + ".txt"), (i + 1) + "/" + c + ":r;msg:Rendering... "); 
	} 
	// file_put_contents (Server.MapPath ("/rt1a.txt"), "Finalizing... "); 
	file.Close (); 
	file_put_contents (Server.MapPath ("/AnimOv/data/id" + id + ".txt"), main_w + "\r\n" + main_h + "\r\n" + m.GetPack ().fps); 
	Response.Write ("Render complete!"); 
%>