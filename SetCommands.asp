<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
	function RequestParam (name){ 
		var a; 
		var b; 
		a= Request.Form(name) + ""; 
		b= Request.QueryString(name) + ""; 
		if (a == "undefined") return b; 
		else return a; 
	} 
	%><!--#include file="VirtualDOM.asp"--><% 
	%><!--#include virtual="/script.asp"--><% 
	%><!--#include file="RenderCommands.asp"--><% 
	var id= parseInt (RequestParam ("id")); 
	var cmd= RequestParam ("text"); 
	if (!cmd) cmd= "<end of message>"; 
	if (cmd.split ("<end of message>").length > 1){ 
			cmd= cmd.split ("<end of message>")[0]; 
		var arr; 
		var last= parseInt (RequestParam ("last")); 
		var text; 
		var a; 
		var b; 
		var c; 
		var i; 
		var j; 
		var path1; 
		var commands= []; 
		if (id){ 
			text= file_get_contents (path1= Server.MapPath ("/AnimOv/data/id" + id + ".aoc")); 
			if (!text) text= "<end of message>"; 
			arr= text.split ("<end of message>"); 
			if (arr.length > 1){ 
				text= arr[0]; 
				arr= text.split ("\r\n"); 
				j= arr.length; 
				for (i= 0; i < j; i++){ 
					if (parseInt (arr[i].split ("\t")[0]) > last) commands.push (arr[i]); 
				} 
				arr= cmd.split ("\r\n"); 
				for (i= 0; i < arr.length; i++){ 
					if (arr[i].split ("<stop>").length < 2) continue; 
					a= RenderCommands (commands.join ("\r\n") + "\r\n<end of message>", false, arr[i]); 
					arr[i]= a[1]; 
				} 
				b= ""; 
				c= j; 
				for (i= j; i - j < arr.length; i++){ 
					if (!arr[i - j].length) continue; 
					b += c + "\t" + arr[i - j] + "\r\n"; 
					c++; 
				} 
				file_put_contents (path1, text + b + "<end of message>"); 
			} else Response.Write ("Error:  Data file corrupt."); 
		} else Response.Write ("Error:  Invalid ID value."); 
	} else Response.Write ("Error:  Invalid input. \r\n\r\ntext: \r\n\r\n" + cmd); 
%>