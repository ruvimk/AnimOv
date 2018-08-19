<?php 
	header ("Content-Type: text/plain"); 
	session_write_close (); 
	$id= $_REQUEST["id"] * 1; 
	$file= "C:/WBord/wwwroot/AnimOv/public/id$id.zip"; 
	$last= filemtime ($file); 
	if ($last){ 
		print implode ("GMT", explode ("_", date ("l, F j, Y, g:i:s a T (_P)", $last))); 
	} else print "Never. "; 
?>