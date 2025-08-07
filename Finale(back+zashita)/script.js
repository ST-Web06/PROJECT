document.addEventListener('DOMContentLoaded', function() {
    const ledImage = document.getElementById('led-image');
    const toggleButton = document.getElementById('toggle-led');
    const statusMessage = document.getElementById('status-message');
    
    
    let ledState = false;
    
    toggleButton.addEventListener('click', function() {
        ledState = !ledState;
        
        toggleLed(ledState);
    });
    
    async function toggleLed(state) {
    try {
        const response = await fetch('api.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `led_state=${state ? '1' : '0'}`
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();

        updateLedStatus(state);
        statusMessage.textContent = data.message;
        console.log(statusMessage.textContent);

    } catch (error) {
        console.error('Ошибка при переключении светодиода:', error);
        ledState = !state;
        statusMessage.textContent = 'Ошибка при изменении состояния';
    }
}

document.getElementById('send-text').addEventListener('click', async function() {
    const textInput = document.getElementById('display-text');
    const statusDiv = document.getElementById('display-status');
    
    if (!textInput.value.trim()) {
        statusDiv.textContent = 'Введите текст для отправки';
        statusDiv.className = 'status-message error';
        return;
    }

    try {
        const response = await fetch('api.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `action=display_text&text=${encodeURIComponent(textInput.value)}`
        });

        if (!response.ok) {
            throw new Error('Ошибка сети');
        }

        const data = await response.json();

        statusDiv.textContent = data.message || 'Текст успешно отправлен';
        statusDiv.className = 'status-message success';
        
        setTimeout(() => {
            statusDiv.textContent = '';
            statusDiv.className = 'status-message';
        }, 3000);

    } catch (error) {
        console.error('Ошибка:', error);
        statusDiv.textContent = 'Ошибка при отправке текста';
        statusDiv.className = 'status-message error';
    }
});
    
    function updateLedStatus(state) {
        if (state) {
            ledImage.src = 'images/led-on.png';
            ledImage.alt = 'LED is ON';
        } else {
            ledImage.src = 'images/led-off.png';
            ledImage.alt = 'LED is OFF';
        }
    }
});