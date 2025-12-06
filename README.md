# 80円 SR ARMマイコン評価F/W開発

秋月電子で80円(税込み)のST ARMマイコンの評価用F/W個人開発リポジトリ

## 開発環境

### H/W開発環境

- マイコン
  - [STM32C011F4P6](https://www.stmcu.jp/stm32/stm32c0/stm32c0x1/97032/)🔗
    - [購入元 秋月電子リンク](https://akizukidenshi.com/catalog/g/g118189/)🔗

### S/W開発環境

- IDE
  - [STM32CubeIDE for Visual Studio Code - V3.6.4](https://www.st.com/ja/development-tools/stm32vscode.html)🔗
- コンパイラ
  - CMake
  - ST Clang GCC
  - 最適化
    - Release ... `-Os` (サイズ優先)
- SDK
  - ST LLドライバ

## マイコンのスペック

- [データシート](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)🔗

- [リファレンスマニュアル](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)🔗

| 機能 | 本数 |
| --- | --- |
| CPU | ARM Cortex-M0+ |
| Clock | 48 MHz |
| Flash | 16 KB |
| SRAM | 8 KB |
| DMA | x3本 |
| GPIO | x18本 |
| I2C | x1本 @100KHz, 400KHz, 1MHz|
| I2S | x1本 <br>※I2Sと排他で使用可 |
| SPI | x1本 @24Mbit/s<br>※I2Sと排他で使用可 |
| UART | x2本 |
| タイマー | ・16bit 高機能タイマー x1本 (@TIM1)<br>・16bit 汎用タイマー x4本 (@TIM3/14/16/17) |
| PWM | 16bit x6本 |
| RTC | x1本 |
| WDT | x2本 (@IWDG、WWDG)|
| ADC | 12bit x15本 @2.5MSps |
| DAC | (N/A) |
