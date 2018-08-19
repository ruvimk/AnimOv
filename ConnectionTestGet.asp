<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
	function RequestParam (name){ 
		var a; 
		var b; 
		a= Request.Form(name) + ""; 
		b= Request.QueryString(name) + ""; 
		if (a == "undefined") return b; 
		else return a; 
	} 
	Session.Abandon (); 
	%><!--#include file="VirtualDOM.asp"--><% 
	%><!--#include virtual="/script.asp"--><% 
	var size= parseInt (RequestParam ("size")); 
	var i; 
	if (!size) size= 1024; 
	Response.ContentType= "text/plain"; 
	for (i= 0; i < size; i++) Response.Write (String.fromCharCode (Math.floor (Math.random () * 128) || 1)); 
%>