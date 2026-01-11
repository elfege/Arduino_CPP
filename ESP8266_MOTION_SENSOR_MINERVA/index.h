const char MAIN_page[] PROGMEM = R"=====(
  <html>
    <head>
      <center>
        <h1 style=color:blue;><center>MOTION SENSOR MINERVA</center></h1>
        <p><h4 style=color:red;><span id="state"></span> </h4></p>
      </center>
    </head>

  <body onload="REFRESH()">
    <center>
    <div>
      <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="REFRESH()"> REFRESH </button>
      <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
      <button id="HARDRESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/hardreset', id)"> HARD RESET </button>
      <button id="DEBUG" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/logs', id)"> DEBUG </button>
      <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button>
     
  
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

      //setTimeout(function(){location.reload()}, 10000);
      getMac();
      getDebug();
      getTimeInfo()
    
      setInterval(function(){getDebug();}, 1000);
      setInterval(function(){getTimeInfo();}, 10000);
      setInterval(function(){getMac();}, 30000);
    
    //document.addEventListener('DOMContentLoaded', MainFunc, false);

    

  </script>


  </html>
)=====";
