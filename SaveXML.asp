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
	var text= RequestParam ("data"); 
	if (id && id != 0) file_put_contents (Server.MapPath ("/AnimOv/data/id" + id + ".xml"), text); 
%>