const char simplifiedPage[] PROGMEM = R"=====(
  <html>
    <head>
      <center>
        <h1 style=color:blue;><center>AC BEDROOM</center></h1>
        <p><h4 style=color:red;><span id="state"></span> </h4></p>
      </center>
    </head>

  <body onload="REFRESH()">
    <center>
    <div>
      <button id="COOL" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/cool', id)"> COOL </button>
      <button id="HEAT" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/heat', id)"> HEAT </button>
      <button id="AUTO" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/auto', id)"> AUTO </button>
      <button id="OFF" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/off', id)"> OFF </button>
      <button id="TURBO" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/turbo', id)"> TURBO </button>
    </div>
    <div class="slidecontainer">
      <p></p>
      <h8 style=color:red; >Set Point: <span id="temp"></span>  &nbsp  &nbsp </h8>
      <div> <input type="range" min="60" max="90" value="0" class="slider" id="tempRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </div>
    </div>

    
    </center>
  </body>

  <script>

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

    
    var slider = document.getElementById("tempRange");
    var output = document.getElementById("temp");
    output.innerHTML = slider.value;
   
    slider.oninput = function() {
      output.innerHTML = this.value;
      getTempVal();
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
          console.log(xhr.responseText);
        }
      };
    };    

    function sendSlider(id, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + id + "=" + value;
      xhr.open("GET", url, true);
      xhr.send();  
      //document.getElementById("debug").innerHTML = url
    };
     
      getTempVal();
      var interval5 = setInterval(function(){getClockInfo();}, 500);

  </script>


  </html>
)=====";
