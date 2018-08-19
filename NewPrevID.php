<?php 
	session_write_close (); 
	// print "0"; 
	$root= $_SERVER["DOCUMENT_ROOT"]; 
	$cid= $_REQUEST["cid"] * 1; 
	if ($cid){ 
		$cid *= 1000000; 
		$cid= floor ($cid); 
		if (!file_exists ("$root/AnimOv/prev/c$cid.txt")){ 
			$prev_id= $cid; 
		} 
	} 
	while (!$prev_id || file_exists ("$root/AnimOv/prev/c$prev_id.txt")){ 
		$prev_id= rand (); 
	} 
	print $prev_id; 
?>