document.addEventListener('DOMContentLoaded', function() {
    const ledImage = document.getElementById('led-image');
    const toggleButton = document.getElementById('toggle-led');
    const statusMessage = document.getElementById('status-message');
    
    // Текущее состояние светодиода
    let ledState = false;
    
    toggleButton.addEventListener('click', function() {
        // Инвертируем состояние
        ledState = !ledState;
        
        // Отправляем запрос на сервер
        toggleLed(ledState);
    });
    
    async function toggleLed(state) {
    try {
        // 1. Отправляем запрос на сервер
        const response = await fetch('api.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `led_state=${state ? '1' : '0'}`
        });

        // 2. Проверяем успешность HTTP-ответа
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        // 3. Парсим JSON-ответ
        const data = await response.json();

        // 4. Обновляем интерфейс
        updateLedStatus(state);
        statusMessage.textContent = data.message;
        setTimeout(() => { statusMessage.textContent = ''; }, 2000);

    } catch (error) {
        // 5. Обрабатываем ошибки
        console.error('Ошибка при переключении светодиода:', error);
        ledState = !state; // Откатываем состояние
        statusMessage.textContent = 'Ошибка при изменении состояния';
    }
}

document.getElementById('send-text').addEventListener('click', async function() {
    const textInput = document.getElementById('display-text');
    const statusDiv = document.getElementById('display-status');
    
    // Проверяем, что текст не пустой
    if (!textInput.value.trim()) {
        statusDiv.textContent = 'Введите текст для отправки';
        statusDiv.className = 'status-message error';
        return;
    }

    try {
        // Отправляем текст на сервер
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

        // Показываем статус отправки
        statusDiv.textContent = data.message || 'Текст успешно отправлен';
        statusDiv.className = 'status-message success';
        
        // Очищаем поле через 3 секунды
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