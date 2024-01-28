# Useful Information
## How to play
Just download the file and compile it on your system, then run it as any other C++ program.
## Displaying UTF-8 characters
### For Linux:
It should work correctly and no fixes should have to be made to display UTF-8 characters (suits).
### For Windows:
If suits aren't displaying correctly on Windows Powershell, you can run the following command to let PS know that it should use UTF-8 when communicating with external programs:
```
$OutputEncoding = [Console]::InputEncoding = [Console]::OutputEncoding = New-Object System.Text.UTF8Encoding
```
You can check this [answer](https://stackoverflow.com/questions/49476326/displaying-unicode-in-powershell/49481797#49481797) in Stack Overflow for more information.
