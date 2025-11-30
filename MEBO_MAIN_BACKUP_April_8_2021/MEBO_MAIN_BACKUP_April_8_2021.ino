const char MAIN_page[] PROGMEM = R"=====(

<html> 
  <head>
  <style>
  table{
    width:100%
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
  width:100px;height:55px
}

</style>
  <title>MEBO</title>
    </center>
      <table>
      <tr>
      <th>
        <button id="stopall" onclick="MainFunc('/stopall', id)"> STOP ALL! </button>
        <button id="logs"  onclick="MainFunc('/logs')"> LOGS </button>
        <button id="dock" onclick="MainFunc('/dock')"> DOCK </button>
      </th>     
      <th> 
        <button id"reset"               onclick="MainFunc('/reset', id)"> RESET </button>
        <button id="buttonPowerOFF"     onclick="buttonPowerOFF_f()"> OFF </button>      
        <button id="buttonCharger"      onclick="buttonChargerF('/charge')"> CHARGER </button>
        <button id="buttonPowerOFF_AUX" onclick="buttonPowerOFF_f_AUX()"> SHUT DOWN </button>
        <button id="buttonPowerON_AUX"  onclick="buttonPowerON_f_AUX()"> START </button>
      </th>
      <th>        
        <iframe width="500" height="200" src="http://192.168.10.222/term.html" title="Serial Terminal Output" style="border:1px solid black;">
        </iframe>
      </th> 
      <th>     
        <h1><button id="volts" style=width:100px;height:45px;margin-left:0;color:white;background-color:red> BATT. LEVEL</button> VOLTS</h1>
 
      </th>
    </center>    
  </head>
