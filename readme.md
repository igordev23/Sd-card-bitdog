# Projeto: Leitura e Escrita em Cartão SD com Raspberry Pi Pico

Este projeto demonstra como inicializar um cartão SD via interface SPI, escrever coordenadas geográficas em um arquivo (`coords.txt`) e ler seu conteúdo usando a biblioteca FatFs no Raspberry Pi Pico.

## Funcionalidades

- Inicializa a comunicação SPI com o cartão SD
- Monta o sistema de arquivos FAT
- Escreve coordenadas geográficas simuladas em um arquivo de texto
- Lê e exibe o conteúdo do arquivo no terminal
- Loop contínuo de escrita/leitura a cada 5 segundos

## Requisitos

- Placa **Raspberry Pi Pico**
- Cartão **microSD** com formatação FAT32
- Conexões SPI:
  - MISO: GPIO 16
  - CS: GPIO 17
  - SCK: GPIO 18
  - MOSI: GPIO 19
- Biblioteca **FatFs**
- SDK do Pico (pico-sdk)

## Conexões SPI

| Sinal | GPIO |
|-------|------|
| MISO  | 16   |
| CS    | 17   |
| SCK   | 18   |
| MOSI  | 19   |

## Como funciona

1. A função `init_spi_sdcard()` configura os pinos SPI e desativa o cartão SD inicialmente.
2. A função `write_to_sd()` monta o sistema de arquivos, abre ou cria o arquivo `coords.txt` e adiciona uma linha com dados simulados.
3. A função `read_from_sd()` abre o mesmo arquivo e exibe seu conteúdo no terminal.
4. No `main()`, essas duas operações se repetem a cada 5 segundos.

## Exemplo de saída no terminal

```c
Inicializando SD Card...
SPI SDCard initialized
✅ SD Card montado
✅ Dados escritos (33 bytes)
📄 Conteúdo de coords.txt:
Lat: -5.12345, Lon: -42.98765
Lat: -5.12345, Lon: -42.98765
```

## Observações

- A função `get_fattime()` retorna um timestamp fixo exigido pela FatFs (pode ser customizada para refletir tempo real).
- Certifique-se de que a biblioteca FatFs está corretamente integrada ao seu projeto.

## Licença

Este projeto é de livre uso para fins educacionais e de prototipagem.
