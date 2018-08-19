<?php 
	header ("Content-Type: text/plain"); 
	session_write_close (); 
	$id= $_REQUEST["id"] * 1; 
	$file= "C:/WBord/wwwroot/AnimOv/public/id$id.zip"; 
	$small= $_REQUEST["small"]; 
	if (($small == "true") | ($small * 1)) $file= "C:/WBord/wwwroot/AnimOv/public/small$id.zip"; 
	print $file; 
?>