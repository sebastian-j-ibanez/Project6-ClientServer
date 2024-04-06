@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Prompt the user for input
SET /P userInput=How many planes to take off?: 

REM Display the user's input
ECHO Number of planes taking off: %userInput%

SET /P startingId=What is your starting Id?:
ECHO What ID would you like to start at: %startingId%

SET /A max=%userInput%+%startingId%

REM Execute a command for the specified number of iterations
FOR /L %%x IN (%startingId%, 1, %max%) DO (
	ECHO Iteration %%x
	
	START %~dp0\x64\Release\Project6-Client.exe %%x
)

SET /P endMenu=Press the enter key to end program: 
