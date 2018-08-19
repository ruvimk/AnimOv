<?php 
	header ("Content-Type: text/event-stream"); 
	session_write_close (); 
	$id= $_REQUEST["id"] * 1; 
	$file= "C:/WBord/wwwroot/AnimOv/public/id$id.zip"; 
	$small= $_REQUEST["small"]; 
	if (($small == "true") | ($small * 1)) $file= "C:/WBord/wwwroot/AnimOv/public/small$id.zip"; 
	$lossy= $_REQUEST["lossy"]; 
	if (($lossy == "true") | ($lossy * 1)) $lossy= true; 
	else $lossy= false; 
	$zip= zip_open ($file); 
	$start= $_REQUEST["start"]; 
	$fps= $_REQUEST["fps"]; 
	if (!$fps) $fps= 0; 
	$fps= ceil ($fps); 
	if (!$start) $start= 0; 
	$number= 0; 
	if ($zip){ 
		// print "Listing ZIP contents ($file)... \r\n\r\n\r\n\r\n"; 
		while ($zip_entry= zip_read ($zip)){ 
			$number++; 
			if ($start){ 
				$start--; 
				continue; 
			} 
			$name= zip_entry_name ($zip_entry); 
			$size= zip_entry_filesize ($zip_entry); 
			$compsize= zip_entry_compressedsize ($zip_entry); 
			$compmthd= zip_entry_compressionmethod ($zip_entry); 
			// print "Name: $name\r\n"; 
			// print "Size: $size\r\n"; 
			// print "Compressed Size: $compsize\r\n"; 
			// print "Compression Method: $compmthd\r\n"; 
			// print "\r\n"; 
			if (zip_entry_open ($zip, $zip_entry, "r")){ 
				if ($lossy){ 
					$bin= zip_entry_read ($zip_entry, $size); 
					$img= imagecreatefromstring ($bin); 
					$bin= "stream$id_pic" . rand () . ".jpg"; 
					imagejpeg ($img, $bin); 
					imagedestroy ($img); 
					$buff= base64_encode (file_get_contents ($bin)); 
					unlink ($bin); 
					if (!$buff) $buff= base64_encode (zip_entry_read ($zip_entry, $size)); 
				} else { 
					$buff= base64_encode (zip_entry_read ($zip_entry, $size)); 
				} 
				print "data: $buff\r\n\r\n"; 
				flush (); 
				$buff= ""; 
				zip_entry_close ($zip_entry); 
				if (connection_aborted ()){ 
					zip_close ($zip); 
					break; 
				} 
			} else { 
				// print "Could not read entry contents. \r\n\r\n\r\n\r\n"; 
			} 
			if (!(($number - $start - 1) % $fps)) sleep (1); 
		} 
		zip_close ($zip); 
	} else { 
		// print "Error:  Could not open ZIP archive $file\r\n"; 
	} 
	print "data: done\r\n\r\n"; 
?>