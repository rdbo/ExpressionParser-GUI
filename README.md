# ExpressionParser-GUI
<h1>Setting up the solution</h1>
<ul>
  <li>Download and install <a href="https://www.microsoft.com/en-us/download/details.aspx?id=6812">DirectX SDK</a></li>
  <li>Open the solution on Visual Studio and open the project Properties</li>
  <li>Go to VC++ Directories -> Include Directories. Click on 'Edit' and select the Include folder <br/>located on your DirectX SDK installation path. It is generally this one: <br/><i>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Include\</i> or <i>%DXSDK_DIR%\Include\</i>
  <li>Now go to VC++ Directories -> Library Directories. Click on 'Edit' and select the library folder <br/> located on your DirectX SDK installation path. It is generally this one - choose x86 for 32bit and x64 for 64bit: <br/><i>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Lib\</i> or <i>%DXSDK_DIR%\Lib\</i></li>
  <li><b>Done!</b></li>
</ul>

<h1>How to use</h1>
<p>Enter an expression in the input box, like '5*5' or 'sqrt(9)' and then click on 'Parse'. You'll have the result display right after the 'Output:' text</p>
<p>What to input?</p>
<p>Operators: +-*/!^%</p>
<p>Math functions: pow(x,y), sqrt(x), log(x), exp(x), sin(x), cos(x), tan(x), atan(x), atan2(y,x), abs(x), floor(x), ceil(x)</p>

![alt text](https://github.com/rdbo/ExpressionParser-GUI/blob/master/expr_parser_screenshot.PNG)
