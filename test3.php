<?php 
	header ("content-type: text/plain"); 
	$from= $_REQUEST["from"]; 
	$text= file_get_contents ("$from.txt"); 
	print base64_decode ($text); 
?>