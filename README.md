# Servo motor PWM

Este projeto tem como objetivo apresentar as funcionalidades do PWM em um Raspberry Pi Pico W, para isso faz uso do acionamento de um servo motor em simulação e de um LED na prática.

## Como executar o código
    1-Baixar ou utilizar git clone no repositório.
          git clone https://github.com/MSoares0079/EmbarcaTech_Servo_motor_PWM.git
    2-Ter o VS Code instalado e configurado.
    3-Ter a extensão do Raspberry Pi Pico instalada.
    4-Compilar o arquivo "Servo_motor_PWM.c" utilizando o compilador do Raspberry Pi Pico.
    5-Rodar o código um Raspberry ou utilizar o "diagram.json" através da extensão do Wokwi.
    6-Caso deseje visualizar o comportamento em um LED seguir instruções do segundo vídeo.
## Lógica do código

O PWM foi definido para funcionar na frequência de 50 Hz,
portanto o código precisa configurar essa frequência através da definição de dois elementos: o wrap e o divisor de frequência.
Visando uma maior capacidade de variação nos niveis de tensão,
ou seja duty cyles mais precisos foi definido um valor arbitrario elevado para o wrap de 10.000. 
Já o divisor de frequência é obtido por uma fórmula, substituindo a frequência do sistema,
o wrap escolhido e a frequência desejada foi adquirido o valor de 250.


O codigo foi desenvolvido levando em consideração que o acionamento do servo motor não é uma dedicação exclusiva do microcontrolador. 
Logo, as mudanças dos valores de duty cycle, o que reflete na posição do servo motor,
ocorrem em interrupções o que permite uma folga do processador executar outras tarefas.  


As interrupções ocorrem através de temporizadores os quais são: 3 one shot e 1 periódico. 
Na simulação do servo motor é carregado um duty cycle no PWM para sua posição inicial e acionado o primeiro temporizador one shot,
após 5s é carregado outro valor de duty cycle e acionado o segundo temporizador one shot.
Em que novamente após 5s altera a posição do servo motor e aciona o último temporizador oneshot.
O terceiro temporizador one shot é acionado 5s depois do anterior e serve apenas para ativar o temporizado períodico.
Esse tem um período de 10ms e altera o valor do duty cycle de forma suave(+-0,025%), 
essa rotina é limitada por um valor máximo e mínimo e quando é atingido o valor máximo ocorre um decrescimento até atingir o mínimo e vice versa.

## Vídeo explicativo
(VS Code-Servo Motor)
https://drive.google.com/file/d/11t8CpOB0rvCu78U8q5IdyKi6ZJtEXC5C/view?usp=sharing


(Placa BitDogLab-LED)
https://drive.google.com/file/d/11oJN0zQ8HyMGjclqsN02X4d3Ke9JWLww/view?usp=sharing
