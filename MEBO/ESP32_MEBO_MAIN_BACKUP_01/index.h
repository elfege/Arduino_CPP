const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>

    <button style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stopall')"> STOP ALL! </button>
    <button style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset')"> RESET </button>
    <button id="buttonPower" style=width:100px;height:45px;margin-left:0;color:white;background-color:black onclick="buttonPowerF()"> OFF </button>


  </head>
  <body>

    <table style="width:100%;text-align: center;">
      <tr>
        <th>MOVMENT</th>
        <th>VIDEO</th> 
        <th>ARM CONTROL</th>
      </tr>
      <tr>
        <td>

          <div><p><button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/fwd')"> ^ </button>

            <div>
              <p>
                <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/left')"> < </button>
                  <button style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/stop')"> STOP </button>
                  <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/right')"> > </button>
                  </p>
              <p><button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/rvs')"> V </button></p>
            </div>
            <hr></hr>
          <p>MOTION IS: <span id="MVT-state">__</span> </p><p> </p>
          <div><p><input type="text" size="40" onkeydown="keyfunction(event)" onkeyup="MainFunc('/stop')"></p><div>
            </td>
            <td>
              <p> <button onclick="buttonCamF('/ledstate')" id="buttonCam">TURN ON</button> </p>
              <p>
                <iframe width="360" height="240" src="https://rtsp.me/embed/RfSNnEyy/" frameborder="0" allowfullscreen></iframe>
              </p>
              <p>
                this video is being broadcasted using http://rtsp.me
              </p>
              <p>
              </p>
              <OBJECT classid='clsid:9BE31822-FDAD-461B-AD51-BE1D1C159921' codebase='http://downloads.videolan.org/pub/videolan/vlc/latest/win32/axvlc.cab' width='360' height='240' id='vlc' events='True'>
                <param name='Src' value='rtsp://admin:1ROtrashrisk@192.168.10.181:554/h264Preview_01_main' />
                <param name='ShowDisplay' value='True' />
                <param name='AutoLoop' value='False' />
                <param name='AutoPlay' value='True' />
                <embed id='vlcEmb' type='application/x-google-vlc-plugin' version='VideoLAN.VLCPlugin.2' autoplay='yes' loop='no' width='640' height='480' target='rtsp://cameraipaddress' ></embed>
            </OBJECT>
          <p>
            FLASH VERSION (I.E. only)
          </p>

      </p>
      </td> 
    <td> 

      <div> 
        <p></p>
        <h8 style=color:red; >ARM</h8> 
        <p></p>        
        <button style=width:100px;height:45px;margin-left:0;font-size:160%; onclick="MainFunc('/ArmUp')"> ^ </button>
      </div>
      <div> 
        <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/ArmDown')"> V </button>
      </div>
      <div>
        <p></p>
        <h8 style=color:red; >FOREARM</h8>
        <p></p>
        <button style=width:100px;height:45px;margin-left:0;font-size:160%; onclick="MainFunc('/ForeArmUp')"> ^  </button>   
      </div>
      <div>
        <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/ForeArmDown')"> V </button>
      </div>
      <div>
        <p></p>
        <h8 style=color:red; >CLAMP</h8>
        <p></p>
        <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/CloseClamp')"> CLOSE  </button>
        <button style=width:100px;height:45px;margin-left:0 onclick="MainFunc('/OpenClamp')"> OPEN </button>
      </div>
    </td>
    </tr>
  <tr>
    <td>
      <h4>PROGRAMMED RIDE</h14>


      <div class="slidecontainer">
        <p></p>
        <h8 style=color:red; >SPEED: <span id="speed"></span>  &nbsp  &nbsp </h8>
        <div> <input type="range" min="140" max="200" value="170" class="slider" id="speedRange" data-orientation="vertical"> </div>
      </div>
      <div class="slidecontainer">
        <div> <h8 style=color:red; >DISTANCE: <span id="distance"></span>  &nbsp </h8> </div>
        <input type="range" min="1" max="200" value="15" class="slider" id="distanceRange">
      </div>



      <p> <button style=width:100px;height:45px;margin-left:0;color:white;background-color:navy blue onclick="execute()"> Execute FWD </button>
        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:magenta onclick="rvsDistF()"> Execute RVS </button></p>

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
      <p><button style=width:100px;height:45px;margin-left:0 onclick="selfdriveF()"> Auto-Roaming </button>
      <p><button style=width:100px;height:45px;margin-left:0 onclick="LineFWDF()">  Follow The Line (fwd)  </button>

        <button style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="CancelStopAllF()"> Reset Drive </button></p>

    </td>
  </tr>
  </table>

</body>
<script>

  var slider = document.getElementById("speedRange");
  var output = document.getElementById("speed");
  output.innerHTML = slider.value;

  slider.oninput = function() {
    output.innerHTML = this.value; 
  }
  slider.onmouseup = function() { 
    sendSlider(this.id, this.value);
  }

  var slider2 = document.getElementById("distanceRange");
  var output2 = document.getElementById("distance");
  output2.innerHTML = slider2.value;

  slider2.oninput = function() {
    output2.innerHTML = this.value; 
  }
  slider2.onmouseup = function() { 
    sendSlider(this.id, this.value);
  }

  function sendSlider(slider, value){
    var xhr = new XMLHttpRequest();
    var url = "/" + slider + "=" + value;
    xhr.open("GET", url, true);
    xhr.send();  
  }


  function keyfunction(event){

    var thecode = event.keyCode;

    if (thecode === 38){
      var xhr = new XMLHttpRequest();
      var url = "/fwd";
    }
    if (thecode === 39){
      var xhr = new XMLHttpRequest();
      var url = "/right";
    }
    if (thecode === 37){
      var xhr = new XMLHttpRequest();
      var url = "/left";
    }
    if (thecode === 40){
      var xhr = new XMLHttpRequest();
      var url = "/rvs";
    }

    xhr.open("GET", url, true);
    xhr.send();

  };


  function buttonCamF($val)
  {
    console.log("button was clicked!");
    var xhr = new XMLHttpRequest();
    var url = $val;


    xhr.open("GET", url, true);
    xhr.send();

    var camButtonStatus = document.getElementById("buttonCam").innerText
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("buttonCam").innerText = this.responseText;
      }
    };
  };
  document.addEventListener('DOMContentLoaded', buttonCamF, false);

  function buttonPowerF()
  {


    const Http = new XMLHttpRequest();

    const url='192.168.10.105/H';
    Http.open("GET", url);
    Http.send();
    Http.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        alert(Http.responseText);
        
      }
console.log(Http.responseText)
    }


  };
  document.addEventListener('DOMContentLoaded', buttonPowerF, false);


  function MainFunc($val)
  {

    var xhr = new XMLHttpRequest();
    var url = $val 

    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("MVT-state").innerHTML = this.responseText;
      }
    };

    xhr.open("GET", url, true);
    xhr.send();
  };
  document.addEventListener('DOMContentLoaded', MainFunc, false);
</script>
</html>
)=====";

// rqSp = parseInt(document.getElementById("speed").value);
