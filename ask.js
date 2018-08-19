function ask_number (question, init_value, callback_fxn){ 
	var t1, t2, t3, t4; 
	var t5, t6, t7, t8; 
	var inp; 
	t1= document.createElement ("div"); 
		t1.style.padding= "2px"; 
		t1.style.position= "absolute"; 
		t1.style.visibility= "hidden"; 
		t1.appendChild (document.createTextNode (question)); 
		t1.appendChild (document.createElement ("br")); 
		t1.appendChild (t2= document.createElement ("input")); 
			inp= t2; 
			t2.style.marginTop= "5px"; 
			t2.value= init_value; 
			t2.onkeyup= function (e){ 
				if (!e) e= window.event; 
				if (e.charCode == 13 || e.keyCode == 13){ 
					var a= parseFloat (this.value); 
					if (!isNaN (a)) callback_fxn (a); 
					document.body.removeChild (this.parentNode); 
				} 
			}; 
		t1.style.left= "0px"; 
		t1.style.top= "0px"; 
	document.body.appendChild (t1); 
		t1.style.left= (window.innerWidth / 2 - t1.offsetWidth / 2) + "px"; 
		t1.style.top= (window.innerHeight / 2 - t1.offsetHeight / 2) + "px"; 
		t1.style.border= "1px #000000 solid"; 
		t1.style.background= "#CCCCFF"; 
		t1.style.visibility= "visible"; 
	inp.focus (); 
	if (inp.setSelectionRange) inp.setSelectionRange (0, inp.value.length); 
	else inp.value= inp.value; 
} 
function ask_string (question, init_value, callback_fxn, callback_keypress){ 
	var t1, t2, t3, t4; 
	var t5, t6, t7, t8; 
	var inp; 
	t1= document.createElement ("div"); 
		t1.style.padding= "2px"; 
		t1.style.position= "absolute"; 
		t1.style.visibility= "hidden"; 
		t1.appendChild (document.createTextNode (question)); 
		t1.appendChild (document.createElement ("br")); 
		t1.appendChild (t2= document.createElement ("input")); 
			inp= t2; 
			t2.style.marginTop= "5px"; 
			t2.value= init_value; 
			t2.onkeyup= function (e){ 
				if (!e) e= window.event; 
				if (e.charCode == 13 || e.keyCode == 13){ 
					var a= this.value; 
					if (a) callback_fxn (a); 
					document.body.removeChild (this.parentNode); 
				} else { 
					this.when_key_pressed (e); 
				} 
			}; 
			t2.when_key_pressed= callback_keypress; 
		t1.style.left= "0px"; 
		t1.style.top= "0px"; 
	document.body.appendChild (t1); 
		t1.style.left= (window.innerWidth / 2 - t1.offsetWidth / 2) + "px"; 
		t1.style.top= (window.innerHeight / 2 - t1.offsetHeight / 2) + "px"; 
		t1.style.border= "1px #000000 solid"; 
		t1.style.background= "#CCCCFF"; 
		t1.style.visibility= "visible"; 
	inp.focus (); 
	if (inp.setSelectionRange) inp.setSelectionRange (0, inp.value.length); 
	else inp.value= inp.value; 
} 