// -----------------------------------------------------------------------------
// ----- Constantes -----
#define verde RA0_bit
#define vermelho RA1_bit
#define amarelo RA2_bit

// -----------------------------------------------------------------------------
// ----- Variaveis globais -----
unsigned short pulsoVerde = 0x00;                                               // variavel para armazenar o pulso do verde
unsigned short pulsoVermelho = 0x00;                                            // variavel para armazenar o pulso do vermelho
unsigned short tempoAmarelo = 0x00;                                             // variavel para armazenar o tempo de amarelo ligado
unsigned short tempoVermelho = 0x05;                                            // variavel que manda a quantidade barrinhas vermelha acenderem
unsigned short tempoVerde = 0x05;                                               // variavel que manda a quantidade barrinhas verde acenderem
unsigned short tempo4MsVerde = 0x00;                                            // variavel para armazenar tempo de 4ms de verde aceso
unsigned short tempo100MsVerde = 0x00;                                          // variavel para armazenar tempo de 100ms de verde acesso
unsigned short tempo4MsVermelho = 0x00;
unsigned short tempo100MsVermelho = 0x00;
unsigned short flag_vd_vm = 0x00;
unsigned short tempo_vd_vm = 0x00;
// -----------------------------------------------------------------------------
// ----- Função de interrupções -----
void interrupt(){                                                               // função para tratar as interrupções

     if(T0IF_bit){                                                              // houve o estouro do TMR0?
         RA3_bit = 0x00;                                                        // porta para mandar informação para o MOC
         T0IF_bit = 0x00;                                                       // limpa a flag do TMR0

         if(amarelo){                                                           // se amarelo ligado
              tempoAmarelo++;                                                   // incrementa o tempo do amarelo
              if(amarelo && tempoAmarelo >= 15 && tempoAmarelo < 27 && verde){  // se tempo do amarelo estiver entre 80 e 108 ms e verde estiver ligado
                  RA3_bit = 0x00;                                               // desliga a porta que vai para o MOC
                  tempoAmarelo = 0x00;                                          // reseta o tempo de amarelo ligado
                  pulsoVerde = 0x01;                                            // informa o pulso Verde
              }

              if(amarelo && tempoAmarelo >= 15 && tempoAmarelo < 27 && vermelho){// e tempo do amarelo estiver entre 80 e 108 ms e vermelho estiver ligado
                  RA3_bit = 0x00;                                               // desliga porta que vai para o MOC
                  tempoAmarelo = 0x00;                                          // reseta tempo de amarelo ligado
                  pulsoVermelho = 0x01;                                         // informa o pulso Vermelho
              }

              if(amarelo && tempoAmarelo >= 28){                                 // se amarelo ligado por tempo maior que 108 ms liga bolacha amarela
                  RB0_bit = 0x00;                                                //
                  RB1_bit = 0x00;                                                //
                  RB2_bit = 0x00;                                                // apaga as barrinhas verde
                  RB3_bit = 0x00;                                                //
                  RB4_bit = 0x00;                                                //
                  
                  tempoAmarelo = 0x00;                                           // reseta tempo do amarelo
                  pulsoVermelho = 0x00;                                          // zera o pulso do vermelho
                  pulsoVerde = 0x00;                                             // zera o pulso do verde
                  tempoVermelho = 0x05;                                          // reseta as barrinhas vermelha
                  tempoVerde = 0x05;                                             // reseta as barrinhas verde
                  flag_vd_vm = 0x00;
                  while(amarelo){
                      RA3_bit = 0x01;                                            // libera porta do MOC para acender a bolacha amarela
                  }
             }
         } else {
             tempoAmarelo = 0x00;
         }

         if(vermelho && pulsoVermelho){                                         // vermelho esta acesso e recebeu o pulso?
             tempo4MsVermelho++;                                                // incrementa variavel de 4ms de vermelho aceso
             if(tempo4MsVermelho == 25){                                        // tempo igual 100ms?
                 tempo4MsVermelho = 0x00;                                       // zera variavel de 4ms de vermelho
                 tempo100MsVermelho++;                                          // incrementa variavel de 100ms de vermelho aceso
                 if(tempo100MsVermelho == 30){                                  // tempo igual 3 segundos?
                     tempo100MsVermelho = 0x00;                                 // zera variavel de tempo de 100ms;
                     if(tempoVermelho > 1){                                     // tempo de vermelho maior que 0?
                         tempoVermelho--;                                       // se sim...decrementa tempo de vermelho se superior a 0 segundos
                     } else {                                                   // senão...
                          tempoVermelho = 0x01;                                 // seta tempo de vermelho para 0 segundos
                     }
                 }
             }
         } else {                                                               // senao...
             tempo4MsVermelho = 0x00;                                           // zera variavel de 4ms de vermelho aceso
             tempo100MsVermelho = 0x00;                                         // zera variavel de 4ms de vermelho aceso
             tempoVermelho = 0x05;                                              // reinicia a variavel para acender as 5 barrinhas vermelha
         }
         
         if(verde && pulsoVerde){                                               // verde esta acesso e recebeu o pulso?
             tempo4MsVerde++;                                                   // incrementa variavel de 4ms de verde aceso
             if(tempo4MsVerde == 25){                                           // tempo igual 100ms?
                 tempo4MsVerde = 0x00;                                          // zera variavel de 4ms de verde
                 tempo100MsVerde++;                                             // incrementa variavel de 100ms de verde aceso
                 if(tempo100MsVerde == 18){                                     // tempo igual 2 segundos?
                     tempo100MsVerde = 0x00;                                    // zera variavel de tempo de 100ms;
                      if(tempoVerde > 1){                                       // tempo de verde maior que 0?
                         tempoVerde--;                                          // se sim... decrementa tempo de verde
                     } else {                                                   // senao...
                          tempoVerde = 0x00;                                    // seta tempo de verde para 0
                     }
                 }
             }
         } else {
             tempo4MsVerde = 0x00;                                              // zera variavel de 4ms de verde aceso
             tempo100MsVerde = 0x00;                                            // zera variavel de 4ms de verde aceso
             tempoVerde = 0x05;                                                 // reinicia a variavel para acender as 5 barrinhas verde
         }
         
         if(verde && vermelho){
             tempo_vd_vm++;
             if(tempo_vd_vm == 25){
                 tempo_vd_vm = 0x00;
                 flag_vd_vm = 0x01;
             }
         } else {
             tempo_vd_vm = 0x00;
         }

     } // fim da verificação se houve o estouro do TMR0

} // fim do interrupt