<body onload="UpdateValues()">

  <table style="width:100%;text-align: center;">
    <tr>
      <th>MOVMENT</th>
      <th>VIDEO</th> 
      <th>ARM CONTROL</th>
    </tr>    
    <tr>
      <td>
        <div><p><button onclick="MainFunc('/fwd', 'MVT-state')"> ^ </button>
          <div>
            <p>
              <button onclick="MainFunc('/left', 'MVT-state')"> < </button>
              <button onclick="MainFunc('/right', 'MVT-state')"> > </button>
               </p>
              <button onclick="MainFunc('/rvs', 'MVT-state')"> V </button>
            </p>
             <p><button onclick="MainFunc('/stop', 'MVT-state')"> STOP </button> </p>
          </div>
          <hr></hr>
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
         Exact Distance: <input type="text" id="distRange" value=""><button onclick="textDistanceRange()">ENTER</button>
         <p id="exactDist"></p>
        </div>

        <p> 
        <div>
        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:navy blue onclick="executeFWD()"> Execute FWD </button>
        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:magenta onclick="executeRVS()"> Execute RVS </button>
        </div>
        </p>
        <p> 
        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:navy blue onclick="MainFunc('/followlinefwd', 'MVT-state')"> UNDOCK </button>
        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:magenta onclick="MainFunc('/followlinervs', 'MVT-state')"> DOCK with line RVS </button>
        </p>

      </td>
      <td>
        <center> 
          <p> </p>
            <iframe width="422" height="250" src="http://192.168.10.15:81/mjpg/MEBO/video.mjpg"></iframe>
       </center>
       <p> </p>
       <h4>PTZ</h4>
        <p> <button style=width:50px;height:20px;margin-left:0;color:white;background-color:blue;font-size:160% id="ptzup" onclick="ptzup()"> ^ </button></p>
        <p> 
        <button style=width:50px;height:20px;margin-left:0;color:white;background-color:blue id="ptzleft" onclick="ptzleft()"> < </button>
        <button style=width:50px;height:20px;margin-left:0;color:white;background-color:blue id="ptzright" onclick="ptzright()"> > </button>
        </p>
        <p> 
        <button style=width:50px;height:20px;margin-left:0;color:white;background-color:blue id="ptzdown" onclick="ptzdown()"> v </button>
        </p>
        <p> 
        <button style=width:120px;height:55px;margin-left:0;color:white;background-color:red  id="stopallstate" onclick="MainFunc('/cancelstopall', id)"> STOP ALL </button>
        <button style=width:120px;height:55px;margin-left:0;color:white;background-color:blue id="ignoreobsstate" onclick="MainFunc('/ignoreobs', id)"> Ignore Obst </button>       
        <button style=width:120px;height:55px;margin-left:0;color:white;background-color:green id="roamingstate" onclick="MainFunc('/selfdrive', id)"> Roam </button>
        </p>           
      </td> 
      <td> 
        <div>
          <button style=width:120px;height:45px;margin-left:0;color:white;background-color:red id="stoparm" onclick="MainFunc('/stoparm', id)"> STOP </button>
          <p></p>
          <button style=width:120px;height:45px;margin-left:0;color:white;background-color:blue id="armcenter" onclick="MainFunc('/armcenter', id)"> CENTER </button>
        </div>  
        <div>    
          <button style=width:100px;height:45px;margin-left:0;font-size:160%; id="armup" onclick="MainFunc('/ArmUp', id)"> ^ 
          </button> <button style=width:30px;height:45px;margin-left:0;font-size:160%; id="armupstep" onclick="MainFunc('/armupstep', id)"> ^ </button>
        </div>

        <div> 
          <button style=width:100px;height:45px;margin-left:0 id="armdown" onclick="MainFunc('/ArmDown', id)"> V </button>
          <button style=width:30px;height:45px;margin-left:0 id"armdownstep" onclick="MainFunc('/armdownstep', id)"> V </button>
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >FOREARM</h8>
          <p></p>
          <button style=width:100px;height:45px;margin-left:0;font-size:160%; id="forearmup" onclick="MainFunc('/ForeArmUp', id)"> ^  </button>  
          </button> <button style=width:30px;height:45px;margin-left:0;font-size:160%; id="forearmupstep" onclick="MainFunc('/forearmupstep', id)"> ^ </button>
        </div>
        <div>
          <button style=width:100px;height:45px;margin-left:0 id"forearmdown" onclick="MainFunc('/ForeArmDown', id)"> V </button>
          <button style=width:30px;height:45px;margin-left:0 id"forearmdowntep" onclick="MainFunc('/forearmdowntep', id)"> V </button> 
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >CLAMP</h8>
          <p></p>
          <button style=width:100px;height:45px;margin-left:0 id="closeclamp" onclick="MainFunc('/CloseClamp', id)"> CLOSE  </button>
          <button style=width:100px;height:45px;margin-left:0 id="openclamp" onclick="MainFunc('/OpenClamp', id)"> OPEN </button>
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >WRIST</h8>
          <p></p>
          <button style=width:100px;height:45px;margin-left:0 id="wristleft" onclick="MainFunc('/wristleft', id)"> <  </button>
          <button style=width:10px;height:45px;margin-left:0 id="wristleftstep" onclick="MainFunc('/wristleftstep', id)"> < </button>
          <button style=width:10px;height:45px;margin-left:0 id="wristrightstep" onclick="MainFunc('/wristrightstep', id)"> > </button>
          <button style=width:100px;height:45px;margin-left:0 id="wristright" onclick="MainFunc('/wristright', id)"> > </button>
              <p>
                <button style=width:100px;height:45px;margin-left:0 id"wristmiddle" onclick="MainFunc('/wristmiddle', id)"> GRAB POSITION </button>
              </p>
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
//    Http.open('GET', 'http://192.168.10.105/L');
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
//      xhr.open('GET', 'http://192.168.10.105/H');

     var url = "/ON";

      xhr.open("GET", url, true);
      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };

     function buttonPowerOFF_f_AUX()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.105/L');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED OFF!");

    };
    //document.addEventListener('DOMContentLoaded', buttonPowerOFF, false);

    function buttonPowerON_f_AUX()
    {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'http://192.168.10.105/H');

      xhr.send();
      console.log(xhr.responseText)
      //alert("ROBOT SUCCESSFULLY TURNED ON!");

    };    
    //document.addEventListener('DOMContentLoaded', buttonPowerON, false);

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
