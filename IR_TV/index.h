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
        <h1 style=color:blue;><center>TV REMOTE</center></h1>
        <p><h4 style=color:red;><span id="state"></span> </h4></p>
      </center>
    </head>

  <body onload="REFRESH()">
    <center>
    <div>
      <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/toggle', id)"> POWER </button>
      <button id="select" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/select', id)"> APPLE ON </button>
      <button id="appletvoff" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/appletvoff', id)"> APPLE OFF</button>
      <button id="appletvmenu" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/appletvmenu', id)"> MENU</button>
      <button id="appletvplaypause" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/appletvplaypause', id)"> PLAY/PAUSE </button>
      <button id="REFRESH" style=width:100px;height:45px;margin-left:0;color:white;background-color:green onclick="REFRESH()"> REFRESH </button>
      <button id="RESET" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
      <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button>
    </div>

    <div>
      <p><button id="up" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/up', id)"><i class="up"></i></button></p>
      <p>
      <button id="left" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/left', id)"><i class="left"></i></button>
      <button id="select" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/select', id)"> SELECT </button>
      <button id="right" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/right', id)"><i class="right"></i></button>
      </p>
      <p>
      <button id="down" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/down', id)"><i class="down"></i></button>
      </p>
      </p>
    </div>
     <p> <textarea id="timeInfo" rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p> <textarea id="macinfo"  rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p></p>
     <iframe width="800" height="500" src="http://192.168.10.210/term.html" title="Serial Terminal Output" style="border:1px solid black;"></iframe> 
   

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

   

      function getpowerstate(){
      var xhr = new XMLHttpRequest();
      var url = "/getpowerstate"
      
      xhr.open("GET", url, true);
      xhr.send();  

      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) 
        {
          var textarea = document.getElementById("powerstatus")
         textarea.innerHTML = this.responseText; 
         textarea.scrollTop = textarea.scrollHeight;
        }
      };
    };   
    
    
      //setTimeout(function(){location.reload()}, 10000);
      getMac();
      getTimeInfo()

      setInterval(function(){getTimeInfo();}, 10000);
      setInterval(function(){getpowerstate();}, 5000);
    
    //

   
     

  </script>


  </html>
)=====";
