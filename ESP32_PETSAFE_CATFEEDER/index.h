const char MAIN_page[] PROGMEM = R"=====(
  <html>
    <head>
      <center>
        <h1 style=color:blue;><center>CAT FEEDER</center></h1>
        <p><h4 style=color:red;><span id="state"></span> </h4></p>
      </center>
    </head>
    
    <body onload="REFRESH()">
    <center>
    <div>
      <button id="feed" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/feed', id)"> FEED </button>
      <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="REFRESH()"> REFRESH </button>
      <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
    </div>
     <div class="slidecontainer">
      <p></p>
      <h8 style=color:red; >QUANTITY: <span id="qty"></span>  &nbsp  &nbsp </h8>
      <div> <input type="range" min="0" max="15" value="0" class="slider" id="qtyRange" data-orientation="vertical" onmouseup="sendSlider(id, value)"> </div>
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
    document.addEventListener('DOMContentLoaded', debug, true);    
    
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
    
    
    function getQtyValue()
    {
      var xhr = new XMLHttpRequest();
      var url = "/qty";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("qty").innerHTML = this.responseText;
          document.getElementById("qtyRange").innerHTML = this.responseText; // update slider
          console.log(xhr.responseText);
        }
      };
    };
    
    var slider = document.getElementById("qtyRange");
    var output = document.getElementById("qty");
    output.innerHTML = slider.value;     
    slider.oninput = function() {
      output.innerHTML = this.value;
      getQtyValue();
    }
    function sendSlider(id, value){
      var xhr = new XMLHttpRequest();
      var url = "/" + id + "=" + value;
      xhr.open("GET", url, true);
      xhr.send();  
      //document.getElementById("debug").innerHTML = url
    };
    function updateSliderVal()
    {
      var xhr = new XMLHttpRequest();
      var url = "/sliderupdate";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          output.innerHTML = this.responseText; // update slider
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
    
      getMac();
      getDebug();
      getTimeInfo()
    
      setInterval(function(){updateSliderVal();}, 1000);
      setInterval(function(){getDebug();}, 2000);
      setInterval(function(){getTimeInfo();}, 10000);
      setInterval(function(){getMac();}, 30000);
    
    //document.addEventListener('DOMContentLoaded', MainFunc, false);
    
    </script>
  </html>
)=====";
