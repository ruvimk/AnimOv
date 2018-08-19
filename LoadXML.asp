<% @language = "JavaScript" %><!--#include file="fileio.asp"--><% 
	function RequestParam (name){ 
		var a; 
		var b; 
		a= Request.Form(name) + ""; 
		b= Request.QueryString(name) + ""; 
		if (a == "undefined") return b; 
		else return a; 
	} 
	var id= parseInt (RequestParam ("id")); 
	Response.ContentType= "text/xml"; 
	Response.Write (file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".xml"))); 
%>