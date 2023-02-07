//https://github.com/Hieromon/PageBuilder/blob/master/examples/DynamicPage/DynamicPage.ino // build more complex pages 

const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <script type="text/javascript" src="http://www.ispyconnect.com/external_viewer2.aspx?id=ANk9M1sYEUKt%2fLk7JWqG0AegfmPwgD6JLwV5jKli2r4%2bLReVgjJsheE29YBDthawhHoUyxN4fC9AgC23JDMGruFZ%2fwuey3ZJa%2b5JupRYKlAVeRlSdnunK6B3%2bg62FBfewhQOCtAuEgUvD88v5X4%2fW1uiTLuNu21LcJiRRVOM280%3d"></script> 

    <meta name="viewport" content="width=device-width, initial-scale=1">  <!––  arrows buttons -->
    <style>
      i {
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
    <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
       <!-- HORIZONTAL SLIDER -->
      .slidecontainer {
        width: 100%;
      }
      
      .sliderV {
        -webkit-appearance: slider-vertical;
        width: 100%;
        height: 125px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
      .slider {
        -webkit-appearance: none;
        width: 100%;
        height: 25px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
      
      .slider:hover {
        opacity: 1;
      }
      
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 25px;
        height: 25px;
        background: #4CAF50;
        cursor: pointer;
      }
      
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        background: #4CAF50;
        cursor: pointer;
      }

      
      </style>
    </center>
  <button id="mainpoweron" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="buttonPowerON_f_AUX()"> PW ON </button>
  <button id="mainpoweroff" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="buttonPowerOFF_f_AUX()"> PW OFF </button>
  <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
  <button id="camoff" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/camoff', id)"> CAM OFF </button>
  <button id="camon" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/camon', id)"> CAM ON </button>
  <button id="logs" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/logs')"> LOGS </button>
  <button id="dock" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/dock')"> DOCK </button>

  <p><button id="chargestatus" style=width:110px;height:55px;margin-left:0;color:white;background-color:red onclick="MainFunc('/charge', id)"> CHARGE ON </button></p>

  <p><span id="volts">Battery Level: </span> Volts </p><p> </p>
  </center>
</head>
<body onload="UpdateValues()">
  <table style="width:100%;text-align: center;">
    <tr>
      <th>MOVMENT</th>
      <th>VIDEO</th> 
      <th></th>
      <th>ARM CONTROL</th>
      <th></th>
    </tr>
    <tr>
      <td>
          <button id="stop" style=width:100px;height:45px;margin-left:0;font-weight:900;color:white;background-color:red onclick="MainFunc('/stop', id)"> STOP </button>
        <p><button id="fwd" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/fwd', id)"><i class="up"></i></button></p>
        <p>
          <button id="left" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/left', id)"><i class="left"></i></button>          
          <button id="right" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/right', id)"><i class="right"></i></button>
        </p>        <p>
        
          <button id="rvs" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/rvs', id)"><i class="down"></i></button>
            </p>        <p>
            <button id="trackfwd" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/trackfwd', id)">TRACK FWD</button>
          <button id="trackback" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/trackback', id)">TRACK BCK</button>
        </p>
    </p>
    <hr></hr>
  <p><span id="MVT-state">current state</span> </p><p> </p>
  <div>
    <p>
      <input type="text" id="textBox1" size="40">
    </p>
  </div>
  

  <div class="slidecontainer">
    <p></p>
    <h8 style=color:red; >SPEED: <span id="speed"></span>  &nbsp  &nbsp </h8>
    <div> <input type="range" min="140" max="200" value="170" class="slider" id="speedRange" class="slider" onmouseup="sendSlider(id, value)"> </div>
  </div>

  </td>
<td>
  <p> 
  <p><script>iSpyFeed();</script>

  </p>

</td> 
<td> 
 
  <div>
   <p></p>
   <div><h8 style=color:red; >FOREARM POSITION: <span id="forearmpostion"></span>  &nbsp  &nbsp </h8>
   <input type="range" min="0" max="180" value="90" class="sliderV" id="forearmrange" onmouseup="sendSlider(id, value)"></div>
   </td> 
   <td> 
    <h8 style=color:red; >TOWER POSITION: <span id="towerpostion"></span>  &nbsp  &nbsp </h8>
    <input type="range" min="0" max="180" value="90" class="slider" id="towerrange" onmouseup="sendSlider(id, value)"> </div>
    <div><h8 style=color:red; >WRIST POSITION: <span id="wristpostion"></span>  &nbsp  &nbsp </h8>
    <input type="range" min="0" max="180" value="90" class="slider" id="wristrange" onmouseup="sendSlider(id, value)"> </div>
     <div><h8 style=color:red; >CLAMP POSITION: <span id="clamppostion"></span>  &nbsp  &nbsp </h8>
    <input type="range" min="0" max="180" value="90" class="slider" id="clamrange" onmouseup="sendSlider(id, value)"> </div>
    </td>
    <td>
    <div><h8 style=color:red; >ARM POSITION: <span id="armpostion"></span>  &nbsp  &nbsp </h8>
     <input type="range" min="0" max="180" value="90" class="sliderV" id="armrange" onmouseup="sendSlider(id, value)"></div>
     <td>

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
    <p>
  </td>
</tr>
</table>

</body>
<script>

  var slider = document.getElementById("speedRange");
  var output = document.getElementById("speed");
  output.innerHTML = slider.value;
  var slider2 = document.getElementById("towerrange");
  var output2 = document.getElementById("towerpostion");
  output2.innerHTML = slider2.value;
  var slider3 = document.getElementById("armrange");
  var output3 = document.getElementById("armpostion");
  output3.innerHTML = slider3.value;
  var slider4 = document.getElementById("forearmrange");
  var output4 = document.getElementById("forearmpostion");
  output4.innerHTML = slider4.value;
  var slider5 = document.getElementById("wristrange");
  var output5 = document.getElementById("wristpostion");
  output5.innerHTML = slider5.value;
  var slider6 = document.getElementById("clamrange");
  var output6 = document.getElementById("clamppostion");
  output6.innerHTML = slider6.value;


  slider.oninput = function() {
    output.innerHTML = this.value; 
  }
  slider2.oninput = function() {
    output2.innerHTML = this.value; 
  }
  slider3.oninput = function() {
    output3.innerHTML = this.value; 
  }
   slider4.oninput = function() {
    output4.innerHTML = this.value; 
  }
   slider5.oninput = function() {
    output5.innerHTML = this.value; 
  }
  slider6.oninput = function() {
    output6.innerHTML = this.value; 
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
  //document.addEventListener('DOMContentLoaded', MainFunc, false);

  function getchargestate(){
    var xhr = new XMLHttpRequest();
    var url = "/getchargestate"

    xhr.open("GET", url, true);
    xhr.send();  

    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) 
      {
        var textarea = document.getElementById("chargestatus")
        textarea.innerHTML = this.responseText; 
        textarea.scrollTop = textarea.scrollHeight;
      }
    };
  };   
  document.addEventListener('DOMContentLoaded', getchargestate, true);

  function buttonPowerOFF_f_AUX()
  {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'http://192.168.10.104/poweroff');

    xhr.send();
    console.log(xhr.responseText)
    //alert("ROBOT SUCCESSFULLY TURNED OFF!");

  };
  //document.addEventListener('DOMContentLoaded', buttonPowerOFF, false);

  function buttonPowerON_f_AUX()
  {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'http://192.168.10.104/poweron');

    xhr.send();
    console.log(xhr.responseText)
    //alert("ROBOT SUCCESSFULLY TURNED ON!");

  };
  //document.addEventListener('DOMContentLoaded', buttonPowerON, false);

  function UpdateValues(){      
    updateVoltsStatus();
  };

  window.setInterval(updateVoltsStatus, 60000);

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

  /*********************************************************************/
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


  setInterval(function(){getchargestate();}, 1000);

</script>


</html>
)=====";
