<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
	function RequestParam (name){ 
		var a; 
		var b; 
		a= Request.Form(name) + ""; 
		b= Request.QueryString(name) + ""; 
		if (a == "undefined") return b; 
		else return a; 
	} 
	function contains (text, substr){ 
		if (!text) return false; 
		if (!substr) return false; 
		if (text.split (substr).length > 1) return true; 
		else return false; 
	} 
	Session.Abandon (); 
	%><!--#include file="VirtualDOM.asp"--><% 
	%><!--#include virtual="/script.asp"--><% 
	var id= parseInt (RequestParam ("id")); 
	var arr; 
	var last= parseInt (RequestParam ("last")); 
	var filt= RequestParam ("filter"); 
	var b; 
	var c; 
	var i; 
	var j; 
	var tmp; 
	var path1; 
	var commands= []; 
	if (id){ 
		text= file_get_contents (path1= Server.MapPath ("/AnimOv/data/id" + id + ".aoc")); 
		arr= text.split ("\r\n"); 
		j= arr.length; 
		for (i= 0; i < j; i++){ 
			if (filt == "vect"){ 
				if ((c= parseInt (arr[i].split ("\t")[0])) > last) tmp= (arr[i] + "\r\n"); 
				else if (!c && c !== 0) tmp= (arr[i] + "\r\n"); 
				if (!contains (tmp, "set")) Response.Write (tmp); 
			} else { 
				if ((c= parseInt (arr[i].split ("\t")[0])) > last) Response.Write (arr[i] + "\r\n"); 
				else if (!c && c !== 0) Response.Write (arr[i] + "\r\n"); 
			} 
		} 
		Response.Write ("<end of message>"); 
	} 
%>