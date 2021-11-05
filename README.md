# Stop-Watch-System

Stop Watch system is implemented using Timer1 in ATmega16 with CTC mode to count the Stop Watch time and six common anode 7-segments.

External Interrupt INT0 is configured with falling edge and a push button with the internal pull-up resistor is connected. If a falling edge is detected,
the Stop Watch time should be reset.

External Interrupt INT1 is configured with raising edge and a push button with the external pull-down resistor is connected. If a rising edge is detected, 
the Stop Watch time should be paused.

External Interrupt INT2 is configured with falling edge and a push button with the internal pull-up resistor is connected. If a falling edge is detected,
the Stop Watch time should be resumed.
