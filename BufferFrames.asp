<% @language = "JavaScript" %><% 
	Session.Abandon (); 
	%><!--#include file="fileio.asp"--><% 
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
	var a; 
	var c; 
	var i; 
	var j; 
	var id= parseInt (RequestParam ("id")); 
	var arr; 
	var text= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".aof")); 
	var start= parseInt (RequestParam ("start")); 
	var len= parseInt (RequestParam ("len")); 
	Response.Buffer= false; 
	if (id){ 
		arr= text.split ("<frame break>"); 
		c= start + len; 
		for (i= start; i < c; i++){ 
			if (!arr[i]) continue; 
			if (arr[i] == "\r\n") continue; 
			if (i - start) Response.Write ("<frame break>"); 
			Response.Write (arr[i]); 
		} 
	} 
%>