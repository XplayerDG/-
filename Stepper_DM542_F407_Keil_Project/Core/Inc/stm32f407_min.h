#ifndef STM32F407_MIN_H
#define STM32F407_MIN_H

#include <stdint.h>

#define __IO volatile

#define PERIPH_BASE           0x40000000UL
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)

#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00UL)
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)

#define FLASH_BASE            0x40023C00UL
#define SCB_BASE              0xE000ED00UL
#define SYSTICK_BASE          0xE000E010UL
#define NVIC_ISER0            (*(__IO uint32_t *)0xE000E100UL)

typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t AHB3ENR;
    uint32_t RESERVED2;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    __IO uint32_t AHB1LPENR;
    __IO uint32_t AHB2LPENR;
    __IO uint32_t AHB3LPENR;
    uint32_t RESERVED4;
    __IO uint32_t APB1LPENR;
    __IO uint32_t APB2LPENR;
    uint32_t RESERVED5[2];
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    uint32_t RESERVED6[2];
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
} RCC_TypeDef;

typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
} TIM_TypeDef;

typedef struct {
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_TypeDef;

typedef struct {
    __IO uint32_t ACR;
} FLASH_TypeDef;

typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __IO uint32_t CALIB;
} SysTick_TypeDef;

#define RCC                  ((RCC_TypeDef *)RCC_BASE)
#define GPIOA                ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOD                ((GPIO_TypeDef *)GPIOD_BASE)
#define TIM2                 ((TIM_TypeDef *)TIM2_BASE)
#define USART1               ((USART_TypeDef *)USART1_BASE)
#define FLASH                ((FLASH_TypeDef *)FLASH_BASE)
#define SysTick              ((SysTick_TypeDef *)SYSTICK_BASE)

#define RCC_AHB1ENR_GPIOAEN  (1UL << 0)
#define RCC_AHB1ENR_GPIODEN  (1UL << 3)
#define RCC_APB1ENR_TIM2EN   (1UL << 0)
#define RCC_APB2ENR_USART1EN (1UL << 4)

#define USART_SR_RXNE        (1UL << 5)
#define USART_SR_TXE         (1UL << 7)
#define USART_CR1_UE         (1UL << 13)
#define USART_CR1_TE         (1UL << 3)
#define USART_CR1_RE         (1UL << 2)

#define TIM_CR1_CEN          (1UL << 0)
#define SYSTICK_CTRL_ENABLE  (1UL << 0)
#define SYSTICK_CTRL_TICKINT (1UL << 1)
#define SYSTICK_CTRL_CLKSRC  (1UL << 2)

extern uint32_t SystemCoreClock;

void SystemInit(void);
void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t millis(void);

#endif
