# CFAG19232C1 Family

Example Seeeduino (Arduino clone) code for the Crystalfontz CFAG19232C and CFAG19232C1 family of displays. More information about these displays can be found at the following links.

- [CFAG19232C1-YYH-TT](https://www.crystalfontz.com/product/cfag19232c1yyhtt)
- [CFAG19232C-TMI-TT](https://www.crystalfontz.com/product/cfag19232ctmitt)

This code is also compatible with the obsoleted [CFAG19232C-YYH-TT](https://www.crystalfontz.com/product/cfag19232cyyhtt).

## Connection Details
### 8080 parallel configuration
| DISPLAY | ARDUINO | Port  | Function - 6800 Parallel |
| :-----: | :-----: | :---: | :----------------------- |
| 1       | GND     | ANY   | GROUND                   |
| 2       | 5V      | ANY   | POWER 5V                 |
| ------- | ------- | ----- | ------------------------ |
| 3       | N/A     | N/A   | NC                       |
| ------- | ------- | ----- | ------------------------ |
| 4       |  D8     | PORTB | Register Select     (RS) |
| 5       |  D9     | PORTB | Read/Write         (R/W) |
| 6       |  D10    | PORTB | Chip Enable          (E) |
| ------- | ------- | ----- | ------------------------ |
| 7       |  D0     | PORTD | (DB0)                    |
| 8       |  D1     | PORTD | (DB1)                    |
| 9       |  D2     | PORTD | (DB2)                    |
| 10      |  D3     | PORTD | (DB3)                    |
| 11      |  D4     | PORTD | (DB4)                    |
| 12      |  D5     | PORTD | (DB5)                    |
| 13      |  D6     | PORTD | (DB6)                    |
| 14      |  D7     | PORTD | (DB7)                    |
| ------- | ------- | ----- | ------------------------ |
| 15      | N/A     | N/A   | NC                       |
| 16      | N/A     | N/A   | NC                       |
