<?php
header('Content-Type: application/json');

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
 
try {
    $input = json_decode(file_get_contents('php://input'), true) ?: $_REQUEST;
    
    if (isset($input['lcd'])) { 
        $response = sendSerialCommand("LCD:" . $input['lcd']);
        
    }
    elseif (isset($input['cmd'])) { 
        $response = sendSerialCommand($input['cmd']);
        
    }
    else {
        echo json_encode([
            'error' => 'Неверный запрос',
            'usage' => [
                'Для LCD' => '{"lcd":"Ваш текст"}',
                'Для LED' => '{"cmd":"LED_ON"}'
            ]
        ], JSON_UNESCAPED_UNICODE);
    }
    
} catch (Exception $e) {
    echo json_encode([
        'error' => $e->getMessage()
    ], JSON_UNESCAPED_UNICODE);
}
?>