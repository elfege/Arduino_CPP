/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */
const char MAIN_page[] PROGMEM = R"=====(
  <html>
    <head>      
      <table style="width:100%">
        <tr>         
         <center><th><h1 style=color:blue;><center>WINDOW JESSICA</center></h1></th></center>         
        </tr>
      </table>
       
      <table style="width:100%">
        <tr>
          <th><p><h4 style=color:red;><span id="clock"></span> </h4></th>
          <th><p> <h4 style=color:black;><span id="timeInfo"></span> </h4></th>
          <th><h8 style=color:red;>MAC:</h8><h8 style=color:black;><span id="macinfo"> </span> </h8></th>
          <th><h8 style=color:red;>IP:</h8><h8 style=color:black;><span id="ipinfo"> </span> </h8></th>
          <th><h8 style=color:red;>SSID:</h8><h8 style=color:black;><span id="ssidinfo"> </span> </h8></th>
          <th><h8 style=color:red;>RSSI:</h8><h8 style=color:black;><span id="rssiinfo"> </span> </h8></th>
          <th><h4 style=color:red;><span id="state"></span> </h4></th>
        </tr>
      </table>
    </head>

  <body>
    <center>
      <table style="width:100%">
        <tr>
               <th> <button id="OPEN" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/open', id)"> OPEN </button> </th> 
               <th> <button id="CLOSE" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/close', id)"> CLOSE </button> </th> 
               <th> <button id="STOP" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stop', id)"> STOP </button> </th> 
               <th> <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="REFRESH()"> REFRESH </button> </th> 
               <th> <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/resetRTCVars', id)"> RESET RTC </button> </th> 
               <th> <button id="DEBUG" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/logs', id)"> DEBUG </button></th> 
               <th> <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button></th> 
               <th> <button id="CALIBRATE" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/calibrate', id)"> CALIBRATE </button></th> 
               <th> <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button></th> 
               <th> <button id="HARDRESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/hardreset', id)"> HARD RESET </button></th> 
        </tr>
    </table>
    <table style="width:relative">
     <tr>      
      
      <th> <h8 style=color:red; >SPEED: <span id="speed"></span>  &nbsp  &nbsp </h8>
      <input type="range" min="0" max="100" value="0" class="slider" id="speedRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </th>
      <th> <button id="plusten" style=width:45px;height:40px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/plusten', id)"> +10 </button></th> 
      <th> <button id="minusten" style=width:45px;height:40px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/minusten', id)"> -10 </button></th>
     </tr>
   </table>

   <iframe width="800" height="500" src="http://192.168.10.207/term.html" title="Serial Terminal Output" style="border:1px solid black;">
      </iframe>
      <p> 
       <table style="width:100%">
        <tr>
          <th><iframe width="500" height="100" src="http://192.168.10.204/index2" title="Windows Living" style="border:1px solid black;">
          </iframe></th>
          <th><iframe width="500" height="100" src="http://192.168.10.206/index2" title="Windows Office South" style="border:1px solid black;">
          </iframe></th>
         </tr>
         <tr>
          <th><iframe width="500" height="100" src="http://192.168.10.203/index2" title="Windows Bedroom" style="border:1px solid black;">
          </iframe></th>
          <th><iframe width="500" height="100" src="http://192.168.10.205/index2" title="Windows Minerva" style="border:1px solid black;">
          </iframe></th>
        </tr>
       </table>
     </p> 
        
    </center>
  </body>

  <script>

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
    getNetworkInfo();

    function getTimeInfo(){
      var xhr = new XMLHttpRequest();
      var url = "/getTimeInfo"
      
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
      
            document.getElementById("timeInfo").innerText = this.responseText;  
            //console.log(xhr.responseText);
         
        }
      };   
    };

    var message = "";
    var previousMessage = "";  
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
  
    var slider = document.getElementById("speedRange");
    var output = document.getElementById("speed");
    output.value = slider.value; // update the slider position
   
    slider.oninput = function() {
      output.innerHTML = this.value;
      output.value = slider.value; // update the slider position
      //getSpeedValue(); // don't run it from here because it'll restore the old value before the server had time to set the new value. Let the interval scheduled take care of this
    }    

    function sendSlider(id, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + id + "=" + value;
      xhr.open("GET", url, true);
      xhr.send();  
      
      //getSpeedValue();
      
    };
    function getSpeedValue()
    {
      var xhr = new XMLHttpRequest();
      var url = "/speed";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("speed").innerHTML = this.responseText;
          document.getElementById("speedRange").innerHTML = this.responseText; // update the slider position
          //console.log("getSpeedValue returns: "+xhr.responseText);
        }
      };
      
    };


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
          //console.log(xhr.responseText);
          //document.getElementById("debug").innerHTML = xhr.responseText
        }
      };   
    };
    function REFRESH()
    {
      var xhr = new XMLHttpRequest();
      var url = "/refresh";
      xhr.open("GET", url, true);
      xhr.send();
            
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
         document.getElementById("state").innerText = this.responseText
          //console.log(xhr.responseText);
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
          //console.log(xhr.responseText);
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

      getSpeedValue();
//      getDebug();
      getTimeInfo();
      getState();

      var interval1 = setInterval(function(){getState();}, 500);
      var interval2 = setInterval(function(){getSpeedValue();}, 3000);
      var interval3 = setInterval(function(){getNetworkInfo();}, 30000); //RSSI needs to be updated 
      var interval4 = setInterval(function(){getTimeInfo();}, 10000);
      var interval5 = setInterval(function(){getClockInfo();}, 500);
      //setInterval(function(){getMac();}, 240000);

      setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes
      
      function clearAll(){
        clearInterval(interval1);
        clearInterval(interval2);
        clearInterval(interval3);
        clearInterval(interval4);
        //clearInterval(interval5); // clock stays on
        alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
      }
   
  </script>
 </html>
  )=====";

//<p> <textarea id="debug"    rows="10" cols="100" wrap="soft"> </textarea> </p>
