const char simplifiedPage[] PROGMEM = R"=====(
  <html>
    <head>      
      <table style="width:100%">
        <tr>         
         <th><h4 style=color:blue;><center>Window Office N</center></h4></th>
         <th><h8 style=color:red;>speed: <span id="speed"></span></h8></th>
         <th><input type="range" min="0" max="100" value="0" class="slider" width="400" id="speedRange" onmouseup="sendSlider(id, value)"></th>
         <th><h8 style=color:red;><span id="state"></span> </h8></th>      
        </tr>
      </table>
    </head>
  <body>
    <center>
      <table style="width:100%">
          <tr>
           <th> <button id="OPEN" style=width:50px;height:20px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/open', id)"> open </button> </th> 
           <th> <button id="CLOSE" style=width:50px;height:20px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/close', id)"> close </button> </th> 
           <th> <button id="STOP" style=width:50px;height:20px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stop', id)"> stop </button> </th> 
           <th> <button id="ACCESS" style=width:50px;height:20px;margin-left:0;color:white;background-color:red onclick="windowsOpen()"> adv </button> </th> 
          </tr>
      </table>     
    </center>
  </body>

  <script>
    var message = "";
    var previousMessage = "";        
    var slider = document.getElementById("speedRange");
    var output = document.getElementById("speed");
    output.value = slider.value; // update the slider position
   
    slider.oninput = function() {
      output.innerHTML = this.value;
      output.value = slider.value; // update the slider position
      getSpeedValue();
    }  

    function windowsOpen() {
      window.open("http://192.168.10.206");
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
          //console.log(xhr.responseText);
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
     
      getSpeedValue();
      getState();

      var interval1 = setInterval(function(){getState();}, 1000);
      var interval2 = setInterval(function(){getSpeedValue();}, 2000);

      setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes
      
      function clearAll(){
        clearInterval(interval1);
        clearInterval(interval2);
      }
   
   
  </script>


  </html>
  )=====";
