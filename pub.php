<?php 
	header ("Content-Type: text/plain"); 
	session_write_close (); 
	$id= $_REQUEST["id"] * 1; 
	$small= $_REQUEST["small"] * 1; 
	$zipbase= $_REQUEST["zipbase"]; 
	$zipbase= implode ("_", explode ("/", implode ("_", explode ("\\", $zipbase)))); 
	$zipbase= urlencode ($zipbase); 
	$smpk= "C:/WBord/wwwroot/AnimOv/public/small$id.zip"; 
	$file= "C:/WBord/wwwroot/AnimOv/public/id$id.zip"; 
	$from= "C:/WBord/wwwroot/AnimOv/data/$zipbase$id.zip"; 
	copy ($from, $file); 
	$zip= zip_open ($file); 
	if ($zip){ 
		$save= new ZipArchive (); 
		if ($save->open ($smpk, ZIPARCHIVE::CREATE) === true){ 
			while ($entry= zip_read ($zip)){ 
				$name= zip_entry_name ($entry); 
				$size= zip_entry_filesize ($entry); 
				if (zip_entry_open ($zip, $entry, "r")){ 
					$data= zip_entry_read ($entry, $size); 
						$temp= "pub_pic" . rand () . ".png"; 
						file_put_contents ($temp, $data); 
						$a= getimagesize ($temp); 
						unlink ($temp); 
					// $a= getimagesizefromstring ($data); 
					$main_w= $a[0]; 
					$main_h= $a[1]; 
					$main_img= imagecreatefromstring ($data); 
					$data= ""; 
					$save_h= $small * 1; 
					if (!$save_h) $save_h= floor ($main_h / 2); 
					$save_w= round ($main_w * $save_h / $main_h); 
					$save_img= imagecreate ($save_w, $save_h); 
					if (imagecopyresampled ($save_img, $main_img, 0, 0, 0, 0, $save_w, $save_h, $main_w, $main_h)){ 
						$temp= "pub_pic" . rand () . ".png"; 
						imagepng ($save_img, $temp, 9); 
						$buff= file_get_contents ($temp); 
						unlink ($temp); 
						if ($buff){ 
							$save->addFromString ($name, $buff); 
						} 
						imagedestroy ($save_img); 
					} 
					zip_entry_close ($entry); 
					imagedestroy ($main_img); 
					$buff= ""; 
				} 
			} 
		} 
		$save->close (); 
		zip_close ($zip); 
	} else unlink ($smpk); 
	$last= filemtime ($file); 
	if ($last){ 
		print implode ("GMT", explode ("_", date ("l, F j, Y, g:i:s a T (_P)", $last))); 
	} else print "Never. "; 
?>