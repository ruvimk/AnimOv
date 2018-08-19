var standard_edits= []; 
function StandardEdit (parent_element){ 
	var t_this; 
	this.input= new Object (); 
	this.parent_element= parent_element; 
	this.element= document.createElement ("div"); 
	this.onupdate= function (obj_edit){}; 
	this.onchange= function (obj_edit){}; 
	this.pslope= 1 / 8; 
	this.slope= 0.0625; 
	this.value= ""; 
	this.width= "auto"; 
	this.size= "medium"; 
	this.bgcolor= "#444488"; 
	this.fgcolor= "#CCCCFF"; 
	this.min= "none"; 
	this.max= "none"; 
	this.onmousemove= function (e){}; 
	this.dblclick= new Date (); 
	standard_edits.push (this); 
	this.number= standard_edits.length; 
	this.lastMouseUp= new Date (); 
	if (this.parent_element) this.parent_element.appendChild (this.element); 
	this.render= function (){ 
		var t1, t2, t3, t4; 
		var t5, t6, t7, t8; 
		var i; 
		while (this.element.childNodes[0]) this.element.removeChild (this.element.childNodes[0]); 
		t1= document.createElement ("table"); 
			t1.appendChild (t2= document.createElement ("tbody")); 
				t2.appendChild (t3= document.createElement ("tr")); 
					t3.appendChild (t4= document.createElement ("td")); 
						t4.className= "standard_edit" + this.number; 
						t4.style.textAlign= "center"; 
						t4.style.cursor= "default"; 
						t4.onclick= function (e){}; 
						t4.onmouseup= function (e){ 
							if (!e) e= window.event; 
							var a; 
							var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
							if (!t) return; 
							t.value= parseFloat (t.value) + t.pslope; 
							t.input.value= t.value; 
							if (!isNaN (a= parseFloat (t.max))){ 
								if (t.value > a) t.input.value= (t.value= a); 
							} 
							t.onupdate (t); 
							t.onchange (t); 
						}; 
						t4.appendChild (document.createTextNode (unescape ("%u25B2"))); 
				t2.appendChild (t3= document.createElement ("tr")); 
					t3.appendChild (t4= document.createElement ("td")); 
						t4.style.textAlign= "center"; 
						t4.appendChild (t5= document.createElement ("input")); 
							this.input= t5; 
							t_this= t5; 
							t5.className= "allow_select_standard_edit" + this.number; 
							t5.style.border= "none"; 
							t5.style.background= this.bgcolor; 
							t5.style.color= this.fgcolor; 
							t5.style.textAlign= "center"; 
							t5.style.cursor= "default"; 
							t5.style.fontSize= this.size; 
							t5.style.width= this.width; 
							t5.value= this.value; 
							t5.onfocus= function (){ 
								if (this.setSelectionRange){ 
									this.setSelectionRange (0, this.value.length); 
								} else { 
									this.value= this.value; 
								} 
							}; 
							t5.onblur= function (){ 
								var t= standard_edits[parseInt ((this.className + "").split ("standard_edit")[1]) - 1]; 
								if (!t) return; 
								t.value= this.value; 
								if (t.value != t.startValue){ 
									t.onupdate (t); 
									t.onchange (t); 
								} 
							}; 
							t5.onkeydown= function (e){ 
								if (!e) e= window.event; 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								if (!t) return; 
								if (e.charCode == 13 || e.keyCode == 13){ 
									t.value= this.value; 
									this.blur (); 
									t.onupdate (t); 
									t.onchange (t); 
								} 
							}; 
							t5.onmousedown= function (e){ 
								if (!e) e= window.event; 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								if (!t) return; 
								t.inputTime= new Date (); 
								t.mouseDown= true; 
								t.mouseMove= false; 
								t.startX= e.screenX; 
								t.startY= e.screenY; 
								t.startValue= parseFloat (t.value); 
								t.input.focus (); 
								if (t.input.setSelectionRange) t.input.setSelectionRange (0, t.input.value.length); 
								else t.input.value= t.input.value; 
							}; 
							t5.onmousemove= function (e){ 
								if (!e) e= window.event; 
								var a; 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								if (!t) return; 
								if (t.mouseDown && t.startX != e.screenX && t.startY != e.screenY){ 
									t.value= (t.startY - e.screenY) * t.slope + t.startValue; 
									t.input.value= t.value; 
									if (!isNaN (a= parseFloat (t.max))){ 
										if (t.value > a) t.input.value= (t.value= a); 
									} 
									if (!isNaN (a= parseFloat (t.min))){ 
										if (t.value < a) t.input.value= (t.value= a); 
									} 
									t.input.blur (); 
									if (document.body.blur) document.body.blur (); 
									t.onupdate (t); 
								} 
								if (!t.mouseMove && t.startX != e.screenX && t.startY != e.screenY && t.mouseDown){ 
									t.mouseMove= true; 
									t_this.blur (); 
								} 
							}; 
							t5.onmouseup= function (e, ext){ 
								if (!e) e= window.event; 
								var a; 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								if (!t) return; 
								t.mouseDown= false; 
								t.onchange (t); 
								if (t.lastMouseUp && !ext){ 
									a= new Date () - t.lastMouseUp; 
									if (a < 500){ 
										if (new Date () - t.dblclick > 500){ 
											ask_number ("Enter new value:", t.value, function (number){ 
												t.value= number; 
												t.input.value= number; 
												if (!isNaN (a= parseFloat (t.max))){ 
													if (t.value > a) t.input.value= (t.value= a); 
												} 
												if (!isNaN (a= parseFloat (t.min))){ 
													if (t.value < a) t.input.value= (t.value= a); 
												} 
											}); 
											t.dblclick= new Date (); 
										} 
									} else t.lastMouseUp= new Date (); 
								} else t.lastMouseUp= new Date (); 
							}; 
							t5.onclick= function (e){}; 
							t5.ondblclick= function (e){ 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								if (new Date () - t.dblclick > 500){ 
									ask_number ("Enter new value:", t.value, function (number){ 
										t.value= number; 
										t.input.value= number; 
										if (!isNaN (a= parseFloat (t.max))){ 
											if (t.value > a) t.input.value= (t.value= a); 
										} 
										if (!isNaN (a= parseFloat (t.min))){ 
											if (t.value < a) t.input.value= (t.value= a); 
										} 
									}); 
									t.dblclick= new Date (); 
								} 
							}; 
							t5.ontouchstart= function (e){ 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								t.input.onmousedown (e.targetTouches[0]); 
							}; 
							t5.ontouchmove= function (e){ 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								e.preventDefault (); 
								t.input.onmousemove (e.targetTouches[0]); 
							}; 
							t5.ontouchend= function (e){ 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								t.input.onmouseup (e.targetTouches[0]); 
							}; 
							t5.ontouchcancel= function (e){ 
								var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
								t.input.onmouseup (e.targetTouches[0]); 
							}; 
							this.onmousemove= t5.onmousemove; 
				t2.appendChild (t3= document.createElement ("tr")); 
					t3.appendChild (t4= document.createElement ("td")); 
						t4.className= "standard_edit" + this.number; 
						t4.style.textAlign= "center"; 
						t4.style.cursor= "default"; 
						t4.onclick= function (e){}; 
						t4.onmouseup= function (e){ 
							if (!e) e= window.event; 
							var a; 
							var t= standard_edits[parseInt ((t_this.className + "").split ("standard_edit")[1]) - 1]; 
							if (!t) return; 
							t.value= parseFloat (t.value) - t.pslope; 
							t.input.value= t.value; 
							if (!isNaN (a= parseFloat (t.min))){ 
								if (t.value < a) t.input.value= (t.value= a); 
							} 
							t.onupdate (t); 
							t.onchange (t); 
						}; 
						t4.appendChild (document.createTextNode (unescape ("%u25BC"))); 
		this.element.appendChild (t1); 
	}; 
	this.close= function (){ 
		if (this.parent_element) this.parent_element.removeChild (this.element); 
	}; 
} 