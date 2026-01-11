const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
   <center>
    <h4>CATS WATER TANK PURGE</h4>
   </center>
  </head>
<body>
  <center>
    <table style="width:100%;text-align: center;">
      <tr>
        <th> </th>
        <th>  </th> 
        <th> </th> 
      </tr>
      <tr>
        <td>
          <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/on', id)"> PURGE </button>
          <button id="powerstatus" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/nonStopPurge', id)"> CONTINUOUS </button>
          <button id="chargeon" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/off', id)"> STOP </button>
          <button id="refresh" style=width:100px;height:45px;margin-left:0;color:white;background-color:red onclick="MainFunc('/refresh', id)"> REFRESH </button>
          <button id="CLEAR" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/clear', id)"> CLEAR </button>
        </td>
       
        <td>
         <button id"reset" style=width:100px;height:45px;margin-left:0;color:white;background-color:grey onclick="MainFunc('/reset', id)"> RESET </button>
        </td>
      </tr>
  </table>
   <p> <textarea id="timeInfo" rows="1"  cols="50"  wrap="soft"> </textarea> </p>
   <p> <textarea id="macinfo"  rows="1"  cols="50"  wrap="soft"> </textarea> </p>
   <p> Analog Value = <textarea id="wetv"  rows="1"  cols="10"  wrap="soft"> </textarea> </p>
   <p> Wet Status = <textarea id="state"  rows="1"  cols="10"  wrap="soft"> </textarea> </p>
   <p> <textarea id="debug"    rows="10" cols="1000" wrap="soft"> </textarea> </p>
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

    function getWetVal() 
    {
      var xhr = new XMLHttpRequest();
      var url = "/getwetval";
      xhr.open("GET", url, true);
      xhr.send();
     
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("wetv").innerHTML = this.responseText;
          console.log(xhr.responseText);
        }
       };   
     };

    getMac();
    getDebug();
    getTimeInfo();
    getState();

    var interval1 = setInterval(function(){getWetVal();}, 1000);
    var interval2 = setInterval(function(){getDebug();}, 5000);
    var interval3 = setInterval(function(){getTimeInfo();}, 10000);
    var interval1 = setInterval(function(){getState();}, 1000);

    setTimeout(function(){ clearAll(); }, 1800 * 1000); // clear all intervals after 30 minutes

    function clearAll(){
      clearInterval(interval1);
      clearInterval(interval2);
      clearInterval(interval3);
      alert("ALL SCHEDULED TASKS CANCELED ; refresh this page to resume");
    }

    
  </script>
</html>
)=====";
