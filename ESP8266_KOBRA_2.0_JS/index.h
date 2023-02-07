const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
   <center>
    <h4>KOBRA JS POWER CONTROL</h4>
   </center>
  </head>
<body>


<table style="width:100%;text-align: center;">
    <tr>
      <th> POWER </th>
      <th> CHARGER </th> 
      <th> RECOVER </th> 
    </tr>
    <tr>
      <td>
        <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/power', id)"> START </button>
        <button id="cam" style=width:100px;height:45px;margin-left:0;color:white;background-color:blue onclick="MainFunc('/camon', id)"> CAM ON </button>
        <button id="cam" style=width:100px;height:45px;margin-left:0;color:white;background-color:pink onclick="MainFunc('/camoff', id)"> CAM OFF </button>
        <p>
        <button id="reboot" style=width:100px;height:45px;margin-left:0;color:white;background-color:darkblue onclick="MainFunc('/reboot', id)"> REBOOT </button>
        </p>
      </td>
      <td>
        <button id="chargestatus" style=width:110px;height:55px;margin-left:0;color:white;background-color:red onclick="MainFunc('/charge', id)"> CHARGE ON </button>
        </td> 
      <td>
       <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> LOCAL RESET </button>
       <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/remotereset', id)"> REMOTE RESET </button>
       
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
      getTimeInfo()
    

      setInterval(function(){getchargestate();}, 1000);
      setInterval(function(){getpowerstate();}, 1010);
      setInterval(function(){getDebug();}, 20000);
      setInterval(function(){getTimeInfo();}, 10000);
      setInterval(function(){getMac();}, 30000);
  </script>
</html>
)=====";
