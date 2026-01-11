const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <p> CAMERA IS: <span id="MEBORelay-state">__</span> <button onclick="myFunction('/ledstate')"> TOGGLE </button> </p>
    <p>MOTION IS: <span id="MVT-state">__</span> </p><p> </p>


    </form>;

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
        <div><p><input type="text" size="40" onkeydown="keyfunction(event)" onkeyup="MainFunc('/stop')"></p><div>
          </td>
          <td>
            <p>
              <iframe width="360" height="240" src="https://rtsp.me/embed/RfSNnEyy/" frameborder="0" allowfullscreen></iframe>
            </p>
            <p>
              this video is being broadcasted using http://rtsp.me
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
          <td>AUTOMATIONS MANAGEMENT</td> 
          <td>94</td>
        </tr>
        <tr>
          <td>

          </td>
          <td> 
            <p><button style=width:100px;height:45px;margin-left:0 onclick="selfdriveF()"> Auto-Roaming </button></p>

            <p><button style=width:100px;height:45px;margin-left:0 onclick="LineFWDF()">  Follow The Line (fwd)  </button></p>
            <p><button style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="StopAllF()"> STOP ALL! </button>
              <button style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="CancelStopAllF()"> Reset Drive </button></p>
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


    function myFunction($val)
    {
      console.log("button was clicked!");
      var xhr = new XMLHttpRequest();
      var url = $val;

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("MEBORelay-state").innerHTML = this.responseText;
        }
      };

      xhr.open("GET", url, true);
      xhr.send();
    };
    document.addEventListener('DOMContentLoaded', myFunction, false);

    function MainFunc($val)
    {

      console.log("button was clicked!");
      var xhr = new XMLHttpRequest();
      var url = $val 

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("state").innerHTML = this.responseText;
        }
      };

      xhr.open("GET", url, true);
      xhr.send();
    };
    document.addEventListener('DOMContentLoaded', MainFunc, false);
  </script>
  </html>
)=====";
