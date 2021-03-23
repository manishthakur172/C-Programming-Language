/*
 * State Machine v1.1c

 *
 *  Created on: 22nd March
 *      Author: Manish Thakur
 */

#include <stdio.h>
#include <stdlib.h>

/*****
* State definitions, states are the routine the program jumps to, these are prefixed with an 'S' for easy identification
*****/
enum states { S_OFF, S_ON, S_PROCESS, S_MAX };


/*****
* Event definitions, events trigger a transition and are prefixed with an 'E' for easy identification
*****/
enum events { E_OFF, E_ON, E_START, E_MAX};


/*****
* Function prototypes
*****/
void UponEnter( int State);
void ActionWhileInState( int State );
void UponExit( int State);
enum states StateMachine(enum events event, enum states Current_State);
int ReadKeyInput(void);

/*****
* Main function starts here
*****/
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);	// Lines required as Eclipse has buffer issues when used with Windows and the scanf function
	setvbuf(stderr, NULL, _IONBF, 0);	// Lines required as Eclipse has buffer issues when used with Windows and the scanf function

	// This line and the way the StateMachine function is called inside the while loop
	//avoids using Current_State as a global variable which improves the code
	enum states Current_State = S_OFF;

    while ( Current_State != S_MAX )
    {
		// Function call for the state machine, which then calls the read keys function and waits for the value returned
        Current_State = StateMachine( ReadKeyInput(), Current_State );
    }

    return 0;
}

/*****
* Read Keys function definition containing the code to execute
*****/
int ReadKeyInput()
{
    char input;
    scanf("%c", &input);
    switch (input)
    {
        case '1':
            return E_OFF;
            break;
        case '2':
            return E_ON;
            break;
        case '3':
            return E_START;
            break;
    }
    return E_MAX; //
}

/*****
* State Machine function definition containing the code to execute
*****/
enum states StateMachine(enum events event, enum states Current_State)
{
    int Next_State = Current_State;

    switch ( Current_State )
    {
    case S_OFF:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_ON:
            	Next_State = S_ON;
                break;
        }
        break;
    case S_ON:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_OFF:
            	Next_State = S_OFF;
                break;
            case E_START:
            	Next_State = S_PROCESS;
                break;
        }
        break;
    case S_PROCESS:
        switch (event )
        {
            // A transition to the next state will occur here
        case E_OFF:
        	Next_State = S_OFF;
            break;
        }
        break;
        // The program should never arrive here
		default:
        break;
    }

    if (Next_State != Current_State)
    {
		// Function call for Upon Exit function, it can be omitted but allows extra functionality
        UponExit(Current_State);
		// Function call for Upon Enter function, it can be omitted but allows extra functionality
        UponEnter(Next_State);
        Current_State = Next_State;
    }

    if ( event != E_MAX) ActionWhileInState( Current_State );

    return Next_State;
}

/*****
* Upon Enter function definition containing the code to execute
*****/
void UponEnter( int State)
{
    switch (State)
    {
    case S_OFF:
        // Custom code here to execute when entering the new state
    	puts("This is the UponEnter function running for the S_OFF state");
		break;
    case S_ON:
		// Custom code here to execute when entering the new state
    	puts("This is the UponEnter function running for the S_ON state");
		break;
    case S_PROCESS:
		// Custom code here to execute when entering the new state
    	puts("This is the UponEnter function running for the S_PROCESS state");
		break;
    }
}

/*****
* Action While function definition containing the code to execute
*****/
void ActionWhileInState( int State)
{
    switch (State)
    {
    case S_OFF:
		// Custom code here to execute while in the current state
    	puts("Current state S_OFF - ActionWhileInState function");
		break;
    case S_ON:
		// Custom code here to execute while in the current state
    	puts("Current state S_ON - ActionWhileInState function");
		break;
    case S_PROCESS:
		// Custom code here to execute while in the current state
    	puts("Current state S_PROCESS - ActionWhileInState function");
		break;

    }
}

/*****
* Upon Exit function definition containing the code to execute
*****/
void UponExit( int State)
{
    switch (State)
    {
    case S_OFF:
        // Custom code here to execute when exiting the current state
    	puts("This is the UponExit function running for the S_OFF state");
		break;
    case S_ON:
		// Custom code here to execute when exiting the current state
    	puts("This is the UponExit function running for the S_ON state");
		break;
    case S_PROCESS:
		// Custom code here to execute when exiting the current state
    	puts("This is the UponExit function running for the S_PROCESS state");
		break;
    }
}
