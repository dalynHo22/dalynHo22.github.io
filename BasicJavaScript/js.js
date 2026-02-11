// Global counter variable
let counter = 0;

// Increase counter
function tickUp(){
    counter++;
    document.getElementById("counter").textContent = counter;
}

/* A method is just a function that belongs to an object. 
  document.getElementById() → method of document */

/* .textContent works with plain text. 
  It's a property that gets or sets the text inside an element. */

// Decrease counter
function tickDown(){
    counter--; 
    document.getElementById("counter").textContent = counter; 
}

function runForLoop(){
    let result = " ";
    for (let i = 0; i<= counter; i++){
        result += i + " " ;
    }
    document.getElementById("forLoopResult").textContent = result; 
}

function showOddNumbers(){
    let oddResults = " ";
    
    for(let i = 1; i<= counter; i++){
        if(i % 2 !== 0 ){
            oddResults += i + " ";
        }
    }
    document.getElementById("oddNumberResult").textContent = oddResults; 

}
