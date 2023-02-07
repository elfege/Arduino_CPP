const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>      
  <table style="width:100%">
    <tr>         
      <center><th><h1 style=color:blue;><center>AC LIVING-ROOM</center></h1></th></center>         
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
              <th><button id="OFF" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/off', id)"> OFF </button></th> 
              <th><button id="STOP ACTIVITY" style=width:200px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stopAll', id)"> STOP ACTIVITY </button></th> 
              <th><button id="TURBO" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/turbo', id)"> TURBO </button></th> 
              <th><button id="LED" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/ledtoggle', id)"> LED </button></th> 
              <th><button id="LEDBYPASS" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/ledtogglebypass', id)"> LED (byp)</button></th> 
            </tr>
          </table>
          <div class="slidecontainer">
            <p></p>
            <h8 style=color:red; >Set Point: <span id="temp"></span>  &nbsp  &nbsp </h8>
            <div> <input type="range" min="60" max="90" value="0" class="slider" id="tempRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </div>
          </div>
          <iframe width="800" height="500" src="http://192.168.10.212/term.html" title="Serial Terminal Output" style="border:1px solid black;">
          </iframe>

        </center>
        <script src="https://code.jquery.com/jquery-3.6.0.js"></script>
        <script src="https://code.jquery.com/ui/1.13.2/jquery-ui.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
        <script src="https://unpkg.com/jquery"></script>
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
        
        function MainFunc(name, id)
        {
          var xhr = new XMLHttpRequest();
          var url = name;
          xhr.open("GET", url, true);
          xhr.send();
          xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("state").innerText = this.responseText;              
              console.log(xhr.responseText);
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

        var slider = document.getElementById("tempRange");
        var output = document.getElementById("temp");
        output.innerHTML = slider.value; // update the displayed value
        
        slider.oninput = function() {
          output.innerHTML = this.value;
          output.value = slider.value; // update the slider position
          //getTempVal();
        }
        function getTempVal()
        {
          var xhr = new XMLHttpRequest();
          var url = "/temp";
          xhr.open("GET", url, true);
          xhr.send();
          xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("temp").innerHTML = this.responseText;
              slider.value = this.responseText;// update the slider position
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

              const data = this.responseText;
              console.log("checkStates data:" + data);
              
              const arr = data.split(",");
              const newArr = []
              arr.forEach((val) => {
                  newArr.push(val.replace(" ", ":").split(":"))
              })
              const Obj = Object.fromEntries(newArr)
              console.log("entries:" + Obj)
              for (let a of Object.keys(Obj)) {
                  console.log(a + ":" + Obj[a])
                  if (a == "tempRange") {
                    console.log("updating slider value to ", Obj[a]);
                      $(`#${a}`).val(Obj[a]);
                  }
                  else {
                      $(`#${a}`).text(Obj[a]);
                  }
              }              
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
                console.log(xhr.responseText);
             
            }
          };   
        };

        function sendSlider(id, value){
          var xhr = new XMLHttpRequest();
          var url = "/" + id + "=" + value;
          xhr.open("GET", url, true);
          xhr.send();
        };

        getMac();
        getTimeInfo();
        getTempVal();

        var interval1 = setInterval(function(){getState();}, 500);
        var interval3 = setInterval(function(){getTimeInfo();}, 10000);
        var interval5 = setInterval(function(){getClockInfo();}, 500);

        function clearAll(){
          clearInterval(interval1);
          clearInterval(interval2);
          clearInterval(interval3);
          alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
        };
        setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes
        </script>
</html>
)=====";
