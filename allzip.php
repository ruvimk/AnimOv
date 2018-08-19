<?php 
	header ("Content-Type: text/plain"); 
	session_write_close (); 
	$file= "C:/WBord/wwwroot/AnimOv/testX.zip"; 
	$zip= zip_open ($file); 
	if ($zip){ 
		print "Listing ZIP contents ($file)... \r\n\r\n\r\n\r\n"; 
		while ($zip_entry= zip_read ($zip)){ 
			$name= zip_entry_name ($zip_entry); 
			$size= zip_entry_filesize ($zip_entry); 
			$compsize= zip_entry_compressedsize ($zip_entry); 
			$compmthd= zip_entry_compressionmethod ($zip_entry); 
			print "Name: $name\r\n"; 
			print "Size: $size\r\n"; 
			print "Compressed Size: $compsize\r\n"; 
			print "Compression Method: $compmthd\r\n"; 
			print "\r\n"; 
			if (zip_entry_open ($zip, $zip_entry, "r")){ 
				$text= zip_entry_read ($zip_entry, $size); 
				print "Contents: \r\n$text\r\n<end of message>\r\n\r\n\r\n\r\n"; 
				$text= ""; 
				zip_entry_close ($zip_entry); 
			} else { 
				print "Could not read entry contents. \r\n\r\n\r\n\r\n"; 
			} 
		} 
		zip_close ($zip); 
	} else { 
		print "Error:  Could not open ZIP archive $file\r\n"; 
	} 
?>