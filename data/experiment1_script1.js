var arr= text.split ("\n"); 
var result= [arr[0]]; 
var i; 
for (i= 0; i + 1 < arr.length; i++){ 
result.push (arr[i]); 
result.push (arr[i+1]); 
result.push (arr[i+1]); 
} 
document.getElementById ("text").value= result.join ("\n"); 