const char MAIN_page[] PROGMEM = R"=====(
  <html>
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
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
      <center>
        <h1 style=color:blue;><center>BED SENSOR</center></h1>
        <p><h4 style=color:red;><span id="state">STATUS</h4></p>
      </center>
    </head>

  <body onload="refresh()">
    <center>
    <div>
    
      <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="refresh()"> REFRESH </button>
      <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
      <button id="DEBUG" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/logs', id)"> DEBUG </button>
      <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button>
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

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById(id).innerText = this.responseText;
          console.log(xhr.responseText);
        }
      };   
    };
    //document.addEventListener('DOMContentLoaded', MainFunc, false);

    function refresh()
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

    function getState() 
    {
      var xhr = new XMLHttpRequest();
      var url = "/getstate";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("state").innerHTML = this.responseText;
          console.log(xhr.responseText);
        }
       };   
     };
    
      //setTimeout(function(){location.reload()}, 10000);
      getMac();
      getDebug();
      getTimeInfo()
      getState();

      var interval1 = setInterval(function(){getState();}, 10000);
      var interval2 = setInterval(function(){getDebug();}, 30000);
      var interval3 = setInterval(function(){getTimeInfo();}, 10000);
      //setInterval(function(){getMac();}, 240000);

      setTimeout(function(){ clearAll(); }, 30 * 60 * 1000); // clear all intervals after 30 minutes

      function clearAll(){
        clearInterval(interval1);
        clearInterval(interval2);
        clearInterval(interval3);
        alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
      }
    
    //document.addEventListener('DOMContentLoaded', MainFunc, false);

   
     

  </script>


  </html>
)=====";