// -----------------------------------------------------------------------------
// ----- Função Principal -----
void main() {

     CMCON = 0x07;                                                              // desabilita os comparados internos do PIC
     OPTION_REG = 0x83;                                                         // prescaler(1 : 16) associado as TMR0 ~4ms
     GIE_bit = 0x01;                                                            // habilita as interrupções globais
     PEIE_bit = 0x01;                                                           // habilita as interrupções por perifericos
     T0IE_bit = 0x01;                                                           // habilita as interrupções por estouro do TMR0
     TMR0 = 0x00;                                                               // inicia o TMR0 em 0
     TRISB = 0x00;                                                              // faz com que todo o PORTB seja saida digital
     PORTB = 0x00;                                                              // inicia todo o PORTB em LOW
     TRISA = 0x27;                                                              // RA0, RA1, RA2 e RA5 sao entrada / restante saida
     PORTA = 0x20;                                                              // inicia todo PORTB em LOW - restante em LOW


     while(1){
          if(verde && !flag_vd_vm){                                                            // se verde estiver ligado
                switch(tempoVerde){
                     case 5:                                                    // acende as 5 barrinhas verde
                         RB5_bit = 0x00;                                        //
                         RB6_bit = 0x00;                                        //
                         RB7_bit = 0x00;                                        // Apaga as barrinhas vermelha
                         RA6_bit = 0x00;                                        //
                         RA7_bit = 0x00;                                        //
                         
                         RB0_bit = 0x01;
                         RB1_bit = 0x01;
                         RB2_bit = 0x01;
                         RB3_bit = 0x01;
                         RB4_bit = 0x01;
                         break;
                     case 4:                                                    // acende as 4 barrinhas verde
                         RB0_bit = 0x01;
                         RB1_bit = 0x01;
                         RB2_bit = 0x01;
                         RB3_bit = 0x01;
                         RB4_bit = 0x00;
                         break;
                     case 3:                                                    // acende as 3 barrinhas verde
                         RB0_bit = 0x01;
                         RB1_bit = 0x01;
                         RB2_bit = 0x01;
                         RB3_bit = 0x00;
                         RB4_bit = 0x00;
                         break;
                     case 2:                                                    // acende as 2 barrinhas verde
                         RB0_bit = 0x01;
                         RB1_bit = 0x01;
                         RB2_bit = 0x00;
                         RB3_bit = 0x00;
                         RB4_bit = 0x00;
                         break;
                     case 1:                                                    // acende a 1 barrinha verde
                         RB0_bit = 0x01;
                         RB1_bit = 0x00;
                         RB2_bit = 0x00;
                         RB3_bit = 0x00;
                         RB4_bit = 0x00;
                         break;
                     case 0:                                                    // apaga as 5 barrinhas verde
                         RB0_bit = 0x00;
                         RB1_bit = 0x00;
                         RB2_bit = 0x00;
                         RB3_bit = 0x00;
                         RB4_bit = 0x00;
                         break;
                     default:                                                   // acende as 5 barrinhas verde
                         RB0_bit = 0x01;
                         RB1_bit = 0x01;
                         RB2_bit = 0x01;
                         RB3_bit = 0x01;
                         RB4_bit = 0x01;
                         break;
                } // fim do switch
          } else if(verde && flag_vd_vm){
                RB0_bit = 0x00;
                RB1_bit = 0x00;
                RB2_bit = 0x00;
                RB3_bit = 0x00;
                RB4_bit = 0x00;
          }

          if(vermelho && !flag_vd_vm){                                                         // se vermelho estiver ligado
                switch(tempoVermelho){
                      case 5:                                                   // acende as 5 barrinhas vermelha
                           RB0_bit = 0x00;                                      //
                           RB1_bit = 0x00;                                      //
                           RB2_bit = 0x00;                                      // apaga as barrinhas verde
                           RB3_bit = 0x00;                                      //
                           RB4_bit = 0x00;                                      //
                  
                           RB5_bit = 0x01;
                           RB6_bit = 0x01;
                           RB7_bit = 0x01;
                           RA6_bit = 0x01;
                           RA7_bit = 0x01;
                           break;
                      case 4:                                                   // acende as 4 barrinhas vermelha
                           RB5_bit = 0x01;
                           RB6_bit = 0x01;
                           RB7_bit = 0x01;
                           RA6_bit = 0x01;
                           RA7_bit = 0x00;
                           break;
                      case 3:                                                   // acende as 3 barrinhas vermelha
                           RB5_bit = 0x01;
                           RB6_bit = 0x01;
                           RB7_bit = 0x01;
                           RA6_bit = 0x00;
                           RA7_bit = 0x00;
                           break;
                      case 2:                                                   // acende as 2 barrinhas vermelha
                           RB5_bit = 0x01;
                           RB6_bit = 0x01;
                           RB7_bit = 0x00;
                           RA6_bit = 0x00;
                           RA7_bit = 0x00;
                           break;
                      case 1:                                                   // acende 1 barrinha vermelha
                           RB5_bit = 0x01;
                           RB6_bit = 0x00;
                           RB7_bit = 0x00;
                           RA6_bit = 0x00;
                           RA7_bit = 0x00;
                           break;
                      case 0:                                                   // apaga as 5 barrinhas vermelha
                           RB5_bit = 0x00;
                           RB6_bit = 0x00;
                           RB7_bit = 0x00;
                           RA6_bit = 0x00;
                           RA7_bit = 0x00;
                           break;
                      default:                                                  // acende as 5 barrinhas vermelha
                           RB5_bit = 0x01;
                           RB6_bit = 0x01;
                           RB7_bit = 0x01;
                           RA6_bit = 0x01;
                           RA7_bit = 0x01;
                           break;
                } // fim do switch
          } else if(vermelho && flag_vd_vm){
                RB5_bit = 0x00;
                RB6_bit = 0x00;
                RB7_bit = 0x00;
                RA6_bit = 0x00;
                RA7_bit = 0x00;
          }
     } // fim do while
} // fim do main
