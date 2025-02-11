#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define MOTOR 22// Pino do servo motor
#define TEMPO1 5000//Tempo dos temporizadores oneshot
#define TEMPO2 10//Tempo dos temporizador periodico
static volatile int sinal = 1;//Define se na rotina o PWM aumenta(1) ou diminui(-1)
static volatile float MIN = 2.5,MAX =12.0,duty_cycle = 2.5,taxa=0.025;//Valores mínimos,máximos e inicial do duty clycle na rotina.
//static volatile float MIN = 0.0,MAX =100.0,duty_cycle = 0,taxa=20;//Valores mínimos,máximos e inicial do duty clycle na rotina.
struct repeating_timer timer;

//Protótipo das funções
void setup_pwm(uint gpio, float duty_cycle);
int64_t turn_off_callback1(alarm_id_t id, void *user_data);
int64_t turn_off_callback2(alarm_id_t id, void *user_data);
int64_t turn_off_callback3(alarm_id_t id, void *user_data);
bool repeating_timer_callback(struct repeating_timer *t);

int main() {
    stdio_init_all();
    setup_pwm(MOTOR, 12.0);//Servo motor em posição de 180º
    add_alarm_in_ms(TEMPO1, turn_off_callback1, NULL, false);//Programa o temporizador oneshot 1
    while(true){
        printf("Processador ocupado em outra tarefa\n");//Indica que o processador poderia estar ocupado com outra tarefa
        sleep_ms(100);
    }
    return 0;
}
void setup_pwm(uint gpio, float duty_cycle) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);// Configura GPIO para PWM  
    uint slice = pwm_gpio_to_slice_num(gpio);// Obtém o slice associado ao pino GPIO
    pwm_set_wrap(slice, 10000);// Define o contador máximo (wrap) para 10.000
    pwm_set_clkdiv(slice, 250.0f);// Configura divisor de clock para 250 par obter um clock base de 500kHz  
    pwm_set_gpio_level(gpio, 10000 * (duty_cycle / 100.0));// Define o nível do duty cycle em porcentagem
    pwm_set_enabled(slice, true);// Ativa o PWM
}
int64_t turn_off_callback1(alarm_id_t id, void *user_data) {
    setup_pwm(MOTOR, 7.35);//Servo motor em posição de 90º
    add_alarm_in_ms(TEMPO1, turn_off_callback2, NULL, false);//Programa o temporizador oneshot 2
    return 0;// Retorna 0 para indicar que o alarme não deve se repetir.
}
int64_t turn_off_callback2(alarm_id_t id, void *user_data) {
    setup_pwm(MOTOR, 2.5);//Servo motor em posição de 0º
    add_alarm_in_ms(TEMPO1, turn_off_callback3, NULL, false);//Programa o temporizador oneshot 3
    return 0;// Retorna 0 para indicar que o alarme não deve se repetir.
}
int64_t turn_off_callback3(alarm_id_t id, void *user_data) {
    add_repeating_timer_ms(TEMPO2, repeating_timer_callback, NULL, &timer);//Programa o temporizador periodico
    return 0;// Retorna 0 para indicar que o alarme não deve se repetir.
}
bool repeating_timer_callback(struct repeating_timer *t) {
    duty_cycle +=sinal*taxa;//O duty cycle varia 0,025% a cada iteração, alternando até obter o valor máximo ou minimo 
    if(duty_cycle>=MAX){
        sinal = -1;//Quando chega no valor máximo o sinal troca e depois o duty cyle reduz 0,025% por iteração
    }else if(duty_cycle<=MIN){
        sinal = 1;//Quando chega no valor minimo o sinal troca e depois o duty cyle aumenta 0,025% por iteração
    }
    setup_pwm(MOTOR, duty_cycle);//Altero o PWM com o valor do duty cycle atual 
    return true;// Retorna 1 para indicar que o alarme deve se repetir.
}
