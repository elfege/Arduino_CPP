const char MAIN_page[] PROGMEM = R"=====(
  <html>
    <head>
      <center>
        <h1 style=color:blue;><center>WINDOW LIVING (FIX)</center></h1>
        <p><h4 style=color:red;><span id="state"></span> </h4></p>
      </center>
    </head>

  <body>
    <center>
    <table style="width:100%">
        <tr>
               <th> <button id="OPEN" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/open', id)"> OPEN </button> </th> 
               <th> <button id="CLOSE" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/close', id)"> CLOSE </button> </th> 
               <th> <button id="STOP" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stop', id)"> STOP </button> </th> 
               <th> <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="REFRESH()"> REFRESH </button> </th> 
               <th> <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button> </th> 
               <th> <button id="DEBUG" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/logs', id)"> DEBUG </button></th> 
               <th> <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button></th> 
               <th> <button id="FORCECLOSE" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/forceclose', id)"> FORCE CLOSE </button></th> 
               <th> <button id="FORCEOPEN" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/forceopen', id)"> FORCE OPEN</button></th> 
               <th> <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button></th> 
               <th> <button id="HARDRESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/hardreset', id)"> HARD RESET </button></th> 
        </tr>
    </table>
    <div class="slidecontainer">
      <p></p>
      <h8 style=color:red; >SPEED: <span id="speed"></span>  &nbsp  &nbsp </h8>
      <div> <input type="range" min="1" max="100" value="0" class="slider" id="speedRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </div>
    </div>
    
     <p> <textarea id="timeInfo" rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p> <textarea id="macinfo"  rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p> <textarea id="debug"    rows="10" cols="100" wrap="soft"> </textarea> </p>
   
    </center>
  </body>

  <script>

    function getMac(){
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
    };

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
    output.innerHTML = slider.value;
   
    slider.oninput = function() {
      output.innerHTML = this.value;
      displaySpeedValue();
    }    

    function sendSlider(id, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + id + "=" + value;
      xhr.open("GET", url, true);
      xhr.send();  
      
      displaySpeedValue();
      
    };
    function displaySpeedValue()
    {
      var xhr = new XMLHttpRequest();
      var url = "/speed";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("speed").innerHTML = this.responseText;
          console.log(xhr.responseText);
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
          console.log(xhr.responseText);
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
          console.log(xhr.responseText);
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

      getMac();
      displaySpeedValue();
      getDebug();
      getTimeInfo();
      getState();

      setInterval(function(){getState();}, 10000);
      setInterval(function(){displaySpeedValue();}, 30000);
      setInterval(function(){getDebug();}, 1000);
      setInterval(function(){getTimeInfo();}, 10000);
      //setInterval(function(){getMac();}, 240000);
    
   
  </script>


  </html>
  )=====";
