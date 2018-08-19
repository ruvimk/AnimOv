<?php 
	session_write_close (); 
	$root= $_SERVER["DOCUMENT_ROOT"]; 
	$pid= $_REQUEST["pid"]; 
	$cmd= $_REQUEST["cmd"]; 
	$iterations= 0; 
	$timeout= 5; 
	$filename= "$root/AnimOv/prev/c$pid.txt"; 
	if (file_exists ($filename)){ 
		while (filesize ($filename)){ 
			usleep (20 * 1000); 
			$iterations++; 
			if ($iterations > $timeout * 1000 / 20){ 
				// header ("HTTP/1.0 500 Internal Server Error"); 
				print "Error:  System taking too long to accept commands. \r\n"; 
				exit (0); 
			} 
		} 
		file_put_contents ($filename, $cmd); 
		header ("Content-Type: text/plain"); 
		print "Success! Command sent. \r\n"; 
	} else { 
		// header ("HTTP/1.0 404 Not Found"); 
		header ("Content-Type: text/plain"); 
		print "Error:  Preview session not found. \r\n"; 
	} 
?>