const char MAIN_page[] PROGMEM = R"=====(
<html>

<head>
  <style>
    /* CSS styles for button container */
    .button-container {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 10px;
      margin-top: 20px;
    }

    /* CSS styles for buttons */
    .btn {
      width: 150px;
      height: 50px;
      font-weight: bold;
      border-radius: 5px;
    }

    #feed {
      background-color: blue;
      color: white;
    }

    #REFRESH {
      background-color: green;
      color: white;
    }

    #RESET {
      background-color: gray;
      color: white;
    }

    /* CSS styles for serial output terminal */
    #terminal {
      width: 100%;
      height: 300px;
      border: 1px solid black;
      margin-top: 20px;
    }
  </style>
</head>

<body>
  <div class="container">
    <div class="row justify-content-center my-3">

      <!-- Central column for content -->
      <div class="col-xs">
        <!-- CAT FEEDER Title -->
        <h1 style="color:blue; text-align:center;">DRY FOOD CAT FEEDER  <span id="fill_state"><span></h1>

        <!-- State Display -->
        <div style="text-align:center;">
          <h4 style="color:red;"><span id="state"></span></h4>
        </div>

        <!-- Button Container -->
        <div style="text-align:center; margin-bottom: 20px;">
          <button id="feed" class="btn btn-primary">FEED</button>
          <button id="REFRESH" class="btn btn-secondary">REFRESH</button>
          <button id="RESET" class="btn btn-danger">RESET</button>
        </div>

        <!-- Quantity Slider and Label -->
        <div style="text-align:center; margin-bottom: 20px;">
          <h8 style="color:red;">QUANTITY: <span id="qty"></span></h8>
          <input type="range" min="0" max="15" value="0" class="slider mx-2" id="qtyRange">
        </div>

        <!-- Information Rows, now as individual blocks -->
        <div style="text-align:center; margin-bottom: 20px;">
          <div><span id="timeInfo"></span></div>
          <div><span id="macinfo"></span></div>
        </div>
      </div>
    </div>

    <div class="row justify-content-center my-3">
      <!-- Serial Output Terminal -->
      <div class="col-xs-4">
      </div>
      <div class="col-xs-4 m-5">
        <div style="text-align:center;">
          <h4 id="serial">Serial Output</h4>
          <iframe id="terminal" src="http://192.168.10.209/term.html" style="width:50%; height:600px;"></iframe>
        </div>
        <div class="col-xs-4">
        </div>
      </div>
    </div>

  </div>
  </div>
  </div>

  <!-- JavaScript libraries -->
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>

  <!-- JavaScript code -->
  <script>
    $(document).ready(function () {
      // Function to send GET requests to the server
      function mainFunc(endpoint) {
        axios.get('/' + endpoint)
          .then(response => {
            $('#state').text(response.data);
            console.log(response.data);
          })
          .catch(error => console.error('Error:', error));
      }

      // Event listeners for buttons
      $('#feed').click(() => mainFunc('feed'));
      $('#REFRESH').click(() => refresh());
      $('#RESET').click(() => mainFunc('reset'));

      // Function to refresh data
      function refresh() {
        mainFunc('refresh');
      }

      // Function to update slider value
      function updateSliderVal() {
        axios.get('/sliderupdate')
          .then(response => {
            $('#qty').text(response.data); // Update slider
            console.log(response.data);
          })
          .catch(error => console.error('Error:', error));
      }

      // Event listener for slider input
      $('#qtyRange').on('input change', function () {
        $('#qty').text(this.value);
        axios.get('/qtyRange=' + this.value)
          .then(response => console.log('Slider Updated:', response.data))
          .catch(error => console.error('Error:', error));
      });

      // Function to get additional information
      function getInfo(endpoint, elementId) {
        axios.get('/' + endpoint)
          .then(response => {
            $('#' + elementId).text(response.data);
            console.log(response.data);
          })
          .catch(error => console.error('Error:', error));
      }

      // fill state FULL / EMPTY
      function getFillState(endpoint, elementId) {
        axios.get('/' + endpoint)
          .then(response => {
            $('#' + elementId).text(response.data);
            if(response.data === "EMPTY"){
              $('#' + elementId).css("color", "red");
            }
            else {
              $('#' + elementId).css("color", "green");
            }
            console.log(response.data);
          })
          .catch(error => console.error('Error:', error));
      }

      // Initializations
      getInfo('getMac', 'macinfo');
      getInfo('getTimeInfo', 'timeInfo');
      setInterval(() => updateSliderVal(), 1000);
      setInterval(() => getInfo('getTimeInfo', 'timeInfo'), 10000);
      setInterval(() => getInfo('getMac', 'macinfo'), 30000);
      setInterval(() => getFillState('fill_state', 'fill_state'), 10000);
    });
  </script>
</body>

</html>
)=====";