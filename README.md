# UART_Transmit_Receive
## Proje Açıklaması
USARTDriver, UART/USART protokolü üzerinden veri gönderme (transmit) ve alma (receive) işlemlerini gerçekleştiren bir sürücüdür. Bu sürücü, mikrodenetleyici projelerinde seri iletişimi kolaylaştırmak amacıyla tasarlanmıştır.

## Özellikler
- UART/USART üzerinden veri gönderme (transmit)
- UART/USART üzerinden veri alma (receive)
- Donanım akış kontrolü desteği
- Konfigüre edilebilir baud rate, parity, stop bit ve word length ayarları

## Kurulum
Bu sürücüyü projenize dahil etmek için aşağıdaki adımları izleyin:

1. `USARTDriver` klasörünü projenize kopyalayın.
2. `uart.h` ve `uart.cpp` dosyalarını projenize ekleyin.
3. `main.cpp` dosyanıza gerekli başlık dosyalarını ekleyin.

## Kullanım

### UARTBase Sınıfı Kullanımı
UARTBase sınıfı, UART/USART işlemlerini gerçekleştirmek için kullanılır.

#### Örnek Kod
