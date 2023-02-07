const char MAIN_page[] PROGMEM = R"=====(

<html> 
  <head>
  <style>
  table{
    width:100%;
    text-align: center;
}
th, td {
  border: 1px solid blue;
}
p{
  border: 0px ;
  padding: 0px;
  margin: 0px;
}
button {
  width:100px;
  height:35px;
  background-color: grey;
  border: none;
  color: white;
  padding: 20px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 12px;
}
.arrow {
  border: solid black;
  border-width: 0 3px 3px 0;
  display: inline-block;
  padding: 3px;
}
.right {
  transform: rotate(-45deg);
  -webkit-transform: rotate(-45deg);
}

.left {
  transform: rotate(135deg);
  -webkit-transform: rotate(135deg);
}

.up {
  transform: rotate(-135deg);
  -webkit-transform: rotate(-135deg);
}

.down {
  transform: rotate(45deg);
  -webkit-transform: rotate(45deg);
}

</style>
  <head>
    <center>
      <h1 style="color:blue;">MEBO CONTROL INTERFACE</h1>
    </center>    
  </head>
<body onload="UpdateValues()">

  <table style="width:100%;text-align: center;">

    <tr>
      <td>
        <table style=width:100%;>
         <th style=border:0px>
          <td style=border:0px>
           <button id"stopall" style=background-color:red;font-size:20px;height:100px;width:100px;text-align:center onclick="MainFunc('/stopall', id)">STOP ALL!</button>
           </td>
         </th>
         <th style=border:0px>
          <td style=border:0px>
           <button id="logs"   onclick="MainFunc('/logs')"> LOGS </button>
           <button id="dock"   onclick="MainFunc('/dock')"> DOCK </button>
          </td>
         </th>
       </table> 
      </td> 
      <td>
        <h4 style=margin:0px>Serial Output</h4>       
        <iframe width="500" height="200" src="http://192.168.10.222/term.html" style="border:1px solid black;">
        </iframe>
      </td>
      <td>
      <h1><button id="volts" style=width:100px;height:100px;font-size:16px;color:white;background-color:red> BATTERY LEVEL</button> VOLTS</h1>
          <button id"reset"               onclick="MainFunc('/reset', id)"> RESET </button>
          <button id="buttonPowerOFF"     onclick="buttonPowerOFF_f()"> OFF </button> 
          <button id="buttonPowerON"      onclick="buttonPowerON_f()"> START </button>    
          <button id="buttonCharger"      onclick="buttonChargerF('/charge')"> CHARGER </button> 
      </td> 
    </tr> 
    <tr>
      <th>MOVMENT</th>
      <th>VIDEO</th> 
      <th>ARM CONTROL</th>
    </tr>  
    <tr>
      <td>
        <p>
         <button onclick="MainFunc('/fwd', 'MVT-state')"> <i class="arrow up"></i> </button></p>
         <button onclick="MainFunc('/left', 'MVT-state')"> <i class="arrow left"></i> </button>
         <button onclick="MainFunc('/right', 'MVT-state')"> <i class="arrow right"></i> </button>
         <p><button onclick="MainFunc('/rvs', 'MVT-state')"> <i class="arrow down"></i> </button>
        </p>
        <p><button onclick="MainFunc('/stop', 'MVT-state')"> STOP </button> </p>
        <p><span id="MVT-state">current state</span> </p><p> </p>
        <div>
          <p>
            <input type="text" id="textBox1" size="40">
          </p>
        </div>
        <h4>PROGRAMMED RIDE</h4>


        <div class="slidecontainer">
          <p></p>
          <h8 style=color:red; >SPEED: <span id="speed"></span>  &nbsp  &nbsp </h8>
          <div> <input type="range" min="140" max="200" value="170" class="slider" id="speedRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </div>
        </div>
        <div class="slidecontainer">
          <div> <h8 style=color:red; >DISTANCE: <span id="distance"></span>  &nbsp </h8> </div>
          <input type="range" min="1" max="200" value="15" class="slider" id="distanceRange" onmouseup="sendSlider(id, value)">
        </div>
        <div>
         <span>Enter a distance to run:</span>
         <input type="text" id="distRange" value=""><button onclick="textDistanceRange()">ENTER</button>
         <p id="exactDist"></p>
        </div>
        <p>
        </p>        
        <p> 
        <table style=border:none>
          <td  style=border:none>
            <p>
              <button style=height:60 onclick="executeFWD()"> Execute FWD </button>          
            </p>
              <button style=height:60 onclick="executeRVS()"> Execute RVS </button>
          
          </td>
          <td  style=border:none>
            <p>
              <button style=height:60 onclick="MainFunc('/followlinervs', 'MVT-state')"> DOCK RVS </button>
            </p>
              <button style=height:60 onclick="MainFunc('/followlinefwd', 'MVT-state')"> UNDOCK </button>
          </td>
          </table>
        </p>

      </td>
      <td>
        <center> 
          <p> </p>
            <iframe width="422" height="250" src="http://192.168.10.15:81/mjpg/MEBO/video.mjpg"></iframe>
       </center>
       <p> </p>
       <h4>PTZ</h4>
        <p> <button onclick="ptzup()"> ^ </button></p>
        <p> 
        <button onclick="ptzleft()"> < </button>
        <button onclick="ptzright()"> > </button>
        </p>
        <p> 
        <button onclick="ptzdown()"> v </button>
        </p>
        <p> 
        <button style=height:60 onclick="MainFunc('/cancelstopall', id)"> STOP ALL </button>
        <button style=height:60 onclick="MainFunc('/ignoreobs', id)"> Ignore Obst </button>       
        <button style=height:60 onclick="MainFunc('/selfdrive', id)"> Roam </button>
        </p>           
      </td> 
      <td> 
        <div>
          <p>
          <button onclick="MainFunc('/stoparm', id)"> STOP </button>
          </p>
          <p>
          <button onclick="MainFunc('/armcenter', id)"> CENTER </button>
          </p>
          <p>---------------</p>
        </div>  
        <div>    
          <button onclick="MainFunc('/ArmUp', id)"> <i class="arrow up"></i></button> 
          <button onclick="MainFunc('/armupstep', id)"> step <i class="arrow up"></i> </button>
        </div>

        <div> 
          <button onclick="MainFunc('/ArmDown', id)"> <i class="arrow down"></i> </button>
          <button onclick="MainFunc('/armdownstep', id)"> step <i class="arrow down"></i> </button>
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >FOREARM</h8>
          <p></p>
          <button onclick="MainFunc('/ForeArmUp', id)"> <i class="arrow up"></i>  </button>  
          </button> <button onclick="MainFunc('/forearmupstep', id)"> step <i class="arrow up"></i> </button>
        </div>
        <div>
          <button onclick="MainFunc('/ForeArmDown', id)"> <i class="arrow down"></i> </button>
          <button onclick="MainFunc('/forearmdowntep', id)"> step <i class="arrow down"></i> </button> 
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >CLAMP</h8>
          <p></p>
          <button onclick="MainFunc('/CloseClamp', id)"> CLOSE  </button>
          <button onclick="MainFunc('/OpenClamp', id)"> OPEN </button>
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >WRIST</h8>
          <p></p>
          <table>
            <tr>
              <button onclick="MainFunc('/wristleft', id)"> <i class="arrow left"></i>  </button>
              <button onclick="MainFunc('/wristright', id)"> <i class="arrow right"></i> </button>
            </tr>
            <tr>
              <th style=border:none>
                <button style=width:25;background-color:yellow;  onclick="MainFunc('/wristleftstep', id)"> <i class="arrow left"></i> </button>
                <button style=width:25;background-color:yellow;  onclick="MainFunc('/wristrightstep', id)"> <i class="arrow right"></i> </button>
              </th>
            </tr>
            <tr>
            <th style=border:none>
                <button style=height:60;width:100;text-align:center onclick="MainFunc('/wristmiddle', id)"> GRAB POSITION </button>
              </tr>
            </th>
            </table>
         </div>
            </td>
          </tr>
        <tr>
          <td>
          </td>
          <td> 
            --
          </td> 
          <td>
             </td>
        </tr>
  </table>

    
    
    </body>
  <script>

    var slider = document.getElementById("speedRange");
    var output = document.getElementById("speed");
    output.innerHTML = slider.value;
    var slider2 = document.getElementById("distanceRange");
    var output2 = document.getElementById("distance");
    output2.innerHTML = slider2.value;

    slider.oninput = function() {
      output.innerHTML = this.value; 
    }
    slider2.oninput = function() {
      output2.innerHTML = this.value; 
    }

     function textDistanceRange()
    {
      var x = document.getElementById("distRange").value;
      document.getElementById("exactDist").innerHTML = x; 
      sendSlider("distanceRange", x)
    }

    function sendSlider(id, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + id + "=" + value;
      xhr.open("GET", url, true);
      xhr.send();  
    }


    textBox1.addEventListener('keyup', (e) => 
                              {     
      MainFunc('/stop', 'MVT-state');
      console.log(`keyup event`);
    });

   
    
    function buttonChargerF(name){
      console.log("button was clicked!");
      var xhr = new XMLHttpRequest();
      var url = name;

      xhr.open("GET", url, true);
      xhr.send();

      var buttonChargerStatus = document.getElementById("buttonCharger").innerText
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("buttonCharger").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
      getvolts();
    };
    //document.addEventListener('DOMContentLoaded', buttonChargerF, false);

    function getvolts(){
      console.log("button was clicked!");
      var xhr = new XMLHttpRequest();
      var url = "/getvolts";

      xhr.open("GET", url, true);
      xhr.send();
      
      var buttonChargerStatus = document.getElementById("volts").innerText
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("volts").innerText = this.responseText + "V";
          console.log(xhr.responseText);
        }
      };
    };


    function buttonPowerOFF_f()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.105/L'); // so it can send it even when ESP is no longer responding
      xhr.send();
      xhr = new XMLHttpRequest();
      var url = "/OFF";
      xhr.open("GET", url, true);
      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED OFF!");

    };
    //document.addEventListener('DOMContentLoaded', buttonPowerOFF, false);

    function buttonPowerON_f()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.105/H'); // so it can send it even when ESP is no longer responding or off
      xhr.send();
      xhr = new XMLHttpRequest();
      var url = "/ON";
      xhr.open("GET", url, true);
      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");
    };

    
    function ptzup()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.15:81/cam/MEBO/pos=2&user=admin&pw=!ROtrashriska$1977');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };   
    function ptzdown()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.15:81/cam/MEBO/pos=3&user=admin&pw=!ROtrashriska$1977');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };
    function ptzleft()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.15:81/cam/MEBO/pos=0&user=admin&pw=!ROtrashriska$1977');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };
    function ptzright()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.15:81/cam/MEBO/pos=1&user=admin&pw=!ROtrashriska$1977');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };


    function MainFunc(name, id)
    {
      var xhr = new XMLHttpRequest();
      var url = name;
      xhr.open("GET", url, true);
      xhr.send();

        xhr.onreadystatechange = function() {
          if(id === 'MVT-state' || id === 'stopall' || id === 'stopallstate' || id === 'ignoreobsstate' || id === 'roamingstate'){
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById(id).innerText = this.responseText;
              console.log(xhr.responseText);
            }
          } 
      else {
        console.log("not MVT-state");
      }
     };    
    };
    //document.addEventListener('DOMContentLoaded', MainFunc, false);

    function executeFWD(){
      console.log("EXEC FWD");
      var xhr = new XMLHttpRequest();
      var url = "/executefwd";
      xhr.open("GET", url, true);
      xhr.send();
    };
    //document.addEventListener('DOMContentLoaded', executeFWD, false);

    function executeRVS(){
      console.log("EXEC RVS");
      var xhr = new XMLHttpRequest();
      var url = "/executervs";
      xhr.open("GET", url, true);
      xhr.send();
    };
    //document.addEventListener('DOMContentLoaded', executeRVS, false);

    function UpdateValues(){      
      updateChargerStatus();
      updateVoltsStatus();
    };

    window.setInterval(updateVoltsStatus, 60000);
     
    function updateChargerStatus(){
      var xhr = new XMLHttpRequest();
      var url = "/updatecharge";

      xhr.open("GET", url, true);
      xhr.send();

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("buttonCharger").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
    };
    //document.addEventListener('DOMContentLoaded', updateChargerStatus, false);// can use this instead of "load". TO DO... 
    
    
    function updateVoltsStatus(){
      var xhr = new XMLHttpRequest();
      var url = "/getvolts";
      xhr.open("GET", url, true);
      xhr.send();

        xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("volts").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
    };
    //document.addEventListener('DOMContentLoaded', updateVoltsStatus, false);

  </script>
  <script>
  var pressed = false;
      textBox1.addEventListener('keydown', (e) => {
        if (!e.repeat){
          console.log(`SENDING DATA. key property value is "${e.key}"`);
  
          var thecode = e.keyCode;
  
          if (thecode === 38){
            url = "/fwd";
          }
          else if (thecode === 39){
            url = "/right";
          }
          else if (thecode === 37){
            url = "/left";
          }
          else if (thecode === 40){
            var url = "/rvs";
          }
          else if(thecode === 16 || thecode === 32 || thecode === 17 || thecode === 35 || thecode === 46){     
            var url = "/stop"; 
          }
          
          var xhr = new XMLHttpRequest();
          xhr.open("GET", url, true);
          xhr.send();
  
            xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("MVT-state").innerText = this.responseText;
              console.log(xhr.responseText);
            }
          };
        }
        else {
          console.log(`keydown event repeats. key property value is "${e.key}"`);
        }
      });
  </script>
    
  
  </html>
)=====";
