function LED() {
    alert('Светодиод');
    console.log('LED');

};
function display() {
    alert('display');
};


const led = document.getElementById('led');
console.log(led);
const disp = document.getElementById('display');
led.addEventListener("click", LED);
disp.addEventListener("click", display);