<?php
header('Content-Type: application/json');
session_start();


function sendSerialCommand($cmd) {
    $port = fopen("COM4", "w+b");
    if (!$port) return ['error' => 'Ошибка порта'];
    
    exec("mode COM4 BAUD=9600 PARITY=n DATA=8 STOP=1 XON=on TO=on");
    stream_set_timeout($port, 1);
    
    fwrite($port, $cmd . "\r\n");
    fflush($port);
    
    $response = "";
    $start = microtime(true);
    while ((microtime(true) - $start) < 0.5) {
        $response .= fread($port, 128);
        if (strpos($response, "\n") !== false) break;
    }
    
    fclose($port);
    return trim($response);
}

input = file_get_contents('php://input');
parse_str($input, $data);

// Обработка LED
if (isset($data['led_state'])) {
    $command = ($data['led_state'] == '1') ? "LED_ON" : "LED_OFF";
    $response = sendToSerial($command);
    
    echo json_encode([
        'success' => true,
        'command' => $command,
        'response' => $response,
        'message' => ($data['led_state'] == '1') ? 'Светодиод включен' : 'Светодиод выключен'
    ]);
    exit;
}

// Обработка TEXT
if (isset($data['action']) && $data['action'] === 'display_text' && isset($data['text'])) {
    $text = trim($data['text']);
    if (empty($text)) {
        http_response_code(400);
        echo json_encode(['error' => 'Текст не может быть пустым']);
        exit;
    }
    
    $command = "TEXT:" . $text;
    $response = sendToSerial($command);
    
    echo json_encode([
        'success' => true,
        'command' => $command,
        'response' => $response,
        'message' => 'Текст отправлен на дисплей'
    ]);
    exit;
}

// Обработка новых команд (для расширения проекта)
if (isset($data['cmd'])) {
    $response = sendToSerial($data['cmd']);
    
    echo json_encode([
        'success' => true,
        'command' => $data['cmd'],
        'response' => $response,
        'message' => 'Команда выполнена'
    ]);
    exit;
}

http_response_code(400);
echo json_encode(['error' => 'Неверный запрос']);
?>