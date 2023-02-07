const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
      <table style="width:100%">
        <tr>         
         <center><th><h1 style=color:blue;><center>CATS WATER TANK PURGE SYSTEM</center></h1></th></center>         
        </tr>
      </table>
       
      <table style="width:100%">
        <tr>
          <th><h8 style=color:blue;><span id="clock"></span> </h8></th>
          <th><h8 style=color:black;><span id="timeInfo"></span> </h8></th>
          <th><h8 style=color:red;>MAC:</h8><h8 style=color:black;><span id="macinfo"> </span> </h8></th>
          <th><h8 style=color:red;>IP:</h8><h8 style=color:black;><span id="ipinfo"> </span> </h8></th>
          <th><h8 style=color:red;>SSID:</h8><h8 style=color:black;><span id="ssidinfo"> </span> </h8></th>
          <th><h8 style=color:red;>RSSI:</h8><h8 style=color:black;><span id="rssiinfo"> </span> </h8></th>
          <th><h8 style=color:blue;><span id="wetv"></span> </h8></th>
          <th><h8 style=color:red;><span id="state"></span> </h8></th>
        </tr>
      </table>
    </head>
<body>
  <center>
    <table style="width:100%;text-align: center;">
      <tr>
        <th> </th>
        <th>  </th> 
        <th> </th> 
      </tr>
      <tr>
        <td>
          <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/on', id)"> PURGE </button>
          <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/nonStopPurge', id)"> CONTINUOUS </button>
          <button id="chargeon" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/off', id)"> STOP </button>
          <button id="refresh" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/refresh', id)"> REFRESH </button>
          <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button>
          <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
        </td>
      </tr>
  </table>
  <iframe width="800" height="300" src="http://192.168.10.227/term.html" title="Serial Terminal Output" style="border:1px solid black;">
      </iframe>
   
  </center>
</body>

<!-**************************************************************************************-->
<!-                              SCRIPTS                                         -->
<!-**************************************************************************************-->
<script>

 function MainFunc(name, id)
    {
      var xhr = new XMLHttpRequest();
      var url = name;
      xhr.open("GET", url, true);
      xhr.send();

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById(id).innerText = this.responseText;
          console.log(xhr.responseText);
        }   
      };    
    };
    document.addEventListener('DOMContentLoaded', MainFunc, false);

    function getTimeInfo(){
      var xhr = new XMLHttpRequest();
      var url = "/getTimeInfo"
      
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
      
            document.getElementById("timeInfo").innerText = this.responseText;  
            console.log(xhr.responseText);
         
        }
      };   
    };

    function getDebug(){
      var xhr = new XMLHttpRequest();
      var url = "/debug"
      
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) 
        {
          var textarea = document.getElementById("debug")
         textarea.innerHTML = this.responseText; 
         textarea.scrollTop = textarea.scrollHeight;
        }
      };
    };   
    
    function getState() 
    {
      var xhr = new XMLHttpRequest();
      var url = "/getstate";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("state").innerHTML = this.responseText;
          console.log(xhr.responseText);
        }
       };   
     };
    function getClockInfo(){
          var xhr = new XMLHttpRequest();
          var url = "/getClock"
          
          xhr.open("GET", url, true);
          xhr.send();  
    
          xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
          
                document.getElementById("clock").innerText = this.responseText;  
                //console.log(xhr.responseText);
             
            }
          };   
        };
     function getNetworkInfo(){
      var xhr = new XMLHttpRequest();
      var url = "/getMac"
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("macinfo").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      }; 
      var xhr = new XMLHttpRequest();
      var url = "/getIP"
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ipinfo").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };  
      var xhr = new XMLHttpRequest();
      var url = "/getSSID"
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ssidinfo").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };  
       var xhr = new XMLHttpRequest();
      var url = "/getRSSI"
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("rssiinfo").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };  
    };
    

    function getWetVal() 
    {
      var xhr = new XMLHttpRequest();
      var url = "/getwetval";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("wetv").innerHTML = this.responseText;
          console.log(xhr.responseText);
        }
       };   
     };

    getNetworkInfo();
    getDebug();
    getTimeInfo();
    getState();

    var interval1 = setInterval(function(){getWetVal();}, 1000);
    var interval2 = setInterval(function(){getClockInfo();}, 500);
    var interval3 = setInterval(function(){getTimeInfo();}, 10000);
    var interval4 = setInterval(function(){getState();}, 500);

    setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes

    function clearAll(){
      clearInterval(interval1);
      clearInterval(interval2);
      clearInterval(interval3);
      alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
    }

    
  </script>
</html>
)=====";
