document.addEventListener('DOMContentLoaded', function () {
    async function sendCommand(cmd) {
        const res = await fetch('api.php', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ cmd })
        });
    }

    async function sendLCD() {
        const text = document.getElementById('lcdText').value;
        const res = await fetch('api.php', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ lcd: text })
        });
    }
});