🌊 Projeto Sensor de Enchentes FIAP
---


🚀 Sobre o Projeto
---
Este projeto utiliza um sensor ultrassônico, um display LCD com I2C, um relógio em tempo real (RTC) e EEPROM para monitorar e registrar o nível da água, ajudando a detectar enchentes. Ele aciona LEDs indicativos e um buzzer para alertas sonoros conforme o nível de risco.


📸 Imagem do Projeto
---
![Captura de tela 2025-06-04 224914](https://github.com/user-attachments/assets/fac6fbe6-da31-41a7-a97e-012d4b1839e4)



🛠️ COMPONENTES UTILIZADOS
---

| Ícone | Componente                      | Descrição                                    |
| ----- | ------------------------------- | -------------------------------------------- |
| 🧩    | **Arduino Uno**                 | Microcontrolador principal do projeto        |
| 🌊    | **Sensor Ultrassônico HC-SR04** | Mede a distância do nível da água            |
| 📟    | **Display LCD 16x2 com I2C**    | Exibe informações sobre nível e status       |
| ⏰     | **RTC DS1307**                  | Mantém o horário real para registro de dados |
| 💾    | **EEPROM**                      | Armazena o histórico de medições             |
| 🔔    | **Buzzer**                      | Emite alertas sonoros para níveis críticos   |
| 💡    | **LEDs (7 unidades)**           | Indicadores visuais do nível da água         |

🎯 Funcionalidades
---

📏 Mede a distância do nível da água via sensor ultrassônico.

🔴 Acende LEDs conforme o nível da água sobe, indicando a proximidade do risco.

🔔 Aciona o buzzer quando o nível atinge um estado crítico (≤ 20cm).

🕒 Exibe a distância e o status de risco no LCD junto com a hora atual.

💾 Salva os dados da medição e status na EEPROM a cada 10 segundos para histórico.

🌊 Animação personalizada no LCD durante a inicialização para melhor experiência visual.


⚙️ Mapeamento dos pinos
---

| Componente          | Pino Arduino | Função                    |
| ------------------- | ------------ | ------------------------- |
| Sensor Ultrassônico | 9 (Trig)     | Disparo do pulso          |
|                     | 10 (Echo)    | Recebe o pulso            |
| LCD I2C             | SDA (A4)     | Dados I2C                 |
|                     | SCL (A5)     | Clock I2C                 |
| RTC DS1307          | SDA (A4)     | Dados I2C (compartilhado) |
|                     | SCL (A5)     | Clock I2C (compartilhado) |
| Buzzer              | 12           | Alerta sonoro             |
| LED 1 (baixo nível) | 2            | Indicador nível baixo     |
| LED 2               | 3            | Indicador nível médio     |
| LED 3               | 4            | Indicador nível alto      |
| LED 4               | 5            | Indicador nível maior     |
| LED 5               | 6            | Indicador nível crítico   |
| LED 6               | 7            | Indicador extra           |
| LED 7               | 8            | Indicador extra           |


💡 Lógica de classificação dos leds e alerta
---

🟡 Nível Atenção (21 cm a 150 cm)

- LEDs: acendem gradativamente (do 2 ao 7) conforme o nível sobe

Buzzer: desligado

🔴 Nível Crítico (≤ 20 cm)

- LEDs: todos ligados e piscando

Buzzer: ligado continuamente

🧪 Guia para simular o projeto de monitoramento de enchentes no Wokwi
Acesse o Wokwi:
---
Entre no site https://wokwi.com.

🔧 Passo a passso para montar o projeto
---

- Preparar os Componentes: Separe Arduino, sensor ultrassônico, LCD I2C, RTC, LEDs, buzzer, resistores e cabos jumper.

- Conectar Sensor Ultrassônico: Ligue o pino Trig no Arduino pino 9 e Echo no pino 10.

- Conectar LCD e RTC: Use o barramento I2C — conecte SDA ao A4 e SCL ao A5.

- Conectar LEDs: Ligue os LEDs nos pinos 2 a 8, cada um com resistor de 1kΩ em série para limitar corrente.

- Conectar Buzzer: Ligue o buzzer no pino 12.

- Programar Arduino: Carregue o código com a lógica de medição, exibição e alerta.

- Testar: Ligue o sistema e simule níveis de água para verificar LEDs, buzzer e dados no LCD.

- Ajustes Finais: Verifique o armazenamento na EEPROM e o funcionamento do RTC.

- coloque o codigo.ino do projeto


🛑 Problemas Enfrentados
---

- Interferência no Sensor Ultrassônico: Ruído em medições causou leituras imprecisas, resolvido com filtros de software e uso de medianas.

- Sincronização do RTC: Problemas iniciais com comunicação I2C, solucionados ajustando o cabeamento e usando bibliotecas atualizadas.

- Gestão da EEPROM: Limitação na quantidade de gravações exigiu implementação de controle para evitar desgaste excessivo.

- Consumo de Energia: Muitos LEDs causaram picos, mitigado com resistores corretos e revisão do circuito.


🌟 Diferencias do projeto
---

- Registro automático e histórico das medições com data/hora.

- Alerta sonoro e visual progressivo, facilitando decisão rápida em situações de risco.

- Animação inicial no LCD para maior usabilidade e apresentação.

- Uso eficiente do barramento I2C para minimizar cabos e facilitar integração.


🎯 Considerações Finais
---

Este projeto de monitoramento do nível do rio utilizando Arduino e sensores variados demonstra uma solução prática e eficiente para a prevenção de riscos relacionados a enchentes. A integração do sensor ultrassônico com o LCD, buzzer, LEDs e a memória EEPROM permite uma leitura contínua e confiável do nível da água, além do registro histórico dos dados, fundamental para análises futuras.

O uso do RTC garante que as medições sejam corretamente temporalizadas, o que é essencial para identificar tendências e situações de risco em tempo real. A classificação visual por LEDs e alertas sonoros facilita a rápida identificação do estado do rio, contribuindo para a tomada de decisões ágeis em situações de emergência.

Além disso, o projeto é flexível e pode ser expandido para incorporar novos sensores e funcionalidades, como comunicação via rede para alertas remotos, melhorando ainda mais a capacidade de prevenção de desastres naturais.

Este sistema representa uma importante ferramenta para a comunidade, contribuindo para a segurança e proteção das populações vulneráveis às enchentes.

- 📍 link do projeto: https://wokwi.com/projects/432312991035928577

- 🎥 link do video no youtube explicando o projeto: https://youtu.be/yH5Np1vKtuc


👨‍💻 Autores
---

- Rafael Augusto Carmona

- Enzo Hort Ramos

- Eduardo Tolentino



