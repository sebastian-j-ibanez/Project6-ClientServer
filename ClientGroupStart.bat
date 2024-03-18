@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Prompt the user for input
SET /P userInput=How many planes to take off?: 

REM Display the user's input
ECHO Number of planes taking off: %userInput%

REM Execute a command for the specified number of iterations
FOR /L %%x IN (1, 1, %userInput%) DO (
	ECHO Iteration %%x
	
	START %~dp0\x64\Debug\Project6-Client.exe --%%x
)

SET /P endMenu=Press the enter key to end program: 