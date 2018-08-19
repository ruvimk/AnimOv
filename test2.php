<?php 
	$text= file_get_contents ("test20output.txt"); 
	file_put_contents ("img2.png", base64_decode ($text)); 
?>