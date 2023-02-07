const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>      
  <table style="width:100%">
    <tr>         
      <center><th><h1 style=color:blue;><center>AC BEDROOM MINERVA</center></h1></th></center>         
    </tr>
  </table>

  <table style="width:100%">
    <tr>
      <th><p><h4 style=color:red;><span id="clock"></span> </h4></th>
      <th><p><h4 style=color:black;><span id="timeInfo"></span> </h4></th>
      <th><p> <h4 style=color:black;><span id="macinfo"> </span> </h4></th>
      <th><h4 style=color:red;><span id="state"></span> </h4></th>
    </tr>
  </table>
</head>
      <body onload="REFRESH()">
        <center>
          <table style="width:100%">
            <tr>
              <th><button id="COOL" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/cool', id)"> COOL </button></th> 
              <th><button id="HEAT" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/heat', id)"> HEAT </button></th> 
              <th><button id="FAN" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="MainFunc('/fan', id)"> FAN </button></th>
              <th><button id="AUTO" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="MainFunc('/auto', id)"> AUTO </button></th> 
              <th><button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="REFRESH()"> REFRESH </button></th> 
              <th><button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="MainFunc('/reset', id)"> RESET </button></th> 
              <th><button id="DEBUG" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/logs', id)"> DEBUG </button></th> 
              <th><button id="POWER" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/power', id)"> POWER </button></th> 
              <th><button id="STOP ACTIVITY" style=width:200px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stopAll', id)"> STOP ACTIVITY </button></th> 
              <th><button id="MODE" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/modeswitch', id)"> MODE </button></th> 
            </tr>
          </table>
          
          <iframe width="800" height="500" src="http://192.168.10.213/term.html" title="Serial Terminal Output" style="border:1px solid black;">
          </iframe>

        </center>
      </body>

      <script>

      function MainFunc(name, id)
      {
        var xhr = new XMLHttpRequest();
        var url = name;
        xhr.open("GET", url, true);
        xhr.send();
        //document.getElementById("debug").innerHTML = url
      
      
        xhr.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("state").innerText = this.responseText;
            console.log(xhr.responseText);
            //document.getElementById("debug").innerHTML = xhr.responseText
          }
        };    
      };
    

       
        </script>
</html>
)=====";
