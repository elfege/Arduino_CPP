const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
   <script type="text/javascript" src="http://www.ispyconnect.com/external_viewer2.aspx?id=ANk9M1sYEUKt%2fLk7JWqG0AegfmPwgD6JLwV5jKli2r4%2bLReVgjJsheE29YBDthawUzmLHokF8LrJUXZx8cffsdYxOoG5jKukQZMHuqYsmGXvRXGIN9Jdw40Xu%2b%2fBqvh0mBYftdJT66%2fRB5CHhxeZFHODaW35ofu5Vej3k4VHK5k%3d"></script>
    </center>
      <button id"stopall" style=width:100px;height:45px;margin-left:0;color:white;background-color:orange onclick="MainFunc('/stopall', id)"> STOP ALL! </button>
      <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
      <button id="buttonPowerOFF" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="buttonPowerOFF_f()"> OFF </button>
     <!-- <button id="buttonPowerON" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="buttonPowerON_f()"> ON </button> -->
      
      <button id="buttonCharger" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="buttonChargerF('/charge')"> CHARGER </button>
      <button id="logs" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/logs')"> LOGS </button>
      <button id="dock" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/dock')"> DOCK </button>
     <button id="buttonPowerOFF_AUX" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="buttonPowerOFF_f_AUX()"> SHUT DOWN </button>
      <button id="buttonPowerON_AUX" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="buttonPowerON_f_AUX()"> START </button>
     
      <p><span id="volts">DC current</span>Volts </p><p> </p>
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
        <div><p><button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/fwd', 'MVT-state')"> ^ </button>
          <div>
            <p>
              <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/left', 'MVT-state')"> < </button>
              <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/right', 'MVT-state')"> > </button>
               </p>
              <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/rvs', 'MVT-state')"> V </button>
            </p>
             <p><button style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stop', 'MVT-state')"> STOP </button> </p>
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
        <p> <button tyle=width:100px;height:45px;margin-left:0;color:white;background-color:blue id="buttonCam" onclick="buttonCamF('/camstate', 'buttonCam')" >CAMERA</button> </p>
        <p><script>iSpyFeed();</script></p>
        <p> <script>iSpyPTZ();</script></p>
        
        <button style=width:120px;height:45px;margin-left:0;color:white;background-color:blue id="stopallstate" onclick="MainFunc('/cancelstopall', id)"> Reset Drive </button></p>
        <button style=width:120px;height:55px;margin-left:0;color:white;background-color:blue id="ignoreobsstate" onclick="MainFunc('/ignoreobs', id)"> Ignore Obsctacles </button></p>
        
      </td> 
      <td> 

        <div> 
          <p></p>
          <h8 style=color:red; >ARM</h8> 
          <p></p>
        </div>  
        <div>
          <button style=width:120px;height:45px;margin-left:0;color:white;background-color:red id="stoparm" onclick="MainFunc('/stoparm', id)"> STOP </button>
          <p></p>
          <button style=width:120px;height:45px;margin-left:0;color:white;background-color:blue id="armcenter" onclick="MainFunc('/armcenter', id)"> CENTER </button>
        </div>  
        <div>    
          <button style=width:100px;height:45px;margin-left:0;font-size:160%; id="armup" onclick="MainFunc('/ArmUp', id)"> ^ 
          </button> <button style=width:20px;height:45px;margin-left:0;font-size:160%; id="armupstep" onclick="MainFunc('/armupstep', id)"> ^ </button>
        </div>

        <div> 
          <button style=width:100px;height:45px;margin-left:0 id="armdown" onclick="MainFunc('/ArmDown', id)"> V </button>
          <button style=width:20px;height:45px;margin-left:0 id"armdownstep" onclick="MainFunc('/armdownstep', id)"> V </button>
        </div>
        <div>
          <p></p>
          <h8 style=color:red; >FOREARM</h8>
          <p></p>
          <button style=width:100px;height:45px;margin-left:0;font-size:160%; id="forearmup" onclick="MainFunc('/ForeArmUp', id)"> ^  </button>   
        </div>
        <div>
          <button style=width:100px;height:45px;margin-left:0 id"forearmdown" onclick="MainFunc('/ForeArmDown', id)"> V </button>
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
          <td></td> 
          <td>AUTOMATIONS MANAGEMENT</td>
        </tr>
        <tr>
          <td>

          </td>
          <td> 

          </td> 
          <td>
            <p><button style=width:100px;height:45px;margin-left:0 id="roamingstate" onclick="MainFunc('/selfdrive', id)"> Roam </button></p>
          </td>
        </tr>
  </table>
<p>
<iframe src="http://www.ispyconnect.com/external_viewer3.aspx?id=ANk9M1sYEUKt%2fLk7JWqG0AegfmPwgD6JLwV5jKli2r4%2bLReVgjJsheE29YBDthaw7FFaytzJWwlXmmjetJ3OTWwGsDz7wG4o51WEBwjrKzZmbLIZbHp3wYzodwQYZjU74J%2f%2fJiDc%2bLTQ0MQAwroTiirQD7qDxZ01p2p0LA1pTuVVrVH4T03JASkhBYnG8SA7" width="100%" height="600px" frameborder="0"></iframe>

 <!--  /*SMALLER FLASH ISPY VERSION Gets google chrome error */
 <iframe src="http://www.ispyconnect.com/external_viewer3.aspx?id=ANk9M1sYEUKt%2fLk7JWqG0AegfmPwgD6JLwV5jKli2r4%2bLReVgjJsheE29YBDthaw7FFaytzJWwlXmmjetJ3OTWwGsDz7wG4o51WEBwjrKzZmbLIZbHp3wYzodwQYZjU74J%2f%2fJiDc%2bLTQ0MQAwroTikyyBBQ2I8U%2fNX6GK%2f1pHvMF8jJiJVjEUWxmteNb536c" width="100%" height="600px" frameborder="0"></iframe> -->
  <!-- <iframe width="360" height="240" src="https://rtsp.me/embed/8G3NQ87t/" frameborder="0" allowfullscreen></iframe> -->
</p>

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

    function sendSlider(slider, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + slider + "=" + value;
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
          document.getElementById("volts").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
    };


    function buttonCamF(name)
    {
      console.log("button was clicked!");
      var xhr = new XMLHttpRequest();
      var url = name;

      xhr.open("GET", url, true);
      xhr.send();

      var camButtonStatus = document.getElementById("buttonCam").innerText
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("buttonCam").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
    };
    //document.addEventListener('DOMContentLoaded', buttonCamF, false);

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
      updateCamStatus();
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
    
    function updateCamStatus(){
      var xhr = new XMLHttpRequest();
      var url = "/updatecam"; // different than camstate which triggers the switch, we just want the status here
      xhr.open("GET", url, true);
      xhr.send();

        xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("buttonCam").innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };
    };
    //document.addEventListener('DOMContentLoaded', updateCamStatus, false);
    
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
