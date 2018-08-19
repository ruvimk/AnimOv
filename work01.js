var set= new Object (); 
var hlf= text.split ("<stop>"); 
if (hlf.length == 1) alert ("Bad command."); 
var arr= hlf[0].split ("\t")[1].split ("set")[1].split (" "); 
var i; 
for (i= 0; i < arr.length; i++){ 
	if (!arr[i]) continue; 
	if (arr[i] == "="){ 
		
	} else if (arr[i] == "in"){ 
		
	} else if (arr[i] == "for"){ 
		
	} else if (arr[i] == "and"){ 
		
	} 
} 