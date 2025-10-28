# BITWISE OPERATORS

---
## Operator: & (AND)
Result is 1 if both bits are 1

- Truth table
`0 & 0 = 0`
`1 & 0 = 0`
`0 & 1 = 0`
`1 & 1 = 1`

- Exemple
`1011 0110 & 0000 1111 = 0000 0110`
`1111 0000 & 1100 1100 = 1100 0000`

---

## Operator: | (OR)
Result is 1 if at leat one bit is 1

- Truth table
`0 | 0 = 0`
`1 | 0 = 1`
`0 | 1 = 1`
`1 | 1 = 1`

- Exemple
`0101 1010 | 0010 0100 = 0111 1110`
`0000 1111 | 0011 0000 = 0011 1111`

---
## Operator: ^ (XOR)
Result is 1 if the bits are differents

- Truth table
`0 ^ 0 = 0`
`1 ^ 0 = 1`
`0 ^ 1 = 1`
`1 ^ 1 = 0`

- Exemple
`0000 1001 ^ 0000 1010 = 0000 0011`
`1010 1010 ^ 1111 0000 = 0101 1010`

---
## Operator: ~ (NOT)
Reverse all bits

- Exemple
`~0000 1111 = 1111 0000`
`~0101 0110 = 1010 1001`

---
## Operator: << (Left shift)
Shift n to the left. Inset n 0 to the right

- Exemple
`0000 0011 << 2 = 0000 1100`
`0011 0001 << 2 = 1100 0100`

---
## Operator: >> (Right shift)
Shift n to the right. Inset n 0 to the left

- Exemple
`1001 0000 >> 4 = 0000 1001`
`0110 0110 >> 3 = 0000 1100`

---
## Usefull combinaisons

- #### set a specific bit to 1
	`1000 0000 |= (1 << 4) = 1001 0000`
	- `(1 << 4) = 0001 0000`
	- `1000 0000 | 0001 0000 = 1001 0000`
- #### set a specific bit to 0
	`1111 1100 &= ~(1 << 4) = 1110 1100`
	- `~(1 << 4) = 1110 1111`
	- `1111 1100 & 1110 1111 = 1110 1100`
- #### toggle a specific bit
	`0010 1100 ^ (1 << 3) = 0010 0100`
	- `(1 << 3) = 0000 1000`
	- `0010 1100 ^ 0000 1000 = 0010 0100`

	`0010 0100 ^ (1 << 3) = 0010 1100`
	- `(1 << 3) = 0000 1000`
	- `0010 0100 ^ 0000 1000 = 0010 1100`

