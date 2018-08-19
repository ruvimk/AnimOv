var vector_selects= []; 
function VectorSelect (txt, fig, cam){ 
	this.element= txt; 
	this.figures= fig; 
	this.cameras= cam; 
	this.selected= ""; 
	vector_selects.push (this); 
	this.number= vector_selects.length; 
	this.onselect= function (path){}; 
	this.selectType= ""; 
	this.smallScreen= 0; 
	this.render= function (){ 
		var base_left= 0; 
		var tab_width= 20; 
		var small_font= 20; 
		var t1, t2, t3, t4; 
		var t5, t6, t7, t8; 
		var cam= new Object (); 
		var fig; 
		var txt= this.element; 
		var a; 
		var i; 
			cam.x= cam.y= cam.z= 0; 
			cam.aboutX= cam.aboutY= cam.aboutZ= 0; 
			cam.rh= Math.PI / 4; 
			cam.rv= Math.PI * 3 / 16; 
			cam.zoom= 0; 
		fig= AbsoluteFromFigures (this.figures, cam); 
		ClearElement (txt); 
		txt.style.backgroundColor= "#FF4444"; 
		txt.style.padding= "2px"; 
		txt.style.paddingLeft= "5px"; 
		/*txt.appendChild (t1= document.createElement ("div")); 
			t1.style.color= "#882222"; 
			t1.appendChild (document.createTextNode ("smallScreen: " + this.smallScreen)); */
		for (i in this.cameras){ 
			txt.appendChild (t1= document.createElement ("div")); 
				t1.className= "VectorSelect_" + this.number + "_Camera__" + i; 
				if (i == this.selected) t1.style.background= "#8888FF"; 
				t1.style.paddingLeft= base_left + "px"; 
				t1.style.cursor= "default"; 
				t1.style.color= "#882222"; 
				if (this.smallScreen) t1.style.fontSize= small_font + "px"; 
				t1.onclick= function (e){}; 
				t1.onmouseup= function (e){ 
					var t= vector_selects[parseInt (this.className.split ("_")[1]) - 1]; 
					t.selectType= "camera"; 
					t.selected= this.className.split ("Camera__")[1]; 
					t.onselect (t.selected); 
				}; 
				t1.appendChild (t2= document.createElement ("span")); 
					t2.style.fontWeight= "bold"; 
					t2.appendChild (document.createTextNode (" [CAM] ")); 
				t1.appendChild (document.createTextNode (" " + i)); 
		} 
		for (i= 0; i < fig.length; i++){ 
			a= fig[i].path.split ("."); 
			txt.appendChild (t1= document.createElement ("div")); 
				t1.className= "VectorSelect_" + this.number + "_Vector__" + fig[i].path; 
				if (fig[i].path == this.selected) t1.style.background= "#8888FF"; 
				if (a[0] == "figures") t1.style.paddingLeft= (base_left + tab_width * (a.length - 2)) + "px"; 
				else t1.style.paddingLeft= (base_left + tab_width * (a.length - 1)) + "px"; 
				t1.style.cursor= "default"; 
				t1.style.color= "#882222"; 
				if (this.smallScreen) t1.style.fontSize= small_font + "px"; 
				t1.onclick= function (e){}; 
				t1.onmouseup= function (e){ 
					var t= vector_selects[parseInt (this.className.split ("_")[1]) - 1]; 
					t.selectType= "vector"; 
					t.selected= this.className.split ("Vector__")[1]; 
					t.onselect (t.selected); 
				}; 
				t1.appendChild (document.createTextNode (fig[i].path)); 
		} 
	}; 
} 