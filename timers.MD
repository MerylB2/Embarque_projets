
## control registers for each timers:
	- timer0:
		TCCR0A
		TCCR0B
	- timer1:
		TCCR1A
		TCCR1B
	- timer2:
		TCCR2A
		TCCR2B

---
## value registers for each timers:
	timer0 (8 bits): value-> TCNT0
	timer1 (16 bits): value-> TCNT1 (TCNT1H and TCNT1L)
	timer2 (8 bits): value-> TCNT2

---
## comparaison registers for each timers
	timer0:
		OCR0A
		OCR0B
	timer1:
		OCR1AH, OCR1AL
		OCR1BH, OCR1BL
	timer2:
		OCR2A
		OCR2B
---
## flag registers for each timers
	timer0: TIFR0
		TOV0 ->(0) = toggle to 1 when timer overflow
		OCF0A ->(1) = toggle to 1 when timer0 value = comparaison register A
		OCF0B ->(2) = toggle to 1 when timer0 value = comparaison register B
	timer1: TIFR1
		TOV1 ->(0) = toggle to 1 when timer overflow
		OCF1A ->(1) = toggle to 1 when timer1 value = comparaison register A
		OCF1B ->(2) = toggle to 1 when timer1 value = comparaison register B
	timer2: TIFR2
		TOV2 ->(0) = toggle to 1 when timer overflow
		OCF2A ->(1) = toggle to 1 when timer2 value = comparaison register A
		OCF2B ->(2) = toggle to 1 when timer2 value = comparaison register B

---
## inpteruption registers for each timers
	timer0: TIMSK0
		OCIE0B ->(2) = set to 1 to make interup when OCF0B flag is raised
		OCIE0A ->(1) = set to 1 to make interup when OCF0A flag is raised
		TOIE0 ->(0) = set to 1 to make interup when TOV0 flag is raised
	timer1: TIMSK1
		OCIE1B ->(2) = set to 1 to make interup when OCF1B flag is raised
		OCIE1A ->(1) = set to 1 to make interup when OCF1A flag is raised
		TOIE1 ->(0) = set to 1 to make interup when TOV1 flag is raised
	timer2: TIMSK2
		OCIE2B ->(2) = set to 1 to make interup when OCF2B flag is raised
		OCIE3A ->(1) = set to 1 to make interup when OCF2A flag is raised
		TOIE3 ->(0) = set to 1 to make interup when TOV2 flag is raised

---
## prescaler: (for MCU)
	MCU: 1, 2, 4, 8, 16, 32, 64, 128, 256

---
## predivisor: (for timers)
	timer0: 1, 8, 64, 256, 1024
	timer1: 1, 8, 64, 256, 1024
	timer2: 1, 8, 32, 64, 128, 256, 1024

---
## compute timer frequence:
	MCU: 16MHz
	prescaler: 4
	timer2 predivisor: 32
		-> 16MHz / 4 / 32 = 128 kHz.

---
## change prescaler value with CLKPR register:
	1/ set the CLKPCE (MSB of CLKPR) to 1 and other to 0
		-> 1000 0000 in CLKPR
	2/ before 4 clock time deadline, set the CLKPCE to 0 and set
	   to the last 4 bit the wished prescaler value (CLKPR3, CLKPR2, CLKPR1, CLKPR0)

	(3)(2)(1)(0)(res)
	 0  0  0  0  -> 1
	 0  0  0  1  -> 2
	 0  0  1  0  -> 4
	 0  0  1  1  -> 8
	 0  1  0  0  -> 16
	 0  1  0  1  -> 32
	 0  1  1  0  -> 64
	 0  1  1  1  -> 128
	 1  0  0  0  -> 256

	-> 0000 xxxx in CLKPR

---
## change predivisor values in associated registers
	timer0: TCCR0B (CS02 ->(2) CS01 ->(1) CS00 ->(0))
	(2)(1)(0)(res)
	 0  0  0  -> STOP
	 0  0  1  -> /1
	 0  1  0  -> /8
	 0  1  1  -> /64
	 1  0  0  -> /256
	 1  0  1  -> /1024

	timer1: TCCR1B (CS12 ->(2) CS11 ->(1) CS10 ->(0))
	(2)(1)(0)(res)
	 0  0  0  -> STOP
	 0  0  1  -> /1
	 0  1  0  -> /8
	 0  1  1  -> /64
	 1  0  0  -> /256
	 1  0  1  -> /1024

	timer2: TCCR2B (CS22 ->(2) CS21 ->(1) CS20 ->(0))
	(2)(1)(0)(res)
	 0  0  0  -> STOP
	 0  0  1  -> /1
	 0  1  0  -> /8
	 0  1  1  -> /32
	 0  1  1  -> /64
	 1  0  1  -> /128
	 1  0  0  -> /256
	 1  0  1  -> /1024
---
