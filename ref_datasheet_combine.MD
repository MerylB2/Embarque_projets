
ATmega48A/PA/88A/PA/168A/PA/328/P Datasheet

Section : 14. I/O-Ports 
─────────────────────────  
Page 73  : 14.1 Overview  
Page 74  : 14.2 Ports as General Digital I/O 
Page 75  : 14.2.1 Configuring the Pin 
Page 76  : 14.2.2 Toggling the Pin (PINB toggle!)
Page 77  : 14.2.3 Switching Between Input and Output
Page 78  : 14.2.4 Reading the Pin Value
Page 78  : 14.2.6 Alternate Port Functions
Page 92  : 14.4 Register Description (DDRB, PORTB)

  Registres importants :
  • DDRB   (Data Direction Register B)     - Page 92 
  • PORTB  (Port B Data Register)          - Page 93 
  • PINB   (Port B Input Pins)             - Page 93 

```

### **Timer/Counter1 (16-bit)**
```

Section : 16. 16-bit Timer/Counter1          ──────────────────────────────────                   
                                                      
Page 108 : 16.1 Features                         
Page 108 : 16.2 Overview                         
Page 109 : 16.3 Accessing 16-bit Registers      
Page 111 : 16.4 Timer/Counter Clock Sources     
Page 111 : 16.5 Counter Unit                     
Page 112 : 16.6 Input Capture Unit              
Page 113 : 16.7 Output Compare Units            
Page 114 : 16.8 Compare Match Output Unit       
Page 116 : 16.9 Modes of Operation              
Page 117 : 16.9.2 Clear Timer on Compare (CTC)  
Page 121 : 16.9.5 Fast PWM Mode                 
Page 123 : 16.9.6 Phase Correct PWM Mode        
Page 128 : 16.11 Register Description           
                                                      
  Registres Timer1 :                                  
  • TCCR1A (Control Register A)            - Page 128
  • TCCR1B (Control Register B)            - Page 130
  • TCNT1H/L (Counter Register)            - Page 132
  • OCR1AH/L (Output Compare Register A)   - Page 132
  • OCR1BH/L (Output Compare Register B)   - Page 133
  • ICR1H/L (Input Capture Register)       - Page 133
  • TIMSK1 (Interrupt Mask Register)       - Page 134
  • TIFR1 (Interrupt Flag Register)        - Page 135

```

### **Clock System**
```

Section : 9. System Clock and Clock Options         
───────────────────────────────────────────          
                                                      
Page 26  : 9.1 Clock Systems and Distribution   
Page 27  : 9.2 Clock Sources                    
Page 29  : 9.6 External Crystal/Ceramic Resonator      
Page 35  : 9.12 Register Description            

```

### **Instruction Set**
```

Section : 33. Instruction Set Summary               
────────────────────────────────────                 
                                                      
Page 430 : Complete Instruction Set Summary     
Page 430 : NOP - No Operation (1 cycle)         
Page 431 : SBIW - Subtract Immediate from Word (2 cyc) 
Page 432 : BRNE - Branch if Not Equal (1/2 cycles)     
Page 433 : DEC - Decrement (1 cycle)            
Page 434 : RET - Return from Subroutine (4 cycles)     

```

### **Memory & Pin Configuration**
```
Page 1   : Pin Configuration / Pinout            
Page 7   : ATmega328P Features Summary           
Page 9   : 6.1 Block Diagram                    
Page 11  : 7. AVR CPU Core                      
Page 14  : 8. AVR Memories                      
Page 288 : 29. Register Summary



