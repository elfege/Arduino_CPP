const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
   <center>
    <h4>COMPUTER SLEEP SWITCH</h4>
   </center>
  </head>
<body>


<table style="width:100%;text-align: center;">
    <tr>
      <th> POWER </th>
      <th> OVERRIDE FAILSAFE </th> 
      <th> RECOVER </th> 
    </tr>
    <tr>
      <td>
        <p></p>
        <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/togglepower', id)"> SWITCH </button>
      </td>
      <td>
       <p>
        <button id="override" style=width:100px;height:45px;margin-left:0;color:white;background-color:darkblue onclick="MainFunc('/override', id)">OVERRIDE</button>
       </p>
       <p>
        <button id="nosleep" style=width:100px;height:45px;margin-left:0;color:white;background-color:darkblue onclick="MainFunc('/nosleep', id)">DON'T SLEEP</button>
       </p>
       <p>
        <button id="shutdownbool" style=width:100px;height:45px;margin-left:0;color:white;background-color:darkblue onclick="MainFunc('/shutdownbooltoggle', id)">ALLOW SHUTDOWN</button>
       </p>
      </td> 
      <td>
        <p></p>
         <button id="hardreboot" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/hardreboot', id)"> HARD REBOOT </button>
        <p></p>
         <button id="shutDown" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/shutDown', id)"> SHUTDOWN </button>
        <p></p>
       <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
      </td>
     </tr>

     </table>

     <center>
     <p> <textarea id="timeInfo" rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p> <textarea id="macinfo"  rows="1"  cols="50"  wrap="soft"> </textarea> </p>
     <p> <textarea id="debug"    rows="10" cols="100" wrap="soft"> </textarea> </p>
     </center>

</body>

<!-**************************************************************************************-->
<!-                              SCRIPTS                                         -->
<!-**************************************************************************************-->
<script>

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
    document.addEventListener('DOMContentLoaded', MainFunc, false);
    
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
    document.addEventListener('DOMContentLoaded', getpowerstate, true);
        
      getMac();
      getDebug();
      getTimeInfo();
      MainFunc("/overrideCheck", "override");
      MainFunc("/shutdownbool", "shutdownbool");
      MainFunc("/nosleepcheck", "nosleep");
    
      var interval1 = setInterval(function(){getState();}, 10000);
      var interval2 = setInterval(function(){getDebug();}, 5000);
      var interval3 = setInterval(function(){getTimeInfo();}, 10000);
      var interval4 = setInterval(function(){MainFunc("/overrideCheck", "override");}, 5000);
      var interval5 = setInterval(function(){MainFunc("/shutdownbool", "shutdownbool");}, 5000);
      //setInterval(function(){getMac();}, 240000);

      

      setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes

      function clearAll(){
      clearInterval(interval1);
      clearInterval(interval2);
      clearInterval(interval3);
      clearInterval(interval4);
      clearInterval(interval5);
      alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
      };
      
  </script>
</html>
)=====";
