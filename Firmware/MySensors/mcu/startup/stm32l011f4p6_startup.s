
	.syntax unified
	.cpu cortex-m0plus
	.fpu softvfp
	.thumb

.global  g_pfnVectors
.global  LoopForever

.section .text.LoopForever,"ax",%progbits

	.section  .text.Reset_Handler
		.weak  Reset_Handler
		.type  Reset_Handler, %function

Reset_Handler:

	ldr   r0, =_estack
	mov   sp, r0

	movs  r1, #0
	b  LoopCopyDataInit

CopyDataInit:
	ldr  r3, =_sidata
	ldr  r3, [r3, r1]
	str  r3, [r0, r1]
	adds  r1, r1, #4

LoopCopyDataInit:
	ldr  r0, =_sdata
	ldr  r3, =_edata
	adds  r2, r0, r1
	cmp  r2, r3
	bcc  CopyDataInit
	ldr  r2, =_sbss
	b  LoopFillZerobss

FillZerobss:
	movs  r3, #0
	str  r3, [r2]
	adds r2, r2, #4

LoopFillZerobss:
	ldr  r3, = _ebss
	cmp  r2, r3
	bcc  FillZerobss

	bl  main

LoopForever:
    b LoopForever

.size  Reset_Handler, .-Reset_Handler

	.section  .isr_vector,"a",%progbits
	.type  g_pfnVectors, %object
	.size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word  WWDG_IRQHandler
  .word  PVD_IRQHandler
  .word  RTC_IRQHandler
  .word  FLASH_IRQHandler
  .word  RCC_IRQHandler
  .word  EXTI0_1_IRQHandler
  .word  EXTI2_3_IRQHandler
  .word  EXTI4_15_IRQHandler
  .word  0
  .word  DMA1_Channel1_IRQHandler
  .word  DMA1_Channel2_3_IRQHandler
  .word  DMA1_Channel4_5_IRQHandler
  .word  ADC1_COMP_IRQHandler
  .word  LPTIM1_IRQHandler
  .word  0
  .word  TIM2_IRQHandler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  TIM21_IRQHandler
  .word  0
  .word  0
  .word  I2C1_IRQHandler
  .word  0
  .word  SPI1_IRQHandler
  .word  0
  .word  0
  .word  USART2_IRQHandler
  .word  LPUART1_IRQHandler
  .word  0
  .word  0

   .weak      NMI_Handler
   .thumb_set NMI_Handler,LoopForever

   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,LoopForever

   .weak      SVC_Handler
   .thumb_set SVC_Handler,LoopForever

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,LoopForever

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,LoopForever

   .weak      WWDG_IRQHandler
   .thumb_set WWDG_IRQHandler,LoopForever

   .weak      PVD_IRQHandler
   .thumb_set PVD_IRQHandler,LoopForever

   .weak      RTC_IRQHandler
   .thumb_set RTC_IRQHandler,LoopForever

   .weak      FLASH_IRQHandler
   .thumb_set FLASH_IRQHandler,LoopForever

   .weak      RCC_IRQHandler
   .thumb_set RCC_IRQHandler,LoopForever

   .weak      EXTI0_1_IRQHandler
   .thumb_set EXTI0_1_IRQHandler,LoopForever

   .weak      EXTI2_3_IRQHandler
   .thumb_set EXTI2_3_IRQHandler,LoopForever

   .weak      EXTI4_15_IRQHandler
   .thumb_set EXTI4_15_IRQHandler,LoopForever

   .weak      DMA1_Channel1_IRQHandler
   .thumb_set DMA1_Channel1_IRQHandler,LoopForever

   .weak      DMA1_Channel2_3_IRQHandler
   .thumb_set DMA1_Channel2_3_IRQHandler,LoopForever

   .weak      DMA1_Channel4_5_IRQHandler
   .thumb_set DMA1_Channel4_5_IRQHandler,LoopForever

   .weak      ADC1_COMP_IRQHandler
   .thumb_set ADC1_COMP_IRQHandler,LoopForever

   .weak      LPTIM1_IRQHandler
   .thumb_set LPTIM1_IRQHandler,LoopForever

   .weak      TIM2_IRQHandler
   .thumb_set TIM2_IRQHandler,LoopForever

   .weak      TIM21_IRQHandler
   .thumb_set TIM21_IRQHandler,LoopForever

   .weak      I2C1_IRQHandler
   .thumb_set I2C1_IRQHandler,LoopForever

   .weak      SPI1_IRQHandler
   .thumb_set SPI1_IRQHandler,LoopForever

   .weak      USART2_IRQHandler
   .thumb_set USART2_IRQHandler,LoopForever

   .weak      LPUART1_IRQHandler
   .thumb_set LPUART1_IRQHandler,LoopForever