┌─────────────────────────────────────────────────────────────┐
│  CHAPITRE : 14. I/O-Ports                                   │
│  ─────────────────────────────────────────────              │
│                                                             │
│  Section 14.1    : Overview                                 │
│  Section 14.2    : Ports as General Digital I/O            │
│  Section 14.2.1  : Configuring the Pin                     │
│  Section 14.2.2  : Toggling the Pin ⭐ (PINB toggle!)       │
│  Section 14.2.3  : Switching Between Input and Output      │
│  Section 14.2.4  : Reading the Pin Value                   │
│  Section 14.2.6  : Alternate Port Functions                │
│  Section 14.4    : Register Description for I/O Ports      │
│                                                             │
│  Registres :                                                │
│  • DDRB   - Section 14.4.3                                 │
│  • PORTB  - Section 14.4.2                                 │
│  • PINB   - Section 14.4.1                                 │
└─────────────────────────────────────────────────────────────┘
```

### **Timer/Counter1 (16-bit)**
```
┌─────────────────────────────────────────────────────────────┐
│  CHAPITRE : 16. 16-bit Timer/Counter1 with PWM             │
│  ──────────────────────────────────────────────              │
│                                                             │
│  Section 16.1    : Features                                 │
│  Section 16.2    : Overview                                 │
│  Section 16.3    : Accessing 16-bit Registers              │
│  Section 16.4    : Timer/Counter Clock Sources             │
│  Section 16.5    : Counter Unit                             │
│  Section 16.7    : Output Compare Units                    │
│  Section 16.8    : Compare Match Output Unit               │
│  Section 16.9    : Modes of Operation ⭐                    │
│  Section 16.9.2  : Clear Timer on Compare (CTC) ⭐         │
│  Section 16.9.5  : Fast PWM Mode ⭐                         │
│  Section 16.9.6  : Phase Correct PWM Mode                  │
│  Section 16.11   : Register Description                    │
│                                                             │
│  Registres Timer1 :                                         │
│  • TCCR1A  - Section 16.11.1 (Control Register A)          │
│  • TCCR1B  - Section 16.11.2 (Control Register B)          │
│  • TCNT1   - Section 16.11.3 (Counter Value)               │
│  • OCR1A   - Section 16.11.4 (Output Compare A)            │
│  • OCR1B   - Section 16.11.5 (Output Compare B)            │
│  • ICR1    - Section 16.11.6 (Input Capture Register)      │
│  • TIMSK1  - Section 16.11.8 (Interrupt Mask)              │
│  • TIFR1   - Section 16.11.9 (Interrupt Flags)             │
│                                                             │
│  Tables importantes :                                       │
│  • Table 16-2 : Clock Select Bit Description               │
│  • Table 16-3 : Compare Output Mode, Non-PWM               │
│  • Table 16-4 : Compare Output Mode, Fast PWM              │
│  • Table 16-5 : Waveform Generation Mode Bit Description   │
└─────────────────────────────────────────────────────────────┘
```

### **Clock System**
```
┌─────────────────────────────────────────────────────────────┐
│  CHAPITRE : 9. System Clock and Clock Options               │
│  ───────────────────────────────────────────────             │
│                                                             │
│  Section 9.1  : Clock Systems and their Distribution       │
│  Section 9.2  : Clock Sources                              │
│  Section 9.6  : External Crystal/Ceramic Resonator         │
│  Section 9.12 : Register Description                       │
└─────────────────────────────────────────────────────────────┘
```

### **Instruction Set**
```
┌─────────────────────────────────────────────────────────────┐
│  CHAPITRE : 33. Instruction Set Summary                     │
│  ────────────────────────────────────                        │
│                                                             │
│  Instructions importantes :                                 │
│  • NOP   - No Operation (1 cycle)                          │
│  • SBIW  - Subtract Immediate from Word (2 cycles)         │
│  • BRNE  - Branch if Not Equal (1/2 cycles)                │
│  • DEC   - Decrement (1 cycle)                             │
│  • RET   - Return from Subroutine (4 cycles)               │
│  • LDI   - Load Immediate (1 cycle)                        │
│  • SBI   - Set Bit in I/O Register (2 cycles)              │
└─────────────────────────────────────────────────────────────┘