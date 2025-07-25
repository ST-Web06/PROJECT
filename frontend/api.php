<?php
header('Content-Type: application/json');
session_start();

// Получаем необработанные POST-данные
$input = file_get_contents('php://input');
parse_str($input, $data);

// Обработка команды для светодиода
if (isset($data['led_state'])) {
    $_SESSION['led_state'] = (bool)$data['led_state'];
    
    // Здесь код для отправки на микроконтроллер
    // $command = $_SESSION['led_state'] ? "LED_ON" : "LED_OFF";
    // sendToMicrocontroller($command);
    
    echo json_encode([
        'success' => true,
        'message' => $_SESSION['led_state'] ? 'Светодиод включен' : 'Светодиод выключен'
    ]);
    exit;
}

// Обработка текста для дисплея
if (isset($data['action']) && $data['action'] === 'display_text' && isset($data['text'])) {
    $text = trim($data['text']);
    
    // Валидация текста
    if (empty($text)) {
        http_response_code(400);
        echo json_encode(['error' => 'Текст не может быть пустым']);
        exit;
    }
    
    // Здесь код для отправки на дисплей микроконтроллера
    // sendToMicrocontroller("DISPLAY_TEXT:" . $text);
    
    // Сохраняем текст в сессии
    $_SESSION['display_text'] = $text;
    
    echo json_encode([
        'success' => true,
        'message' => 'Текст отправлен на дисплей'
    ]);
    exit;
}

http_response_code(400);
echo json_encode(['error' => 'Неверный запрос']);
?>