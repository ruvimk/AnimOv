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
		var a1; 
		var a; 
		var i; 
		var j; 
			cam.x= cam.y= cam.z= 0; 
			cam.aboutX= cam.aboutY= cam.aboutZ= 0; 
			cam.rh= Math.PI / 4; 
			cam.rv= Math.PI * 3 / 16; 
			cam.zoom= 0; 
		fig= AbsoluteFromFiguresListing (this.figures); 
		ClearElement (txt); 
		txt.style.backgroundColor= "#FF4444"; 
		txt.style.padding= "2px"; 
		txt.style.paddingLeft= "5px"; 
		/*txt.appendChild (t1= document.createElement ("div")); 
			t1.style.color= "#882222"; 
			t1.appendChild (document.createTextNode ("smallScreen: " + this.smallScreen)); */
		txt.appendChild (t1= document.createElement ("div")); 
			t1.className= "VectorSelect_" + this.number + "_Background__Background"; 
			if (this.selected == "Background") t1.style.background= "#8888FF"; 
			t1.style.paddingLeft= base_left + "px"; 
			t1.style.cursor= "default"; 
			t1.style.color= "#882222"; 
			if (this.smallScreen) t1.style.fontSize= small_font + "px"; 
			//t1.onclick= function (e){}; 
			t1.onclick= function (e){ 
				var t= vector_selects[parseInt (this.className.split ("_")[1]) - 1]; 
				t.selectType= "vector"; 
				t.selected= this.className.split ("Background__")[1]; 
				t.onselect (t.selected); 
			}; 
			t1.appendChild (t2= document.createElement ("span")); 
				t2.style.fontWeight= "bold"; 
				t2.appendChild (document.createTextNode (" [BG] ")); 
			t1.appendChild (document.createTextNode (" Background")); 
		for (i in this.cameras){ 
			txt.appendChild (t1= document.createElement ("div")); 
				t1.className= "VectorSelect_" + this.number + "_Camera__" + i; 
				if (i == this.selected) t1.style.background= "#8888FF"; 
				t1.style.paddingLeft= base_left + "px"; 
				t1.style.cursor= "default"; 
				t1.style.color= "#882222"; 
				if (this.smallScreen) t1.style.fontSize= small_font + "px"; 
				//t1.onclick= function (e){}; 
				t1.onclick= function (e){ 
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
		fig_cmd= ""; 
		for (i= 0; i < fig.length; i++){ 
			if (fig[i].path == "Background" || fig[i].path == "figures.Background") continue; 
			a= fig[i].path.split ("."); 
			txt.appendChild (t1= document.createElement ("div")); 
				t1.className= "VectorSelect_" + this.number + "_Vector__" + fig[i].path; 
				if (fig[i].path == this.selected) t1.style.background= "#8888FF"; 
				if (a[0] == "figures") t1.style.paddingLeft= (base_left + tab_width * (a.length - 2)) + "px"; 
				else t1.style.paddingLeft= (base_left + tab_width * (a.length - 1)) + "px"; 
				t1.style.cursor= "default"; 
				t1.style.color= "#882222"; 
				if (this.smallScreen) t1.style.fontSize= small_font + "px"; 
				//t1.onclick= function (e){}; 
				t1.onclick= function (e){ 
					var t= vector_selects[parseInt (this.className.split ("_")[1]) - 1]; 
					t.selectType= "vector"; 
					t.selected= this.className.split ("Vector__")[1]; 
					t.onselect (t.selected); 
				}; 
				a1= fig[i].path.split ("."); 
				if (a1[0] == "figures") a1.shift (); 
				t1.appendChild (document.createTextNode (a1.join ("."))); 
			for (j in fig[i]){ 
				if (!fig[i][j]) continue; 
				if (fig_cmd) fig_cmd += "\r\n"; 
				fig_cmd += "\tset figure " + fig[i].path + "." + j + " " + fig[i][j] + " @" + counter + "<stop>"; 
			} 
		} 
	}; 
} 
function AbsoluteFromFiguresListing (figures, root){ 
	var arr= []; 
	var ref; 
	var tmp; 
	var i; 
	var r= root? root:figures; 
	for (i in figures){ 
		if (i == "name") continue; 
		if (i == "path") continue; 
		if (i == "x") continue; 
		if (i == "y") continue; 
		if (i == "z") continue; 
		if (i == "aboutX") continue; 
		if (i == "aboutY") continue; 
		if (i == "aboutZ") continue; 
		if (i == "opc") continue; 
		if (i == "net") continue; 
		if (i == "len") continue; 
		arr.push (ref= new Object ()); 
		ref.path= figures[i].path; 
		ref.x= figures[i].x; 
		ref.y= figures[i].y; 
		ref.z= figures[i].z; 
		ref.aboutX= figures[i].aboutX; 
		ref.aboutY= figures[i].aboutY; 
		ref.aboutZ= figures[i].aboutZ; 
		ref.opc= figures[i].opc; 
		tmp= AbsoluteFromFiguresListing (figures[i], r); 
		arr= arr.concat (tmp); 
	} 
	return arr; 
} 