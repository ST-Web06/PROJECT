<?php
header('Content-Type: application/json');
session_start();

$input = file_get_contents('php://input');
parse_str($input, $data);

if (isset($data['led_state'])) {
    $_SESSION['led_state'] = (bool)$data['led_state'];
    
    echo json_encode([
        'success' => true,
        'message' => $_SESSION['led_state'] ? 'Светодиод включен' : 'Светодиод выключен'
    ]);
    exit;
}

if (isset($data['action']) && $data['action'] === 'display_text' && isset($data['text'])) {
    $text = trim($data['text']);
    
    if (empty($text)) {
        http_response_code(400);
        echo json_encode(['error' => 'Текст не может быть пустым']);
        exit;
    }
    
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