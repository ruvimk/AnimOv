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
	var id= parseInt (RequestParam ("id")); 
	var pack; 
	var text= file_get_contents (Server.MapPath ("/AnimOv/data/id" + id + ".xml")); 
	if (id){ 
		Response.ContentType= "text/xml"; 
		pack= ImportFromXML (text); 
		Response.Write (ExportToXML (pack)); 
	} else { 
		Response.Write ("Error:  Invalid ID value. \r\n"); 
	} 
%>